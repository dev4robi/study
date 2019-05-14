#include "clirecord.h"
#include "cliglobal.h"
#include "commonlib.h"

/*
 * Client Attributes
 */
Attribute g_astCliHeadAttrs[] = { /* attrId : 1 */
	{ "idCode",				"�ĺ��ڵ�",		"X",  0,  1, "S" },
	{ "taskComp",			"��������",		"X",  1,  2, "10" },
	{ "bankCode",			"�����ڵ�",		"9",  3,  3, "081" },
	{ "companyCode",		"��ü�ڵ�",		"X",  6,  8, "KSANP001" },
	{ "comissioningDate",	"��ü�Ƿ�����",		"9", 14,  6, "180404" },
	{ "processingDate",		"��üó������",		"9", 20,  6, NULL },
	{ "motherAccountNum",	"����¹�ȣ",		"9", 26, 14, "25791005094404" },
	{ "transferType",		"��ü����",		"9", 40,  2, "51" },
	{ "companyNum",			"ȸ���ȣ",		"9", 42,  6, "000000" },
	{ "resultNotifyType",	"ó������뺸����",	"X", 48,  1, "1" },
	{ "transferCnt",		"��������",		"X", 49,  1, "1" },
	{ "password",			"��й�ȣ",		"X", 50,  8, "4380" },
	{ "blank",				"����",			"X", 58, 19, NULL },
	{ "format",				"Format",		"X", 77,  1, "1" },
	{ "van",				"VAN",			"X", 78,  2, NULL },
	{ "newLine",			"���๮��",		"X", 80,  2, "\r\n" }
};

Attribute g_astCliDataAttrs[] = { /* attrId : 2 */
	{ "idCode",					"�ĺ��ڵ�",		"X",  0,  1, "D" },
	{ "dataSerialNum",			"�������Ϸù�ȣ",		"9",  1,  6, NULL },
	{ "bankCode",				"�����ڵ�",		"9",  7,  3, NULL },
	{ "accountNum",				"���¹�ȣ",		"X", 10, 14, NULL },
	{ "requestTransferPrice",	"��ü��û�ݾ�",		"9", 24, 11, NULL },
	{ "realTransferPrice",		"������û�ݾ�",		"9", 35, 11, NULL },
	{ "recieverIdNum",			"�ֹ�/����ڹ�ȣ",	"X", 46, 13, NULL },
	{ "processingResult",		"ó�����",		"X", 59,  1, NULL },
	{ "disableCode",			"�Ҵ��ڵ�",		"X", 60,  4, NULL },
	{ "briefs",					"����",			"X", 64, 12, NULL },
	{ "blank",					"����",			"X", 76,  4, NULL },
	{ "newLine",				"���๮��",		"X", 80,  2, "\r\n" }
};

Attribute g_astCliTailAttrs[] = { /* attrId : 3 */
	{ "idCode",					"�ĺ��ڵ�",	"X",  0,  1, "E" },
	{ "totalRequestCnt",		"���ǷڰǼ�",	"9",  1,  7, NULL },
	{ "totalRequestPrice",		"���Ƿڱݾ�",	"9",  8, 13, NULL },
	{ "normalProcessingCnt",	"����ó���Ǽ�",	"9", 21,  7, NULL },
	{ "normalProcessingPrice",	"����ó���ݾ�",	"9", 28, 13, NULL },
	{ "disableProcessingCnt",	"�Ҵ�ó���Ǽ�",	"9", 41,  7, NULL },
	{ "disableProcessingPrice",	"�Ҵ�ó���ݾ�",	"9", 48, 13, NULL },
	{ "recoveryCode",			"�����ȣ",	"X", 61,  8, "3706" },
	{ "blank",					"����",		"X", 79, 11, NULL },
	{ "newLine",				"���๮��",	"X", 80,  2, "\r\n" }
};

/*
 * Server Attributes
 */
Attribute g_astSvrHeadAttrs[] = { /* attrId : 4 */
	{ "idCode",				"�ĺ��ڵ�",	"C",  0,  9, NULL },
	{ "companyCode",		"��ü�ڵ�",	"C",  9,  8, NULL },
	{ "bankCode2",			"�����ڵ�2",	"C", 17,  2, NULL },
	{ "msgCode",			"�޽����ڵ�",	"C", 19,  4, NULL },
	{ "workTypeCode",		"���������ڵ�",	"C", 23,  3, NULL },
	{ "transferCnt",		"�۽�Ƚ��",	"C", 26,  1, NULL },
	{ "msgNum",				"������ȣ",	"N", 27,  6, NULL },
	{ "transferDate",		"��������",	"D", 33,  8, NULL },
	{ "transferTime",		"���۽ð�",	"T", 41,  6, NULL },
	{ "responseCode",		"�����ڵ�",	"C", 47,  4, NULL },
	{ "bankResponseCode",	"���������ڵ�",	"C", 51,  4, NULL },
	{ "lookupDate",			"��ȸ����",	"D", 55,  8, NULL },
	{ "lookupNum",			"��ȸ��ȣ",	"N", 63,  6, NULL },
	{ "bankMsgNum",			"����������ȣ",	"C", 69, 15, NULL },
	{ "bankCode3",			"�����ڵ�3",	"C", 84,  3, NULL },
	{ "spare",				"����",		"C", 87, 13, NULL }
};

