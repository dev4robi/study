#include "climain.h"
#include "stdheader.h"
#include "climsgsocket.h"
#include "cliglobal.h"
#include "commonlib.h"
#include "msgfileio.h"
#include "clirecord.h"

/*
 * [ Workflow ]
 * 1. �������� ����
 * 2. �� ���� �б�			   -- (���� ������ŭ �ݺ�)
 * 3. ���ڵ� �з�				|
 * 4. ������������ ��ȯ			|
 * 5. ���а����� ���Ͽ� ����		| [����]
 * 6. ������ �����ϱ�(��ȸ���Ĺ)		| -> 12. ���� �������� [0000:]�� ���� ��� �����͸� �߰��Ͽ� ����
 * 7. �������� ����ޱ�			| -> 13. ������ �������� �� �翬���Ͽ� ����
 * 8. ���������� �����ϱ�			|
 * 9. ����� ���� ����		   --
 * 10. ǥ����, ����� ����
 * 11. ��� ���� ���� �ݱ�
 * +1. �α� ���� �ۼ�
 * +2. ���� �ַܼ� ���
 * +3. ȯ�氪 global.c���� ����
 *
 * < 1�� ���� >
 * (1), (2), (3), (+2)
 *
 * < 2�� ���� >
 * [1], [2], [3], (4), (5), (6), (7), (8), (9), (10), (11), [+2], (+3)
 *
 * < 3�� ���� >
 * [1], [2], [3], [4], [5], [6], [7], [8], [9], [10], [11], [12], [13], [+1], [+2], [+3]
 *
 */

int main(int argc, char **argv) {
	if (checkArgc(argc) != 0)
	{
		return -1;
	}
	
	if (initGlobalEnvs(argc, argv) != 0)
	{
		return -1;
	}

	if (clientWork() != 0)
	{
		return -1;
	}
	
	return freeGlobalEnvs();
}

int checkArgc(int argc) {
	if (argc <= 1)
	{
		fprintf(stderr, "'startCli.bat' ���Ϸ� ���α׷��� �����Ͻʽÿ�.\n");
		return -1;
	}
	
	return 0;
}

