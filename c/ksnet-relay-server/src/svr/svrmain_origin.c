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
 
int clrSocket(SOCKET hSocket, SOCKET *pSocket, int length);

int main(int argc, char** argv) {
	int errorCode = 0;

	if (0 != checkArgc(argc))
	{
		fprintf(stderr, ".bat���Ϸ� ������Ѿ� �մϴ�.\n");
		return -1;
	}
	
	if (0 != initGlobalEnvs(argc, argv))
	{
		fprintf(stderr, "���� ȯ�溯�� ���� ����.\n");
		return -1;
	}
	
	if (0 != (errorCode = serverWork()))
	{
		fprintf(stderr, "���� ���� �߻�. (Code:%d)\n", errorCode);
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
	int					MaxClients = FD_SETSIZE / 2, MaxServers = 2, SzBuf = 512; /* FD_SETSIZE(64) */
	static int			msgId = 0;
	char				cliRecvBuf[512]; /* SzBuf */
	char				svrRecvBuf[512]; /* SzBuf */
	WSADATA				wsaData;
	SOCKET				tempCliSocket, tempSvrSocket, hostSocket, svrSocket[2], /* MaxServers */ cliSocket[32]; /* MaxClients */
	SOCKET				cliSvrMapping[2]; /* MaxServers */
	struct sockaddr_in	svrAddr, hostAddr, cliAddr;
	int					i, j, selSocCnt, byteLen, cliRecvLen, svrRecvLen, errCode, curSessionIdx = 0;
	fd_set				readFds;
	
	fprintf(stdout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	fprintf(stdout, "* ���� ���� ����...\n");
	
	/* Make log file */
	g_LOG = openMsgFile(g_OUT_LOG_FILE_PATH, "w");
	fprintf(stderr, "�α� ���� �Ϸ�.\n");
	
	/* Init */
	fprintf(stderr, "�ʱ�ȭ ����.\n");
	
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.s_addr = inet_addr(g_SERVER_IP);
	svrAddr.sin_port = htons(atoi(g_SERVER_PORT));
	
	fprintf(stderr, "svrAddr �ʱ�ȭ �Ϸ�. (%s:%s)\n", g_SERVER_IP, g_SERVER_PORT);
	
	for (i = 0; i < MaxClients; ++i)
	{
		cliSocket[i] = 0;
	}
	
	for (i = 0; i < MaxServers; ++i)
	{
		svrSocket[i] = 0;
		cliSvrMapping[i] = 0;
	}
	
	fprintf(stderr, "cli/SvrSocket[%d/%d] �ʱ�ȭ �Ϸ�.\n", MaxClients, MaxServers);
	
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "WSAStartup() ����. (Code:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "WSAStartup() �Ϸ�.\n");
	
	/* Create socket */
	if (INVALID_SOCKET == (hostSocket = socket(AF_INET, SOCK_STREAM, 0)))
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "socket() ���� ����. (Code:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "socket() ���� �Ϸ�.\n");
	
	/* Binding */
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_addr.s_addr = INADDR_ANY;
	hostAddr.sin_port = htons(atoi(g_RELAY_PORT));
	
	if (SOCKET_ERROR == bind(hostSocket, (struct sockaddr*)&hostAddr, sizeof(hostAddr)))
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "bind() ����. (Code:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "bind() �Ϸ�. (%s:%s)\n", INADDR_ANY, g_RELAY_PORT);
	
	/* Listen */
	if (SOCKET_ERROR == listen(hostSocket, 8))
	{
		errCode = WSAGetLastError();
		fprintf(stderr, "listen() ����. (Code:%d)\n", errCode);
		return errCode;
	}
	
	fprintf(stderr, "listen() �Ϸ�.\n");
	fprintf(stderr, "���� ���� ����.\n");
	
	while (1)
	{
		/* Clear the socket fd set */
		FD_ZERO(&readFds);
			
		/* Add host and server socket to fd set */
		FD_SET(hostSocket, &readFds);
		
		for (i = 0; i < MaxServers; ++i)
		{
			if (svrSocket[i] != 0)
			{
				FD_SET(svrSocket[i], &readFds);
			}
		}
		
		/* Add client sockets to fd set */
		for (i = 0; i < MaxClients; ++i)
		{
			tempCliSocket = cliSocket[i];
			
			if (tempCliSocket > 0)
			{
				FD_SET(tempCliSocket, &readFds);
			}
		}
		
		/* Wait for and activity on any of the sockets, timeout is NULL, so wait indefinitely */
		/*fprintf(stderr, "select() �����...\n"); */
		
		if (SOCKET_ERROR == (selSocCnt = select(0, &readFds, NULL, NULL, NULL)))
		{
			fprintf(stderr, "select() ����. (Code:%d)\n", WSAGetLastError());
			continue;
		}
		else if (0 == selSocCnt)
		{
			continue;
		}
		
		/* fprintf(stderr, "select() : %d\n", selSocCnt); */
		
		/* If something happened on the host socket, then its an incoming connection */
		if (FD_ISSET(hostSocket, &readFds))
		{
			if (0 > (tempCliSocket = accept(hostSocket, (struct sockaddr*)&cliAddr, (int*)&byteLen)))
			{
				fprintf(stderr, "accept() ����. (Client:%s:%d, Code:%d)\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), tempCliSocket);
				continue;
			}
			
			for (i = 0; i < MaxClients; ++i)
			{
				if (cliSocket[i] == 0)
				{
					cliSocket[i] = tempCliSocket;
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ���ӽ�Ŵ.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
					break;
				}
			}
			
			if (MaxClients == i)
			{
				closesocket(tempCliSocket);
				fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ���� �����Ŵ. (�ִ� Ŭ���̾�Ʈ �� ����)\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
			}
		}
		
		/* Read operation on client socket */
		for (i = 0; i < MaxClients; ++i)
		{
			tempCliSocket = cliSocket[i];
			
			/* If cli presend in read sockets */
			if (FD_ISSET(tempCliSocket, &readFds))
			{
				--selSocCnt;
				
				/* Check svr mapping is empty */
				if (cliSvrMapping[curSessionIdx] != 0)
				{
					continue; /* If cliSvrMapping is full, try next time */
				}
				
				/* Get details of the client */
				getpeername(tempCliSocket, (struct sockaddr*)&cliAddr, (int*)&byteLen);
				
				/* Check if it was for closing, and also read the incoming msg
				   recv doesn't place a NULL terminator at the end of the string (whilst printf %s assumes there is one) */
				if (SOCKET_ERROR == (cliRecvLen = recv(tempCliSocket, cliRecvBuf, SzBuf, 0)))
				{
					errCode = WSAGetLastError();
					
					if (WSAECONNRESET == errCode)
					{
						/* Client disconnected. init client array, get socket detail and print */
						cliSocket[i] = 0;
						fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ������ ���� ����.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
						continue;
					}
					
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) recv() ����. (Code:%d)\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i, errCode);
					continue;
				}
				else if (0 == cliRecvLen)
				{
					/* Client disconnected. init client array, get socket detail and print */
					cliSocket[i] = 0;
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ���������� ���� ����.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
					continue;
				}
				else
				{
					/* Cli recv logging */
					writeLog(g_LOG, cliRecvBuf, cliRecvLen, "rcvcli(", ")");
					
					/* Send data to server */
					tempSvrSocket = svrSocket[curSessionIdx];
					
					/* Mapping curSessionIdx and cliSocket */
					cliSvrMapping[curSessionIdx] = tempCliSocket;
					
					while (SOCKET_ERROR == send(tempSvrSocket, cliRecvBuf, cliRecvLen, 0))
					{
						fprintf(stderr, "���� ��Ĺ(%s:%d) ����� �õ�. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
						
						while (INVALID_SOCKET == (tempSvrSocket = socket(PF_INET, SOCK_STREAM, 0)))
						{
							fprintf(stderr, "���� ��Ĺ(%s:%d) ���� ����. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
							Sleep(100);
						}
						
						while (SOCKET_ERROR == connect(tempSvrSocket, (struct sockaddr*)&svrAddr, sizeof(svrAddr)))
						{
							fprintf(stderr, "����(%s:%d) ���� ����. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
							Sleep(100);
						}
						
						svrSocket[curSessionIdx] = tempSvrSocket;
						fprintf(stderr, "���� ��Ĺ(%s:%d) ����� ����. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
					}
					
					/* Svr send logging */
					writeLog(g_LOG, cliRecvBuf, cliRecvLen, "sndsvr(", ")");
					
					/* Set next session */
					curSessionIdx = (++curSessionIdx) % MaxServers;
				}
			}
			
			if (selSocCnt <= 0) break;
		}
		
		/* Read operation on Server socket */
		for (i = 0; i < MaxServers; ++i)
		{
			tempSvrSocket = svrSocket[i];
			tempCliSocket = cliSvrMapping[i];
			
			/* If svr presend in read sockets */
			if (FD_ISSET(tempSvrSocket, &readFds))
			{
				--selSocCnt;
				
				if (SOCKET_ERROR == (svrRecvLen = recv(tempSvrSocket, svrRecvBuf, SzBuf, 0)))
				{
					errCode = WSAGetLastError();
					
					if (errCode == WSAECONNRESET)
					{
						closesocket(tempCliSocket);
						clrSocket(tempCliSocket, cliSocket, MaxClients);
						svrSocket[i] = 0;
						cliSvrMapping[i] = 0;
						fprintf(stderr, "����(%s:%d)�� ������ ������ �����. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
						writeLog(g_LOG, NULL, 0, "rcvsvr(socket closed by error", ")");
						continue;
					}
				}
				else if (0 == svrRecvLen)
				{
					closesocket(tempCliSocket);
					clrSocket(tempCliSocket, cliSocket, MaxClients);
					svrSocket[i] = 0;
					cliSvrMapping[i] = 0;
					fprintf(stderr, "����(%s:%d)���� ���� �����Ŵ. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
					writeLog(g_LOG, NULL, 0, "rcvsvr(socket closed by server", ")");
					continue;
				}
				else
				{
					/* Get cliSocket from mapping array and initialize */
					cliSvrMapping[i] = 0;
					
					/* Svr recv logging */
					writeLog(g_LOG, svrRecvBuf, svrRecvLen, "rcvsvr(", ")");
					
					/* Send to client */
					send(tempCliSocket, svrRecvBuf, svrRecvLen, 0);
					
					/* Remove client */
					closesocket(tempCliSocket);
					clrSocket(tempCliSocket, cliSocket, MaxClients);
					
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ���� �����Ŵ.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
					
					/* Cli send logging */
					writeLog(g_LOG, svrRecvBuf, svrRecvLen, "clisnd(", ")");
				}
			}
			
			if (selSocCnt <= 0) break;
		}
	}
	
	/* Close client sockets */
	for (i = 0; i < MaxClients; ++i)
	{
		if (cliSocket[i] != 0)
		{
			closesocket(cliSocket[i]);
			cliSocket[i] = 0;
		}
	}
	
	/* Close server sockets */
	for (i = 0; i < MaxServers; ++i)
	{
		if (svrSocket[i] != 0)
		{
			closesocket(svrSocket[i]);
			svrSocket[i] = 0;
		}
	}
	
	/* Close host socket */
	closesocket(hostSocket);
	WSACleanup();
	
	fprintf(stdout, "* ���� ���� ����.\n");
	fprintf(stdout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	
	return 0;
}

int clrSocket(SOCKET hSocket, SOCKET *pSocket, int length) {
	int i = 0;
	
	for (i = 0; i < length; ++i)
	{
		if (pSocket[i] == hSocket)
		{
			pSocket[i] = 0;
			return 0;
		}
	}
	
	return -1;
}