Attribute g_astSvrDataAttrs[] = { /* attrId : 5 */
	{ "withdrawalAccountNum",		"�ĺ��ڵ�",	"C",  0, 15, NULL },
	{ "bankBookPassword",			"��ü�ڵ�",	"C",  9,  8, NULL },
	{ "recoveryCode",				"�����ڵ�2",	"C", 17,  6, NULL },
	{ "withdrawalAmount",			"�޽����ڵ�",	"N", 19, 13, NULL },
	{ "afterWithdrawalBalanceSign",	"���������ڵ�",	"C", 23,  1, NULL },
	{ "afterWithdrawalBalance",		"�۽�Ƚ��",	"N", 26, 13, NULL },
	{ "depositBankCode2",			"������ȣ",	"C", 27,  2, NULL },
	{ "depositAccountNum",			"��������",	"C", 33, 15, NULL },
	{ "fees",						"���۽ð�",	"N", 41,  9, NULL },
	{ "transferTime",				"�����ڵ�",	"T", 47,  6, NULL },
	{ "depositAccountBriefs",		"���������ڵ�",	"C", 51, 20, NULL },
	{ "cmsCode",					"��ȸ����",	"C", 55, 16, NULL },
	{ "identificationNum",			"��ȸ��ȣ",	"C", 63, 13, NULL },
	{ "autoTransferClassification",	"����������ȣ",	"C", 69,  2, NULL },
	{ "withdrawalAccountBriefs",	"�����ڵ�3",	"C", 84, 20, NULL },
	{ "depositBankCode3",			"����",		"C", 87,  3, NULL },
	{ "salaryClassification",		"�ĺ��ڵ�",	"C",  0,  1, NULL },
	{ "spare",						"��ü�ڵ�",	"C",  9, 37, NULL }
};

int g_arAttrSize[] = {
	sizeof(CliHeadRecord),
	sizeof(CliDataRecord),
	sizeof(CliTailRecord),
	sizeof(SvrHeadRecord),
	sizeof(SvrDataRecord)
};

int g_arAttrLen[] = {
	sizeof(g_astCliHeadAttrs) / sizeof(Attribute),
	sizeof(g_astCliDataAttrs) / sizeof(Attribute),
	sizeof(g_astCliTailAttrs) / sizeof(Attribute),
	sizeof(g_astSvrHeadAttrs) / sizeof(Attribute),
	sizeof(g_astSvrDataAttrs) / sizeof(Attribute)
};

/* Common Record Func */
void initRecord(void *pRecord, int szRecord) {
	memset(pRecord, 0, szRecord);
}

int makeRecord(char *pData, int szData, void *pstOutRecord, int szRecord) {
	if (szData > szRecord)
	{
		fprintf(stderr, "���� ����. (szData:%d > szRecord)\n", szData, szRecord);
		return -1;
	}
	
	if (memcpy(pstOutRecord, pData, szRecord) == NULL)
	{
		fprintf(stderr, "memcpy() ����.\n", szData, szRecord);
		return -1;
	}
	
	return 0;
}

