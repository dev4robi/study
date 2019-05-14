package com.robi.ksnet;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class Logger {

	// 클래스 내부 상수 //
	private static final String[] LogTypeSymbols = new String[] { "[DBG]", "[INF]", "[WAN]", "[ERR]", "[FAT]" };
	private static final SimpleDateFormat SimpleDateFmt = new SimpleDateFormat("[yyyy-MM-dd HH:mm:ss]");	

	// 클래스 공개 상수 //
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

	// 클래스 내부 변수 //
	private String LogFileNameStr = null;		// 로그파일 이름 (yyyyMMdd.log)
	private int LastDate = -1;					// 일자 변동 감지
	private String CachedTimeStr = null;		// 로그 작성시간 캐시 문자열 ([yyyy-MM-dd HH:mm:ss])
	private int LastSec = -1;					// 초 변동 감지
	private File LogDir = null;					// 로그 경로
	private File LogFile = null;				// 로그 파일
	private PrintStream LogPs = null;			// 로그 출력 스트림
	private boolean RequestFileRefresh = true;	// LogDir, LogFile, LogPs 갱신이 필요한지 확인하는 스위치 변수
	private String MissingLogStr = null;		// 오류로 인해 출력하지 못한 마지막 로그
	
	// 클래스 변수 //
	public boolean LOGGER_INITIALIZED = false;	// 로거 초기화 여부
	public String LOG_FILENAME_PREFIX;			// 로그파일명 접두사
	public String LOG_DIR;						// 로그 저장 폴더
	public String LOG_CHARSET;					// 로그 체어셋

	// 클래스 정적 메서드 //
	// 로거 생성
	public static Logger getInstance() {
		return new Logger();
	}

	// 클래스 메서드 //
	// 로거 초기화
	public boolean init(String filenamePrefix, String logDir, String charset) {
		if (!setLogFilenamePrefix(filenamePrefix) || !setLogDir(logDir) || !setLogCharset(charset)) {
			return false;
		}
		
		LOGGER_INITIALIZED = true;
		return true;
	}

	// 로거 닫기
	public void close() {
		try {
			if (LogPs != null) LogPs.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 로그파일명 접두사 설정
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

	// 로그 경로설정
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

	// 로그 체어셋 설정
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

	// 에러로그 생성
	public void error(Object... logObjs) {
		this.log(this.ERROR, logObjs);
	}

	// 정보로그 생성
	public void info(Object... logObjs) {
		this.log(this.INFO, logObjs);
	}

	// 로그 생성
	public void log(int logLv, Object... logObjs) {
		// 미초기화시 초기화
		if (!LOGGER_INITIALIZED) {
			if (init(DEFAULT_LOG_PREFIX, DEFAULT_LOG_DIR, DEFAULT_LOG_CHARSET)) {
				return;
			}
		}

		// 로그 바디 생성
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

				logBodySb.setLength(logBodySb.length() - 4); // 마지막 " <- " 문자 제거
				logBodySb.append(" }");
			}
			else {
				logBodySb.append(logObj.toString());
			}

			logBodySb.append(' ');
		}

		logBodySb.setLength(logBodySb.length() - 1); // 마지막줄 " " 제거
		
		synchronized (Logger.class) {
			// 시간정보 문자열 생성
			String curTimeStr = null;
			Calendar curCal = Calendar.getInstance();
			int curSec = curCal.get(Calendar.SECOND);
			int curDate = curCal.get(Calendar.DATE);
			
			if (curSec != LastSec) { // 1초 이상 시간 경과 시 캐싱된 시간 갱신
				CachedTimeStr = SimpleDateFmt.format(curCal.getTime());
				LastSec = curSec;
			}

			curTimeStr = CachedTimeStr; // "[yyyy-MM-dd HH:mm:ss]"

			// 로그파일 이름 생성
			if (RequestFileRefresh || curDate != LastDate) { // 일자 변경 시 로그파일명 갱신
				LogFileNameStr = String.format("%s.%s%s%s.log", LOG_FILENAME_PREFIX, curTimeStr.substring(1, 5),
											   curTimeStr.substring(6, 8), curTimeStr.substring(9, 11)); // "yyyyMMdd.log"
				LastDate = curDate;
				RequestFileRefresh = true;
			}

			// 로그 헤더 생성
			logHeadSb.append(curTimeStr).append(':').append(LogTypeSymbols[logLv]).append(' ');

			// 로그 헤더 + 바디
			logHeadSb.append(logBodySb);
			logStr = logHeadSb.toString();
			logHeadSb.setLength(0);
			logBodySb.setLength(0);

			// [Note] RequestFileRefresh값이 true값이 되는 경우
			// 1. Logger 초기화 직후
			// 2. 일자가 변경되어 새 로그 파일이 생성되야 하는 경우
			// 3. 기존 출력 스트림이 손상되어 새 출력 스트림이 생성되어야 하는 경우

			// 로그폴더/파일 생성 및 출력스트림 처리
			if (RequestFileRefresh) {
				// 기존 출력 스트림 닫기
				try {
					if (LogPs != null) LogPs.close();
				}
				catch (Exception e1) {
					e1.printStackTrace();
				}
				finally {
					LogPs = null;
				}

				// 로그폴더 확인 및 생성
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
				
				// 파일 생성 + 출력 스트림 준비
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
				// 파일 검사
				if (!LogFile.exists()) {
					throw new Exception("LogFileMissingException!");
				}

				// 파일에 출력
				if (LogPs != null) {
					// 마지막으로 출력못한 로그가 있으면 출력
					if (MissingLogStr != null) {
						LogPs.println(MissingLogStr);
						MissingLogStr = null;
					}

					// 현재 로그 출력
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

	// traceId로 추적기능을 제공하는 로그
	public void traceLog(int logType, String traceId, Object... logObjs) {
		if (!this.LOGGER_INITIALIZED) {
			this.init(DEFAULT_LOG_PREFIX, DEFAULT_LOG_DIR, DEFAULT_LOG_CHARSET);
		}

		Object[] newLogObjs = new Object[logObjs.length + 2];
		int lastStkIdx = 1;

		for (int i = 2; i < newLogObjs.length; ++i) {
			newLogObjs[i] = logObjs[i - 2];
		}

		// 로그 헤더1 생성
		String logHeader1 = String.format("[%s]", traceId);

		// 로그 헤더2 생성
		StringBuilder logHeaderSb2 = new StringBuilder();
		StackTraceElement[] stkTraceElem = new Throwable().getStackTrace();

		logHeaderSb2.append('[').append(stkTraceElem[lastStkIdx].getFileName());

		if (logType != Logger.INFO) {	
			int maxStkLimit = Math.min(stkTraceElem.length, 2); // 기본: 콜스택 2단계까지 출력
			
			logHeaderSb2.append("(").append(stkTraceElem[lastStkIdx].getLineNumber()).append(").");

			if (logType != Logger.INFO) {
				maxStkLimit = Math.min(stkTraceElem.length, 3); // INFO로그 제외: 콜스택 3단계까지 출력
			}
			else {
				logHeaderSb2.append("..");
			}

			for (int j = maxStkLimit; j >= lastStkIdx; --j) {
				logHeaderSb2.append(stkTraceElem[j].getMethodName()).append("().");
			}

			logHeaderSb2.setLength(logHeaderSb2.length() - 1); // 마지막 '.' 제거
		}

		logHeaderSb2.append("] ---");

		// 생성 해더 1,2를 대입
		newLogObjs[0] = logHeader1;
		newLogObjs[1] = logHeaderSb2.toString();
		logHeaderSb2.setLength(0);

		// 로그 출력
		this.log(logType, newLogObjs);
	}
}