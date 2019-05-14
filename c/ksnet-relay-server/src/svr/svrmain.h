#ifndef __SVRMAIN_H__
#define __SVRMAIN_H__

#include "stdheader.h"

typedef struct _TOSocket {
	SOCKET hSoc;			/* ���� �ڵ� */
	long long dwConTime;	/* ���ӽ��� �ð� */
	int msgId;				/* �޽��� ���� ��ȣ */
} TOSocket;

int main(int argc, char** argv);
int checkArgc(int argc);
int serverWork();
int clrCliSocket(SOCKET hSocket, TOSocket *pSocket, int length);
int clrCliSocketByIdx(int index, TOSocket *pSocket);

#endif