int clientWork() {
	char		arLineBuf[512] = { 0, };
	int			szLineBuf = sizeof(arLineBuf);
	CliRecord	cliRecord;
	SvrRecord	svrRecord;
	SOCKET		hSocket;
	long long	startTime = 0, lastRecvTime = 0;
	int			szCliRecord = sizeof(CliRecord), szSvrRecord = sizeof(SvrRecord);
	FILE*		pInMsgFile = NULL;
	FILE*		pOutMsgFile = NULL;
	int			szReadMsg = -1, lineIdx = 0;
	long long	timeDelta = 0;
	float		avgSendPerSec = 0.00f;
	long long	MaxSendPerSec = 0;
	
	fprintf(stdout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	fprintf(stdout, "* ���α׷� ����...\n");
	startTime = currentTimeMillis();

	/* �������� I/O */
	pInMsgFile = openMsgFile(g_IN_MSG_FILE_PATH, "r+");		/* �Է� �������� ����(�б�����) */
	pOutMsgFile = openMsgFile(g_OUT_MSG_FILE_PATH, "w");	/* ��� �������� ����(��������) */
	g_LOG = openMsgFile(g_OUT_LOG_FILE_PATH, "w");			/* ��� �������� ����(��������) */
	
	if (pInMsgFile == NULL || pOutMsgFile == NULL || g_LOG == NULL)
	{
		fprintf(stdout, "* ����/�α� ���� �Ǵ� ���� ����!\n");
		goto exit;
	}
	
	fprintf(stdout, "* ����/�α� ���� �� ���� �Ϸ�!\n");
	
	/* ��ȯ�� ǥ���� ���Ͽ� ���� */
	initRecord(&cliRecord, szCliRecord);
	initRecord(&svrRecord, szSvrRecord);
	cvtSvr2CliRecord(&svrRecord, &cliRecord, 0);
	writeMsg(pOutMsgFile, byteToString(arLineBuf, (char*)&cliRecord.stHead, sizeof(CliHeadRecord)), 0, 0);	
	fprintf(stdout, "* ǥ���� �ۼ� �Ϸ�!\n");
	fprintf(stdout, "* ���� �б� �� �ۼ�����...\n");
	
	while (1)
	{
		++lineIdx;
		initRecord(&cliRecord, szCliRecord);
		initRecord(&svrRecord, szSvrRecord);
		
		/* �� ���� �б� */
		if ((szReadMsg = readMsgLine(pInMsgFile, arLineBuf, szLineBuf)) <= 0) break;
		
		/* ���ڵ� �з��� ���� */
		switch (arLineBuf[0])
		{
			case 'D':
			{
				if (makeRecord(arLineBuf, szReadMsg, &(cliRecord.stData), sizeof(CliDataRecord)) < 0)
				{
					fprintf(stderr, "Ŭ�� �����ͺ�(Idx:%d) ���� ����.\n", lineIdx);
				}
				
				break;
			}
			case 'S':
			case 'E':
			{
				--lineIdx;
				continue;
			}
			default:
			{
				fprintf(stderr, "������ ������. (line:%d, idCode:%c).\n", lineIdx, arLineBuf[0]);
				continue;
			}
		}
		
		/* ������������ ��ȯ */
		if (cvtCli2SvrRecord(&cliRecord, &svrRecord) < 0) continue;
		
		/* ���������� ��� ���Ͽ� ���� */
		writeMsg(pOutMsgFile, byteToString(arLineBuf, (char*)&cliRecord.stData, sizeof(CliDataRecord)), lineIdx, 0);
		
		/* ���Ӽӵ� ���, ������ �ۼ��� �� �α� */
		MaxSendPerSec = (long)(1000.0 / g_MAX_SEND_SPEED_PER_SEC); /* �Ҽ��� ���� */

		while (1)
		{
			float asspsCntDelta = g_AVG_SEND_SPEED_PER_SEC - avgSendPerSec; /* AvgSendSpeedPerSec count delta */

			timeDelta = currentTimeMillis() - lastRecvTime;
			avgSendPerSec = (lineIdx * 1000.0f) / (currentTimeMillis() - startTime);

			if (asspsCntDelta < 0.00f)
			{
				Sleep(1);
				continue;
			}
			
			if (timeDelta < MaxSendPerSec)
			{
				Sleep(1);
				continue;
			}
			
			if (-1 == sendAndRecv(&hSocket, &svrRecord, (g_REUSABLE_SOCKET[0] == 'Y' ? 1 : 0)))
			{
				fprintf(stdout, "* ���� ���� ����. (%d)\n", lineIdx);
				break;
			}
			
			lastRecvTime = currentTimeMillis();
			break;
		}
		
		/* Ŭ���������� ��ȯ */
		if (cvtSvr2CliRecord(&svrRecord, &cliRecord, -1) < 0) continue;
		
		/* ��������� ��� ���Ͽ� ���� */
		writeMsg(pOutMsgFile, byteToString(arLineBuf, (char*)&cliRecord.stData, sizeof(CliDataRecord)), lineIdx, 0);
		
		pushpopxy(1, 5);
		fprintf(stdout, "* ���� �б� �� �ۼ�����... (%d) [��� ���� �ӵ� : %.02f/s] [���� ���� �ӵ� : %.02f/s]             \n", lineIdx, avgSendPerSec, 1000.0f / timeDelta);
		pushpopxy(0, 0);
	}
	
	fprintf(stdout, "* ���� �б� �� �ۼ���, ���� �ۼ� �Ϸ�!\n");
	
	/* ��ȯ�� ����� ���� �� ���Ͽ� ���� */
	initRecord(&cliRecord, szCliRecord);
	initRecord(&svrRecord, szSvrRecord);
	cvtSvr2CliRecord(&svrRecord, &cliRecord, 1);
	writeMsg(pOutMsgFile, byteToString(arLineBuf, (char*)&cliRecord.stTail, sizeof(CliTailRecord)), lineIdx, 0);
	fprintf(stdout, "* ����� �ۼ� �Ϸ�!\n");

exit:
	/* ���� ��Ĺ �ݱ� */
	if (g_REUSABLE_SOCKET[0] == 'Y')
	{
		closeSocket(&hSocket);
	}
	
	/* ��� ���� ���� �ݱ� */
	closeMsgFile(pInMsgFile);
	closeMsgFile(pOutMsgFile);
	closeMsgFile(g_LOG);
	pInMsgFile = NULL;
	pOutMsgFile = NULL;
	g_LOG = NULL;
	
	fprintf(stdout, "* ���α׷� ����... (���� �ð�: %.03f��)\n", timeDeltaMillis(startTime, currentTimeMillis()));
	fprintf(stdout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	
	return 0;
}