int setRecordData(char *pRecord, char* pData, char *pAttrKey, Attribute astAttributes[]) {
	Attribute *pAttr = NULL;
	char *pAttrType = NULL;
	char *pDefaultVal = NULL;
	char arBuf1[64] = { 0, }, arBuf2[64] = { 0, }, arBuf3[64] = { 0, }, arBuf4[64] = { 0, };
	int szBuf1 = sizeof(arBuf1), szBuf2 = sizeof(arBuf2), szBuf3 = sizeof(arBuf3), szBuf4 = sizeof(arBuf4);
	int szAttrData = -1;
	int idx = -1;
	int loopI = 0;
	
	/* �Ӽ����� �˻� */
	if (astAttributes == g_astCliDataAttrs)
	{
		idx = 1;
	}
	else if (astAttributes == g_astSvrHeadAttrs)
	{
		idx = 3;
	}
	else if (astAttributes == g_astSvrDataAttrs)
	{
		idx = 4;
	}
	else if (astAttributes == g_astCliHeadAttrs)
	{
		idx = 0;
	}
	else if (astAttributes == g_astCliTailAttrs)
	{
		idx = 2;	
	}
	
	for (loopI = 0; loopI < g_arAttrLen[idx]; ++loopI)
	{
		if (strcmp(pAttrKey, astAttributes[loopI].pAttrKey) == 0)
		{
			pAttr = &astAttributes[loopI];
			break;
		}
	}
	
	if (pAttr == NULL)
	{
		fprintf(stderr, "Ű ��('%s')�� ���� �Ӽ��� ã�� �� ����. (astAttributes:%p)\n", pAttrKey, &astAttributes);
		return -1;
	}
	
	pAttrType = pAttr->pAttrType;
	pDefaultVal = pAttr->pDefaultVal;
	szAttrData = pAttr->szAttr;
	
	/* �� �ʱ�ȭ �� ���� */
	if ((strcmp(pAttrType, "X") == 0 ) || (strcmp(pAttrType, "C") == 0) || /* �����е�, �·����� */
		(strcmp(pAttrType, "D") == 0 ) || (strcmp(pAttrType, "T") == 0)) 
	{
		memset(pRecord, ' ', szAttrData);
		
		if (pData == NULL) /* �⺻�� ���� */
		{
			if (pDefaultVal != NULL)
			{	
				memcpy(pRecord, pAttr->pDefaultVal, min(szAttrData, strlen(pDefaultVal)));
			}
			else { /* �ʱ�ȭ�� ���� */ }
		}
		else /* �Ű����� ���� */
		{
			memcpy(pRecord, pData, min(szAttrData, strlen(pData)));
		}
	}
	else if (strcmp(pAttrType, "9") == 0 || strcmp(pAttrType, "N") == 0) /* 0�е�, ������� */
	{
		memset(pRecord, '0', szAttrData);

		if (pData == NULL)
		{
			if (pDefaultVal != NULL)
			{
				memcpy(arBuf1, pAttr->pDefaultVal, min(szBuf1, strlen(pDefaultVal)));
				itoa(szAttrData, arBuf2, 10);
				sprintf(arBuf3, "%%0%ss", arBuf2);
				sprintf(arBuf4, arBuf3, arBuf1);
				memcpy(pRecord, arBuf4, min(szAttrData, strlen(arBuf4)));
			}
			else {}
		}
		else
		{
			memcpy(arBuf1, pData, min(szBuf1, strlen(pData)));	/* arBuf1 : pData("12345")	 */
			itoa(szAttrData, arBuf2, 10);						/* arBuf2 : szAttrData("13") */
			sprintf(arBuf3, "%%0%ss", arBuf2);					/* arBuf3 : "%013s"			 */
			sprintf(arBuf4, arBuf3, arBuf1);					/* arBuf4 : "0000000012345"  */
			memcpy(pRecord, arBuf4, min(szAttrData, strlen(arBuf4)));
		}
	}
	else /* ���� */
	{
		fprintf(stderr, "�� �� ���� �Ӽ� ����. (attrType = %s)", pAttrType);
		return -1;
	}
	
	return 0;
}

/* 
 * Client Record Func
 */
