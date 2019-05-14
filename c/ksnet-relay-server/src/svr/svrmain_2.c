#include "svrmain.h"
#include "stdheader.h"
#include "svrglobal.h"
#include "svrsocket.h"
#include "msgfileio.h"
#include "commonlib.h"

/*
 * [ Workflow ]
 * 1. ���α׷� ���� �� �ʱ�ȭ.
 * 2. �߰��������� ���μ����� ���� �ΰ� ����. (Select ���)
 * 3. Ŭ���̾�Ʈ���� �߰������� ������ ����.	----------------��
 * 4. ������ �����ư��� ���μ����� ����.					| -> ������� ȸ��(Record ����)��ŭ �ݺ�
 * 5. ������� ����� Ŭ���̾�Ʈ���� ������.	----------------��
 *    + ������ �������� ���� ����, Ŭ���̾�Ʈ ���۴�� ���� �ݾƼ� Ŭ�� ���� �˸�.
 * 6. �ۼ��� �α� �ۼ�
 *
 */
 
#define MAX_CLIENTS 32
#define MAX_SERVERS 2
#define MAX_BUFFER_SZ 2048

typedef struct _KsSocket 		/* Ÿ�Ӿƿ� ���� */
{
	SOCKET hSocket;				/* ���� �ڵ� */
	struct sockaddr_in addr;	/* ���� �ּ� */
	long long dwConTime;		/* ���� accept���� �ð� */
	char rcvBuf[MAX_BUFFER_SZ];	/* ���� ������ ���� */
	
} KsSocket;

typedef struct _RelaySvr
{
	WSADATA				wsaData;
	KsSocket			hostSocket;
	KsSocket			cliSockets[MAX_CLIENTS], svrSockets[MAX_SERVERS];
	struct timeval		selTimeout;
	struct sockaddr_in	hostAddr, cliAddr, svrAddr;
	fd_set				fdSet;
	
} RelaySvr;

static RelaySvr g_relaySvr;

int checkArgc(int argc);
int serverWork();
int initServer(RelaySvr *relaySvr);
void initKsSocket(KsSocket *ksSocket);
int selectorWork();
int recvAndWork(int *selSocCnt);
int acceptClient();
int closeClient();

int main(int argc, char** argv) {
	int errorCode = 0;

	if (0 != checkArgc(argc))
	{
		fprintf(stderr, "[ERR] .bat���Ϸ� ������Ѿ� �մϴ�.\n");
		return -1;
	}
	
	if (0 != initGlobalEnvs(argc, argv))
	{
		fprintf(stderr, "[ERR] ���� ȯ�溯�� ���� ����.\n");
		return -1;
	}
	
	if (0 != (errorCode = serverWork()))
	{
		fprintf(stderr, "[ERR] ���� ���� �߻�. (Code:%d)\n", errorCode);
		return -1;
	}
	
	return freeGlobalEnvs();
}

int checkArgc(int argc) {
	if (1 >= argc)
	{
		return -1;
	}
	
	return 0;
}

