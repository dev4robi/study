package com.ksnet.main.client;

import java.io.File;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Collections;

import com.ksnet.net.*;
import com.ksnet.util.*;

public class ClientMain {	
	// ����
	public static void main(String[] args) {
		// ����
		long beginTime = 0, endTime = 0, runTime = 0;
		HashMap<String, byte[]> envVarMap = new HashMap<String, byte[]>();
		KsFileReader ksFileReader = null;
        KsFileWriter ksFileWriter = null;
		RecordConverter svrRecordConverter = null, cliRecordConverter = null;
		RecordTransceiver recordTransceiver = null;
		RecordPrinter recordPrinter = null;
		
		// �ʱ�ȭ
		try {
			System.out.println("========================================================================");

			beginTime = System.currentTimeMillis();
			
			init(envVarMap, args);
			
			endTime = System.currentTimeMillis();
			runTime = endTime - beginTime;
			
			System.out.println("> �ʱ�ȭ �Ϸ� : " + (runTime / 1000.0) + "��");
			beginTime = System.currentTimeMillis();
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			
			// ���� �б� �۾��� �񵿱� ���� �۾��� ���� Ŭ���� (�񵿱� ���� �۾���, ����->Ŭ�� ���ڵ� ��ȯ ����)
			String inFilePath = new String(envVarMap.get("INPUT_FILE_PATH"));
			String outFilePath = new String(envVarMap.get("OUTPUT_FILE_PATH"));
			
			KsFileWriter.copyFromFile(new File(inFilePath), new File(outFilePath)); // ���� ����
			
			ksFileReader = new KsFileReader(inFilePath);
			ksFileWriter = new KsFileWriter(outFilePath, AttributeManager.getInst().getRecordSizeFromAttributeMap("HanaAttrClient_Data"));
			
			// Ŭ��->����->Ŭ�� ���ڵ� ��ȯ�� ���� Ŭ����
			svrRecordConverter = new RecordConverter(null, "HanaRecordServer", "", envVarMap); // ������ ��ȯ
			cliRecordConverter = new RecordConverter(null, "HanaRecordClient", "", envVarMap); // Ŭ��� ��ȯ
			
			// ������ �����͸� ������ Ŭ����
			String svrIp = new String(envVarMap.get("FB_IP"));
			int svrPort = Integer.parseInt(new String(envVarMap.get("FB_PORT")));
			

			boolean reusableSocketMode = new String(envVarMap.get("REUSABLE_SOCKET_MODE")).toUpperCase().equals("TRUE") ? true : false;
			int socketCnt = Integer.parseInt(new String(envVarMap.get("SOCKET_CNT")));
			
			recordTransceiver = new RecordTransceiver(reusableSocketMode, svrIp, svrPort, socketCnt, 50,
													  cliRecordConverter, ksFileWriter, envVarMap);
			
			// ���ڵ� ����� ���� Ŭ����
			recordPrinter = new RecordPrinter(null);
			
			// ��� ���� ǥ���� ����
			cliRecordConverter.setOutRecordSubTypeName("Head");
			ksFileWriter.write(cliRecordConverter.convert(new Record("HanaRecordServer", "", 0, null)).toByteAry(), 0);
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_CRIT, e);
			finishProgram(recordTransceiver, cliRecordConverter, svrRecordConverter, ksFileReader, ksFileWriter, 0);
		}
		
		// ���� ������ ���κ��� �����鼭 ��ȯ �� ����, ���Ϸ� ���� ����
		int lineCnt = 0;
		
