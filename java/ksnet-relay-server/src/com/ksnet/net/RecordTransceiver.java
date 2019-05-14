package com.ksnet.net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.channels.IllegalBlockingModeException;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;
import java.util.LinkedList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.text.SimpleDateFormat;

import com.ksnet.util.*;

public class RecordTransceiver {

	private boolean reusableSocketMode;				// ���� ���� ���(true), ��ȸ�� ���� ���(false) ���

	private String ip;								// ���� ������
	private int port;								// ���� ��Ʈ
	private int sendTryCnt;							// ���� �õ� ����
	private long sendDelay;							// ���� ��� �ð�
	private int sendWaitingRecordListMaxSize;		// �ִ� ���۴�� ����ũ ũ��
	
	private List<Socket> socketList;					// �ۼ��� ���� ����Ʈ (Thread-Safe ArrayList)
	private List<Record> sendWaittingRecordList;		// ������ �� ������ ����Ʈ (Thread-Safe LinkedList)
	private List<Record> fileWriteLeftRecordList;		// ���� �ۼ��۾��� ���� ���ڵ� (Thread-Safe LinkedList)
	private List<Integer> fileWriteDoneRecordIndexList;	// ���� �ۼ��۾��� �Ϸ�� ���ڵ� �ε��� (Thread-Safe ArrayList)

	private RecordConverter cliRecordConverter;		// �������� ���� ���ڵ带 Ŭ���̾�Ʈ �������� �����ϴ� Ŭ����
	private KsFileWriter ksFileWriter;				// .rpy ���Ͽ� ���� ����� ���� Ŭ����
	private TransceiveLogger fileLogger;			// �ۼ��� �ΰ�
	
	private SendSocketThread sendSocketThread;		// ���ۿ� ���� ������ ������
	private RecvSocketThread recvSocketThread;		// ���ſ� ���� ������ ������
	private Thread sendThread;						// ���ۿ� ������
	private Thread recvThread;						// ���ſ� ������
	
	private HashMap<String, byte[]> envVarMap;		// ȯ�溯��
	
	public RecordTransceiver (boolean reusableSocketMode, String ip, int port, int socketCnt, int sendWaitingRecordListMaxSize, RecordConverter cliRecordConverter, KsFileWriter ksFileWriter, HashMap<String, byte[]> envVarMap) throws Exception {
		this.reusableSocketMode = reusableSocketMode;
		
		this.ip = ip;
		this.port = port;
		this.sendTryCnt = 0;
		this.sendDelay = 1000;					// �ʱ� ���� ���ð� 1000ms
		this.sendWaitingRecordListMaxSize = 50;	// �ִ� 50������ ���۴�� ����Ʈ�� ���� ����
		
		this.socketList = Collections.synchronizedList(new ArrayList<Socket>());			// Thread-Safe ���� ����Ʈ
		
		if (this.reusableSocketMode) {
			addSocket(socketCnt);
		}

		this.sendWaittingRecordList = Collections.synchronizedList(new LinkedList<Record>());	// Thread-Safe ������ �� ������ ����Ʈ
		this.fileWriteLeftRecordList = Collections.synchronizedList(new LinkedList<Record>());	// Thread-Safe ���� �ۼ��۾��� ���� ���ڵ� ����Ʈ
		this.fileWriteDoneRecordIndexList = Collections.synchronizedList(new ArrayList<Integer>());
		
		this.cliRecordConverter = cliRecordConverter;
		this.ksFileWriter = ksFileWriter;
		this.fileLogger = new TransceiveLogger("res/output/Client.out");
		
		final byte[] bPrefix = { 0x02 }, bSuffix = { 0x03 };
		this.sendSocketThread = new SendSocketThread(this, ip, port, socketList, fileWriteLeftRecordList, fileWriteDoneRecordIndexList, bPrefix, bSuffix, sendWaittingRecordList, fileLogger, envVarMap);
		this.recvSocketThread = new RecvSocketThread(this, ip, port, socketList, fileWriteLeftRecordList, fileWriteDoneRecordIndexList, bPrefix, bSuffix, cliRecordConverter, ksFileWriter, fileLogger, envVarMap);
		
		sendThread = new Thread(sendSocketThread);
		recvThread = new Thread(recvSocketThread);
		sendThread.start();
		recvThread.start();
		
		this.envVarMap = envVarMap;
	}
	
	public long getSendDelay() {
		return sendDelay;
	}
	
	public synchronized void setSendDelay(long sendDelay) {
		this.sendDelay = sendDelay;
	}
	
	public boolean isReusableSocketMode() {
		return reusableSocketMode;
	}
	
	public int getSendWaitingRecordListMaxSize() {
		return sendWaitingRecordListMaxSize;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	// �ش� ������ŭ ��Ĺ ���� (������ ���� �Ұ�, �߰��� ����)
	public void addSocket(int addCnt) {
		for (int i = 0; i < addCnt; ++i) {
			boolean hasError = false;
				
			try {
				socketList.add(new Socket(ip, port));
			}
			catch (IOException ioe) {
				// If an error occurs during the connection.
				Logger.logln(Logger.LogType.LT_ERR, ioe);
				Logger.logln(Logger.LogType.LT_ERR, "���� ���� ����. (Ip: " + ip + ", Port: " + port + ")");
				hasError = true;
			}
			catch (IllegalBlockingModeException ibme) {
				// If ths socket has an associated channel, and the channel is in non-blocking mode.
				Logger.logln(Logger.LogType.LT_ERR, ibme);
				Logger.logln(Logger.LogType.LT_ERR, "���� ���� ����. (non-blocking mode)");
				hasError = true;
			}
			catch (IllegalArgumentException iae) {
				// If endpoint is null or a SocketAddress subclass not supported by this socket.
				Logger.logln(Logger.LogType.LT_ERR, iae);
				Logger.logln(Logger.LogType.LT_ERR, "���� �ּ� ����. (Ip: " + ip + ", Port: " + port + ")");
				hasError = true;
			}
			finally {
				if (hasError) {
					Logger.logln(Logger.LogType.LT_CRIT, "IP: " + ip + ", Port: " + port + " ���� ���� ����. ���� ���� Ȯ�ιٶ��ϴ�.");
					System.exit(-1);
				}
			}
		}
	}
	
	private void destroySocketAll() {
		for (Socket socket : socketList) {
			try {
				socket.close();
				socket = null;
			}
			catch (Exception e) {
				Logger.logln(Logger.LogType.LT_ERR, e);
				Logger.logln(Logger.LogType.LT_ERR, "���� �ݱ� ����.");
			}
		}
		
		socketList.clear();
	}
	
	// �ۼ��ű� �ݱ�
	public void close() {
		try {
			// Send ������ ����
			sendSocketThread.close();
			sendThread.join();
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_ERR, e);
		}
		
		try {
			// Recv ������ ����
			recvSocketThread.close();
			recvThread.join();
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_ERR, e);
		}
		
