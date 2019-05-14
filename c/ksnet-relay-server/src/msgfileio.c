#include "msgfileio.h"
#include "commonlib.h"

FILE* openMsgFile(char *pFileName, char *pOption) {
	FILE *pMsgFile = NULL;
	
	if ((pMsgFile = fopen(pFileName, pOption)) == NULL)
	{
		fprintf(stderr, "����(%s) I/O�۾��� �����߽��ϴ�. (pMsgFile == NULL)\n", pFileName);
		return NULL;
	}
	
	return pMsgFile;
}

int readMsgLine(FILE *pFile, char *pOutLineBuf, int outBufSize) {
	if (pOutLineBuf == NULL)
	{
		fprintf(stderr, "�ùٸ��� ���� �迭. (pOutLineBuf == NULL)\n");
		return -1;
	}
	else if (pFile == NULL)
	{
		fprintf(stderr, "������ �������� �ʽ��ϴ�. (pFile == NULL)\n");
		return -1;
	}
	
	if (fgets(pOutLineBuf, outBufSize, pFile) == NULL) /* EOF */
	{
		return 0;
	}
	
	return strlen(pOutLineBuf) + 1;
}

int writeMsg(FILE *pFile, char *pData, int lineAtFile, int useNewLine) {
	int dataLen = -1;
	int loopI = 0;
	
	dataLen = strlen(pData);
	
	if (pData == NULL)
	{
		fprintf(stderr, "�ùٸ��� ���� ������. (pData == NULL)\n");
		return -1;
		
	}
	else if (pFile == NULL)
	{
		fprintf(stderr, "������ �������� �ʽ��ϴ�. (pFile == NULL)\n");
		return -1;
	}

#ifdef __WINDOWS__
	/* '\r'���ڸ� ��¿��� ���� */
	for (loopI = dataLen; loopI > -1; --loopI)
	{
		if (pData[loopI] == '\r')
		{
			pData[loopI] = '\n';
			pData[dataLen - 1] = '\0';
			--dataLen;
			break;
		}
	}
#endif

	if (lineAtFile != -1)
	{
		fseek(pFile, dataLen * lineAtFile, SEEK_SET);
	}
	
	if (useNewLine == 1)
	{
		fputs(pData, pFile);
		fputc('\n', pFile);
	}
	else
	{
		fputs(pData, pFile);
	}
	
	return 0;
}

int closeMsgFile(FILE *pFile) {
	if (pFile == NULL)
	{
		fprintf(stderr, "������ �������� �ʽ��ϴ�. (pFile == NULL)\n");
		return -1;
	}
	
	return fclose(pFile);
}

int writeLog(FILE *pFile, char *pData, int szData, char *pPrefixStr, char *pSuffixStr) {
	char arBuf[512] = { 0, };
	char arDataBuf[512] = { 0, };
	char pTime[7] = { 0, };
	
	getTime(pTime, 2);
	byteToString(arDataBuf, pData, szData);
	sprintf(arBuf, "%s:%s%s%s", pTime, pPrefixStr, arDataBuf, pSuffixStr); /* hhmmss */

	return writeMsg(pFile, arBuf, -1, 1);
}