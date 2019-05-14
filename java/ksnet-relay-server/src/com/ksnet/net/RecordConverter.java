package com.ksnet.net;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Date;
import java.text.SimpleDateFormat;

import com.ksnet.util.*;

public class RecordConverter {

	public static final byte[] NEW_LINE = { '\r', '\n' };

	private Record inRecord;						// �Է� ���ڵ�
	private String outRecordTypeName;				// ��� ���ڵ� Ÿ�Ը�
	private String outRecordSubTypeName;			// ��� ���ڵ� ����Ÿ�Ը�
	private HashMap<String, byte[]> envVarMap;		// ȯ�溯�� �ؽø�
	private HashMap<String, byte[]> localVarMap;	// �������� �ؽø�

	public RecordConverter(Record inRecord, String outRecordTypeName, String outRecordSubTypeName, HashMap<String, byte[]> envVarMap) {
		this.inRecord = inRecord;
		this.outRecordTypeName = outRecordTypeName;
		this.outRecordSubTypeName = outRecordSubTypeName;
		this.envVarMap = envVarMap;
		this.localVarMap = new HashMap<String, byte[]>();
	}
	
	public Record convert() {
		String inRecordTypeName = inRecord.getTypeName();
		
		// �ϳ����� ������ȯ
		if (inRecordTypeName.equals("HanaRecordServer") || inRecordTypeName.equals("HanaRecordClient")) {
			return hana_record_convertion(outRecordTypeName);
		}
		
		return null;
	}
	
	public Record convert(Record inRecord) {
		setInRecord(inRecord);
		return convert();
	}
	
