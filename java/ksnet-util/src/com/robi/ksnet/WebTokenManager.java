package com.robi.ksnet;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.OverlappingFileLockException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Formatter;
import java.util.HashMap;

public class WebTokenManager {

    public static String DEFAULT_WEB_TOKEN_DIR = "rb_web_tokens/";  // ����ū ���� ��ġ(����Ʈ)

    // - ����ū ���� : [ {svrCode}{svcCode}{systimesec(Base62)}{TodayWtIdCounter(Base62)} ]
    // - ��ū ���� : [ �����ڵ�(2) + �����ڵ�(2) + �ʴ����ð�����(6) + ����(1) + ���Ͻ�����(4) + ����(1) ] => 16�ڸ�
    private static final long WtIdCounterOffset = 1000;  // ��ū ��ȣ ���� ������
    private static long TodayWtIdCounter = 0;            // ���� �� ��ū ī����
    private static int TodayDate = -1;                   // ���� ����
    private static long LastTokenSecond = -1;            // ���������� ��µ� ������ ��
    private static String LastTokenTimeCacheStr = null;  // ���������� ��µ� ��ū �ð����� ���ڿ�

    // ������ ����
    public static void setDefaultWebTokenDir(String newWebTokenDir) {
        DEFAULT_WEB_TOKEN_DIR = newWebTokenDir;
    }

    // ���� �� ��ū�� ��ȯ
    public static synchronized String genWebTokenStr(String svrCode, String svcCode, Logger logger) {
        Calendar cal = Calendar.getInstance();
        int day = cal.get(Calendar.DAY_OF_MONTH);
        long sysTimeSec = cal.getTimeInMillis() / 1000L; // �ʴ����� ����

        if (day != TodayDate) { // ���ڰ� �����
            TodayWtIdCounter = WtIdCounterOffset;
            TodayDate = day;
        }
        
        ++TodayWtIdCounter;

        StringBuilder sb = new StringBuilder();

        if (sysTimeSec != LastTokenSecond) {
            // 1���̻� ����� ȣ��: �ð����� �����, ĳ�� ��ü
            LastTokenSecond = sysTimeSec;
            LastTokenTimeCacheStr = ("000000" + Base62.Encode(sysTimeSec)); // ĳ�õ� �ð����� ���ڿ� ����
            int cacheLen = LastTokenTimeCacheStr.length();
            LastTokenTimeCacheStr = LastTokenTimeCacheStr.substring(cacheLen - 6, cacheLen);
        }

        String b62WtIdCount  = ("0000" + Base62.Encode(TodayWtIdCounter));
        int countLen = b62WtIdCount.length();
        b62WtIdCount = b62WtIdCount.substring(countLen - 4, countLen);

        String randomPadding = RandomManager.genRandomStr(2, "ALPHA_NUMBER");

        sb.append(svrCode).append(svcCode).append(LastTokenTimeCacheStr)
          .append(randomPadding.charAt(0)).append(b62WtIdCount).append(randomPadding.charAt(1));

        return sb.toString();
    }

    // ��ū ��ȣ Ȯ��
    public static boolean checkWebTokenPw(String wtDirStr, String wtId, String wtPw, Logger logger) {
        HashMap<String, String> readMap = new HashMap<String, String>();

        if (!readWebToken(wtDirStr, wtId, wtPw, readMap, logger)) { // ��ū�� ���������� �о����� �н�����üũ ���
            return false;
        }
        
        return true;
    }