int serverWork() {
	int		selSocCnt, errCode;
	
	fprintf(stdout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	fprintf(stdout, "* �߰����� ���� ����...\n");
	
	/* Server initialization */
	if ((errCode = initServer(&g_relaySvr)) != 0)
	{
		fprintf(stderr, "[ERR] �ʱ�ȭ ���� �߻�. (Code:%d)\n");
		return errCode;
	}
	
	fprintf(stderr, "���� ���� ����.\n");
	
	/* Main loop */
	while (1)
	{
		/* Selector work */
		if ((selSocCnt = selectorWork()) <= 0)
		{
			fprintf(stderr, "* select() Ÿ�Ӿƿ�/���� ��ȯ(%d).\n", selSocCnt);
			continue;
		}
		
		/* Receive and work */
		recvAndWork(&selSocCnt);
	}
	
	return 0;
}

int initServer(RelaySvr *relaySvr) {
	int		i, errCode;
	
	fprintf(stderr, "���� �ʱ�ȭ ����.\n");
	
	/* Log */
	g_LOG = openMsgFile(g_OUT_LOG_FILE_PATH, "w");
	fprintf(stderr, "�α� ���� �Ϸ�.\n");
	
	/* Variables */
	initKsSocket(&relaySvr->hostSocket);
	
	for (i = 0; i < MAX_CLIENTS; ++i)
	{
		initKsSocket(&relaySvr->cliSockets[i]);
	}
	
	for (i = 0; i < MAX_SERVERS; ++i)
	{
		initKsSocket(&relaySvr->svrSockets[i]);
	}
	
	fprintf(stderr, "cli/svrSockets[%d/%d] �ʱ�ȭ �Ϸ�.\n", MAX_CLIENTS, MAX_SERVERS);
	
	/* Select blocking timeout */
	relaySvr->selTimeout.tv_sec = 2;
    relaySvr->selTimeout.tv_usec = 0;
	fprintf(stderr, "selectTimeout �ʱ�ȭ �Ϸ�. (tv_sec:%ld, tv_usec:%ld)\n",
			relaySvr->selTimeout.tv_sec, relaySvr->selTimeout.tv_usec);
	
	/* Winsock */
	if (WSAStartup(MAKEWORD(2, 2), &relaySvr->wsaData) != 0)
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "WSAStartup() ����. (WSACode:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "WSAStartup() �Ϸ�.\n");
	
	/* Host socket */	
	relaySvr->svrAddr.sin_family = AF_INET;
	relaySvr->svrAddr.sin_addr.s_addr = inet_addr(g_SERVER_IP);
	relaySvr->svrAddr.sin_port = htons(atoi(g_SERVER_PORT));

	if (((relaySvr->hostSocket).hSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "socket() ���� ����. (WSACode:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "socket() ���� �Ϸ�.\n");
	
	/* Binding */
	relaySvr->hostAddr.sin_family = AF_INET;
	relaySvr->hostAddr.sin_addr.s_addr = INADDR_ANY;
	relaySvr->hostAddr.sin_port = htons(atoi(g_RELAY_PORT));
	
	if (bind(relaySvr->hostSocket.hSocket, (struct sockaddr*)&(relaySvr->hostAddr),
		sizeof(relaySvr->hostAddr)) == SOCKET_ERROR)
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "bind() ����. (WSACode:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "bind() �Ϸ�. (%s:%s)\n", INADDR_ANY, g_RELAY_PORT);
	
	/* Listen */
	if (listen(relaySvr->hostSocket.hSocket, 32) == SOCKET_ERROR)
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "listen() ����. (WSACode:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "listen() �Ϸ�.\n");
	fprintf(stderr, "���� �ʱ�ȭ �Ϸ�.\n");
	
	return 0;
}

void initKsSocket(KsSocket *ksSocket) {
	memset(ksSocket, 0x00, sizeof(KsSocket));
}

int selectorWork() {
	SOCKET		tempSocket;
	long long	dwCurTime, dwTimeoutTime;
	int			i, selSocCnt, errCode;
	
	/* Init FD_SET */
	FD_ZERO(&g_relaySvr.fdSet);
	
	/* Set host socket to FD_SET */
	FD_SET(g_relaySvr.hostSocket.hSocket, &g_relaySvr.fdSet);
	
	/* Set server sockets to FD_SET */
	for (i = 0; i < MAX_SERVERS; ++i)
	{
		tempSocket = g_relaySvr.svrSockets[i].hSocket;
		
		if (tempSocket == 0) continue;
		
		FD_SET(tempSocket, &g_relaySvr.fdSet);
	}
	
	/* Set client sockets to FD_SET and check timeout */
	dwCurTime = currentTimeMillis();
	
	for (i = 0; i < MAX_CLIENTS; ++i)
	{
		tempSocket = g_relaySvr.cliSockets[i].hSocket;
		
		if (tempSocket == 0) continue;
		
		/* Check client timeout */
		dwTimeoutTime = g_relaySvr.cliSockets[i].dwConTime +
						(long long)g_CLI_SOC_TIMEOUT_DELAY;
		
		if (dwCurTime > dwTimeoutTime)
		{
			closeClient(tempSocket);
			continue;
		}
		
		FD_SET(tempSocket, &g_relaySvr.fdSet);
	}
	
	/* Select read socket and return count of selected sockets */
	if ((selSocCnt = select(0, &g_relaySvr.fdSet, 0, 0, &g_relaySvr.selTimeout)) == SOCKET_ERROR)
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "select() ���� �߻�. (WSACode:%d)\n", errCode);
		return errCode;
	}
	
	return selSocCnt;
}

int recvAndWork(int *selSocCnt) {
	struct sockaddr_in cliAddr;
	int		i, errCode;
	
	if (*selSocCnt <= 0) return 0;
	
	/* Accept client */
	if (FD_ISSET(g_relaySvr.hostSocket.hSocket, &g_relaySvr.fdSet))
	{
		--(*selSocCnt);
		
		if ((errCode = acceptClient()) < 0) return errCode;
	}
	
	/* Recv from client */
	for (i = 0; i < MAX_CLIENTS; ++i)
	{
		
	}
	
	/* Recv from server */
	for (i = 0 ; i < MAX_SERVERS; ++i)
	{
		
	}
}

int acceptClient() {
	SOCKET				hSocket;
	struct sockaddr_in	cliAddr;
	int					i, length, errCode;
	
	if ((hSocket = accept(g_relaySvr.hostSocket.hSocket, (struct sockaddr*)&cliAddr, (int*)&length)) < 0)
	{
		fprintf(stderr, "[ERR] accept() ����. (Client:%s:%d, WSACode:%d)\n", inet_ntoa(cliAddr.sin_addr),
				ntohs(cliAddr.sin_port), WSAGetLastError());
		errCode = hSocket;
		return errCode;
	}
	
	for (i = 0; i < MAX_CLIENTS; ++i)
	{
		if (g_relaySvr.cliSockets[i].hSocket == 0)
		{
			g_relaySvr.cliSockets[i].hSocket 	= hSocket;
			memcpy(&g_relaySvr.cliSockets[i].addr, &cliAddr, sizeof(struct sockaddr));
			g_relaySvr.cliSockets[i].dwConTime	= currentTimeMillis();
			fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ���ӽ�Ŵ.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
			return hSocket;
		}
	}
	
	if (i == MAX_CLIENTS)
	{
		fprintf(stderr, "[ERR] acceptClient() Ŭ���̾�Ʈ�� ����á���ϴ�. (%d/%d)\n", i, MAX_CLIENTS);
		return -1;
	}
	
	return -1;
}

int closeClient(SOCKET hSocket) {
	struct sockaddr_in	*cliAddr;
	int					i, errCode;
	
	if (hSocket < 0) return -1;
	
	for (i = 0; i < MAX_CLIENTS; ++i)
	{
		if (g_relaySvr.cliSockets[i].hSocket == hSocket)
		{
			cliAddr = &g_relaySvr.cliSockets[i].addr;
			fprintf(stderr, "Ŭ���̾�Ʈ(IP[%s:%d]:Idx[%d]) ���� �����Ŵ.\n", inet_ntoa(cliAddr->sin_addr),
					ntohs(cliAddr->sin_port), i);
			closesocket(hSocket);
			initKsSocket(&g_relaySvr.cliSockets[i]);			
			return i;
		}
	}
	
	return MAX_CLIENTS;
}