package com.ksnet.net;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Map;
import java.util.HashMap;
import java.util.ArrayList;

import com.ksnet.util.*;

public class AttributeManager {
	
	public static final String KEYWORD_ATTRFILES = "attrFiles:";
	
	private static AttributeManager attributeManager; // �̱��� Ŭ����
	private static HashMap<String, HashMap<String, Attribute>> attributeMapMap;	// Attribute�������� ���� ���� ���� ��
	private static HashMap<String, Integer> attributeSizeMap; // Attribute���� ũ�⸦ ���� ��
	
	// ������
	private AttributeManager() {}
	
	// �ʱ�ȭ
	public static void InitManager(String configFilePath) throws Exception {
		// �̱��� ��ü ����
		attributeManager = new AttributeManager();
		
		// �ؽø� �ʱ�ȭ
		attributeMapMap = new HashMap<String, HashMap<String, Attribute>>();
		attributeSizeMap = new HashMap<String, Integer>();
		
		// �������� ���� ���ڿ� ����Ʈ�� ��ȯ
		KsFileReader ksFileReader = new KsFileReader(configFilePath);
		ArrayList<String> configStrList = ksFileReader.readLines();
		
		// �������� KEYWORD_ATTRFILES Ű���� �˻�
		ArrayList<String> configFilePathList = new ArrayList<String>();
		for (String configStr : configStrList) {
			int attrFilesBeginIndex = configStr.indexOf(KEYWORD_ATTRFILES);
			
			if (attrFilesBeginIndex != -1) {
				String configFilePathStr = configStr.substring(attrFilesBeginIndex + KEYWORD_ATTRFILES.length(), configStr.length());
				
				for (String filePath : configFilePathStr.split(",")) {
					configFilePathList.add(filePath.trim());
				}

				break;
			}
		}

		// �Ӽ����� �о AttributeMapMap ä������
		//for (String attrFilePath : configFilePathList) {
		//	updateAttributeMapMap(attrFilePath);
		//}
		
		// �ӽ÷� �ϵ��ڵ�
		HashMap<String, Attribute> attrMap = new HashMap<String, Attribute>();
		attrMap.put("h_idCode",					new Attribute(1,	"�ĺ��ڵ�",		"h_idCode",					"X",	0,	1,	"S".getBytes()			));
		attrMap.put("h_taskComp",				new Attribute(2,	"��������",		"h_taskComp",				"X",	1,	2,	"10".getBytes()			));
		attrMap.put("h_bankCode",				new Attribute(3,	"�����ڵ�",		"h_bankCode",				"9",	3,	3,	"081".getBytes()		));
		attrMap.put("h_companyCode",			new Attribute(4,	"��ü�ڵ�",		"h_companyCode",			"X",	6,	8,	"KSANP001".getBytes()	));
		attrMap.put("h_comissioningDate",		new Attribute(5,	"��ü�Ƿ�����",		"h_comissioningDate",		"9",	14,	6,	"180404".getBytes()		));
		attrMap.put("h_processingDate",			new Attribute(6,	"��üó������",		"h_processingDate",			"9",	20,	6,	null					));
		attrMap.put("h_motherAccountNum",		new Attribute(7,	"����¹�ȣ",		"h_motherAccountNum",		"9",	26,	14,	"25791005094404".getBytes()	));
		attrMap.put("h_transferType",			new Attribute(8,	"��ü����",		"h_transferType",			"9",	40,	2,	"51".getBytes()			));
		attrMap.put("h_companyNum",				new Attribute(9,	"ȸ���ȣ",		"h_companyNum",				"9",	42,	6,	"000000".getBytes()		));
		attrMap.put("h_resultNotifyType",		new Attribute(10,	"ó������뺸����",	"h_resultNotifyType",		"X",	48,	1,	"1".getBytes()			));
		attrMap.put("h_transferCnt",			new Attribute(11,	"��������",		"h_transferCnt",			"X",	49,	1,	"1".getBytes()			));
		attrMap.put("h_password",				new Attribute(12,	"��й�ȣ",		"h_password",				"X",	50,	8,	"4380".getBytes()		));
		attrMap.put("h_blank",					new Attribute(13,	"����",			"h_blank",					"X",	58,	19,	null					));
		attrMap.put("h_format",					new Attribute(14,	"Format",		"h_format",					"X",	77,	1,	"1".getBytes()			));
		attrMap.put("h_van",					new Attribute(15,	"VAN",			"h_van",					"X",	78,	2,	null					));
		attrMap.put("h_newLine",				new Attribute(16,	"���๮��",		"h_newLine",				"X",	80,	2,	null					));
		attributeMapMap.put("HanaAttrClient_Head", attrMap);
		
		attrMap = new HashMap<String, Attribute>();
		attrMap.put("d_idCode",					new Attribute(1,	"�ĺ��ڵ�",		"d_idCode",					"X",	0,	1,	"D".getBytes()			));
		attrMap.put("d_dataSerialNum",			new Attribute(2,	"������ �Ϸù�ȣ",	"d_dataSerialNum",			"9",	1,	6,	null					));
		attrMap.put("d_bankCode",				new Attribute(3,	"�����ڵ�",		"d_bankCode",				"9",	7,	3,	null					));
		attrMap.put("d_accountNum",				new Attribute(4,	"���¹�ȣ",		"d_accountNum",				"X",	10,	14,	null					));
		attrMap.put("d_requestTransferPrice",	new Attribute(5,	"��ü��û�ݾ�",		"d_requestTransferPrice",	"9",	24,	11,	null					));
		attrMap.put("d_realTransferPrice",		new Attribute(6,	"������ü�ݾ�",		"d_realTransferPrice",		"9",	35,	11,	null					));
		attrMap.put("d_recieverIdNum",			new Attribute(7,	"�ֹ�/����ڹ�ȣ",	"d_recieverIdNum",			"X",	46,	13,	null					));
		attrMap.put("d_processingResult",		new Attribute(8,	"ó�����",		"d_processingResult",		"X",	59,	1,	null					));
		attrMap.put("d_disableCode",			new Attribute(9,	"�Ҵ��ڵ�",		"d_disableCode",			"X",	60,	4,	null					));
		attrMap.put("d_briefs",					new Attribute(10,	"����",			"d_briefs",					"X",	64,	12,	null					));
		attrMap.put("d_blank",					new Attribute(11,	"����",			"d_blank",					"X",	76,	4,	null					));
		attrMap.put("d_newLine",				new Attribute(12,	"���๮��",		"d_newLine",				"X",	80,	2,	null					));
		attributeMapMap.put("HanaAttrClient_Data", attrMap);
		
		attrMap = new HashMap<String, Attribute>();
		attrMap.put("t_idCode",					new Attribute(1,	"�ĺ��ڵ�",		"t_idCode",					"X",	0,	1,	"E".getBytes()			));
		attrMap.put("t_totalRequestCnt",		new Attribute(2,	"���ǷڰǼ�",		"t_totalRequestCnt",		"9",	1,	7,	null					));
		attrMap.put("t_totalRequestPrice",		new Attribute(3,	"���Ƿڱݾ�",		"t_totalRequestPrice",		"9",	8,	13,	null					));
		attrMap.put("t_normalProcessingCnt",	new Attribute(4,	"����ó���Ǽ�",		"t_normalProcessingCnt",	"9",	21,	7,	null					));
		attrMap.put("t_normalProcessingPrice",	new Attribute(5,	"����ó���ݾ�",		"t_normalProcessingPrice",	"9",	28,	13,	null					));
		attrMap.put("t_disableProcessingCnt",	new Attribute(6,	"�Ҵ�ó���Ǽ�",		"t_disableProcessingCnt",	"9",	41,	7,	null					));
		attrMap.put("t_disableProcessingPrice",	new Attribute(7,	"�Ҵ�ó���ݾ�",		"t_disableProcessingPrice",	"9",	48,	13,	null					));
		attrMap.put("t_recoveryCode",			new Attribute(8,	"�����ȣ",		"t_recoveryCode",			"X",	61,	8,	"3706".getBytes()		));
		attrMap.put("t_blank",					new Attribute(9,	"����",			"t_blank",					"X",	69,	11,	null					));
		attrMap.put("t_newLine",				new Attribute(10,	"���๮��",		"t_newLine",				"X",	80,	2,	null					));
		attributeMapMap.put("HanaAttrClient_Tail", attrMap);
		
		attrMap = new HashMap<String, Attribute>();
		attrMap.put("h_idCode",							new Attribute(1,	"�ĺ��ڵ�",		"h_idCode",							"C",	0,		9,	null));
		attrMap.put("h_companyCode",					new Attribute(2,	"��ü�ڵ�",		"h_companyCode",					"C",	9,		8,	null));
		attrMap.put("h_bankCode2",						new Attribute(3,	"�����ڵ�2",		"h_bankCode2",						"C",	17,		2,	null));
		attrMap.put("h_msgCode",						new Attribute(4,	"�޽����ڵ�",		"h_msgCode",						"C",	19,		4,	null));
		attrMap.put("h_workTypeCode",					new Attribute(5,	"���������ڵ�",		"h_workTypeCode",					"C",	23,		3,	null));
		attrMap.put("h_transferCnt",					new Attribute(6,	"�۽�Ƚ��",		"h_transferCnt",					"C",	26,		1,	null));
		attrMap.put("h_msgNum",							new Attribute(7,	"������ȣ",		"h_msgNum",							"N",	27,		6,	null));
		attrMap.put("h_transferDate",					new Attribute(8,	"��������",		"h_transferDate",					"D",	33,		8,	null));
		attrMap.put("h_transferTime",					new Attribute(9,	"���۽ð�",		"h_transferTime",					"T",	41,		6,	null));
		attrMap.put("h_responseCode",					new Attribute(10,	"�����ڵ�",		"h_responseCode",					"C",	47,		4,	null));
		attrMap.put("h_bankResponseCode",				new Attribute(11,	"���� �����ڵ�",		"h_bankResponseCode",				"C",	51,		4,	null));
		attrMap.put("h_lookupDate",						new Attribute(12,	"��ȸ����",		"h_lookupDate",						"D",	55,		8,	null));
		attrMap.put("h_lookupNum",						new Attribute(13,	"��ȸ��ȣ",		"h_lookupNum",						"N",	63,		6,	null));
		attrMap.put("h_bankMsgNum",						new Attribute(14,	"����������ȣ",		"h_bankMsgNum",						"C",	69,		15,	null));
		attrMap.put("h_bankCode3",						new Attribute(15,	"�����ڵ�3",		"h_bankCode3",						"C",	84,		3,	null));
		attrMap.put("h_spare",							new Attribute(16,	"����",			"h_spare",							"C",	87,		13,	null));
		attrMap.put("dt_withdrawalAccountNum",			new Attribute(17,	"��� ���¹�ȣ",		"dt_withdrawalAccountNum",			"C",	100,	15,	null));
		attrMap.put("dt_bankBookPassword",				new Attribute(18,	"���� ��й�ȣ",		"dt_bankBookPassword",				"C",	115,	8,	null));
		attrMap.put("dt_recoveryCode",					new Attribute(19,	"�����ȣ",		"dt_recoveryCode",					"C",	123,	6,	null));
		attrMap.put("dt_withdrawalAmount",				new Attribute(20,	"��� �ݾ�",		"dt_withdrawalAmount",				"N",	129,	13,	null));
		attrMap.put("dt_afterWithdrawalBalanceSign",	new Attribute(21,	"��� �� �ܾ׺�ȣ",	"dt_afterWithdrawalBalanceSign",	"C",	142,	1,	null));
		attrMap.put("dt_afterWithdrawalBalance",		new Attribute(22,	"��� �� �ܾ�",		"dt_afterWithdrawalBalance",		"N",	143,	13,	null));
		attrMap.put("dt_depositBankCode2",				new Attribute(23,	"�Ա� �����ڵ�2",	"dt_depositBankCode2",				"C",	156,	2,	null));
		attrMap.put("dt_depositAccountNum",				new Attribute(24,	"�Ա� ���¹�ȣ",		"dt_depositAccountNum",				"C",	158,	15,	null));
		attrMap.put("dt_fees",							new Attribute(25,	"������",			"dt_fees",							"N",	173,	9,	null));
		attrMap.put("dt_transferTime",					new Attribute(26,	"��ü �ð�",		"dt_transferTime",					"T",	182,	6,	null));
		attrMap.put("dt_depositAccountBriefs",			new Attribute(27,	"�Ա� ���� ����",	"dt_depositAccountBriefs",			"C",	188,	20,	null));
		attrMap.put("dt_cmsCode",						new Attribute(28,	"CMS�ڵ�",		"dt_cmsCode",						"C",	208,	16,	null));
		attrMap.put("dt_identificationNum",				new Attribute(29,	"�ſ�Ȯ�ι�ȣ",		"dt_identificationNum",				"C",	224,	13,	null));
		attrMap.put("dt_autoTransferClassification",	new Attribute(30,	"�ڵ���ü ����",		"dt_autoTransferClassification",	"C",	237,	2,	null));
		attrMap.put("dt_withdrawalAccountBriefs",		new Attribute(31,	"��� ���� ����",	"dt_withdrawalAccountBriefs",		"C",	239,	20,	null));
		attrMap.put("dt_depositBankCode3",				new Attribute(32,	"�Ա� �����ڵ�3",	"dt_depositBankCode3",				"C",	259,	3,	null));
		attrMap.put("dt_salaryClassification",			new Attribute(33,	"�޿� ����",		"dt_salaryClassification",			"C",	262,	1,	null));
		attrMap.put("dt_spare",							new Attribute(34,	"����",			"dt_spare",							"C",	263,	37,	null));
		attributeMapMap.put("HanaAttrServer", attrMap);
	}
	