int cvtCli2SvrRecord(CliRecord *pstCliRecord, SvrRecord *pstOutSvrRecord) {	
	char arTime[15] = { '\0', }; /* YYYYMMDDhhmmss */
	char arBuf1[256] = { '\0', };
	int szBuf1 = sizeof(arBuf1);

	if (pstCliRecord == NULL || pstOutSvrRecord == NULL)
	{
		fprintf(stderr, "�Ű����� ����. (pstCliRecord:%p || pstOutSvrRecord:%p)", pstCliRecord, pstOutSvrRecord);
		return -1;
	}

	/* ǥ����, ����δ� ���� */	
	if (pstCliRecord->stData.idCode[0] != 'D')
	{
		return -1;
	}
	
	getTime(arTime, 3);
	
	/* SvrHead */
	/* idCode[9](�ĺ��ڵ�) */
	setRecordData(pstOutSvrRecord->stHead.idCode, NULL, "idCode", g_astSvrHeadAttrs);
	
	/* companyCode[8](��ü�ڵ�:FB_PARENT_COMP_CODE) */
	setRecordData(pstOutSvrRecord->stHead.companyCode, g_FB_PARENT_COMP_CODE, "companyCode", g_astSvrHeadAttrs);
	
	/* bankCode2[2](�����ڵ�2:FB_PARENT_BANK_CODE_2) */
	setRecordData(pstOutSvrRecord->stHead.bankCode2, g_FB_PARENT_BANK_CODE_2, "companyCode", g_astSvrHeadAttrs);
	
	/* msgCode[4](�޽����ڵ�) */
	setRecordData(pstOutSvrRecord->stHead.msgCode, "0100", "msgCode", g_astSvrHeadAttrs);
	
	/* workTypeCode[3](���������ڵ�) */
	setRecordData(pstOutSvrRecord->stHead.workTypeCode, "100", "workTypeCode", g_astSvrHeadAttrs);
	
	/* transferCnt[1](�۽�Ƚ��) */
	setRecordData(pstOutSvrRecord->stHead.transferCnt, "1", "transferCnt", g_astSvrHeadAttrs);

	/* msgNum[6](������ȣ) */
	byteToString(arBuf1, pstCliRecord->stData.dataSerialNum, sizeof(pstCliRecord->stData.dataSerialNum));
	setRecordData(pstOutSvrRecord->stHead.msgNum, arBuf1, "msgNum", g_astSvrHeadAttrs);

	/* transferDate[8](��������) */
	byteToString(arBuf1, arTime, 8); /* YYYYMMDD */
	setRecordData(pstOutSvrRecord->stHead.transferDate, arBuf1, "transferDate", g_astSvrHeadAttrs);
	
	/* transferTime[6](���۽ð�) */
	byteToString(arBuf1, arTime + 8, 6); /* hhmmss */
	setRecordData(pstOutSvrRecord->stHead.transferTime, arBuf1, "transferTime", g_astSvrHeadAttrs);
	
	/* responseCode[4](�����ڵ�) */
	setRecordData(pstOutSvrRecord->stHead.responseCode, NULL, "responseCode", g_astSvrHeadAttrs);
	
	/* bankResponseCode[4](���������ڵ�) */
	setRecordData(pstOutSvrRecord->stHead.bankResponseCode, NULL, "bankResponseCode", g_astSvrHeadAttrs);
	
	/* lookupDate[8](��ȸ����) */
	setRecordData(pstOutSvrRecord->stHead.lookupDate, NULL, "lookupDate", g_astSvrHeadAttrs);
	
	/* lookupNum[6](��ȸ��ȣ) */
	setRecordData(pstOutSvrRecord->stHead.lookupNum, NULL, "lookupNum", g_astSvrHeadAttrs);
	
	/* bankMsgNum[15](����������ȣ) */
	setRecordData(pstOutSvrRecord->stHead.bankMsgNum, NULL, "bankMsgNum", g_astSvrHeadAttrs);
	
	/* bankCode3[3](�����ڵ�3:FB_PARENT_BANK_CODE_3) */
	setRecordData(pstOutSvrRecord->stHead.bankCode3, g_FB_PARENT_BANK_CODE_3, "bankCode3", g_astSvrHeadAttrs);
	
	/* spare[13](����) */
	setRecordData(pstOutSvrRecord->stHead.spare, NULL, "spare", g_astSvrHeadAttrs);

	/* SvrData */
	/* withdrawalAccountNum[15](��ݰ��¹�ȣ:FB_PARENT_ACCOUNT_NUMB) */
	setRecordData(pstOutSvrRecord->stData.withdrawalAccountNum, g_FB_PARENT_ACCOUNT_NUMB, "withdrawalAccountNum", g_astSvrDataAttrs);
	
	/* bankBookPassword[8](�����й�ȣ) */
	setRecordData(pstOutSvrRecord->stData.bankBookPassword, NULL, "bankBookPassword", g_astSvrDataAttrs);
	
	/* recoveryCode[6](�����ȣ) */
	setRecordData(pstOutSvrRecord->stData.recoveryCode, NULL, "recoveryCode", g_astSvrDataAttrs);
	
	/* withdrawalAmount[13](��ݱݾ�) */
	byteToString(arBuf1, pstCliRecord->stData.requestTransferPrice, sizeof(pstCliRecord->stData.requestTransferPrice));
	setRecordData(pstOutSvrRecord->stData.withdrawalAmount, arBuf1, "withdrawalAmount", g_astSvrDataAttrs);
	
	/* afterWithdrawalBalanceSign[1](������ܾ׺�ȣ) */
	setRecordData(pstOutSvrRecord->stData.afterWithdrawalBalanceSign, NULL, "afterWithdrawalBalanceSign", g_astSvrDataAttrs);
	
	/* afterWithdrawalBalance[13](������ܾ�) */
	setRecordData(pstOutSvrRecord->stData.afterWithdrawalBalance, NULL, "afterWithdrawalBalance", g_astSvrDataAttrs);
	
	/* depositBankCode2[2](�Ա������ڵ�2:FB_DEPOSIT_BANK_CODE_2) */
	setRecordData(pstOutSvrRecord->stData.depositBankCode2, g_FB_DEPOSIT_BANK_CODE_2, "depositBankCode2", g_astSvrDataAttrs);
	
	/* depositAccountNum[15](�Աݰ��¹�ȣ) */
	byteToString(arBuf1, pstCliRecord->stData.accountNum, sizeof(pstCliRecord->stData.accountNum));
	setRecordData(pstOutSvrRecord->stData.depositAccountNum, arBuf1, "depositAccountNum", g_astSvrDataAttrs);
	
	/* fees[9](������) */
	setRecordData(pstOutSvrRecord->stData.fees, NULL, "fees", g_astSvrDataAttrs);
	
	/* transferTime[6](��ü�ð�) */
	byteToString(arBuf1, arTime + 8, 6); /* hhmmss */
	setRecordData(pstOutSvrRecord->stData.transferTime, arBuf1, "transferTime", g_astSvrDataAttrs);
	
	/* depositAccountBriefs[20](�Աݰ�������:FB_PARENT_COMP_NAME) */
	setRecordData(pstOutSvrRecord->stData.depositAccountBriefs, g_FB_PARENT_COMP_NAME, "depositAccountBriefs", g_astSvrDataAttrs);
	
	/* cmsCode[16](CMS�ڵ�) */
	setRecordData(pstOutSvrRecord->stData.cmsCode, NULL, "cmsCode", g_astSvrDataAttrs);
	
	/* identificationNum[13](�ſ�Ȯ�ι�ȣ) */
	setRecordData(pstOutSvrRecord->stData.identificationNum, NULL, "identificationNum", g_astSvrDataAttrs);
	
	/* autoTransferClassification[2](�ڵ���ü����) */
	setRecordData(pstOutSvrRecord->stData.autoTransferClassification, NULL, "autoTransferClassification", g_astSvrDataAttrs);
	
	/* withdrawalAccountBriefs[20](��ݰ�������) */
	byteToString(arBuf1, pstCliRecord->stData.briefs, sizeof(pstCliRecord->stData.briefs));
	setRecordData(pstOutSvrRecord->stData.withdrawalAccountBriefs, arBuf1, "withdrawalAccountBriefs", g_astSvrDataAttrs);
	
	/* depositBankCode3[3](�Ա������ڵ�3:FB_DEPOSIT_BANK_CODE_3) */
	setRecordData(pstOutSvrRecord->stData.depositBankCode3, g_FB_DEPOSIT_BANK_CODE_3, "depositBankCode3", g_astSvrDataAttrs);
	
	/* salaryClassification[1](�޿�����) */
	setRecordData(pstOutSvrRecord->stData.salaryClassification, NULL, "salaryClassification", g_astSvrDataAttrs);
	
	/* spare[37](����) */
	setRecordData(pstOutSvrRecord->stData.spare, NULL, "spare", g_astSvrDataAttrs);
	
	return 0;
}

