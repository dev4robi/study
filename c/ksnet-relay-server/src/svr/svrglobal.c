#include "svrglobal.h"

char *g_SERVER_IP;		
char *g_SERVER_PORT;		
char *g_RELAY_PORT;		
char *g_OUT_LOG_FILE_PATH;
int g_CLI_SOC_TIMEOUT_DELAY;

FILE *g_LOG;

int setStrEnv(char *pGvarName, char **pGvar, char *pData) {
	int szData = 0;
	
	if (pData == NULL)
	{
		fprintf(stderr, "����! '%s'�� null�� �����Ϸ� �մϴ�. (pData == NULL)\n", pGvarName);
		*pGvar = NULL;
		return 0;
	}
	
	szData = strlen(pData) + 1;		/* +1 : null('\0')���� ���� */
	*pGvar = (char*)malloc(szData);
	memcpy(*pGvar, pData, szData);
	
	return 0;
}

int setIntEnv(char *pGvarName, int *pGvar, int data) {
	if (pGvar == NULL)
	{
		fprintf(stderr, "����. (pGvar == NULL)\n");
		return -1;
	}
	
	*pGvar = data;
	
	return 0;	
}

int initGlobalEnvs(int argc, char **argv) {
	setStrEnv("g_SERVER_IP",			&g_SERVER_IP,			argv[1]);
	setStrEnv("g_SERVER_PORT",			&g_SERVER_PORT,			argv[2]);
	setStrEnv("g_RELAY_PORT",			&g_RELAY_PORT,			argv[3]);
	setStrEnv("g_OUT_LOG_FILE_PATH",	&g_OUT_LOG_FILE_PATH,	argv[5]);

	/* ----------------------------------------------------------------*/
	
	setIntEnv("g_CLI_SOC_TIMEOUT_DELAY", &g_CLI_SOC_TIMEOUT_DELAY, atoi(argv[4]));
	
	return 0;
}

int freeGlobalEnvs() {
	free(g_SERVER_IP);
	free(g_SERVER_PORT);
	free(g_RELAY_PORT);
	free(g_OUT_LOG_FILE_PATH);
	
	g_SERVER_IP = NULL;
	g_SERVER_PORT = NULL;
	g_RELAY_PORT = NULL;
	g_OUT_LOG_FILE_PATH = NULL;
	
	/* ----------------------------------------------------------------*/
	
	g_CLI_SOC_TIMEOUT_DELAY = 0;
	
	return 0;
}