		// �α����� �ۼ�
		fileLogger.writeToFile();
		
		// ���� �ı�
		destroySocketAll();
		
		// ���� ���
		int recvFailedCnt = fileWriteLeftRecordList.size();
		int recvSuccessCnt = sendTryCnt - recvFailedCnt;
		float recvFailedPercent = recvFailedCnt * 100.0f / sendTryCnt;
		float recvSuccessPercent = 100.0f - recvFailedPercent;
		
		System.out.println();
		System.out.println(Global.FC_GREEN + String.format("***** ���� �õ� ����: %d, ���� ���� ����: %d(%.02f%%), ���� ���� ����: %d(%.02f%%) ******", sendTryCnt, recvSuccessCnt, recvSuccessPercent, recvFailedCnt, recvFailedPercent) + Global.FC_RESET);
		
		if (recvFailedCnt > 0) {
			System.out.print(Global.FC_GREEN + String.format("***** ������ ���ڵ� �ε���: ", sendTryCnt, recvSuccessCnt, recvSuccessPercent, recvFailedCnt, recvFailedPercent) + Global.FC_RESET);
		
			for (Record failedRecord : fileWriteLeftRecordList) {
				System.out.print(Global.FC_GREEN + failedRecord.getIndex() + ", " + Global.FC_RESET);
			}
		}
		System.out.println();
		