void printCliRecord(CliRecord *pstCliRecord) {
	CliHeadRecord *pstHeadRecord = &(pstCliRecord->stHead);
	CliDataRecord *pstDataRecord = &(pstCliRecord->stData);
	CliTailRecord *pstTailRecord = &(pstCliRecord->stTail);
	char arBuf[256] = { 0, };
	int szBuf = sizeof(arBuf);

	if (pstHeadRecord != NULL)
	{
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->idCode,			sizeof(pstHeadRecord->idCode))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->taskComp,			sizeof(pstHeadRecord->taskComp))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->bankCode,			sizeof(pstHeadRecord->bankCode))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->companyCode,		sizeof(pstHeadRecord->companyCode))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->comissioningDate,	sizeof(pstHeadRecord->comissioningDate)));
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->processingDate,	sizeof(pstHeadRecord->processingDate))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->motherAccountNum,	sizeof(pstHeadRecord->motherAccountNum)));
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->transferType,		sizeof(pstHeadRecord->transferType))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->companyNum,		sizeof(pstHeadRecord->companyNum))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->resultNotifyType,	sizeof(pstHeadRecord->resultNotifyType)));
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->transferCnt,		sizeof(pstHeadRecord->transferCnt))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->password,			sizeof(pstHeadRecord->password))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->blank,				sizeof(pstHeadRecord->blank))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->format,			sizeof(pstHeadRecord->format))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->van,				sizeof(pstHeadRecord->van))				);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->newLine,			sizeof(pstHeadRecord->newLine))			);
	}
	
	if (pstDataRecord != NULL)
	{
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->idCode,				sizeof(pstDataRecord->idCode))					);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->dataSerialNum,			sizeof(pstDataRecord->dataSerialNum))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->bankCode,				sizeof(pstDataRecord->bankCode))				);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->accountNum,			sizeof(pstDataRecord->accountNum))				);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->requestTransferPrice,	sizeof(pstDataRecord->requestTransferPrice))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->realTransferPrice,		sizeof(pstDataRecord->realTransferPrice))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->recieverIdNum,			sizeof(pstDataRecord->recieverIdNum))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->processingResult,		sizeof(pstDataRecord->processingResult))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->disableCode,			sizeof(pstDataRecord->disableCode))				);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->briefs,				sizeof(pstDataRecord->briefs))					);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->blank,					sizeof(pstDataRecord->blank))					);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->newLine,				sizeof(pstDataRecord->newLine))					);
	}
	
	if (pstTailRecord != NULL)
	{
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->idCode,				sizeof(pstTailRecord->idCode))					);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->totalRequestCnt,		sizeof(pstTailRecord->totalRequestCnt))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->totalRequestPrice,		sizeof(pstTailRecord->totalRequestPrice))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->normalProcessingCnt,	sizeof(pstTailRecord->normalProcessingCnt))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->normalProcessingPrice,	sizeof(pstTailRecord->normalProcessingPrice))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->disableProcessingCnt,	sizeof(pstTailRecord->disableProcessingCnt))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->disableProcessingPrice,sizeof(pstTailRecord->disableProcessingPrice))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->recoveryCode,			sizeof(pstTailRecord->recoveryCode))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->blank,					sizeof(pstTailRecord->blank))					);
		fprintf(stderr, "%s", byteToString(arBuf, pstTailRecord->newLine,				sizeof(pstTailRecord->newLine))					);
	}                                                               
}

