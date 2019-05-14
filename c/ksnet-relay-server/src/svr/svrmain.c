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
 * [ ���� �ذ� ���� ]
 * 1. �߰������� ���� Ŭ���̾�Ʈ - ���� ���� ������ ��� ������ �ִ�.
 *  1) Ŭ���̾�Ʈ ���� �ڵ�� ������ �ϴ� ��� - �������� �����͸� �������� �� �ش� ���� �ڵ��� �����ִ� ��� Ȥ�� Critical, OS���� �ش� ���� ��ȣ�� �����Ͽ� 
 *     �ٸ� Ŭ���̾�Ʈ�� �ش� ������ ����ϰ� �ȴٸ� �������� ���� �߻�.
 *  2) ���� ��ȣ�� ������ �ϴ� ��� - ���� �ٸ� Ŭ���̾�Ʈ�� ������ ���� ���� ��ȣ�� �����ٸ� �ùٸ� Ŭ���̾�Ʈ���� ������ �� ����.
 *  
 * => ���� ��ȣ�� ������ �����Ͽ� ���� ��ȣ�� Ŭ���̾�Ʈ�� �����ϴ� ����� ���� ������ ���δ�.
 * => ������ ������ �ҽǵǾ� ������� ���ϴ� Ŭ���̾�Ʈ �迭�� ���� ���� �ٸ� Ŭ���̾�Ʈ�� ���� ���� ��츦 ����ؼ�,
 *    Ŭ���̾�Ʈ ���Ͽ� Ÿ�Ӿƿ����� 2000ms ��� �� ����������� �Ѵ�.
 *
 */

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
	static int			msgId = 1;
	char				aryBuf[128];
	char				cliRecvBuf[512]; /* [SzBuf] */
	char				svrRecvBuf[512]; /* [SzBuf] */
	WSADATA				wsaData;
	SOCKET				tempCliSocket, tempSvrSocket, hostSocket, svrSocket[2]; /* [MaxServers] */
	TOSocket			cliSocket[32]; /* [MaxClients] */ /* TimeOut Socket */
	int					idx;
	struct sockaddr_in	svrAddr, hostAddr, cliAddr;
	int					i, j, selSocCnt, byteLen, cliRecvLen, svrRecvLen, errCode, curSessionIdx = 0;
	fd_set				readFds;
	struct timeval		selTimeout;
	long long			dwCurTime;
	
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
		cliSocket[i].hSoc = 0;
		cliSocket[i].dwConTime = 0;
		cliSocket[i].msgId = 0;
	}
	
	for (i = 0; i < MaxServers; ++i)
	{
		svrSocket[i] = 0;
	}
	
	fprintf(stderr, "cli/SvrSocket[%d/%d] �ʱ�ȭ �Ϸ�.\n", MaxClients, MaxServers);
	
	/* Select blocking timeout */
	selTimeout.tv_sec = 2;
    selTimeout.tv_usec = 0;
	fprintf(stderr, "selectTimeout �ʱ�ȭ �Ϸ�. (tv_sec:%ld, tv_usec:%ld)\n", selTimeout.tv_sec, selTimeout.tv_usec);
	
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
		dwCurTime = currentTimeMillis();
		
		for (i = 0; i < MaxClients; ++i)
		{
			tempCliSocket = cliSocket[i].hSoc;
			
			if (tempCliSocket > 0)
			{
				/* Check client TOSocket timeout */
				long long dwTimeDt = dwCurTime - (cliSocket[i].dwConTime + (long long)(g_CLI_SOC_TIMEOUT_DELAY));					
				
				if (dwTimeDt >= 0)
				{
					closesocket(cliSocket[i].hSoc);
					cliSocket[i].hSoc = 0;
					cliSocket[i].dwConTime = 0;
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) Ÿ�Ӿƿ����� ���������Ŵ. (%lldms)\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i, (long long)g_CLI_SOC_TIMEOUT_DELAY + dwTimeDt);
					continue;
				}

				FD_SET(tempCliSocket, &readFds);
			}			
		}
		
		/* Wait for and activity on any of the sockets, timeout is NULL, so wait indefinitely */
		/*fprintf(stderr, "select() �����...\n"); */
		
		if (SOCKET_ERROR == (selSocCnt = select(0, &readFds, NULL, NULL, &selTimeout)))
		{
			fprintf(stderr, "select() ����. (Code:%d)\n", WSAGetLastError());
			continue;
		}
		else if (0 == selSocCnt)
		{
			fprintf(stderr, "select() �����...\n");
			continue;
		}
		
		/* fprintf(stderr, "select() : %d\n", selSocCnt); */
		
		/* If something happened on the host socket, then its an incoming connection */
		if (FD_ISSET(hostSocket, &readFds))
		{
			--selSocCnt;
			
			if (0 > (tempCliSocket = accept(hostSocket, (struct sockaddr*)&cliAddr, (int*)&byteLen)))
			{
				fprintf(stderr, "accept() ����. (Client:%s:%d, Code:%d)\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), tempCliSocket);
				continue;
			}
			
			for (i = 0; i < MaxClients; ++i)
			{
				if (cliSocket[i].hSoc == 0)
				{
					getpeername(cliSocket[i].hSoc, (struct sockaddr*)&cliAddr, (int*)&byteLen);
					cliSocket[i].hSoc = tempCliSocket;
					cliSocket[i].dwConTime = currentTimeMillis();
					cliSocket[i].msgId = 0;
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d) ���ӽ�Ŵ. (%d/%d)\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i + 1, MaxClients);
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
			tempCliSocket = cliSocket[i].hSoc;
			
			/* If cli presend in read sockets */
			if (FD_ISSET(tempCliSocket, &readFds))
			{
				--selSocCnt;
				
				/* Get details of the client */
				getpeername(tempCliSocket, (struct sockaddr*)&cliAddr, (int*)&byteLen);
				
				/* Check if it was for closing, and also read the incoming msg
				   recv doesn't place a NULL terminator at the end of the string (whilst printf %s assumes there is one) */
				memset(cliRecvBuf, 0, SzBuf);
				   
				if (SOCKET_ERROR == (cliRecvLen = recv(tempCliSocket, cliRecvBuf, SzBuf, 0)))
				{
					errCode = WSAGetLastError();
					
					if (WSAECONNRESET == errCode)
					{
						/* Client disconnected. init client array, get socket detail and print */
						cliSocket[i].hSoc = 0;
						cliSocket[i].dwConTime = 0;
						fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ������ ���� ����.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
						continue;
					}
					
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) recv() ����. (Code:%d)\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i, errCode);
					continue;
				}
				else if (0 == cliRecvLen)
				{
					/* Client disconnected. init client array, get socket detail and print */
					cliSocket[i].hSoc = 0;
					cliSocket[i].dwConTime = 0;
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ���������� ���� ����.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), i);
					continue;
				}
				else
				{
					/* Cli recv logging */
					writeLog(g_LOG, cliRecvBuf, cliRecvLen, "rcvcli(", ")");
					
					/* Send data to server */
					tempSvrSocket = svrSocket[curSessionIdx];
					
					/* Add client msgId into send packet's {spare byte(10)} */
					memset(aryBuf, 0x00, sizeof(aryBuf));
					sprintf(aryBuf, "%010d", msgId++);
					memcpy(cliRecvBuf + 285, aryBuf, 10);
					cliSocket[i].msgId = atoi(aryBuf);
					
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
			
			/* If svr presend in read sockets */
			if (FD_ISSET(tempSvrSocket, &readFds))
			{
				--selSocCnt;
				
				memset(svrRecvBuf, 0, SzBuf);
				
				/* ũ��Ƽ�� �̽� �߰� �� �ӽ��ذ�! */
				/* ����, ���� �������� 306byte �̻����� �����͸� ���ƺ��� ���� �ֱ� ������, �ϴ��� 306�� �е��� �����ؾ� �Ѵ�.		*/
				/* ���� �������� �ذ����� ���ؼ���, ���� �����͸� �ӽú����� ��Ҹ� �����, ��Ŷ�� '�ϼ�'���� ���������� �����ؾ� ��.		*/
				/* ����, �α� ������ ũ�Ⱑ 512�� �ۼ��Ǿ� ������ �������!                         						*/
				
				/* [�ذ��] */
				/* 1. 4byte(���̺�) ��ŭ �о��																			*/
				/* 2. 4byte���� ª���� ���� ��ȯ, 4byte�� �о����� ������ ��ȯ�Ͽ� �о�� �� ���̷� ����Ѵ�								*/
				/* 3. �о�� �� ���̸�ŭ FD_SET�� �Ϸ��� ������ �ݺ��Ͽ� �д´�. �о�� �� ���� - ���� ���� �Ͽ� 0�� �ǰų� Ÿ�Ӿƿ�(0.1s) �ɶ����� �ݺ��Ѵ�.	*/
				
				/* if (SOCKET_ERROR == (svrRecvLen = recv(tempSvrSocket, svrRecvBuf, 306, 0))) - Old version */
				if (SOCKET_ERROR == (svrRecvLen = recv(tempSvrSocket, svrRecvBuf, 4, 0)))
				{
					errCode = WSAGetLastError();
					
					if (errCode == WSAECONNRESET)
					{
						svrSocket[i] = 0;
						fprintf(stderr, "����(%s:%d)�� ������ ������ �����. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
						writeLog(g_LOG, NULL, 0, "rcvsvr(socket closed by error", ")");
						continue;
					}
				}
				else if (0 == svrRecvLen)
				{
					svrSocket[i] = 0;
					fprintf(stderr, "����(%s:%d)���� ���� �����Ŵ. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
					writeLog(g_LOG, NULL, 0, "rcvsvr(socket closed by server", ")");
					continue;
				}
				else
				{
					int restLen, readLen;
					fd_set locFdSet;
					struct timeval loctv;
					long long locTimeout;
					
					/* 4byte ������ ���̸� ��� */
					if (svrRecvLen != 4)
					{
						writeLog(g_LOG, NULL, 0, "rcvsvr(fail to read 4byte of header", ")");
						continue;
					}
					
					svrRecvBuf[svrRecvLen] = '\0';
					restLen = atoi(svrRecvBuf);
					readLen = 4;
					loctv.tv_sec = 0;
					loctv.tv_usec = 100000; /* 0.1�� */
					locTimeout = currentTimeMillis() + 100LL; /* 0.1�� */
					
					/* �����͸� �ٽ� ���, ���� ���̸� �� �е��� �� */
					while (1)
					{
						if (restLen <= 0) break;
						
						FD_ZERO(&locFdSet);
						FD_SET(tempSvrSocket, &locFdSet);
						
						if (select(0, &locFdSet, NULL, NULL, &loctv) == 1 && FD_ISSET(tempSvrSocket, &locFdSet))
						{
							if (SOCKET_ERROR == (readLen = recv(tempSvrSocket, svrRecvBuf + readLen, restLen, 0)))
							{
								errCode = WSAGetLastError();
								
								if (errCode == WSAECONNRESET)
								{
									svrSocket[i] = 0;
									fprintf(stderr, "����(%s:%d)�� ������ ������ �����. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
									writeLog(g_LOG, NULL, 0, "rcvsvr(socket closed by error", ")");
									break;
								}
							}
							else if (0 == readLen)
							{
								svrSocket[i] = 0;
								fprintf(stderr, "����(%s:%d)���� ���� �����Ŵ. (Session:%d)\n", inet_ntoa(svrAddr.sin_addr), ntohs(svrAddr.sin_port), curSessionIdx);
								writeLog(g_LOG, NULL, 0, "rcvsvr(socket closed by server", ")");
								break;
							}
							else
							{
								restLen -= readLen;
								svrRecvLen += readLen;
							}
						}
						
						if (currentTimeMillis() > locTimeout) break;
					}
					
					if (restLen != 0) continue; /* 0.1�ʸ� �� �־������� �� ���� ���� ����̰ų� �� ���� ���� ���� ���� */
					
					/* Svr recv logging */
					writeLog(g_LOG, svrRecvBuf, svrRecvLen, "rcvsvr(", ")");
					
					/* Send to client */
					memset(aryBuf, 0x00, sizeof(aryBuf));	/* ���񿡼� msgId ���� */
					memcpy(aryBuf, svrRecvBuf + 285, 10);
					memset(svrRecvBuf + 285, ' ', 10);		/* ���� ���鹮�� ���� */
					idx = atoi(aryBuf);
					tempCliSocket = 0;
					
					for (j = 0; j < MaxClients; ++j)
					{
						if (cliSocket[j].msgId == idx)
						{
							tempCliSocket = cliSocket[j].hSoc;
							break;
						}
					}
					
					if (tempCliSocket == 0) continue; /* ��ǥ Ŭ���̾�Ʈ ������ ����� ����, ���� ���� */
					
					send(tempCliSocket, svrRecvBuf, svrRecvLen, 0);
					
					/* Remove client */
					closesocket(tempCliSocket);
					clrCliSocketByIdx(j, cliSocket);
					
					fprintf(stderr, "Ŭ���̾�Ʈ(%s:%d:%d) ���� �����Ŵ.\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), idx);
					
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
		if (cliSocket[i].hSoc != 0)
		{
			closesocket(cliSocket[i].hSoc);
			cliSocket[i].hSoc = 0;
			cliSocket[i].dwConTime = 0;
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

int clrCliSocket(SOCKET hSocket, TOSocket *pSocket, int length) {
	int i = 0;
	
	for (i = 0; i < length; ++i)
	{
		if (pSocket[i].hSoc == hSocket)
		{
			memset(&pSocket[i], 0x00, sizeof(TOSocket));
			return 0;
		}
	}
	
	return -1;
}

int clrCliSocketByIdx(int index, TOSocket *pSocket)
{
	memset(&pSocket[index], 0x00, sizeof(TOSocket));
	return 0;
}