		// �ʱ�ȭ
		sendWaittingRecordList.clear();
		fileWriteLeftRecordList.clear();
	}
	
	// ���� ���ڵ� �߰�
	public void send(Record sendRecord) {
		sendWaittingRecordList.add(sendRecord);
		++sendTryCnt;
	}
	
	// �ۼ��� �Ϸ� ���� Ȯ��
	public boolean checkTransceiverFinished() {
		// �� ������ ��� Ÿ�Ӿƿ��� ��� true
		if ((sendSocketThread.getThreadTimeoutLeft() == 0 && recvSocketThread.getThreadTimeoutLeft() == 0)) {
			return true;
		}
		
		return false;
	}
	
	// �ۼ��� ������ ���� ���
	public void printWorkLeft() {
		System.out.println();
		System.out.println(Global.FC_YELLOW + "*** [ Time: " + System.currentTimeMillis() + " ]" + Global.FC_RESET);
		System.out.println(Global.FC_YELLOW + "*** ���� ������� ���ڵ�: " + sendWaittingRecordList.size() + "�� / ���� ������� ���ڵ�: " + fileWriteLeftRecordList.size() + "��" + Global.FC_RESET);
		System.out.println(Global.FC_YELLOW + "*** �ۼ� �Ϸ� ���ڵ� : " + recvSocketThread.getFileWriteDoneRecordIndexListSize() + "�� / ���� ���� ��� �ð�: " + getSendDelay() + "ms" + Global.FC_RESET);
		System.out.println(Global.FC_YELLOW + "*** �ʴ� ���� �ӵ�/����: " + String.format("%.2f/%d��", (sendThread.isAlive() ? sendSocketThread.getCurSendPerSec() : 0.00f), (sendThread.isAlive() ? sendSocketThread.getTgtSendPerSec() : 0)) + Global.FC_RESET);
		System.out.println(Global.FC_YELLOW + "*** ���� ������ Ÿ�Ӿƿ�: " + sendSocketThread.getThreadTimeoutLeft() + "ms / ���� ������ Ÿ�Ӿƿ�: " + recvSocketThread.getThreadTimeoutLeft() + "ms" + Global.FC_RESET);
		System.out.println();
	}
	
	// ���� ��� ����Ʈ ũ�� ���� Ȯ��
	public boolean isSendWaittingListFull() {
		if (sendWaittingRecordList.size() >= sendWaitingRecordListMaxSize) {
			return true;
		}
		
		return false;
	}
	
	public SendSocketThread getSendSocketThread() {
		if (sendThread.isAlive()) return sendSocketThread;
		else return null;
	}
	
	public RecvSocketThread getRecvSocketThread() {
		if (recvThread.isAlive()) return recvSocketThread;
		else return null;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class SocketThread implements Runnable {
	
	public static long DEFAULT_THREAD_TIMEOUT;	// n�� ���� �ƹ� ������ ������ ������ Ÿ�Ӿƿ�
	public static int MAX_RESEND_COUNT;			// �ִ� n�� ������ ���
	public static int DEFAULT_RESEND_DELAY;		// n�� �̳� ���Ź��� ���ϸ� ������ �õ�
	
	protected RecordTransceiver recordTransceiver;	// ������ ��Ʈ�ѷ� RecordTransceiver Ŭ����
	
	protected String ip;				// ���� IP
	protected int port;					// ���� Port
	
	protected boolean running;			// ������ ����
	protected long threadTimeoutLeft;	// ���� ������ ���ð� (�и���)
	protected long lastTime;			// ������ �ð� (�и���)
	protected long timeDelta;			// �� ƽ ��� �ð� (�и���)
	protected long workDelayLeft;		// �۾� ������ ���� ���ð�
	
	protected List<Socket> socketList;						// �ۼ��� ���� ����Ʈ (Thread-Safe ArrayList)
	protected List<Record> fileWriteLeftRecordList;			// ���� �ۼ��۾��� ���� ���ڵ� Thread-Safe LinkedList)
	protected List<Integer> fileWriteDoneRecordIndexList; 	// ���� �ۼ��۾��� �Ϸ�� ���ڵ� �ε��� (Thread-Safe ArrayList)
	protected int curWorkSocketIndex;		// ���� ����ؾ� �� ���� �ε���
	protected TransceiveLogger fileLogger;	// �ۼ��� �ΰ�
	
	protected HashMap<String, byte[]> envVarMap;	// ȯ�溯��
	
	public SocketThread(RecordTransceiver recordTransceiver, String ip, int port, List<Socket> socketList, List<Record> fileWriteLeftRecordList, List<Integer> fileWriteDoneRecordIndexList, TransceiveLogger fileLogger, HashMap<String, byte[]> envVarMap) {		
		this.envVarMap = envVarMap;
		
		try {
			DEFAULT_THREAD_TIMEOUT = Long.parseLong(new String(envVarMap.get("SOCKET_THREAD_TIMEOUT")));
			MAX_RESEND_COUNT = Integer.parseInt(new String(envVarMap.get("RECORD_RESEND_MAX_TRY")));
			DEFAULT_RESEND_DELAY = Integer.parseInt(new String(envVarMap.get("RECORD_RESEND_DELAY")));
		}
		catch (NullPointerException npe) {
			Logger.logln(Logger.LogType.LT_ERR, npe.getMessage());
			DEFAULT_THREAD_TIMEOUT = 10000;
			MAX_RESEND_COUNT = 5;
			DEFAULT_RESEND_DELAY = 5000;
		}
		
		this.recordTransceiver = recordTransceiver;
		
		this.ip = ip;
		this.port = port;
		
		this.running = true;
		this.threadTimeoutLeft = DEFAULT_THREAD_TIMEOUT;
		this.lastTime = System.currentTimeMillis();
		this.timeDelta = 0;
		this.workDelayLeft = 0;
		
		this.socketList = socketList;
		this.fileWriteLeftRecordList = fileWriteLeftRecordList;
		this.fileWriteDoneRecordIndexList = fileWriteDoneRecordIndexList;
		
		this.curWorkSocketIndex = 0;
		this.fileLogger = fileLogger;
	}
	
	@Override
	public void run() { }

	public boolean isRunning() {
		return running;
	}
	
	public void setRunning(boolean running) {
		this.running = running;
	}
	
	public long getThreadTimeoutLeft() {
		return threadTimeoutLeft;
	}
	
	public void setThreadTimeoutLeft(long threadTimeoutLeft) {
		this.threadTimeoutLeft = threadTimeoutLeft;
	}
	
	public long getWorkDelayLeft() {
		return workDelayLeft;
	}
	
	public void setWorkDelayLeft(long workDelayLeft) {
		this.workDelayLeft = workDelayLeft;
	}
	
	public int getCurWorkSocketIndex() {
		return curWorkSocketIndex;
	}
	
	public synchronized LinkedList<Record> fileWriteLeftRecordListSyncWork(String work, Record record) {
		work.toLowerCase();
		
		if (work.equals("get")) {
			return new LinkedList<Record>(fileWriteLeftRecordList);
		}
		else if (work.equals("add")) {
			fileWriteLeftRecordList.add(record);
		}
		else if (work.equals("remove")) {
			for (Record removeRecord : fileWriteLeftRecordList) {
				if (removeRecord.getIndex() == record.getIndex()) {
					fileWriteLeftRecordList.remove(removeRecord);
					break;
				}
			}
		}
		else if (work.equals("clear")) {
			fileWriteLeftRecordList.clear();
		}
		
		return null;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ������ �ð� ������Ʈ
	protected boolean updateThreadTime() {
		long curTime = System.currentTimeMillis();
		timeDelta = curTime - lastTime;
		
		// �����尡 Ÿ�Ӿƿ��̸� false
		if ((threadTimeoutLeft -= timeDelta) <= 0) {
			threadTimeoutLeft = 0;
			lastTime = curTime;
			setRunning(false);
			return false;
		}
		
		// ������ �����̰� 0 �̻��� ��� �� ���� ���� ���
		if (true) {
			// Thread Sleep Mode
			if (workDelayLeft > 0) {
				try {
					Thread.sleep(workDelayLeft);
				} catch (InterruptedException ie) {
					Logger.logln(Logger.LogType.LT_ERR, ie);
				}
				
				workDelayLeft = 0;
			}
		}
		else {
			// Busy Waiting Mode
			//if ((workDelayLeft -= timeDelta) > 0) {
			//	lastTime = curTime;
			//	return false;
			//}
			//else {
			//	workDelayLeft = 0;
			//}
		}
		
		lastTime = curTime;
		
		return true;
	}
	
	// ����Ʈ �迭�� ���ڵ� ����
	protected Record makeRecord(String recordType, int recordIndex, byte[] prefixAry, byte[] suffixAry, byte[] byteAry) {
		// ��ȯ�� ����Ʈ �迭 ����
		int recordByteLength = byteAry.length - prefixAry.length - suffixAry.length;
		byte[] recordByte = new byte[recordByteLength];
		
		// Prefix, Suffix�����ϰ� ����
		int beginIndex = prefixAry.length;
		
		for (int i = 0; i < recordByteLength; ++i) {
			recordByte[i] = byteAry[i + beginIndex];
		}
		
		// ���ڵ� ����
		Record rtRecord = new Record(recordType, recordIndex, recordByte);
		
		return rtRecord;
	}
	
	// �迭�� Ȯ���Ͽ� �� �迭�� ��ȯ�ϴ� �Լ�
	protected byte[] makeAppendedByteAry(byte[] leftAry, byte[] rightAry) {
		// �迭 ũ�� ����
		int leftLength = 0, rightLength = 0, rtLength = 0;
		
		if (leftAry != null) {
			leftLength = leftAry.length;
		}
		
		if (rightAry != null) {
			rightLength = rightAry.length;
		}
		
		if ((rtLength = leftLength + rightLength) <= 0) return null; // left, right�迭�� �� �� null�� ���
			
		// ��ȯ�� �迭 ����
		byte[] rtByte = new byte[rtLength];
		int index = 0;
		
		// ���� �迭 ���� ����
		if (leftAry != null) {
			for (int i = 0; i < leftAry.length; ++i) {
				rtByte[index++] = leftAry[i];
			}
		}
		
		// ���� ��� ���� ����
		if (rightAry != null) {
			for (int j = 0; j < rightAry.length; ++j) {
				rtByte[index++] = rightAry[j];
			}
		}
		
		return rtByte;
	}
	
	// [beginIndex ~ endIndex) ������ �迭 ���Ҹ� �����ϰ� �� �迭�� ��ȯ�ϴ� �Լ�
	protected byte[] makeRemovedByteAryByIndex(byte[] originAry, int beginIndex, int endIndex) {
		// �ε��� ��
		if (beginIndex > endIndex) {
			Logger.logln(Logger.LogType.LT_WARN, "beginIndex(" + beginIndex + ") > endIndex(" + endIndex + "�� ���� ��ü�Ͽ� �����մϴ�.");
			
			int tempIndex = beginIndex;
			beginIndex = endIndex;
			endIndex = tempIndex;
		}
		
		// ���� ��
		int originLength = originAry.length;
		int removeLength = endIndex - beginIndex;
		int rtLength = originLength - removeLength;
		
		if (rtLength < 0) {
			Logger.logln(Logger.LogType.LT_ERR, "������ �迭 ������ ���� �迭 ���̸� �ʰ��մϴ�. (originAry.length: " + originLength + ", removeLength: " + removeLength + ")");
			return null;
		}
		else if (rtLength == 0) {
			return null;
		}
		
		// ��ȯ �迭 ����
		byte[] rtByte = new byte[rtLength];
		int rtIndex = 0;

		// ���� ����
		for (int i = 0; i < originAry.length; ++i) {
			if (i < beginIndex || i >= endIndex) {
				if (rtIndex < rtByte.length) {
					rtByte[rtIndex++] = originAry[i];
				}
				else {
					Logger.logln(Logger.LogType.LT_ERR, "�迭 ���� ����. (rtByte.length: " + rtByte.length + ", rtIndex: " + rtIndex + ")");
					break;
				}
			}
		}
		
		return rtByte;
	}
	
	// targetAry�� elementAry�� ������� ���ԵǾ� ������ �ش� �ε��� ��ȯ, ���ԵǾ����� ���� ��� -1�� ��ȯ
	protected int findFromAry(byte[] targetAry, byte[] elementAry) {
		int prefixIndex = -1;
		boolean containPrefix = false;
		
		for (int tgtI = 0; tgtI < targetAry.length; ++tgtI) {
			for (int elemI = 0; elemI < elementAry.length; ++elemI) {
				int recvJ = tgtI + elemI;
				
				if (recvJ >= targetAry.length) {
					containPrefix = false;
					break;
				}
				else if (targetAry[recvJ] != elementAry[elemI]) {
					containPrefix = false;
					break;
				}
				else {
					containPrefix = true;
				}
			}
			
			if (containPrefix == true) {
				prefixIndex = tgtI;
				break;
			}
			else {
				prefixIndex = -1;
			}
		}
		
		return prefixIndex;
	}
		
	// ���� �������� �õ��ϴ� �Լ�
	protected synchronized boolean reconnectSocket(int tryCnt, int tryInterval) {
		boolean isReconnectOk = false;
		int leftTryCnt = tryCnt;
		long curTime = System.currentTimeMillis();
		long nextTryTime = curTime + 1000; // ���� 1�� ���
		
		Logger.logln(Logger.LogType.LT_INFO, "����(Index: " + curWorkSocketIndex + ") ���� ������ ����. �ݱ� �� �������� �õ��մϴ�. (TryingCnt: " + (tryCnt - leftTryCnt) + ")");
		
		// ���� ���� �ݱ�
		while (leftTryCnt > 0) {
			curTime = System.currentTimeMillis();
			
			if (nextTryTime < curTime) {
				nextTryTime = curTime + tryInterval;
				--leftTryCnt;
				
				try {
					Logger.logln(Logger.LogType.LT_INFO, "����(Index: " + curWorkSocketIndex + ") �ݱ� �õ���. (TryingCnt: " + (tryCnt - leftTryCnt) + ")");
					
					Socket errSocket = socketList.get(curWorkSocketIndex);
					
					if (errSocket != null) {
						InputStream is = errSocket.getInputStream();
						int isLeftByte = is.available();
					
						if (isLeftByte > 0) {
							Logger.logln(Logger.LogType.LT_INFO, "InputStream�� ���� ������(+  " + isLeftByte + "Bytes) �д� ��.");
						}
						else {
							Logger.logln(Logger.LogType.LT_INFO, "���Ͽ� ���� ������ ����: " + errSocket.getInputStream().available() + "bytes");
							errSocket.close();
							Logger.logln(Logger.LogType.LT_INFO, "���� �ݱ� ����.");
							break;
						}
					}
				}
				catch (IOException ioe1) {
					Logger.logln(Logger.LogType.LT_ERR, ioe1);
					Logger.logln(Logger.LogType.LT_ERR, "���� �ݱ� ����.");
					break;
				}
			}
		}
		
		leftTryCnt = tryCnt;
		
		// ���� ����� �� ����
		while (leftTryCnt > 0) {
			curTime = System.currentTimeMillis();
			
			if (nextTryTime < curTime) {
				nextTryTime = curTime + tryInterval;
				--leftTryCnt;
				
				// ���ο� ���� ���� �� ������ �õ�
				Logger.logln(Logger.LogType.LT_INFO, "����(Index: " + curWorkSocketIndex + ") ������ �õ���. (TryingCnt: " + (tryCnt - leftTryCnt) + ")");
				
				try {
					socketList.set(curWorkSocketIndex, new Socket(ip, port));
					isReconnectOk = true;
					break;
				}
				catch (IOException ioe) {
					// If an error occurs during the connection.
					Logger.logln(Logger.LogType.LT_ERR, ioe);
					Logger.logln(Logger.LogType.LT_ERR, "���� ������ ����. (Ip: " + ip + ", Port: " + port + ")");
				}
				catch (IllegalBlockingModeException ibme) {
					// If ths socket has an associated channel, and the channel is in non-blocking mode.
					Logger.logln(Logger.LogType.LT_ERR, ibme);
					Logger.logln(Logger.LogType.LT_ERR, "���� ������ ����. (non-blocking mode)");
				}
				catch (IllegalArgumentException iae) {
					// If endpoint is null or a SocketAddress subclass not supported by this socket.
					Logger.logln(Logger.LogType.LT_ERR, iae);
					Logger.logln(Logger.LogType.LT_ERR, "������ �ּ� ����. (Ip: " + ip + ", Port: " + port + ")");
				}
			}
		}
		
		if (isReconnectOk) {
			Logger.logln(Logger.LogType.LT_INFO, "����(Index: " + curWorkSocketIndex + ") ������ ����. (TryingCnt: " + (tryCnt - leftTryCnt) + ")");
		}
		else {
			Logger.logln(Logger.LogType.LT_INFO, "����(Index: " + curWorkSocketIndex + ") ������ ����. (TryingCnt: " + (tryCnt - leftTryCnt) + ")");
			socketList.remove(curWorkSocketIndex);
			curWorkSocketIndex = 0;
		}
		
		return isReconnectOk;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////	

class SendSocketThread extends SocketThread {
	
	private byte[] sendPrefix;
	private byte[] sendSuffix;
	
	private float curSendPerSec;					// ���� �ʴ� ���۷�
	private int tgtSendPerSec;						// ��ǥ �ʴ� ���۷�
	
	private List<Record> sendWaittingRecordList;	// ������ �� ������ ����Ʈ (Thread-Safe LinkedList)

	public SendSocketThread(RecordTransceiver recordTransceiver, String ip, int port, List<Socket> socketList, List<Record> fileWriteLeftRecordList, List<Integer> fileWriteDoneRecordIndexList, byte[] recvPrefix, byte[] recvSuffix, List<Record> sendWaittingRecordList, TransceiveLogger fileLogger, HashMap<String, byte[]> envVarMap) {
		super(recordTransceiver, ip, port, socketList, fileWriteLeftRecordList, fileWriteDoneRecordIndexList, fileLogger, envVarMap);
		
		this.sendPrefix = recvPrefix;
		this.sendSuffix = recvSuffix;
		
		this.curSendPerSec = 0.00f;
		this.tgtSendPerSec = Integer.parseInt(new String(envVarMap.get("RECORD_TGT_SEND_PER_SEC")));

		this.sendWaittingRecordList = sendWaittingRecordList;
		this.fileWriteLeftRecordList = fileWriteLeftRecordList;
		this.fileWriteDoneRecordIndexList = fileWriteDoneRecordIndexList;
	}
	
	@Override
	public void run() {
		long sendStartTime = System.currentTimeMillis();
		int sendCntFromStart = 0;
		byte[] sendStream = null;
		
		while (running) {
			// ������ �ð� ������Ʈ
			if (!updateThreadTime()) continue;
						
			// �ʴ� ���ۼӵ� �ʰ� ����
			if ((curSendPerSec = sendCntFromStart / ((System.currentTimeMillis() - sendStartTime) / 1000.0f)) > tgtSendPerSec) {
				continue;
			}
			
			// ���� ��� ����Ʈ�� ���ڵ� ���� �õ�
			try {
				if ((sendStream = sendWork(sendStream)) == null) { // ���� ����
					++sendCntFromStart;
				}
			}
			catch (IOException ioe) {
				Logger.logln(Logger.LogType.LT_ERR, ioe);
				Logger.logln(Logger.LogType.LT_ERR, Global.FC_WHITE + "OutputStream�� ���� �����Դϴ�. (SocketIndex: " + curWorkSocketIndex + ")" + Global.FC_RESET);
				
				// [Note] OutputStream.write()���� IOException�� �߻��ϴ� ����, InputStream�� �������� ���� �����͸� ������ �� �����. (����, ��Ʈ��ũ ��Ȳ�� ���� ���� �߻��� ���ڵ� -N���� ���ڵ尡 �ҽ� ���ɼ��� �ְ�, 
				// ���� ���� ��忡�� ���� ���� �߻� �� 2���� �����͸� .write() �� �� ���� �������� ���ϹǷ� +2���� �����Ͱ� �߰��� �ҽǵ�)
				
				if (!reconnectSocket(50, 500)) { // �ִ� 50ȸ,0.5�� �������� ������ �õ�
					Logger.logln(Logger.LogType.LT_ERR, Global.FC_RED + "���� �����ӿ� �����Ͽ����ϴ�. (SocketIndex: " + curWorkSocketIndex +  ")" + Global.FC_RESET); // OFT
					
					if (socketList.size() == 0) {
						Logger.logln(Logger.LogType.LT_CRIT, Global.FC_RED + "������ ����� ������ �����ϴ�. ������ ���� �����մϴ�." + Global.FC_RESET); // OFT
						setThreadTimeoutLeft(0);
						continue;
					}
				}
			}
				
			// �������� �������� ���� �����͸� ���۴�� ����Ʈ�� ������Ͽ� ������ �õ�
			resendWork();
			
			// ���� �ӵ� ����
			controlSendSpeed();
		}
		
		Logger.logln(Logger.LogType.LT_INFO, Global.FC_WHITE + "SendSocketThread ����." + Global.FC_RESET);
	}
	
	public void close() {
		setRunning(false);
	}
	
	public float getCurSendPerSec() {
		return curSendPerSec;
	}
	
	public int getTgtSendPerSec() {
		return tgtSendPerSec;
	}
	
	public void setTgtSendPerSec(int tgtSendPerSec) {
		this.tgtSendPerSec = tgtSendPerSec;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ���� �õ�
	private byte[] sendWork(byte[] sendStream) throws IOException {
		Socket sendSocket = null;
		OutputStream os = null;
		Record sendRecord = null;
		
		// ���� ���ۿ��� ������ �����Ͱ� ���� ��� ����Ʈ �����͸� �о��
		if (sendStream == null) {
			if (!sendWaittingRecordList.isEmpty()) { // ������ ���ڵ尡 ����
				// ���� ���� �� ���ڵ� ����Ʈ ��ȯ
				sendRecord = sendWaittingRecordList.remove(0);
				sendStream = makeSendStream(sendRecord);
			}
		}
		
		if (sendStream != null) {					
			// ����
			if (recordTransceiver.isReusableSocketMode()) {
				sendSocket = socketList.get(curWorkSocketIndex);
			}
			else {
				recordTransceiver.addSocket(1);
				sendSocket = socketList.get(socketList.size() - 1);
			}
			
			os = sendSocket.getOutputStream();
			os.write(sendStream, 0, sendStream.length); // IOException �߻� ����
			sendRecord.addSendCnt(1);
			sendRecord.setLastSendTime(System.currentTimeMillis());
			fileWriteLeftRecordListSyncWork("add", sendRecord);
			threadTimeoutLeft = DEFAULT_THREAD_TIMEOUT;
			
			// �ð� ���� �� �α�
			SimpleDateFormat dateTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			String today = dateTime.format(new Date(System.currentTimeMillis()));
			String hour = today.substring(11, 13),	min = today.substring(14, 16),	sec = today.substring(17, 19);
			fileLogger.log(String.format("%s%s%s: snd(%04d)=(", hour, min, sec, sendStream.length), new String(sendStream), ")");
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_WHITE + String.format("\n[Soc%d]%s%s%s: snd(%d)=(%s)\n", curWorkSocketIndex, hour, min, sec, sendStream.length, new String(sendStream)) + Global.FC_RESET); // OFT
			
			// �ʱ�ȭ �� ���� ���� ����
			if (socketList.size() != 0) curWorkSocketIndex = (++curWorkSocketIndex) % socketList.size();
			
			sendStream = null;
		}
		
		return sendStream;
	}
	
	// �������� �������� ���� �����͸� ���۴�� ����Ʈ�� ������Ͽ� ������ �õ�
	private void resendWork() {
		LinkedList<Record> recvWaitingList = fileWriteLeftRecordListSyncWork("get", null);
		int recvWaitingCnt = recvWaitingList.size();
		
		for (Record record : recvWaitingList) {
			long curTime = System.currentTimeMillis();
			long lastSendTime = record.getLastSendTime();
			long lastSendDelta = curTime - lastSendTime;
			int resendCnt = record.getSendCnt();
			
			if (lastSendDelta < DEFAULT_RESEND_DELAY) { // ������ ������ ���
				// ...
			}
			else if (lastSendDelta >= DEFAULT_RESEND_DELAY && resendCnt <= MAX_RESEND_COUNT) { // �������� DEFAULT_RESEND_DELAY�� ����, ������ MAX_RESEND_COUNT�� ����	
				if (sendWaittingRecordList.size() < recordTransceiver.getSendWaitingRecordListMaxSize()) { // ���ť�� ������ �ְ�
					if (fileWriteDoneRecordIndexList.indexOf(record.getIndex()) == -1) { // ���ſϷᰡ ���� ���� ���
						sendWaittingRecordList.add(record);	// ���۴�⿡ �߰�
						Logger.logln(Logger.LogType.LT_INFO, Global.FC_WHITE + record.getIndex() + "�� ���ڵ� ������ �õ�. (������ ���� �� ����ð�: " + lastSendDelta + ", ���� �õ� Ƚ��: " + resendCnt + ")" + Global.FC_RESET);
					}
					
					fileWriteLeftRecordListSyncWork("remove", record); // ���ſϷ� �ưų�, �������� ��� ���Ŵ�⿡�� ����
				}
				else {
					break;
				}
			}
			else {
				--recvWaitingCnt; // ���� �Ұ� ���ڵ� ������ŭ ����
			}
		}
	}
	
	// ���� �ӵ� ����
	private void controlSendSpeed() {
		long sendDelay = 1;
				RecvSocketThread recvSocketThread = recordTransceiver.getRecvSocketThread();
				
				if ((recvSocketThread = recordTransceiver.getRecvSocketThread()) != null) {
					long curTime = System.currentTimeMillis();
					long lastRecvTime = recvSocketThread.getLastRecvTime();
					
					sendDelay = Math.min(1000, (curTime - lastRecvTime) / 2);
				}
				
				sendDelay = Math.max(sendDelay, 1);
				recordTransceiver.setSendDelay(sendDelay);
				setWorkDelayLeft(sendDelay); // ���� �ӵ� ���� �� ������ ������ ����
				
				// Old Version
				/*long sendDelay = recordTransceiver.getSendDelay();
				sendDelay = Math.max(sendDelay, Math.max(recvWaitingCnt, 1)); // '���� �����ð�' vs '�̼��� ������ ����' �� ū ����ŭ ���. (��, �ּ� 1ms��ŭ�� ���)
				recordTransceiver.setSendDelay(sendDelay);
				setWorkDelayLeft(sendDelay / 2); // ���� �ӵ� ���� �� ������ ������ ����
				System.out.println("sendDelay: " + sendDelay / 2);*/
	}
	
	// Record�� ����Ͽ� ���, Prepix, Suffix�� ���� ���ۿ� byte[]����
	private byte[] makeSendStream(Record sendRecord) {
		int streamIndex = 0;
		
		// Record ����Ʈ �迭
		final byte[] recordSteram = sendRecord.toByteAry();
		
		// ����� (0000 : 4byte)
		final int streamHeaderLength = 4; 
		
		// �����ͺ� (Prefix + ���ڵ� + Suffix)
		final int streamDataLength = sendPrefix.length + recordSteram.length + sendSuffix.length;
		
		// ������ ����Ʈ �迭 (��� + �����ͺ�(Prefix+Record+Suffix))
		byte[] sendStream = new byte[streamHeaderLength + streamDataLength];
		
		// Data Length (����� ���� ����)
		final byte[] headerStream = String.format("%04d", streamDataLength).getBytes();
		
		for (int i = 0; i < headerStream.length; ++i) {
			sendStream[streamIndex++] = headerStream[i];
		}
		
		// Prefix Copy (0x02:STX)
		for (int i = 0; i < sendPrefix.length; ++i) {
			sendStream[streamIndex++] = sendPrefix[i];
		}
		
		// Record Copy
		for (int i = 0; i < recordSteram.length; ++i) {
			sendStream[streamIndex++] = recordSteram[i];
		}
		
		// Suffix Copy (0x03:ETX)
		for (int i = 0; i < sendSuffix.length; ++i) {
			sendStream[streamIndex++] = sendSuffix[i];
		}
		
		return sendStream;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class RecvSocketThread extends SocketThread {
	
	public static final int streamHeaderLength = 4;	// ����� ���� (0000:4byte)
	
	private byte[] savedByteAry;	// ������ �о���� ����Ʈ �����͸� ����ִ� �迭
	
	private long lastRecvTime;		// ���������� �����͸� ���� ������ �ð�
	
	private byte[] recvPrefix;
	private byte[] recvSuffix;
	
	private RecordConverter cliRecordConverter;		// �������� ���� ���ڵ带 Ŭ���̾�Ʈ �������� �����ϴ� Ŭ����
	private KsFileWriter ksFileWriter;				// .rpy ���Ͽ� ���� ����� ���� Ŭ����
	
	public RecvSocketThread(RecordTransceiver recordTransceiver, String ip, int port, List<Socket> socketList, List<Record> fileWriteLeftRecordList, List<Integer> fileWriteDoneRecordIndexList, byte[] recvPrefix, byte[] recvSuffix, RecordConverter cliRecordConverter, KsFileWriter ksFileWriter, TransceiveLogger fileLogger, HashMap<String, byte[]> envVarMap) {
		super(recordTransceiver, ip, port, socketList, fileWriteLeftRecordList, fileWriteDoneRecordIndexList, fileLogger, envVarMap);
		
		this.savedByteAry = null;
		
		this.lastRecvTime = System.currentTimeMillis();
		
		this.recvPrefix = recvPrefix;
		this.recvSuffix = recvSuffix;
		
		this.cliRecordConverter = cliRecordConverter;
		this.ksFileWriter = ksFileWriter;
	}
	
	@Override
	public void run() {
		Socket recvSocket = null;	// ���� �۾��� ���� ����
		
		while (running) {
			// ������ �ð� ������Ʈ
			if (!updateThreadTime()) continue;

			try {
				// [���ź�]
				if (recordTransceiver.isReusableSocketMode()) {
					recvSocket = socketList.get(curWorkSocketIndex);
				}
				else {
					recvSocket = null;
					
					if (socketList != null) {
						for (int i = 0; i < socketList.size(); ++i) {
							Socket socket = socketList.get(i);
							
							if (socket.getInputStream().available() != 0) {
								recvSocket = socket;
								break;
							}
						}
					}
				}
				
				if (recvSocket == null) continue;
				
				int inputDataLength = recvSocket.getInputStream().available();

				if (inputDataLength > 0) { // ������ ����Ʈ�� ����
					recvWork(recvSocket, inputDataLength);
				}
				
				if (savedByteAry != null && savedByteAry.length > 0) {
					// [���ܺ�]
					byte[] recordByteAry = cuttingWork(inputDataLength);

					if (recordByteAry == null) continue;
					
					// [������]
					cvtAndWriteWork(recordByteAry);
					
					// ������ �۾��� ���Ϲ�ȣ ����
					if (socketList.size() != 0)	curWorkSocketIndex = (++curWorkSocketIndex) % socketList.size();
				}
			}			
			catch (Exception e) {
				Logger.logln(Logger.LogType.LT_ERR, e);
				
				if (e instanceof IOException) {
					Logger.logln(Logger.LogType.LT_ERR, Global.FC_RED + "InputStream�� ���� �����Դϴ�. (SocketIndex: " + curWorkSocketIndex + ", fileWriteLeftRecordList.size(): " + fileWriteLeftRecordList.size() + ")" + Global.FC_RESET);
					setWorkDelayLeft(1000); // �ּ� 1�ʰ� �۾� ���
				}
			}
			
			setWorkDelayLeft(Math.max(getWorkDelayLeft(), 1)); // ������ ������ ����
		}
		
		// ���� ������ �����͸� ���Ͽ� ���
		writeRecvFailedRecord();
		
		Logger.logln(Logger.LogType.LT_INFO, Global.FC_RED + "RecvSocketThread ����." + Global.FC_RESET);
	}
		
	public void close() {
		setRunning(false);
	}
	
	public int getFileWriteDoneRecordIndexListSize() {
		return fileWriteDoneRecordIndexList.size();
	}
	
	public long getLastRecvTime() {
		return lastRecvTime;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ���ź�
	private void recvWork(Socket recvSocket, int inputDataLength) throws IOException {
		Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "[���� ó�� ����: " + fileWriteLeftRecordList.size() + "]" + Global.FC_RESET); // OFT
		Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "1-1.available(): " + inputDataLength + Global.FC_RESET); // OFT
		
		InputStream is = recvSocket.getInputStream();
		byte[] readByteAry = new byte[inputDataLength];
		
		if (is.read(readByteAry, 0, inputDataLength) != -1) {
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "1-2.readByteLen " + inputDataLength + Global.FC_RESET); // OFT
			
			// ���������� ������ �ð�, Ÿ�Ӿƿ� ����
			lastRecvTime = System.currentTimeMillis();
			threadTimeoutLeft = DEFAULT_THREAD_TIMEOUT;

			// �ð� ���� �� �α�
			SimpleDateFormat dateTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			String today = dateTime.format(new Date(System.currentTimeMillis()));
			String hour = today.substring(11, 13),	min = today.substring(14, 16),	sec = today.substring(17, 19);
			fileLogger.log(String.format("%s%s%s: rcv(%04d)=(", hour, min, sec, readByteAry.length), new String(readByteAry), ")");

			// �ӽ������ Byte�� ����
			this.savedByteAry = makeAppendedByteAry(savedByteAry, readByteAry);
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "1-3.savedByteAryLen: " + savedByteAry.length + Global.FC_RESET); // OFT
			
			// ��ȸ�� ���� ����� ��� ���� �ı�
			if (!recordTransceiver.isReusableSocketMode()) {
				recvSocket.close();
				socketList.remove(recvSocket);
			}
		}
	}
	
	// ���ܺ�
	private byte[] cuttingWork(int inputDataLength) {
		// Prefix:STX(0x02) ã��
		int recvPrefixIdx = findFromAry(savedByteAry, recvPrefix);
		
		if (inputDataLength == 0) {
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "[���� ó�� ����: " +
						 fileWriteLeftRecordList.size() + ", �Ϸ��� ����: " + fileWriteDoneRecordIndexList.size() + "]" + Global.FC_RESET); // OFT
		}
		
		// Prefix:STX�� ���� ���Ź��� ����
		if (recvPrefixIdx < 0) {
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2.failToFoundPrefix( + " + new String(recvPrefix) + ")"); // OFT
			return null; // ���ź� �����
		}
		
		Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2-1.prefixIdx: " + recvPrefixIdx + Global.FC_RESET); // OFT
		
		// Suffix:ETX(0x03) ã��
		int recvSuffixIdx = findFromAry(savedByteAry, recvSuffix);
		
		// Suffix:ETX�� ���� ���Ź��� ����
		if (recvSuffixIdx < 0) {
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2.failToFoundSuffix( + " + new String(recvSuffix) + ")"); // OFT
			return null; // ���ź� �����
		}
		else {
			recvSuffixIdx += recvSuffix.length; // Suffix:ETX �����ͱ��� �����ϱ� ���� ����
		}
		
		Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2-2.suffixIdx: " + recvSuffixIdx + Global.FC_RESET); // OFT
		
		// STX�� ETX�� ��� ���Ź��� ��� (�ǹ��ִ� �����Ͱ� �ϼ��� ���)
		// ���ڵ� ������ ���� �κ� ����Ʈ �迭 ���� (STX/ETX�� ���Եǰ�, ��� 4byte�� ���Ե��� �ʴ� �迭)
		byte[] recordByteAry = Arrays.copyOfRange(savedByteAry, recvPrefixIdx, recvSuffixIdx);
		Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2-3.recordByteAryLen: " + recordByteAry.length + "/302" + Global.FC_RESET); // OFT
		
		// ����� �迭���� �ش� ��� + ������ ����
		this.savedByteAry = makeRemovedByteAryByIndex(savedByteAry, recvPrefixIdx - streamHeaderLength, recvSuffixIdx);
			
		if (savedByteAry != null) Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2-4.afterRemoveFromSavedByteAry: " + savedByteAry.length + Global.FC_RESET); // OFT
		else Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2-4.afterRemoveFromSavedByteAry: 0" + Global.FC_RESET); // OFT
		
		return recordByteAry;
	}
	
	// ������
	private void cvtAndWriteWork(byte[] recordByteAry) {
		// ���� �����͸� ���� ���ڵ�� ����
		Record recvSvrRecord = makeRecord("HanaRecordServer", -1, recvPrefix, recvSuffix, recordByteAry);
		Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2-5.recvSvrRecordIndex: " + recvSvrRecord.getIndex() + Global.FC_RESET); // OFT
		
		int recvSvrRecordIndex = recvSvrRecord.getIndex();
		
		if (fileWriteDoneRecordIndexList.indexOf(recvSvrRecordIndex) == -1) { // ���������� ���� �ε��� ��ȣ�� ���� ���ڵ�
			// �ش� ���ڵ� �ε��� ��ȣ�� �߰�
			fileWriteDoneRecordIndexList.add(recvSvrRecordIndex);
			
			// ������ ���� ���ڵ带 Ŭ���̾�Ʈ ���ڵ�� ����
			cliRecordConverter.setOutRecordSubTypeName("Data");
			Record recvCliRecord = cliRecordConverter.convert(recvSvrRecord);
			
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "2-6.cvtCliRecord (msgNum: " + recvCliRecord.getIndex() + ")" + Global.FC_RESET); // OFT
			
			// ���ڵ� ���Ͽ� ���� ����
			ksFileWriter.write(recvCliRecord.toByteAry(), recvSvrRecordIndex);
		}
		
		// ���� �ۼ���� ����Ʈ���� �ش� �ε����� ���ڵ� ����
		fileWriteLeftRecordListSyncWork("remove", recvSvrRecord);
		
		LinkedList<Record> fileWriteLeftRecordListCpy = fileWriteLeftRecordListSyncWork("get", null);
		
		if (fileWriteLeftRecordListCpy.size() > 0) {
			Logger.logln(Logger.LogType.LT_DEBUG, Global.FC_RED + "���� ������� ���ڵ� �ε���: " + Global.FC_RESET);
			
			for (Record writeLeftRecord : fileWriteLeftRecordListCpy) {
				Logger.log(Logger.LogType.LT_DEBUG, Global.FC_RED + writeLeftRecord.getIndex() + ", " + Global.FC_RESET);
			}
			
			Logger.ln(Logger.LogType.LT_DEBUG); Logger.ln(Logger.LogType.LT_DEBUG);
		}					
	}
	
	// ���� ������ �����͸� ���Ͽ� ���
	private void writeRecvFailedRecord() {
		for (Record failedRecord : fileWriteLeftRecordListSyncWork("get", null)) {
			cliRecordConverter.setOutRecordSubTypeName("Data");
			failedRecord = cliRecordConverter.convert(failedRecord);
			ksFileWriter.write(failedRecord.toByteAry(), failedRecord.getIndex());
		}
	}
}