	public static AttributeManager getInst() {
		return attributeManager;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	public HashMap<String, Attribute> copyOfAttributeMap(String attributeMapName) {
		HashMap<String, Attribute> rtMap = null;
		HashMap<String, Attribute> attributeMap = null;
			
		if ((attributeMap = attributeMapMap.get(attributeMapName)) != null) {
			rtMap = new HashMap<String, Attribute>();
			
			for (Map.Entry<String, Attribute> entry : attributeMap.entrySet()) { // ��ī�� ����
				rtMap.put(entry.getKey(), new Attribute(entry.getValue()));
			}
		}
		else {
			Logger.logln(Logger.LogType.LT_WARN, "\"" + attributeMapName + "\"���� Ű�� ���� attributeMapMap�� �����ϴ�.");
		}

		return rtMap;
	}
	
	public int getRecordSizeFromAttributeMap(String attributeMapName) {
		int rtInt = -1;
		HashMap<String, Attribute> attributeMap = null;

		if ((attributeSizeMap.get(attributeMapName)) != null) {
			rtInt = attributeSizeMap.get(attributeMapName);
		}
		else {
			if ((attributeMap = attributeMapMap.get(attributeMapName)) != null) {
				rtInt = 0;

				for (Map.Entry<String, Attribute> entry : attributeMap.entrySet()) {
					rtInt += entry.getValue().getByteLength();
				}
				
				attributeSizeMap.put(attributeMapName, rtInt);
			}
			else {
				Logger.logln(Logger.LogType.LT_WARN, "\"" + attributeMapName + "\"���� Ű�� ���� attributeMapMap�� �����ϴ�.");
			}
		}
		
		return rtInt;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	// .attr ���� �Ľ�
	private void updateAttributeMapMap(String attrFilePath) throws Exception {
		final int META_ROW_CNT = 2;
		final String KEY_NEWLINE = "\r\n";
		final String KEY_COMMENT = "COMMENT";
		final String KEY_STRING = "STRING";
		final String KEY_INT = "INT";
		final String KEY_BYTE = "BYTE";
		
		// ���ڿ� ��ķ� ���� �б�
		final String[][] strAry2D = cvtAttrFile2StringAry2D(attrFilePath);
		final int rowCnt = strAry2D.length - META_ROW_CNT;
		final int colCnt = strAry2D[0].length;

		// ��� �������� ����� ���� �� �ڷ��� ����Ʈ ����
		// (������ AttrRecord�� �ڷ����� �������ִ� '����'�� Ŭ��������, ���� .attr���� ���� ����� �ڷ����� ����
		//  '����'���� �Ӽ����� ������ �� �ֵ��� �ϱ� ���� �� �ڷ����� �������� �ľ��Ͽ� ����Ʈ�� ������ ��.)
		ArrayList<Integer> colIndexList = new ArrayList<Integer>();
		ArrayList<String> colTypeList = new ArrayList<String>();
		
		for (int col = 0; col < colCnt; ++col) {
			String keyWord = strAry2D[0][col];

			// �ּ� �÷�
			if (keyWord.equals(KEY_COMMENT)) {}
			// ���ڿ� �÷�
			else if (keyWord.equals(KEY_STRING)) {
				colIndexList.add(col);
				colTypeList.add(KEY_STRING);
			}
			// ���� �÷�
			else if (keyWord.equals(KEY_INT)) {
				colIndexList.add(col);
				colTypeList.add(KEY_INT);
			}
			// ����Ʈ �÷�
			else if (keyWord.equals(KEY_BYTE)) {
				colIndexList.add(col);
				colTypeList.add(KEY_BYTE);
			}
			// ���� (������ Ű����)
			else {
				throw new Exception("[����: �� �� ���� Ű���� (File: " + attrFilePath + "\"" + keyWord + "\", row: " + 0 + ", col: " + col + ")]");
			}
		}
		
		// attrMapMap�� attrMap�߰� (HanaAttrClient, HanaAttrServer ����. ���� ���� Attribute Ŭ������ ���� �ʿ�...)
		final String attrFileName = attrFilePath.substring(attrFilePath.lastIndexOf("/") + 1, attrFilePath.lastIndexOf("."));
		int orderCnt = 0;
		HashMap<String, Attribute> attributeMap = new HashMap<String, Attribute>();

		//for (int i = 0; i < colIndexList.size(); ++i) { // (���� �̷�������...)
			//String varType = colTypeList.get(i);
			//int col = colIndexList.get(i);
			// ......
		//}	
		
		try {
			if (attrFileName.equals("HanaAttrClient") || attrFileName.equals("HanaAttrServer")) { // �ϳ����� ���� (Ŭ���/������)
				for (int row = 2; row < rowCnt; ++row) {
					String name = strAry2D[row][1];
					String codeName = strAry2D[row][2];
					String type = strAry2D[row][3];
					int beginIndex = Integer.parseInt(strAry2D[row][4]);
					int byteLength = Integer.parseInt(strAry2D[row][5]);
					byte[] defaultValue = strAry2D[row][6].getBytes();

					attributeMap.put(codeName, new Attribute(row - 1, name, codeName, type, beginIndex, byteLength, defaultValue));
				}
			}
			else {
				throw new Exception("[����: (" + attrFileName + ")�� ������ ���� �����Դϴ�.]");
			}
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_ERR, e);
		}
		
		attributeMapMap.put(attrFileName, attributeMap);
	}
	
	private String[][] cvtAttrFile2StringAry2D(String attrFilePath) {
		// .attr���� ���ڿ��� ����
		KsFileReader ksFileReader = new KsFileReader(attrFilePath);
		ArrayList<String> attrFileStrList = ksFileReader.readLines();
		
		StringBuilder strBuilder = new StringBuilder();
		for (String lineStr : attrFileStrList) {
			System.out.print(">" + lineStr + "\r\n");
			strBuilder.append(lineStr).append("\r\n");
		}
		System.out.println();
		
		// ����� ũ�� ���ϱ�
		String fileStr = strBuilder.toString();
		String[] rowStrAry = fileStr.split("\r\n");			// �� ������ �迭
		final int rowCnt = rowStrAry.length - 1;			// ���� ���� (������ �� �� ����)
		final int colCnt = rowStrAry[0].split("\t").length;	// ���� ����
		String[][] strAry2D = new String[rowCnt][colCnt];	// ��� ������ �迭
		
		// ���� �����͸� ���ȭ
		for (int row = 0; row < rowCnt; ++row) {
			String[] colStrAry = rowStrAry[row].split("\t");
			
			for (int col = 0; col < colCnt; ++col) {
				if (col < colStrAry.length) {
					strAry2D[row][col] = colStrAry[col];
				}
				else {
					strAry2D[row][col] = "";
				}
			}
		}
		
		return strAry2D;
	}
}