/* 
 * Server Record Func 
 */
int cvtSvr2CliRecord(SvrRecord *pstSvrRecord, CliRecord *pstOutCliRecord, int option) {
	static int TotalRequestCnt = 0, TotalRequestAmt = 0;
	static int TotalSuccessCnt = 0, TotalSuccessAmt = 0;
	static int TotalFailureCnt = 0, TotalFailureAmt = 0;
	char arTime[14] = { '\0', }; /* YYYYMMDDhhmmss */
	char arBuf1[256] = { '\0', };
	int szBuf1 = sizeof(arBuf1);
	int transferAmt = -1;
	
	getTime(arTime, 3);

	if (option == 0)
	{
		/* CliHead(ǥ����) */
		/* idCode[1](�ĺ��ڵ�) */
		setRecordData(pstOutCliRecord->stHead.idCode, NULL, "idCode", g_astCliHeadAttrs);
		
		/* taskComp[2](��������) */
		setRecordData(pstOutCliRecord->stHead.taskComp, NULL, "taskComp", g_astCliHeadAttrs);
		
		/* bankCode[3](�����ڵ�) */
		setRecordData(pstOutCliRecord->stHead.bankCode, NULL, "bankCode", g_astCliHeadAttrs);
		
		/* companyCode[8](��ü�ڵ�) */
		setRecordData(pstOutCliRecord->stHead.companyCode, NULL, "companyCode", g_astCliHeadAttrs);
		
		/* comissioningDate[6](��ü�Ƿ�����) */
		setRecordData(pstOutCliRecord->stHead.comissioningDate, NULL, "comissioningDate", g_astCliHeadAttrs);
		
		/* processingDate[6](��üó������) */
		byteToString(arBuf1, arTime, 8); /* YYYYMMDD */
		setRecordData(pstOutCliRecord->stHead.processingDate, arBuf1, "processingDate", g_astCliHeadAttrs);
		
		/* motherAccountNum[14](����¹�ȣ) */
		setRecordData(pstOutCliRecord->stHead.motherAccountNum, NULL, "motherAccountNum", g_astCliHeadAttrs);
		
		/* transferType[2](��ü����) */
		setRecordData(pstOutCliRecord->stHead.transferType, NULL, "transferType", g_astCliHeadAttrs);
		
		/* companyNum[6](ȸ���ȣ) */
		setRecordData(pstOutCliRecord->stHead.companyNum, NULL, "companyNum", g_astCliHeadAttrs);
		
		/* resultNotifyType[1](ó������뺸����) */
		setRecordData(pstOutCliRecord->stHead.resultNotifyType, NULL, "resultNotifyType", g_astCliHeadAttrs);
		
		/* transferCnt[1](��������) */
		setRecordData(pstOutCliRecord->stHead.transferCnt, NULL, "transferCnt", g_astCliHeadAttrs);
		
		/* password[8](��й�ȣ) */
		setRecordData(pstOutCliRecord->stHead.password, NULL, "password", g_astCliHeadAttrs);
		
		/* blank[19](����) */
		setRecordData(pstOutCliRecord->stHead.blank, NULL, "blank", g_astCliHeadAttrs);
		
		/* format[1](Format) */
		setRecordData(pstOutCliRecord->stHead.format, NULL, "format", g_astCliHeadAttrs);
		
		/* van[2](VAN) */
		setRecordData(pstOutCliRecord->stHead.van, "KC", "van", g_astCliHeadAttrs);
		
		/* newLine[2](���๮��) */
		setRecordData(pstOutCliRecord->stHead.newLine, NULL, "newLine", g_astCliHeadAttrs);
	}
	else if (option == 1)
	{
		/* CliTail(�����) */
		/* idCode[1](�ĺ��ڵ�) */
		setRecordData(pstOutCliRecord->stTail.idCode, NULL, "idCode", g_astCliTailAttrs);
		
		/* totalRequestCnt[7](���ǷڰǼ�) */
		sprintf(arBuf1, "%d", TotalRequestCnt);
		setRecordData(pstOutCliRecord->stTail.totalRequestCnt, arBuf1, "totalRequestCnt", g_astCliTailAttrs);
		
		/* totalRequestPrice[13](���Ƿڱݾ�) */
		sprintf(arBuf1, "%d", TotalRequestAmt);
		setRecordData(pstOutCliRecord->stTail.totalRequestPrice, arBuf1, "totalRequestPrice", g_astCliTailAttrs);
		
		/* normalProcessingCnt[7](����ó���Ǽ�) */
		sprintf(arBuf1, "%d", TotalSuccessCnt);
		setRecordData(pstOutCliRecord->stTail.normalProcessingCnt, arBuf1, "normalProcessingCnt", g_astCliTailAttrs);
		
		/* normalProcessingPrice[13](����ó���ݾ�) */
		sprintf(arBuf1, "%d", TotalSuccessAmt);
		setRecordData(pstOutCliRecord->stTail.normalProcessingPrice, arBuf1, "normalProcessingPrice", g_astCliTailAttrs);
		
		/* disableProcessingCnt[7](�Ҵ�ó���Ǽ�) */
		sprintf(arBuf1, "%d", TotalFailureCnt);
		setRecordData(pstOutCliRecord->stTail.disableProcessingCnt, arBuf1, "disableProcessingCnt", g_astCliTailAttrs);
		
		/* disableProcessingPrice[13](�Ҵ�ó���ݾ�) */
		sprintf(arBuf1, "%d", TotalFailureAmt);
		setRecordData(pstOutCliRecord->stTail.disableProcessingPrice, arBuf1, "disableProcessingPrice", g_astCliTailAttrs);
		
		/* recoveryCode[8](�����ȣ) */
		setRecordData(pstOutCliRecord->stTail.recoveryCode, NULL, "recoveryCode", g_astCliTailAttrs);
		
		/* blank[11](����) */
		setRecordData(pstOutCliRecord->stTail.blank, NULL, "blank", g_astCliTailAttrs);
		
		/* newLine[2](���๮��) */
		setRecordData(pstOutCliRecord->stTail.newLine, NULL, "newLine", g_astCliTailAttrs);
		
		/* �ʱ�ȭ */
		TotalRequestCnt = 0, TotalRequestAmt = 0;
		TotalSuccessCnt = 0, TotalSuccessAmt = 0;
		TotalFailureCnt = 0, TotalFailureAmt = 0;
	}
	else
	{
		/* CliData(�����ͺ�) */
		/* idCode[1](�ĺ��ڵ�) */
		setRecordData(pstOutCliRecord->stData.idCode, NULL, "idCode", g_astCliDataAttrs);
		
		/* dataSerialNum[6](�������Ϸù�ȣ) */
		byteToString(arBuf1, pstSvrRecord->stHead.msgNum, sizeof(pstSvrRecord->stHead.msgNum));
		setRecordData(pstOutCliRecord->stData.dataSerialNum, arBuf1, "dataSerialNum", g_astCliDataAttrs);
		
		/* bankCode[3](�����ڵ�) */
		byteToString(arBuf1, pstSvrRecord->stData.depositBankCode3, sizeof(pstSvrRecord->stData.depositBankCode3));
		setRecordData(pstOutCliRecord->stData.bankCode, arBuf1, "bankCode", g_astCliDataAttrs);
		
		/* accountNum[14](���¹�ȣ) */
		byteToString(arBuf1, pstSvrRecord->stData.depositAccountNum, sizeof(pstSvrRecord->stData.depositAccountNum));
		setRecordData(pstOutCliRecord->stData.accountNum, arBuf1, "accountNum", g_astCliDataAttrs);
		
		byteToString(arBuf1, pstSvrRecord->stData.withdrawalAmount, sizeof(pstSvrRecord->stData.withdrawalAmount));
		transferAmt = atoi(arBuf1);
		TotalRequestAmt += transferAmt;		/* �����:���Ƿڱݾ� */
		++TotalRequestCnt;					/* �����:���ǷڰǼ� */
		sprintf(arBuf1, "%d", transferAmt);	/* �ڸ��� ����(13->11)�� ȣȯ�ϱ� ���� ��ȯ */
		
		/* requestTransferPrice[11](��ü��û�ݾ�) */
		setRecordData(pstOutCliRecord->stData.requestTransferPrice, arBuf1, "requestTransferPrice", g_astCliDataAttrs);
		
		/* realTransferPrice[11](������ü�ݾ�) */
		setRecordData(pstOutCliRecord->stData.realTransferPrice, arBuf1, "realTransferPrice", g_astCliDataAttrs);
		
		/* recieverIdNum[13](�ֹ�/����ڹ�ȣ) */
		setRecordData(pstOutCliRecord->stData.recieverIdNum, NULL, "recieverIdNum", g_astCliDataAttrs);
		
		/* processingResult[1](ó�����) */
		byteToString(arBuf1, pstSvrRecord->stHead.bankResponseCode, sizeof(pstSvrRecord->stHead.bankResponseCode));
		
		if (strcmp(arBuf1, "0000") == 0) /* ����ó�� */
		{
			byteToString(arBuf1, "Y", 1);
			TotalSuccessAmt += transferAmt;
			++TotalSuccessCnt;
		}
		else /* �Ҵ�ó�� */
		{
			byteToString(arBuf1, "N", 1);
			TotalFailureAmt += transferAmt;
			++TotalFailureCnt;
		}
		
		setRecordData(pstOutCliRecord->stData.processingResult, arBuf1, "processingResult", g_astCliDataAttrs);
		
		/* disableCode[4](�Ҵ��ڵ�) */
		byteToString(arBuf1, pstSvrRecord->stHead.bankResponseCode, sizeof(pstSvrRecord->stHead.bankResponseCode));
		setRecordData(pstOutCliRecord->stData.disableCode, arBuf1, "disableCode", g_astCliDataAttrs);
		
		/* briefs[12](����) */
		byteToString(arBuf1, pstSvrRecord->stData.withdrawalAccountBriefs, sizeof(pstSvrRecord->stData.withdrawalAccountBriefs));
		setRecordData(pstOutCliRecord->stData.briefs, arBuf1, "briefs", g_astCliDataAttrs);
		
		/* blank[4](����) */
		setRecordData(pstOutCliRecord->stData.blank, NULL, "blank", g_astCliDataAttrs);
		
		/* newLine[2](���๮��) */
		setRecordData(pstOutCliRecord->stData.newLine, NULL, "newLine", g_astCliDataAttrs);
	}

	return 0;
}