		try {
			for (; ; ++lineCnt) {
				// ���� ���� ���κ� �б�
				String recordStr = ksFileReader.readLine();
				
				// ���̻� ���� ������ ������ Ż��
				if (recordStr == null) break;

				// ���� �������� ���ڵ� ����
				Record cliRecord = new Record("HanaRecordClient", lineCnt, recordStr.getBytes());
				
				// ���� ���ڵ�� ��ȯ
				Record toSvrRecord = svrRecordConverter.convert(cliRecord);
				
				// �����ͺθ� ������ ����
				while (true) {
					sleep(1);
					
					if (recordTransceiver.isSendWaittingListFull()) {
						continue;
					}
					else {
						if (toSvrRecord != null) {
							recordTransceiver.send(toSvrRecord);
							
							if (lineCnt % 10 == 0) {
								recordTransceiver.printWorkLeft();
							}
						}

						break;
					}
				}
			}
			
			// ��� ���ڵ��� ���۰� ���� ���Ⱑ �Ϸ�� ������ ���
			long printInterval = 1000;
			long nextSysMsgTime = 0;
			
			while (!recordTransceiver.checkTransceiverFinished()) {
				if (System.currentTimeMillis() > nextSysMsgTime) {
					recordTransceiver.printWorkLeft();
					nextSysMsgTime = System.currentTimeMillis() + printInterval;
				}
				
				sleep(1);
			}
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_CRIT, e);
		}
		finally {
			finishProgram(recordTransceiver, cliRecordConverter, svrRecordConverter, ksFileReader, ksFileWriter, lineCnt);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			
			// ����
			endTime = System.currentTimeMillis();
			runTime = endTime - beginTime;
			System.out.println("> ���� �ۼ��� �Ϸ� : " + (runTime / 1000.0) + "��");
			System.out.println("========================================================================");
		}
	}
	
	// �ʱ�ȭ
	public static void init(HashMap<String, byte[]> envVarMap, String[] args) {
		// ȯ�溯�� �ؽ� �ʱ�ȭ
		if (envVarMap == null) return;
		
		envVarMap.put("INPUT_FILE_PATH", args[0].getBytes()); 
		envVarMap.put("OUTPUT_FILE_PATH", args[1].getBytes());
		envVarMap.put("ATTR_CONFIG_FILE_PATH", args[2].getBytes());

		envVarMap.put("FB_IP", args[3].getBytes());
		envVarMap.put("FB_PORT", args[4].getBytes());
		envVarMap.put("FB_PARENT_BANK_CODE_3", args[5].getBytes());
		envVarMap.put("FB_PARENT_COMP_CODE", args[6].getBytes());
		envVarMap.put("FB_PARENT_ACCOUNT_NUMB", args[7].getBytes());
		envVarMap.put("FB_REQ_FILE", args[8].getBytes());
		envVarMap.put("FB_MSG_NUMB_S", args[9].getBytes());
		envVarMap.put("FB_PARENT_COMP_NAME", args[10].getBytes());
		
		envVarMap.put("REUSABLE_SOCKET_MODE", args[11].getBytes());
		envVarMap.put("SOCKET_CNT", args[12].getBytes());
		envVarMap.put("SOCKET_THREAD_TIMEOUT", args[13].getBytes());
		envVarMap.put("RECORD_RESEND_MAX_TRY", args[14].getBytes());
		envVarMap.put("RECORD_RESEND_DELAY", args[15].getBytes());
		envVarMap.put("RECORD_TGT_SEND_PER_SEC", args[16].getBytes());
		envVarMap.put("LOG_LEVEL", args[17].getBytes());

		envVarMap.put("MessageCode_0100", "0100".getBytes());
		envVarMap.put("MessageCode_0600", "0600".getBytes());
		envVarMap.put("WorkTypeCode_100", "100".getBytes());
		envVarMap.put("WorkTypeCode_101", "101".getBytes());
		envVarMap.put("WorkTypeCode_300", "300".getBytes());
		envVarMap.put("WorkTypeCode_400", "400".getBytes());
		envVarMap.put("ProcessingResultOk", "0000".getBytes());
		
		// �Ŵ��� �ʱ�ȭ
		try {
			AttributeManager.InitManager(new String(envVarMap.get("ATTR_CONFIG_FILE_PATH")));
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_ERR, e);
		}
		
		// �ΰ� �ʱ�ȭ
		String logLevel = new String(envVarMap.get("LOG_LEVEL"));
		
		Logger.setVisibleByLogLevel(logLevel);
	}
	
	// ���α׷� ������
	public static void finishProgram(RecordTransceiver recordTransceiver, RecordConverter cliRecordConverter, RecordConverter svrRecordConverter, KsFileReader ksFileReader, KsFileWriter ksFileWriter, int lineCnt) {
		try {
			// ���� ���� �ݱ�
			if (ksFileReader != null) ksFileReader.close();
			
			// ���۱� ����
			if (recordTransceiver != null) recordTransceiver.close();
			
			// ��� ���� ����� ����
			if (cliRecordConverter != null) cliRecordConverter.setOutRecordSubTypeName("Tail");
			if (ksFileWriter != null) ksFileWriter.write(cliRecordConverter.convert(new Record("HanaRecordServer", "", lineCnt, null)).toByteAry(), lineCnt - 1);
			
			// ���ڵ� ��ȯ�� �� ���� �ݱ�
			if (cliRecordConverter != null) cliRecordConverter.close();
			if (ksFileWriter != null) ksFileWriter.close();
		}
		catch (Exception e) {
			System.out.println("========================================================================");
			System.exit(-1);
		}
	}
	
	// ������ ���
	public static void sleep(int ms) {
		try {
			Thread.sleep(ms);
		}
		catch (InterruptedException ie) {
			Logger.logln(Logger.LogType.LT_ERR, ie);
		}
	}
}