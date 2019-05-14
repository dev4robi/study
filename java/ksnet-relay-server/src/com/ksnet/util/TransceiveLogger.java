package com.ksnet.util;

import java.io.FileOutputStream;
import java.io.IOException;

public class TransceiveLogger {

	private String logFilePath;
	private StringBuffer logBuffer; // Thread-Safe

	// ������
	public TransceiveLogger(String logFilePath) {
		this.logFilePath = logFilePath;
		logBuffer = new StringBuffer();
	}
	
	// �α� ���
	public synchronized void log(String head, String body, String tail) {
		logBuffer.append(head).append(body).append(tail).append("\r\n");
	}

	// ���Ϸ� ���
	public synchronized void writeToFile() {
		if (logBuffer.length() > 0) {
			FileOutputStream fos = null;
			
			try {
				fos = new FileOutputStream(logFilePath, true);
				fos.write(String.valueOf(logBuffer).getBytes());
			}
			catch (IOException ioe1) {
				Logger.logln(Logger.LogType.LT_ERR, "OutputStream ���� Ȥ�� ���� ����.");
				ioe1.printStackTrace();
			}
			finally {
				try {
					if (fos != null) fos.close();
					fos = null;
				}
				catch (IOException ioe2) {
					Logger.logln(Logger.LogType.LT_ERR, "OutputStream �ݱ� ����.");
					ioe2.printStackTrace();
				}
				
				logBuffer.delete(0, logBuffer.length());
			}
		}
	}
}