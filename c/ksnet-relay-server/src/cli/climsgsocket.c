#include "climsgsocket.h"
#include "cliglobal.h"
#include "msgfileio.h"
#include "commonlib.h"

static char STX = 0x2;
static char ETX = 0x3;
static int DataMinSz = 0;
static int DataMaxSz = 1024;
static int PacketMinSz = 6;
static int PacketMaxSz = 1030; /* (PacketMinSz + DataMinSz) */

int sendAndRecv(SOCKET *pSocket, SvrRecord *pstSvrRecord, int option) {
	static int TEST_CNT = 0;
	int serverPort = atoi(g_SERVER_PORT);
	int szPacket = sizeof(SvrRecord) + PacketMinSz;
	char sndBuf[1030] = { 0, };
	char rcvBuf[1030] = { 0, };
	int szBuf = sizeof(sndBuf);
	int errorCode = 0;
	int szErrorCode = sizeof(errorCode);
	int connectionCnt = 0;
	int recvLen = 0;
	
	++TEST_CNT;
	
	if (option == 0) /* ��ȸ�� ��Ĺ */
	{
		/* [Tip] winsock2 doesn't support write() and read() function.
				 (https://stackoverflow.com/questions/4778043/winsock-not-supporting-read-write) */
		makePacketFromMsg(sndBuf, szBuf, (char*)pstSvrRecord, sizeof(SvrRecord)); /* 300 -> 306 */
		
		while (connectionCnt++ < g_RECONNECT_TRY_CNT)
		{
			if (connectSocket(pSocket, g_SERVER_IP, serverPort) != 0)
			{
				fprintf(stderr, "* �߰����� ���� ����. (%d/%d) (Record:%d)\n", connectionCnt, g_RECONNECT_TRY_CNT, TEST_CNT);
				Sleep(100);
				continue;
			}

			send(*pSocket, sndBuf, szPacket, 0);
			writeLog(g_LOG, sndBuf, szPacket, "snd(", ")");
		
			if (0 >= (recvLen = recv(*pSocket, rcvBuf, szPacket, 0)))
			{
				fprintf(stderr, "* ������ ��Ĺ ����, �翬�� �õ�. (%d/%d) (Record:%d)\n", connectionCnt, g_RECONNECT_TRY_CNT, TEST_CNT);
				Sleep(100);
				continue;
			}

			writeLog(g_LOG, rcvBuf, recvLen, "rcv(", ")");
			makeMsgFromPacket((char*)pstSvrRecord, sizeof(SvrRecord), rcvBuf, recvLen); /* 306 -> 300 */

			return closeSocket(pSocket);
		}
		
		return -1;
	}
	else if (option == 1) /* ���� ��Ĺ */
	{
		makePacketFromMsg(sndBuf, szBuf, (char*)pstSvrRecord, sizeof(SvrRecord)); /* 300 -> 306 */
		
		while (1)
		{
			while (send(*pSocket, sndBuf, szPacket, 0) == SOCKET_ERROR) /* ���� ���� �߻� (��Ĺ ������ �ȵưų� �������ų�...) */
			{
				if (connectionCnt == 0)
				{
					++connectionCnt;
					fprintf(stderr, "* ��Ĺ(%s:%d) ���� �õ�. (Record:%d)\n", g_SERVER_IP, serverPort, TEST_CNT);
					connectSocket(pSocket, g_SERVER_IP, serverPort);
					continue;
				}
				
				closeSocket(pSocket);
				
				do /* g_RECONNECT_TRY_CNTȸ �翬�� �õ� */
				{
					fprintf(stderr, "* ������ ��Ĺ ����, �翬�� �õ�. (%d/%d) (Record:%d)\n", connectionCnt, g_RECONNECT_TRY_CNT, TEST_CNT);
					Sleep(100);
				}
				while ((++connectionCnt <= g_RECONNECT_TRY_CNT) && (connectSocket(pSocket, g_SERVER_IP, serverPort) != 0));
				
				if (connectionCnt <= g_RECONNECT_TRY_CNT)
				{
					fprintf(stderr, "* ��Ĺ �翬�� ����. (�õ� Ƚ��: %d)\n", connectionCnt - 1);
					continue;
				}
				
				fprintf(stderr, "* ��Ĺ �翬�� ����. (�õ� Ƚ��: %d)\n", connectionCnt - 1);
				return -1;
			}
			
			writeLog(g_LOG, sndBuf, szPacket, "snd(", ")");
			memset(rcvBuf, 0, szBuf);
			
			if (recv(*pSocket, rcvBuf, szPacket, 0) == SOCKET_ERROR || rcvBuf[0] == 0)
			{
				fprintf(stderr, "* ����(Record:%d) �� ��Ĺ ����(Code:%d). ��Ĺ �ݰ� ������ �õ�.\n", TEST_CNT, WSAGetLastError());
				Sleep(100);
				connectionCnt = 0;
				closeSocket(pSocket);
				continue;
			}
			
			writeLog(g_LOG, rcvBuf, szPacket, "rcv(", ")");
			makeMsgFromPacket((char*)pstSvrRecord, sizeof(SvrRecord), rcvBuf, szPacket); /* 306 -> 300 */
			
			return 0; /* ���� �����ϴ� ���� ���̽� */
		}
	}
	
	return -1;
}