void printSvrRecord(SvrRecord *pstSvrRecord) {
	SvrHeadRecord *pstHeadRecord = &(pstSvrRecord->stHead);
	SvrDataRecord *pstDataRecord = &(pstSvrRecord->stData);
	char arBuf[256] = { 0, };
	int szBuf = sizeof(arBuf);
	
	if (pstHeadRecord != NULL)
	{
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->idCode,			sizeof(pstHeadRecord->idCode))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->companyCode,		sizeof(pstHeadRecord->companyCode))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->bankCode2,			sizeof(pstHeadRecord->bankCode2))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->msgCode,			sizeof(pstHeadRecord->msgCode))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->workTypeCode,		sizeof(pstHeadRecord->workTypeCode))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->transferCnt,		sizeof(pstHeadRecord->transferCnt))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->msgNum,			sizeof(pstHeadRecord->msgNum))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->transferDate,		sizeof(pstHeadRecord->transferDate))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->transferTime,		sizeof(pstHeadRecord->transferTime))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->responseCode,		sizeof(pstHeadRecord->responseCode))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->bankResponseCode,	sizeof(pstHeadRecord->bankResponseCode)));
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->lookupDate,		sizeof(pstHeadRecord->lookupDate))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->lookupNum,			sizeof(pstHeadRecord->lookupNum))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->bankMsgNum,		sizeof(pstHeadRecord->bankMsgNum))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->bankCode3,			sizeof(pstHeadRecord->bankCode3))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstHeadRecord->spare,				sizeof(pstHeadRecord->spare))			);
	}                                        
	
	if (pstDataRecord != NULL)               
	{                                        
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->withdrawalAccountNum,		sizeof(pstDataRecord->withdrawalAccountNum))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->bankBookPassword,			sizeof(pstDataRecord->bankBookPassword))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->recoveryCode,				sizeof(pstDataRecord->recoveryCode))				);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->withdrawalAmount,			sizeof(pstDataRecord->withdrawalAmount))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->afterWithdrawalBalanceSign,sizeof(pstDataRecord->afterWithdrawalBalanceSign))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->afterWithdrawalBalance,	sizeof(pstDataRecord->afterWithdrawalBalance))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->depositBankCode2,			sizeof(pstDataRecord->depositBankCode2))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->depositAccountNum,			sizeof(pstDataRecord->depositAccountNum))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->fees,						sizeof(pstDataRecord->fees))						);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->transferTime,				sizeof(pstDataRecord->transferTime))				);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->depositAccountBriefs,		sizeof(pstDataRecord->depositAccountBriefs))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->cmsCode,					sizeof(pstDataRecord->cmsCode))						);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->identificationNum,			sizeof(pstDataRecord->identificationNum))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->autoTransferClassification,sizeof(pstDataRecord->autoTransferClassification))	);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->withdrawalAccountBriefs,	sizeof(pstDataRecord->withdrawalAccountBriefs))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->depositBankCode3,			sizeof(pstDataRecord->depositBankCode3))			);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->salaryClassification,		sizeof(pstDataRecord->salaryClassification))		);
		fprintf(stderr, "%s", byteToString(arBuf, pstDataRecord->spare,						sizeof(pstDataRecord->spare))						);
	}
}