package com.robi.ksnet;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class Logger {

	// Ŭ���� ���� ��� //
	private static final String[] LogTypeSymbols = new String[] { "[DBG]", "[INF]", "[WAN]", "[ERR]", "[FAT]" };
	private static final SimpleDateFormat SimpleDateFmt = new SimpleDateFormat("[yyyy-MM-dd HH:mm:ss]");	

	// Ŭ���� ���� ��� //
	public static final int DEBUG   = 0;
	public static final int DBG     = 0;
	public static final int INFO    = 1;
	public static final int INF     = 1;
	public static final int WARNING = 2;
	public static final int WAR     = 2;
	public static final int ERROR   = 3;
	public static final int ERR     = 3;
	public static final int FATAL   = 4;
	public static final int FAT     = 4;
	public static final String DEFAULT_LOG_DIR     = "rb_lib_logs";
	public static final String DEFAULT_LOG_PREFIX  = "log";
	public static final String DEFAULT_LOG_CHARSET = "UTF-8";

	// Ŭ���� ���� ���� //
	private String LogFileNameStr = null;		// �α����� �̸� (yyyyMMdd.log)
	private int LastDate = -1;					// ���� ���� ����
	private String CachedTimeStr = null;		// �α� �ۼ��ð� ĳ�� ���ڿ� ([yyyy-MM-dd HH:mm:ss])
	private int LastSec = -1;					// �� ���� ����
	private File LogDir = null;					// �α� ���
	private File LogFile = null;				// �α� ����
	private PrintStream LogPs = null;			// �α� ��� ��Ʈ��
	private boolean RequestFileRefresh = true;	// LogDir, LogFile, LogPs ������ �ʿ����� Ȯ���ϴ� ����ġ ����
	private String MissingLogStr = null;		// ������ ���� ������� ���� ������ �α�
	
	// Ŭ���� ���� //
	public boolean LOGGER_INITIALIZED = false;	// �ΰ� �ʱ�ȭ ����
	public String LOG_FILENAME_PREFIX;			// �α����ϸ� ���λ�
	public String LOG_DIR;						// �α� ���� ����
	public String LOG_CHARSET;					// �α� ü���

	// Ŭ���� ���� �޼��� //
	// �ΰ� ����
	public static Logger getInstance() {
		return new Logger();
	}

	// Ŭ���� �޼��� //
	// �ΰ� �ʱ�ȭ
	public boolean init(String filenamePrefix, String logDir, String charset) {
		if (!setLogFilenamePrefix(filenamePrefix) || !setLogDir(logDir) || !setLogCharset(charset)) {
			return false;
		}
		
		LOGGER_INITIALIZED = true;
		return true;
	}

	// �ΰ� �ݱ�
	public void close() {
		try {
			if (LogPs != null) LogPs.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	// �α����ϸ� ���λ� ����
	public boolean setLogFilenamePrefix(String newFilenamePrefix) {
		if (newFilenamePrefix == null) {
			return false;
		}

		if (LOG_FILENAME_PREFIX != null && LOG_FILENAME_PREFIX.equals(newFilenamePrefix)) {
			return true;
		}

		LOG_FILENAME_PREFIX = newFilenamePrefix;
		RequestFileRefresh = true;

		return true;
	}

	// �α� ��μ���
	public boolean setLogDir(String newLogDir) {
		if (newLogDir == null) {
			return false;
		}

		if (LOG_DIR != null && LOG_DIR.equals(newLogDir)) {
			return true;
		}

		LOG_DIR = newLogDir;
		RequestFileRefresh = true;

		return true;
	}

	// �α� ü��� ����
	public boolean setLogCharset(String newCharset) {
		if (newCharset == null) {
			return false;
		}

		if (LOG_CHARSET != null && LOG_CHARSET.equals(newCharset)) {
			return true;
		}

		LOG_CHARSET = newCharset;
		RequestFileRefresh = true;

		return true;
	}

	// �����α� ����
	public void error(Object... logObjs) {
		this.log(this.ERROR, logObjs);
	}

	// �����α� ����
	public void info(Object... logObjs) {
		this.log(this.INFO, logObjs);
	}

	// �α� ����
	public void log(int logLv, Object... logObjs) {
		// ���ʱ�ȭ�� �ʱ�ȭ
		if (!LOGGER_INITIALIZED) {
			if (init(DEFAULT_LOG_PREFIX, DEFAULT_LOG_DIR, DEFAULT_LOG_CHARSET)) {
				return;
			}
		}

		// �α� �ٵ� ����
		StringBuilder logHeadSb = new StringBuilder();
		StringBuilder logBodySb = new StringBuilder();
		String logStr = null;

		for (Object logObj : logObjs) {
			if (logObj == null) {
				logBodySb.append("null");
			}
			else if (logObj instanceof String) {
				logBodySb.append((String)logObj);
			}
			else if (logObj instanceof Throwable) {
				Throwable throwableLogObj = (Throwable)logObj;

				// "{Exception} (Msg:{Message}) at ["
				logBodySb.append(throwableLogObj.toString()).append(" (Msg:\"").append(throwableLogObj.getMessage()).append("\") at { ");
	
				for (StackTraceElement stElem : throwableLogObj.getStackTrace()) { 
					logBodySb.append(stElem.toString()).append(" <- "); // "{StackTrace} <- "
				}

				logBodySb.setLength(logBodySb.length() - 4); // ������ " <- " ���� ����
				logBodySb.append(" }");
			}
			else {
				logBodySb.append(logObj.toString());
			}

			logBodySb.append(' ');
		}

		logBodySb.setLength(logBodySb.length() - 1); // �������� " " ����
		
		synchronized (Logger.class) {
			// �ð����� ���ڿ� ����
			String curTimeStr = null;
			Calendar curCal = Calendar.getInstance();
			int curSec = curCal.get(Calendar.SECOND);
			int curDate = curCal.get(Calendar.DATE);
			
			if (curSec != LastSec) { // 1�� �̻� �ð� ��� �� ĳ�̵� �ð� ����
				CachedTimeStr = SimpleDateFmt.format(curCal.getTime());
				LastSec = curSec;
			}

			curTimeStr = CachedTimeStr; // "[yyyy-MM-dd HH:mm:ss]"

			// �α����� �̸� ����
			if (RequestFileRefresh || curDate != LastDate) { // ���� ���� �� �α����ϸ� ����
				LogFileNameStr = String.format("%s.%s%s%s.log", LOG_FILENAME_PREFIX, curTimeStr.substring(1, 5),
											   curTimeStr.substring(6, 8), curTimeStr.substring(9, 11)); // "yyyyMMdd.log"
				LastDate = curDate;
				RequestFileRefresh = true;
			}

			// �α� ��� ����
			logHeadSb.append(curTimeStr).append(':').append(LogTypeSymbols[logLv]).append(' ');

			// �α� ��� + �ٵ�
			logHeadSb.append(logBodySb);
			logStr = logHeadSb.toString();
			logHeadSb.setLength(0);
			logBodySb.setLength(0);

			// [Note] RequestFileRefresh���� true���� �Ǵ� ���
			// 1. Logger �ʱ�ȭ ����
			// 2. ���ڰ� ����Ǿ� �� �α� ������ �����Ǿ� �ϴ� ���
			// 3. ���� ��� ��Ʈ���� �ջ�Ǿ� �� ��� ��Ʈ���� �����Ǿ�� �ϴ� ���

			// �α�����/���� ���� �� ��½�Ʈ�� ó��
			if (RequestFileRefresh) {
				// ���� ��� ��Ʈ�� �ݱ�
				try {
					if (LogPs != null) LogPs.close();
				}
				catch (Exception e1) {
					e1.printStackTrace();
				}
				finally {
					LogPs = null;
				}

				// �α����� Ȯ�� �� ����
				LogDir = new File(LOG_DIR);

				if (!LogDir.exists()) {
					if (!LogDir.mkdir()) {
						int timeStrLen = SimpleDateFmt.toPattern().length();
						MissingLogStr = logStr;
						System.out.println(String.format("%s:[FAT] Fail to create log directory '%s'! The logStr '%s' will be discarded!",
														 logStr.substring(0, timeStrLen), LOG_DIR, MissingLogStr));
						return;
					}
				}
				
				// ���� ���� + ��� ��Ʈ�� �غ�
				LogFile = new File(LOG_DIR, LogFileNameStr);
				boolean isAppend = LogFile.exists();

				try {
					LogPs = new PrintStream(new FileOutputStream(LogFile, isAppend), true, LOG_CHARSET);
				}
				catch (Exception e2) {
					int timeStrLen = SimpleDateFmt.toPattern().length();
					MissingLogStr = logStr;
					System.out.println(String.format("%s:[FAT] Fail to create output stream of the log '%s'! The logStr '%s' will be discarded!",
													 logStr.substring(0, timeStrLen), LogFileNameStr, MissingLogStr));
					e2.printStackTrace();
				}
				
				RequestFileRefresh = false;
			}

			try {
				// ���� �˻�
				if (!LogFile.exists()) {
					throw new Exception("LogFileMissingException!");
				}

				// ���Ͽ� ���
				if (LogPs != null) {
					// ���������� ��¸��� �αװ� ������ ���
					if (MissingLogStr != null) {
						LogPs.println(MissingLogStr);
						MissingLogStr = null;
					}

					// ���� �α� ���
					LogPs.println(logStr);
				}
				else {
					RequestFileRefresh = true;
					int timeStrLen = SimpleDateFmt.toPattern().length();
					MissingLogStr = logStr;
					System.out.println(String.format("%s:[FAT] LogPs is null! fail to write log of the file '%s'! The logStr '%s' will be discarded!",
													 logStr.substring(0, timeStrLen), LogFileNameStr, MissingLogStr));
					System.out.println(String.format("%s:[FAT] Set 'RequestFileRefresh = true', retry log file open.", logStr.substring(0, timeStrLen)));
				}
			}
			catch (Exception e3) {
				RequestFileRefresh = true;
				int timeStrLen = SimpleDateFmt.toPattern().length();
				MissingLogStr = logStr;
				System.out.println(String.format("%s:[FAT] Fail to write log file '%s'! The logStr '%s' will be discarded!",
												 logStr.substring(0, timeStrLen), LogFileNameStr, MissingLogStr));
				e3.printStackTrace();
			}
		}
		
		return;
	}

	// traceId�� ��������� �����ϴ� �α�
	public void traceLog(int logType, String traceId, Object... logObjs) {
		if (!this.LOGGER_INITIALIZED) {
			this.init(DEFAULT_LOG_PREFIX, DEFAULT_LOG_DIR, DEFAULT_LOG_CHARSET);
		}

		Object[] newLogObjs = new Object[logObjs.length + 2];
		int lastStkIdx = 1;

		for (int i = 2; i < newLogObjs.length; ++i) {
			newLogObjs[i] = logObjs[i - 2];
		}

		// �α� ���1 ����
		String logHeader1 = String.format("[%s]", traceId);

		// �α� ���2 ����
		StringBuilder logHeaderSb2 = new StringBuilder();
		StackTraceElement[] stkTraceElem = new Throwable().getStackTrace();

		logHeaderSb2.append('[').append(stkTraceElem[lastStkIdx].getFileName());

		if (logType != Logger.INFO) {	
			int maxStkLimit = Math.min(stkTraceElem.length, 2); // �⺻: �ݽ��� 2�ܰ���� ���
			
			logHeaderSb2.append("(").append(stkTraceElem[lastStkIdx].getLineNumber()).append(").");

			if (logType != Logger.INFO) {
				maxStkLimit = Math.min(stkTraceElem.length, 3); // INFO�α� ����: �ݽ��� 3�ܰ���� ���
			}
			else {
				logHeaderSb2.append("..");
			}

			for (int j = maxStkLimit; j >= lastStkIdx; --j) {
				logHeaderSb2.append(stkTraceElem[j].getMethodName()).append("().");
			}

			logHeaderSb2.setLength(logHeaderSb2.length() - 1); // ������ '.' ����
		}

		logHeaderSb2.append("] ---");

		// ���� �ش� 1,2�� ����
		newLogObjs[0] = logHeader1;
		newLogObjs[1] = logHeaderSb2.toString();
		logHeaderSb2.setLength(0);

		// �α� ���
		this.log(logType, newLogObjs);
	}
}