int connectSocket(SOCKET *pSocket, char *pIP, int port) {
	WSADATA stWsaData;
	SOCKADDR_IN stSvrAddr;
	
	if (WSAStartup(MAKEWORD(2, 2), &stWsaData) != 0) /* 2.2 version */
	{
		fprintf(stderr, "WSAStartup() ����.\n");
		return -1;
	}
	
	if ((*pSocket = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		fprintf(stderr, "��Ĺ ���� ����.\n");
		return -1;
	}
	
	memset(&stSvrAddr, 0, sizeof(stSvrAddr));
	stSvrAddr.sin_family = AF_INET;
	stSvrAddr.sin_addr.s_addr = inet_addr(pIP);
	stSvrAddr.sin_port = htons(port);
	
	if (connect(*pSocket, (SOCKADDR*)&stSvrAddr, sizeof(stSvrAddr)) == SOCKET_ERROR)
	{
		return -1;
	}
}

int closeSocket(SOCKET *pSocket) {
	closesocket(*pSocket);
	WSACleanup();
	return 0;
}

/*
 * [ Packet Format ]
 * > 000512345
 * > Size(4)/(1)/Data(0~1024)/(1)
 * > MinSz:6byte, MaxSz:1030, MinDataSz:0, MaxDataSz:1024
 */

char* makePacketFromMsg(char *pBuf, int szBuf, char *pMsg, int szMsg) {
	if (szBuf < szMsg)
	{
		fprintf(stderr, "pMsg/pBuf ũ�� ����. (szBuf:%d < szMsg:%d)\n", szBuf, szMsg);
		return NULL;
	}
	
	if (szMsg > DataMaxSz)
	{
		fprintf(stderr, "pMsg ũ�� ����. (szMsg:%d > %d)\n", szMsg, DataMaxSz);
		return NULL;
	}
	
	sprintf(pBuf, "%04d%c", szMsg + 2, STX); /* szMsg + strlen(STX + ETX) */
	memcpy(pBuf + 5, pMsg, min(szBuf, szMsg));
	pBuf[szMsg + 5] = ETX;
	
	return pBuf;
}

char* makeMsgFromPacket(char *pBuf, int szBuf, char *pPacket, int szPacket) {
	char arPacketSize[5] = { 0, };
	int szMsg = 0;
	
	if (szBuf + PacketMinSz < szPacket)
	{
		fprintf(stderr, "pMsg/szPacket ũ�� ����. (szBuf:%d < szPacket:%d)\n", szBuf, szPacket);
		return NULL;
	}

	strncpy(arPacketSize, pPacket, 4);
	szMsg = atoi(arPacketSize);
	memcpy(pBuf, pPacket + 5, min(szBuf, szMsg));
	
	return pBuf;
}