    // wtId�̸��� wtPw��ȣ�� ���� ��ū������ ���� �� ��� ��ȯ
    public static boolean genWebTokenFile(String wtDirStr, String wtId, String wtPw, Logger logger) {
        if (wtId == null || wtPw == null) {
            return false;
        }

        wtDirStr = checkWebTokenDirStr(wtDirStr);

        File wtDir = new File(wtDirStr);
        File wtFile = null;

        try {
            // ��ū��� �˻� or ����
            if (!wtDir.exists()) {
                if(!wtDir.mkdirs()) {
                    log(logger, Logger.ERROR, wtId, "mkdirs() return false!");
                    return false; // �� ��� ���� ����
                }
            }

            // ��ū���� ����
            wtFile = new File(wtDirStr + wtId);

            if (!wtFile.exists()) {
                if (!wtFile.createNewFile()) {
                    log(logger, Logger.ERROR, "createNewFile() return false!");
                    return false; // �� ��ū ���� ���� ����
                }
            }
            else {
                return false; // ��ū ������ �̹� ������
            }
        }
        catch (Exception e0) {
            log(logger, Logger.ERROR, wtId, "Exception!", e0);
        }

        // ��ū�� wtPw(��ū �н�����)�� ���� �ý��۽ð� ���
        RandomAccessFile wtStream = null;
        FileChannel wtChannel = null;
        FileLock wtLock = null;
        String keyValStr = String.format("{\n\"_wtPw\":\"%s\",\n\"_wtCreatedTime\":\"%d\"\n}", wtPw, System.currentTimeMillis());

        try {
            wtStream = new RandomAccessFile(wtFile, "rw");
            wtChannel = wtStream.getChannel();

            // // FILE LOCKED // //
            wtLock = wtChannel.tryLock(); // Already Locked : throws OverlappingFileLockException
            // // FILE LOCKED // //

            wtChannel.write(ByteBuffer.wrap(keyValStr.getBytes()));
        }
        catch (Exception e3) {
            log(logger, Logger.ERROR, wtId, "Exception!", e3);
        }
        finally {
            try {
                // FILE UNLOCKED //
                if (wtLock != null) {
                    wtLock.release();   // OverlappingFileLockException ���ܸ� �����ϰ��
                                        // �׻� �� �ڵ忡 �������Ѽ� Locking ����
                }
                // FILE UNLOCKED //

                if (wtStream != null) wtStream.close();
                if (wtChannel != null) wtChannel.close();
            }
            catch (Exception e3_2) {
                log(logger, Logger.ERROR, wtId, "Exception!", e3_2);
            }
        }

        return true;
    }

    // �� ��ū���� ����
    public static boolean removeWebTokenFile(String wtDirStr, String wtId, Logger logger) {
        if (wtId == null) {
            return false;
        }

        wtDirStr = checkWebTokenDirStr(wtDirStr);
        
        File wtDir = new File(wtDirStr);
        File wtFile = null;
        
        try {
            // ��ū���� ��� �˻�
            if (!wtDir.exists()) {
                return false;
            }

            wtFile = new File(wtDirStr + wtId);

            // ��ū���� ����
            if (!wtFile.exists()) {
                return false;
            }

            // ���� ����
            wtFile.delete();
            wtFile = null;
        }
        catch (Exception e0) {
            log(logger, Logger.ERROR, wtId, "Exception!", e0);
            return false;
        }

        return true;
    }

    // wtId�̸��� ���� ������ �� �� tkMap�� �����͸� JSON �������� ����
    public static boolean writeWebToken(String wtDirStr, String wtId, String wtPw, HashMap<String, String> tkMap, Logger logger) {
        if (wtId == null || tkMap == null || tkMap.size() < 1) {
            return false;
        }

        wtDirStr = checkWebTokenDirStr(wtDirStr);
        
        File wtDir = new File(wtDirStr);
        File wtFile = null;
        
        try {
            // ��ū���� ��� �˻�
            if (!wtDir.exists()) {
                return false;
            }

            wtFile = new File(wtDirStr + wtId);

            // ��ū���� ����
            if (!wtFile.exists()) {
                return false;
            }
        }
        catch (Exception e0) {
            log(logger, Logger.ERROR, wtId, "Exception!", e0);
        }

        // ���Ͽ� ����
        int retryLimitCnt = 10;
        int retryDelayMillis = (int)(System.currentTimeMillis() % 101L) + 50; // 50~150ms

        if (!fileLockWithReadWrite(wtFile, wtId, wtPw, null, tkMap, retryLimitCnt, retryDelayMillis, logger)) {
            return false;
        }

        return true;
    }

    // wtId�̸��� ���� ������ ���� �� tkMap�� JSON�����͸� ä��
    public static boolean readWebToken(String wtDirStr, String wtId, String wtPw, HashMap<String, String> tkMap, Logger logger) {
        if (wtId == null || wtPw == null || tkMap == null) {
            return false;
        }

        wtDirStr = checkWebTokenDirStr(wtDirStr);

        // ��ū���� ����
        boolean wtPwPassed = false;
        File wtFile = new File(wtDirStr + wtId);

        try {
            if (!wtFile.exists()) {
                return false;
            }
        }
        catch (Exception e0) {
            log(logger, Logger.ERROR, wtId, "Exception!", e0);
            return false;
        }

        // ���Ͽ��� �б�
        int retryLimitCnt = 10;
        int retryDelayMillis = (int)(System.currentTimeMillis() % 101L) + 50; // 50~150ms

        if (!fileLockWithReadWrite(wtFile, wtId, wtPw, tkMap, null, retryLimitCnt, retryDelayMillis, logger)) {
            return false;
        }

        return true;
    }