	public void close() {
		inRecord = null;
		outRecordSubTypeName = null;
		outRecordSubTypeName = null;
		envVarMap = null;
		localVarMap.clear();
		localVarMap = null;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	public Record getInRecord() {
		return inRecord;
	}
	
	public void setInRecord(Record inRecord) {
		this.inRecord = inRecord;
	}
	
	public String getOutRecordTypeName() {
		return outRecordTypeName;
	}
	
	public void setOutRecordTypeName(String outRecordTypeName) {
		this.outRecordTypeName = outRecordTypeName;
	}
	
	public String getOutRecordSubTypeName() {
		return outRecordSubTypeName;
	}
	
	public void setOutRecordSubTypeName(String outRecordSubTypeName) {
		this.outRecordSubTypeName = outRecordSubTypeName;
	}
	
	public HashMap<String, byte[]> getEnvVarMap() {
		return envVarMap;
	}
	
	public void setEnvVarMap(HashMap<String, byte[]> envVarMap) {
		this.envVarMap = envVarMap;
	}
	
	public HashMap<String, byte[]> getLocalVarMap() {
		return localVarMap;
	}
	
	public void setLocalVarMap(HashMap<String, byte[]> localVarMap) {
		this.localVarMap = localVarMap;
	}
		
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	private Record hana_record_convertion(String outRecordTypeName) {
		Record rtRecord = null;
		AttributeManager attrMgr = AttributeManager.getInst();
		
		// �ð� ������
		SimpleDateFormat dateTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String today = dateTime.format(new Date(System.currentTimeMillis()));
		String year = today.substring(0, 4),	month = today.substring(5, 7),	date = today.substring(8, 10);
		String hour = today.substring(11, 13),	min = today.substring(14, 16),	sec = today.substring(17, 19);
		
		// Client -> Server
		if (outRecordTypeName.equals("HanaRecordServer")) {
			String inRecordTypeName = inRecord.getTypeName();
			String inRecordSubTypeName = inRecord.getSubTypeName();
			
			if (inRecordTypeName.equals("HanaRecordClient")) {
				if (inRecordSubTypeName.equals("Data")) {
					int recordLength = attrMgr.getRecordSizeFromAttributeMap("HanaAttrServer");
					byte[] dummyDatas = new byte[recordLength];
					rtRecord = new Record("HanaRecordServer", inRecord.getIndex(), dummyDatas);
					
					// [1.����� (100Byte)]
					// �ĺ� �ڵ� (0~8)
					rtRecord.setData("h_idCode", null);
					
					// ��ü�ڵ� (9~16)
					rtRecord.setData("h_companyCode", envVarMap.get("FB_PARENT_COMP_CODE"));
					
					// �����ڵ�2 (17~18)
					final byte[] bankCode3 = envVarMap.get("FB_PARENT_BANK_CODE_3");
					final byte[] bankCode2 = Arrays.copyOfRange(bankCode3, 1, 3);
					rtRecord.setData("h_bankCode2", bankCode2);
					
					// �޽����ڵ� (19~22)
					final byte[] messageCode = { '0', '1', '0', '0' };
					rtRecord.setData("h_msgCode", messageCode);
					
					// ���������ڵ� (23~25)
					final byte[] workTypeCode = localVarMap.get("loc_hana_svr_h_workTypeCode");
					rtRecord.setData("h_workTypeCode", workTypeCode);
					
					// �۽�Ƚ�� (26)
					final byte[] transferCnt = { '1' };
					rtRecord.setData("h_transferCnt", transferCnt);
					
					// ������ȣ (27~32)
					rtRecord.setData("h_msgNum", inRecord.getData("d_dataSerialNum"));
					
					// �������� (33~40)
					final byte[] transferDate = { (byte)year.charAt(0),  (byte)year.charAt(1), (byte)year.charAt(2), (byte)year.charAt(3),
												  (byte)month.charAt(0), (byte)month.charAt(1), 
												  (byte)date.charAt(0),  (byte)date.charAt(1) };
					rtRecord.setData("h_transferDate", transferDate);
					
					// ���۽ð� (41~46)
					final byte[] transferTime = { (byte)hour.charAt(0), (byte)hour.charAt(1),
												  (byte)min.charAt(0),  (byte)min.charAt(1),
												  (byte)sec.charAt(0),  (byte)sec.charAt(1) };
					rtRecord.setData("h_transferTime", transferTime);
					
					// �����ڵ� (47~50)
					rtRecord.setData("h_responseCode", null);
					
					// ���������ڵ� (51~54)
					rtRecord.setData("h_bankResponseCode", null);
					
					// ��ȸ���� (55~62)
					rtRecord.setData("h_lookupDate", null);
					
					// ��ȸ��ȣ (63~68)
					rtRecord.setData("h_lookupNum", null);
					
					// ����������ȣ (69~83)
					rtRecord.setData("h_bankMsgNum", null);
					
					// �����ڵ�3 (84~86)
					rtRecord.setData("h_bankCode3", bankCode3);
					
					// ���� (87~99)
					rtRecord.setData("h_spare", null);
					
					// [2.������ (200Byte)]
					// [�۱���ü/������ü]
					if (Arrays.equals(messageCode, envVarMap.get("MessageCode_0100"))) {
						if (Arrays.equals(workTypeCode, envVarMap.get("WorkTypeCode_100"))) {
							// ��ݰ��¹�ȣ (100~114)
							rtRecord.setData("dt_withdrawalAccountNum", envVarMap.get("FB_PARENT_ACCOUNT_NUMB"));
							
							// �����й�ȣ (115~122)
							rtRecord.setData("dt_bankBookPassword", null);
							
							// �����ȣ (123~128)
							rtRecord.setData("dt_recoveryCode", null);
							
							// ��ݱݾ� (129~141)
							rtRecord.setData("dt_withdrawalAmount", inRecord.getData("d_requestTransferPrice"));
							
							// ������ܾ׺�ȣ (142)
							rtRecord.setData("dt_afterWithdrawalBalanceSign", null);
							
							// ������ܾ� (143~155)
							rtRecord.setData("dt_afterWithdrawalBalance", null);
							
							// �Ա������ڵ�2 (156~157)
							final byte[] depositBankCode3 = inRecord.getData("d_bankCode");
							final byte[] depositBankCode2 = Arrays.copyOfRange(depositBankCode3, 1, 3);
							rtRecord.setData("dt_depositBankCode2", depositBankCode2);
							
							// �Աݰ��¹�ȣ (158~172)
							rtRecord.setData("dt_depositAccountNum", inRecord.getData("d_accountNum"));
							
							// ������ (173~181)
							rtRecord.setData("dt_fees", null);
							
							// ��ü�ð� (182~187)
							rtRecord.setData("dt_transferTime", transferTime);
							
							// �Աݰ������� (188~207)
							rtRecord.setData("dt_depositAccountBriefs", envVarMap.get("FB_PARENT_COMP_NAME"));
							
							// CMS�ڵ� (208~223)
							rtRecord.setData("dt_cmsCode", null);
							
							// �ſ�Ȯ�ι�ȣ (224~236)
							rtRecord.setData("dt_identificationNum", null);
							
							// �ڵ���ü���� (237~238)
							rtRecord.setData("dt_autoTransferClassification", null);
							
							// ��ݰ������� (239~258)
							rtRecord.setData("dt_withdrawalAccountBriefs", inRecord.getData("d_briefs"));
							
							// �Ա������ڵ�3 (259~261)
							rtRecord.setData("dt_depositBankCode3", depositBankCode3);
							
							// �޿����� (262)
							rtRecord.setData("dt_salaryClassification", null);
							
							// ���� (263~299)
							rtRecord.setData("dt_spare", null);
						}
						// [����]
						else {
							Logger.logln(Logger.LogType.LT_ERR, "�� �� ���� �޽����ڵ�. (workTypeCode: " + new String(workTypeCode) + ", messageCode: " + new String(messageCode) + ")");
							return null;
						}
					}
					// [ó�������ȸ,�ܾ���ȸ,������ȸ]
					else if(Arrays.equals(messageCode, envVarMap.get("MessageCode_0600"))) {
						// [ó�������ȸ]
						if (Arrays.equals(workTypeCode, envVarMap.get("WorkTypeCode_101"))) {
							// ���� ��� �߰�...
						}
						// [�ܾ���ȸ]
						else if (Arrays.equals(workTypeCode, envVarMap.get("WorkTypeCode_300"))) {
							// ���� ��� �߰�...
						}
						// [������ȸ]
						else if (Arrays.equals(workTypeCode, envVarMap.get("WorkTypeCode_400"))) {
							// ���� ��� �߰�...
						}
						// [����]
						else {
							Logger.logln(Logger.LogType.LT_ERR, "�� �� ���� ���������ڵ�. (" + new String(workTypeCode) + ")");
							return null;
						}
					}
					// [����]
					else {
						Logger.logln(Logger.LogType.LT_ERR, "�� �� ���� �޽����ڵ�. (workTypeCode: " + new String(workTypeCode) + ", messageCode: " + new String(messageCode) + ")");
						return null;
					}
				}
				else if (inRecordSubTypeName.equals("Head")) {
					// ǥ���� - ���������ڵ� ���� ������ ����
					byte[] cliWorkTypeCode = inRecord.getData("h_taskComp");
					byte[] svrWorkTypeCode = new byte[cliWorkTypeCode.length + 1];
					
					for (int i = 0; i < cliWorkTypeCode.length; ++i) {
						svrWorkTypeCode[i] = cliWorkTypeCode[i];
					}
					
					svrWorkTypeCode[cliWorkTypeCode.length] = (byte)'0';
					localVarMap.put("loc_hana_svr_h_workTypeCode", svrWorkTypeCode);
				}
				else if (inRecordSubTypeName.equals("Tail")) {
					// ����� - ������ ������ ����
				}
			}
			else {
				Logger.logln(Logger.LogType.LT_ERR, "�ùٸ��� ���� inRecordTypeName. (\"" + inRecordTypeName + "\")");
			}
		}
		// Server -> Client
		else if (outRecordTypeName.equals("HanaRecordClient")) {
			if (outRecordSubTypeName.equals("Data")) {
				// [�����ͺ� (82Byte)]
				int recordLength = attrMgr.getRecordSizeFromAttributeMap("HanaAttrClient_Data");
				byte[] dummyDatas = new byte[recordLength];
				rtRecord = new Record("HanaRecordClient", "Data", inRecord.getIndex(), dummyDatas);

				// �ĺ��ڵ� (0)
				rtRecord.setDataByDefault("d_idCode");
				
				// ������ �Ϸù�ȣ (1~6)
				rtRecord.setData("d_dataSerialNum", inRecord.getData("h_msgNum"));
				
				// �����ڵ� (7~10)
				rtRecord.setData("d_bankCode", inRecord.getData("dt_depositBankCode3"));
	
				// ���¹�ȣ (10~23)
				rtRecord.setData("d_accountNum", inRecord.getData("dt_depositAccountNum"));
				
				// ��ü��û�ݾ� (24~34)
				final byte[] withdrawlAmount = inRecord.getData("dt_withdrawalAmount");
				rtRecord.setData("d_requestTransferPrice", withdrawlAmount);
				{
					// �� �Ƿ�Ƚ�� ����
					final byte[] savedTotalRequestCnt = localVarMap.get("loc_hana_cli_t_totalRequestCnt");
					if (savedTotalRequestCnt != null) { 
						localVarMap.put("loc_hana_cli_t_totalRequestCnt", Long.toString(Long.parseLong(new String(savedTotalRequestCnt)) + 1).getBytes());
					}
					else {
						localVarMap.put("loc_hana_cli_t_totalRequestCnt", "1".getBytes());
					}
					
					// �� �Ƿڱݾ� ����
					final byte[] savedTotalRequestAmount = localVarMap.get("loc_hana_cli_t_totalRequestPrice");
					if (savedTotalRequestAmount != null) { 
						localVarMap.put("loc_hana_cli_t_totalRequestPrice", Long.toString(Long.parseLong(new String(savedTotalRequestAmount)) + Long.parseLong(new String(withdrawlAmount))).getBytes());
					}
					else {
						localVarMap.put("loc_hana_cli_t_totalRequestPrice", withdrawlAmount);
					}
				}

				// ������ü�ݾ� (35~45)
				rtRecord.setData("d_realTransferPrice", inRecord.getData("dt_withdrawalAmount"));
				
				// �ֹ�/����ڹ�ȣ (46~58)
				rtRecord.setData("d_recieverIdNum", null);
				
				// ó����� (59)
				final byte[] bankResponseCode = inRecord.getData("h_bankResponseCode");
				if (Arrays.equals(bankResponseCode, envVarMap.get("ProcessingResultOk"))) { // ���� ó��
					final byte[] procY = { 'Y' };
					rtRecord.setData("d_processingResult", procY);
					{
						// ����ó���Ǽ� ����
						final byte[] savedNormalProcCnt = localVarMap.get("loc_hana_cli_t_normalProcessingCnt");
						if (savedNormalProcCnt != null) {
							localVarMap.put("loc_hana_cli_t_normalProcessingCnt", Long.toString(Long.parseLong(new String(savedNormalProcCnt)) + 1).getBytes());
						}
						else {
							localVarMap.put("loc_hana_cli_t_normalProcessingCnt", "1".getBytes());
						}
						
						// ����ó���ݾ� ����
						final byte[] savedNormalPriceCnt = localVarMap.get("loc_hana_cli_t_normalPriceCnt");
						if (savedNormalPriceCnt != null) { 
							localVarMap.put("loc_hana_cli_t_normalPriceCnt", Long.toString(Long.parseLong(new String(savedNormalPriceCnt)) + Long.parseLong(new String(withdrawlAmount))).getBytes());
						}
						else {
							localVarMap.put("loc_hana_cli_t_normalPriceCnt", withdrawlAmount);
						}
					}
				}
				else { // �Ҵ� ó��
					final byte[] procN = { 'N' };
					rtRecord.setData("d_processingResult", procN);
					{
						// �Ҵ�ó���Ǽ� ����
						final byte[] savedDisableProcCnt = localVarMap.get("loc_hana_cli_t_disableProcessingCnt");
						if (savedDisableProcCnt != null) {
							localVarMap.put("loc_hana_cli_t_disableProcessingCnt", Long.toString(Long.parseLong(new String(savedDisableProcCnt)) + 1).getBytes());
						}
						else {
							localVarMap.put("loc_hana_cli_t_disableProcessingCnt", "1".getBytes());
						}
						
						// �Ҵ�ó���ݾ� ����
						final byte[] savedDisablePriceCnt = localVarMap.get("loc_hana_cli_t_disablePriceCnt");
						if (savedDisablePriceCnt != null) { 
							localVarMap.put("loc_hana_cli_t_disablePriceCnt", Long.toString(Long.parseLong(new String(savedDisablePriceCnt)) + Long.parseLong(new String(withdrawlAmount))).getBytes());
						}
						else {
							localVarMap.put("loc_hana_cli_t_disablePriceCnt", withdrawlAmount);
						}
					}
				}
				
				// �Ҵ��ڵ� (60~63)
				rtRecord.setData("d_disableCode", bankResponseCode);
				
				// ���� (64~75)
				rtRecord.setData("d_briefs", inRecord.getData("dt_withdrawalAccountBriefs"));
				
				// ���� (76~79)
				rtRecord.setData("d_blank", null);
				
				// ���๮�� (80~81)
				rtRecord.setData("d_newLine", NEW_LINE);
			}
			else if (outRecordSubTypeName.equals("Head")) {
				// [ǥ���� (82Byte)]
				int recordLength = attrMgr.getRecordSizeFromAttributeMap("HanaAttrClient_Head");
				byte[] dummyDatas = new byte[recordLength];
				rtRecord = new Record("HanaRecordClient", "Head", 0, dummyDatas);
				
				// �ĺ� �ڵ� (0)
				rtRecord.setDataByDefault("h_idCode");
				
				// ���� ���� (1~2)
				rtRecord.setDataByDefault("h_taskComp");
				
				// ���� �ڵ� (3~5)
				rtRecord.setDataByDefault("h_bankCode");
				
				// ��ü �ڵ� (6~13)
				rtRecord.setDataByDefault("h_companyCode");
				
				// ��ü�Ƿ����� (14~19)
				rtRecord.setDataByDefault("h_comissioningDate");
				
				// ��üó������ (20~25)
				final byte[] h_processingDate = { (byte)year.charAt(0),  (byte)year.charAt(1), (byte)year.charAt(2), (byte)year.charAt(3),
												  (byte)month.charAt(0), (byte)month.charAt(1), 
												  (byte)date.charAt(0),  (byte)date.charAt(1) };
				rtRecord.setData("h_processingDate", h_processingDate);
				
				// ����¹�ȣ (26~39)
				rtRecord.setDataByDefault("h_motherAccountNum");
				
				// ��ü���� (40~41)
				rtRecord.setDataByDefault("h_transferType");
				
				// ȸ���ȣ (42~47)
				rtRecord.setDataByDefault("h_companyNum");
				
				// ó������뺸���� (48)
				rtRecord.setDataByDefault("h_resultNotifyType");
				
				// �������� (49)
				rtRecord.setDataByDefault("h_transferCnt");
				
				// ��й�ȣ (50~57)
				rtRecord.setDataByDefault("h_password");
				
				// ���� (58~76)
				rtRecord.setData("h_blank", null);
				
				// Format (77)
				rtRecord.setDataByDefault("h_format");
				
				// VAN (78~79)
				final byte[] bVan = { 'K', 'C' };
				rtRecord.setData("h_van", bVan);
				
				// ���๮�� (80~81)
				rtRecord.setData("h_newLine", NEW_LINE);
			}
			else if (outRecordSubTypeName.equals("Tail")) {
				// [����� (82Byte)]
				int recordLength = attrMgr.getRecordSizeFromAttributeMap("HanaAttrClient_Tail");
				byte[] dummyDatas = new byte[recordLength];
				
				rtRecord = new Record("HanaRecordClient", "Tail", Integer.parseInt(new String(localVarMap.get("loc_hana_cli_t_totalRequestCnt"))), dummyDatas);
				
				// �ĺ��ڵ� (0)
				rtRecord.setDataByDefault("t_idCode");
				
				// ���ǷڰǼ� (1~7)
				rtRecord.setData("t_totalRequestCnt", localVarMap.get("loc_hana_cli_t_totalRequestCnt"));
				
				// ���Ƿڱݾ� (8~20)
				rtRecord.setData("t_totalRequestPrice", localVarMap.get("loc_hana_cli_t_totalRequestPrice"));
				
				// ����ó���Ǽ� (21~27)
				rtRecord.setData("t_normalProcessingCnt", localVarMap.get("loc_hana_cli_t_normalProcessingCnt"));
				
				// ����ó���ݾ� (28~40)
				rtRecord.setData("t_normalProcessingPrice", localVarMap.get("loc_hana_cli_t_normalPriceCnt"));
				
				// �Ҵ�ó���Ǽ� (41~47)
				rtRecord.setData("t_disableProcessingCnt", localVarMap.get("loc_hana_cli_t_disableProcessingCnt"));
				
				// �Ҵ�ó���ݾ� (48~60)
				rtRecord.setData("t_disableProcessingPrice", localVarMap.get("loc_hana_cli_t_disablePriceCnt"));
				
				// �����ȣ (61~68)
				rtRecord.setDataByDefault("t_recoveryCode");
				
				// ���� (69~79)
				rtRecord.setData("t_blank", null);
				
				// ���๮�� (80~81)
				rtRecord.setData("t_newLine", NEW_LINE);
			}
		}
		
		return rtRecord;
	}
}