    // ���� �� ���� -> ���� '�б�(Read) / �б�+����(Write)' -> ���� �� ����
    private static boolean fileLockWithReadWrite(File wtFile, String wtId, String wtPw, HashMap<String, String> wtReadMap,
                                                 HashMap<String, String> wtWriteMap, int retryLimitCnt, int retryDelay, Logger logger) {
        // [Note]
        // 1) ����ū �б� �۾��� ���϶� ���� �� ���� JSON�� �Ľ��Ͽ� wtReadMap�� ��ƺ����� �� ������ �������� ��ȯ
        // 2) ����ū ���� �۾��� �б� �۾� ����, �߰��� �Ķ���Ͱ��� wtReadMap�� ���Ͽ� ���Ͽ� ���� ����ϰ� �� ������ �������� ��ȯ
        
        if (wtReadMap == null && wtWriteMap != null) {
            wtReadMap = new HashMap<String, String>();
        }
        else if (wtReadMap == null && wtWriteMap == null) {
            return false;
        }

        boolean rtResult = false;
        boolean fileReadComplete = false;
        RandomAccessFile wtStream = null;
        FileChannel wtChannel = null;
        FileLock wtLock = null;

        // [Note]
        // - wtChannel.tryLock() ���� �ٸ� ���μ���/�����忡���� ���� �б�/���⸦ ������ �� �ִ�.
        // - Ư���� ���� tryLock()�� ������ try-catch-finally ���� �ȿ��� �б�/����� ����������,
        //   �ش� ������ ��� ������ ������ �õ��ϸ� IOException�� �߻��Ѵ�. (������ ������ Locking�� �ƴ� ��)
        //   �׷��� ������ ������ �и����� ���ϰ� �ϳ��� ������ �� �ۼ��ߴ�.
        
        while ((retryLimitCnt--) > 0) {
            try {
                wtStream = new RandomAccessFile(wtFile, "rw");
                wtChannel = wtStream.getChannel();

                // // FILE LOCKED // //
                wtLock = wtChannel.tryLock(); // ������ �̹� ������� : throws OverlappingFileLockException
                // // FILE LOCKED // //

                // �б� �۾� ����
                // ������ ����Ʈ ������ �о wtStr�� ����
                ByteBuffer readBuf = ByteBuffer.allocate((int)wtStream.length());
                wtChannel.read(readBuf);
                String wtStr = new String(readBuf.array());
                fileReadComplete = true;

                // JSON ���ڿ� �Ľ�
                int keyBgnIdx = -1, keyEndIdx = -1, valBgnIdx = -1, valEndIdx = -1;
                String key = null, val = null;

                if ((keyBgnIdx = wtStr.indexOf("{")) == -1) {
                    log(logger, Logger.ERROR, wtId, String.format("keyBgnIdx error! (wtFileName:%s, keyBgnIdx:%d)", wtFile.getName(), keyBgnIdx));
                    return false;
                }

                while ((keyBgnIdx = wtStr.indexOf("\"", keyBgnIdx)) != -1) {
                    // Key
                    ++keyBgnIdx;

                    if ((keyEndIdx = wtStr.indexOf("\":", keyBgnIdx)) == -1) {
                        log(logger, Logger.ERROR, wtId, String.format("keyEndIdx error! (wtFileName:%s, keyBgnIdx:%d, keyEndIdx:%d)", wtFile.getName(), keyBgnIdx, keyEndIdx));
                        break;
                    }

                    if (keyBgnIdx < keyEndIdx) {
                        // Found key
                        key = wtStr.substring(keyBgnIdx, keyEndIdx);
                    }
                    else {
                        log(logger, Logger.ERROR, wtId, String.format("null key error! (wtFileName:%s, keyBgnIdx:%d, keyEndIdx:%d)", wtFile.getName(), keyBgnIdx, keyEndIdx));
                        break;
                    }

                    keyEndIdx += 2;

                    // Value
                    if ((valBgnIdx = wtStr.indexOf("\"", keyEndIdx)) == -1) {
                        log(logger, Logger.ERROR, wtId, String.format("valBgnIdx error! (wtFileName:%s, keyEndIdx:%d, valBgnIdx:%d)", wtFile.getName(), keyEndIdx, valBgnIdx));
                        break;
                    }

                    ++valBgnIdx;

                    if ((valEndIdx = wtStr.indexOf("\"", valBgnIdx)) == -1) {
                        log(logger, Logger.ERROR, wtId, String.format("valEndIdx error! (wtFileName:%s, valBgnIdx:%d, valEndIdx:%d)", wtFile.getName(), valBgnIdx, valEndIdx));
                        break;
                    }

                    if (valBgnIdx < valEndIdx) {
                        // Found value
                        val = wtStr.substring(valBgnIdx, valEndIdx);
                    }
                    else if (valBgnIdx == valEndIdx) {
                        // Value is null
                        val = null;
                    }
                    else {
                        log(logger, Logger.ERROR, wtId, String.format("valBgn/EndIdx error! (wtFileName:%s, valBgnIdx:%d, valEndIdx:%d)", wtFile.getName(), valBgnIdx, valEndIdx));
                        break;
                    }

                    keyBgnIdx = Math.min(valEndIdx + 1, wtStr.length());

                    // Put key and val to wtReadMap
                    wtReadMap.put(key, val);
                }

                // ��ū �н����� üũ
                String readWtPw = wtReadMap.get("_wtPw");

                if (readWtPw == null || (!readWtPw.equals(wtPw))) {
                    // �н����� Ʋ�� �� ���� ������ �ı�
                    log(logger, Logger.WARNING, wtId, String.format("Wrong password! (wtFileName:%s, inputWtPw:%s)", wtFile.getName(), wtPw));
                    wtReadMap.clear();
                    rtResult = false;
                    break;
                }
                else {
                    // wtWriteMap�� ���޵� ��� : ���� �۾� ����
                    if (wtWriteMap != null && wtWriteMap.size() > 0) {
                        // ���������� ������ �ð��� �߰�
                        wtWriteMap.put("_wtLastModifiedTime", String.valueOf(System.currentTimeMillis()));
                        
                        // ���� �����Ϳ� �߰��� �����͸� ��ħ
                        wtReadMap.putAll(wtWriteMap);

                        StringBuffer keyValSb = new StringBuffer();
                        keyValSb.append("{\n");

                        for (String writeKey : wtReadMap.keySet()) {
                            keyValSb.append("\"").append(writeKey).append("\":\"")
                                    .append(wtReadMap.get(writeKey)).append("\",\n"); // "writeKey":"writeVal",\n
                        }

                        keyValSb.setLength(keyValSb.length() - 2); // ������ ",\n" ���� ����
                        keyValSb.append("\n}");

                        wtStream.setLength(0); // ���� ���� �� ����� ���ο� �������� ��
                        wtChannel.write(ByteBuffer.wrap(keyValSb.toString().getBytes()));
                    }

                    rtResult = true;
                }

                break; // ���� ��, �ݺ��� Ż��
            }
            catch (OverlappingFileLockException e1) {
                // ������ ���� ���� {retryDelay}ms �Ŀ� ��õ�
                try { Thread.sleep(retryDelay); } catch (Exception e1_1) { 
                    log(logger, Logger.ERROR, wtId, String.format("Exception! (wtFileName:%s)", wtFile.getName()), e1_1);
                }
                continue;
            }
            catch (Exception e2) {
                log(logger, Logger.ERROR, wtId, String.format("Exception! (wtFileName:%s)", wtFile.getName()), e2);
            }
        }

        try {
            // FILE UNLOCKED //
            if (wtLock != null) {   // OverlappingFileLockException ���ܸ� �����ϰ��
                wtLock.release();   // �׻� �� �ڵ忡 �����Ͽ� Locking ����
            }
            // FILE UNLOCKED //

            if (wtStream != null) wtStream.close();
            if (wtChannel != null) wtChannel.close();
        }
        catch (Exception e3) {
            // ���� : ������ �̰��� ������ ��쿡�� ���� ���� ��Ǯ�Ȱų� ä���� ���������� ������ �ʾ��� �� ����!
            log(logger, Logger.ERROR, wtId, String.format("Exception! (wtFileName:%s)", wtFile.getName()), e3);
            return false;
        }

        if (!fileReadComplete) {
            // ���� �б� ����
            log(logger, Logger.ERROR, wtId, String.format("File read failed! (wtFileName:%s)", wtFile.getName()));
            return false;
        }

        return rtResult;
    }

    // ��ū ������� �˻� (���� ������ ���ڿ��� �׻� '/' �� �ǵ��� ��)
    private static String checkWebTokenDirStr(String wtDirStr) {
        if (wtDirStr == null) {
            wtDirStr = DEFAULT_WEB_TOKEN_DIR;
        }

        int dirStrLen = wtDirStr.length();

        if (wtDirStr.charAt(dirStrLen - 1) != '/') {
            wtDirStr += "/";
        }

        return wtDirStr;
    }

    // WebTokenManager�ΰ�
    private static void log(Logger logger, int logType, String traceId, Object... logObjs) {
        if (logger != null) {
            logger.traceLog(logType, traceId, logObjs);
        }
	}
}