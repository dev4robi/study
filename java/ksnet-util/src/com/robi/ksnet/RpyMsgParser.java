package com.robi.ksnet;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class RpyMsgParser {
    // [Ŭ���� ���� ���] //
    public static final int WECHAT_QR_V1 = 1;   // �������� QR���� ����
    public static final int ALIPAY_QR_V1 = 2;   // �˸����� QR���� ����

    // [Ŭ���� ���� ����] //
    private IpgMsg rpyMsg;    // �Ľ� ��� ����
    private String traceId;   // Ʈ���̽� ID
    private Logger logger;    // �ΰ�

    // [������] //
    public RpyMsgParser(IpgMsg rpyMsg, String traceId, Logger logger) {
        this.rpyMsg = rpyMsg;
        this.traceId = traceId;
        this.logger = logger;
    }

    // [�޼���] //
    // ������ �Ľ��Ͽ� ������ ��ȯ
    public Map<String, String> parse(int msgType) {
        if (rpyMsg == null) {
            log(Logger.ERROR, "'rpyMsg' is null!");
            return null;
        }

        switch (msgType) {
            case WECHAT_QR_V1:
                return parse_WECHAT_QR_V1(); // �������� QR���� ����
            case ALIPAY_QR_V1:
                return parse_ALIPAY_QR_V1(); // �˸����� QR���� ����
            default:
                return null;
        }
    }

    // MsgParser �α�
    private void log(int logType, Object... logObjs) {
        if (this.logger != null) {
            logger.traceLog(Logger.ERROR, this.traceId, "[MsgParse]", logObjs);
        }
    }

    // WECHAT_QR_V1 ���� (2019.03.11)
    private Map<String, String> parse_WECHAT_QR_V1() {
        Map<String, String> rtMap = null;
        Map<String, IpgMsg.MsgAttr> msgAttr = rpyMsg.getMsgAttrMap();
        int excTrace = 0;

        try {
            // += [�������� ��ȸ ����] ==========================================================================================+
            if (msgAttr == IpgMsg.RpyShopInfoMsgAttr) {
                String shop_name 		= null;	// ��������
                String entr_numb		= null;	// ����ڹ�ȣ
                String telephone		= null;	// ����������ó
                String date_time		= null;	// �ŷ�����
                String currency_code	= null;	// ��ȭ�ڵ�
                String appid			= null;	// WeChat-KSNET Service ID
                String mch_id			= null;	// WeChat-KSNET-Merchant ID
                String sub_mch_id		= null;	// WeChat-KSNET-Merchant-SubMerchant ID
                String api_key			= null;	// WeChat-KSNET API key (�ؽü����)
                String secret			= null;	// WeChat-KSNET APP Secret (openid ȹ���)
                String goods_id			= null; // WeChat-KSNET-Merchand�� �����
                String status           = null; // �����ڵ�
        
                // shop_info_byte(shop_info) : ��������(50)+��������ȭ��ȣ(15)+�������޴�����ȣ(15)+�������ּ�(130)+APP_Secret(32)
                byte[] shop_info_byte = rpyMsg.get("shop_info").getBytes();
                excTrace = 1;
                // shop_name(notice2) : ������ ��(����)
                shop_name            = rpyMsg.get("notice2").trim();
                excTrace = 2;
                // entr_numb(notice1) : ����ڹ�ȣ(xxxyyzzzzz) -> xxx-yy-zzzzz
                String entr_numb_raw = rpyMsg.get("notice1").trim();
                entr_numb			 = String.format("%s-%s-%s", entr_numb_raw.substring(0, 3),			// xxx
                                                                entr_numb_raw.substring(3, 5),			// yy
                                                                entr_numb_raw.substring(5, 10)).trim();	// zzzzz
                excTrace = 3;
                // telephone(shop_info(50-65)) : ������ ��ȭ��ȣ
                telephone		= new String(Arrays.copyOfRange(shop_info_byte, 50, 65)).trim();
                // date_time(deal_date) : �ŷ��Ͻ�(yymmddhhMMss) -> 20yy-mm-dd hh:MM:ss
                String date_time_raw = rpyMsg.get("deal_date").trim();
                date_time			 = String.format("20%s-%s-%s %s:%s:%s", date_time_raw.substring(0, 2),		// yy
                                                                            date_time_raw.substring(2, 4),   	// mm
                                                                            date_time_raw.substring(4, 6),   	// dd
                                                                            date_time_raw.substring(6, 8),   	// hh
                                                                            date_time_raw.substring(8, 10),  	// MM
                                                                            date_time_raw.substring(10, 12));	// ss
                excTrace = 4;
                // currency_code : ��ȭ�ڵ� (410:KRW, 840:USD)
                currency_code	= rpyMsg.get("reserved").trim();
                excTrace = 5;
                // appid
                appid           = (rpyMsg.get("working_key_index") + rpyMsg.get("working_key")).trim();
                excTrace = 6;
                // mch_id
                mch_id		    = entr_numb_raw.substring(10, 20).trim();
                excTrace = 7;
                // sub_mch_id
                sub_mch_id	    = rpyMsg.get("shop_numb").trim();
                excTrace = 8;
                // api_key
                api_key		    = rpyMsg.get("ksnet_reserved").trim();
                excTrace = 9;
                // secret
                secret		    = new String(Arrays.copyOfRange(shop_info_byte, 210, 242)).trim();
                excTrace = 10;
                // goods_id
                goods_id	    = rpyMsg.get("comp_info").trim();
                excTrace = 11;
                // status
                status          = rpyMsg.get("status");
                excTrace = 12;

                // �ʿ� �־� ��ȯ
                rtMap = new HashMap<String, String>();
                rtMap.put("shop_name",      shop_name);
                rtMap.put("entr_numb",      entr_numb);
                rtMap.put("telephone",      telephone);
                rtMap.put("date_time",      date_time);
                rtMap.put("currency_code",  currency_code);
                rtMap.put("appid",          appid);
                rtMap.put("mch_id",         mch_id);
                rtMap.put("sub_mch_id",     sub_mch_id);
                rtMap.put("api_key",        api_key);
                rtMap.put("secret",         secret);
                rtMap.put("goods_id",       goods_id);
                rtMap.put("status",         status);
            }
            // += [���ι�ȣ ���� ����] ==========================================================================================+
            else if (msgAttr == IpgMsg.RpyArrovalNumMsgAttr) {
                String shop_name 		= null;	// ��������
                String entr_numb		= null;	// ����ڹ�ȣ
                String telephone		= null;	// ����������ó
                String date_time		= null;	// �ŷ�����
                String currency_code	= null;	// ��ȭ�ڵ�
                String appid			= null;	// WeChat-KSNET Service ID
                String mch_id			= null;	// WeChat-KSNET-Merchant ID
                String sub_mch_id		= null;	// WeChat-KSNET-Merchant-SubMerchant ID
                String api_key			= null;	// WeChat-KSNET API key (�ؽü����)
                String secret			= null;	// WeChat-KSNET APP Secret (openid ȹ���)
                String goods_id			= null; // WeChat-KSNET-Merchand�� �����
                String approval_numb    = null; // ���ι�ȣ(ä��)
                String status           = null; // �����ڵ�
        
                // shop_info_byte(shop_info) : ��������(50)+��������ȭ��ȣ(15)+�������޴�����ȣ(15)+�������ּ�(130)+APP_Secret(32)
                byte[] shop_info_byte = rpyMsg.get("shop_info").getBytes();
                excTrace = 1;
                // shop_name(notice2) : ������ ��(����)
                shop_name 		= rpyMsg.get("notice2").trim();
                excTrace = 2;
                // entr_numb(notice1) : ����ڹ�ȣ(xxxyyzzzzz) -> xxx-yy-zzzzz
                String entr_numb_raw = rpyMsg.get("notice1").trim();
                entr_numb			 = String.format("%s-%s-%s", entr_numb_raw.substring(0, 3),			// xxx
                                                                entr_numb_raw.substring(3, 5),			// yy
                                                                entr_numb_raw.substring(5, 10)).trim();	// zzzzz
                excTrace = 3;
                // telephone(shop_info(50-65)) : ������ ��ȭ��ȣ
                telephone		= new String(Arrays.copyOfRange(shop_info_byte, 50, 65)).trim();
                // date_time(deal_date) : �ŷ��Ͻ�(yymmddhhMMss) -> 20yy-mm-dd hh:MM:ss
                String date_time_raw = rpyMsg.get("deal_date").trim();
                date_time			 = String.format("20%s-%s-%s %s:%s:%s", date_time_raw.substring(0, 2),		// yy
                                                                            date_time_raw.substring(2, 4),   	// mm
                                                                            date_time_raw.substring(4, 6),   	// dd
                                                                            date_time_raw.substring(6, 8),   	// hh
                                                                            date_time_raw.substring(8, 10),  	// MM
                                                                            date_time_raw.substring(10, 12));	// ss
                excTrace = 4;
                // currency_code : ��ȭ�ڵ� (410:KRW, 840:USD)
                currency_code	= rpyMsg.get("reserved").trim();
                excTrace = 5;
                // appid
                appid           = (rpyMsg.get("working_key_index") + rpyMsg.get("working_key")).trim();
                excTrace = 6;
                // mch_id
                mch_id		    = entr_numb_raw.substring(10, 20).trim();
                excTrace = 7;
                // sub_mch_id
                sub_mch_id	    = rpyMsg.get("shop_numb").trim();
                excTrace = 8;
                // api_key
                api_key		    = rpyMsg.get("ksnet_reserved").trim();
                excTrace = 9;
                // secret
                secret		    = new String(Arrays.copyOfRange(shop_info_byte, 210, 242)).trim();
                excTrace = 10;
                // goods_id
                goods_id	    = rpyMsg.get("comp_info").trim();
                excTrace = 11;
                // approval_numb
                approval_numb   = rpyMsg.get("approval_numb").trim();
                excTrace = 12;
                // status
                status          = rpyMsg.get("status");
                excTrace = 13;

                // �ʿ� �־� ��ȯ
                rtMap = new HashMap<String, String>();
                rtMap.put("shop_name",      shop_name);
                rtMap.put("entr_numb",      entr_numb);
                rtMap.put("telephone",      telephone);
                rtMap.put("date_time",      date_time);
                rtMap.put("currency_code",  currency_code);
                rtMap.put("appid",          appid);
                rtMap.put("mch_id",         mch_id);
                rtMap.put("sub_mch_id",     sub_mch_id);
                rtMap.put("api_key",        api_key);
                rtMap.put("secret",         secret);
                rtMap.put("goods_id",       goods_id);
                rtMap.put("approval_numb",  approval_numb);
                rtMap.put("status",         status);
            }
            // += [�ŷ��뺸 ����] ==============================================================================================+
            else if (msgAttr == IpgMsg.RpyPayNotifyAttr) {
                String date_time    = null; // �뺸 �Ͻ�
                String message1     = null; // �޽���1
                String message2     = null; // �޽���2
                String status       = null; // �����ڵ�

                // date_time : �뺸 �Ͻ�(yymmddhhMMss)
                String date_time_raw = rpyMsg.get("deal_date").trim();
                date_time			 = String.format("20%s-%s-%s %s:%s:%s", date_time_raw.substring(0, 2),		// yy
                                                                            date_time_raw.substring(2, 4),   	// mm
                                                                            date_time_raw.substring(4, 6),   	// dd
                                                                            date_time_raw.substring(6, 8),   	// hh
                                                                            date_time_raw.substring(8, 10),  	// MM
                                                                            date_time_raw.substring(10, 12));	// ss
                excTrace = 1;
                // message1, message2
                message1             = rpyMsg.get("message1").trim();
                message2             = rpyMsg.get("message2").trim();
                excTrace = 2;
                // status
                status               = rpyMsg.get("status");
                excTrace = 3;

                // �ʿ� �־� ��ȯ
                rtMap = new HashMap<String, String>();
                rtMap.put("date_time",  date_time);
                rtMap.put("message1",   message1);
                rtMap.put("message2",   message2);
                rtMap.put("status",     status);
            }
            // += [�ļ��� ������ �����Ӽ�] =======================================================================================+
            else {
                log(Logger.ERROR, "Undefined 'msgAttr' in parser! (MsgName: '" + rpyMsg.getMsgName() +  "', msgAttr: '" + msgAttr + "')");
                return null;
            }
        }
        catch (Exception e) {
            log(Logger.ERROR, "Exception! (MsgName: '" + rpyMsg.getMsgName() +  "', " + "ExcTrace:'" + excTrace + "')", e);
            return null;
        }

        return rtMap;
    }

    // ALIPAY_QR_V1 ���� (2019.03.11)
    private Map<String, String> parse_ALIPAY_QR_V1() {
        Map<String, String> rtMap = null;
        Map<String, IpgMsg.MsgAttr> msgAttr = rpyMsg.getMsgAttrMap();
        int excTrace = 0;

        try {
            // += [�������� ��ȸ ����] ==========================================================================================+
            if (msgAttr == IpgMsg.RpyShopInfoMsgAttr) {
                String shop_name        = null; // ��������
                String entr_numb        = null; // ����ڹ�ȣ
                String telephone        = null; // ����������ó
                String date_time        = null; // �ŷ��Ͻ�
                String currency_code    = null; // ��ȭ�ڵ�
                String partnerId        = null; // Alipay-KSNET Partner ID
                String privateKey       = null; // Alipay-KSNET Private Key
                String sec_mch_id       = null; // KSNET ������ ID
                String sec_mch_industry = null; // ������ �����
                String status           = null; // ������

                // shop_info_byte(shop_info) : ��������(50)+��������ȭ��ȣ(15)+�������޴�����ȣ(15)+�������ּ�(130)
                byte[] shop_info_byte = rpyMsg.get("shop_info").getBytes();
                excTrace = 1;
                // shop_name(notice2) : ������ ��(����)
                shop_name            = rpyMsg.get("notice2").trim();
                excTrace = 2;
                // entr_numb(notice1) : ����ڹ�ȣ(xxxyyzzzzz) -> xxx-yy-zzzzz
                String entr_numb_raw = rpyMsg.get("notice1").trim();
                entr_numb			 = String.format("%s-%s-%s", entr_numb_raw.substring(0, 3),			    // xxx
                                                                 entr_numb_raw.substring(3, 5),			    // yy
                                                                 entr_numb_raw.substring(5, 10)).trim();	// zzzzz
                excTrace = 3;
                // telephone(shop_info(50-65)) : ������ ��ȭ��ȣ
                telephone		= new String(Arrays.copyOfRange(shop_info_byte, 50, 65)).trim();
                // date_time(deal_date) : �ŷ��Ͻ�(yymmddhhMMss) -> 20yy-mm-dd hh:MM:ss
                String date_time_raw = rpyMsg.get("deal_date").trim();
                date_time			 = String.format("20%s-%s-%s %s:%s:%s", date_time_raw.substring(0, 2),		// yy
                                                                            date_time_raw.substring(2, 4),   	// mm
                                                                            date_time_raw.substring(4, 6),   	// dd
                                                                            date_time_raw.substring(6, 8),   	// hh
                                                                            date_time_raw.substring(8, 10),  	// MM
                                                                            date_time_raw.substring(10, 12));	// ss
                excTrace = 4;
                // currency_code : ��ȭ�ڵ� (410:KRW, 840:USD)
                currency_code       = rpyMsg.get("reserved").trim();
                excTrace = 5;
                // partnerId
                partnerId           = (rpyMsg.get("working_key_index") + rpyMsg.get("working_key")).trim();
                excTrace = 6;
                // privateKey
                privateKey          = rpyMsg.get("ksnet_reserved").trim();
                excTrace = 7;
                // sec_mch_id
                sec_mch_id          = rpyMsg.get("shop_numb").trim();
                excTrace = 8;
                // sec_mch_industry
                sec_mch_industry    = rpyMsg.get("comp_info").trim();
                excTrace = 9;
                // status
                status              = rpyMsg.get("status");
                excTrace = 10;

                // �ʿ� �־� ��ȯ
                rtMap = new HashMap<String, String>();
                rtMap.put("shop_name",          shop_name);
                rtMap.put("entr_numb",          entr_numb);
                rtMap.put("telephone",          telephone);
                rtMap.put("date_time",          date_time);
                rtMap.put("currency_code",      currency_code);
                rtMap.put("partnerId",          partnerId);
                rtMap.put("privateKey",         privateKey);
                rtMap.put("sec_mch_id",         sec_mch_id);
                rtMap.put("sec_mch_industry",   sec_mch_industry);
                rtMap.put("status",             status);
            }
            // += [���ι�ȣ ���� ����] ==========================================================================================+
            else if (msgAttr == IpgMsg.RpyArrovalNumMsgAttr) {
                String shop_name        = null; // ��������
                String entr_numb        = null; // ����ڹ�ȣ
                String telephone        = null; // ����������ó
                String date_time        = null; // �ŷ��Ͻ�
                String currency_code    = null; // ��ȭ�ڵ�
                String partnerId        = null; // Alipay-KSNET Partner ID
                String privateKey       = null; // Alipay-KSNET Private Key
                String sec_mch_id       = null; // KSNET ������ ID
                String sec_mch_industry = null; // ������ �����
                String approval_numb    = null; // ���ι�ȣ(ä��)
                String status           = null; // ������

                // shop_info_byte(shop_info) : ��������(50)+��������ȭ��ȣ(15)+�������޴�����ȣ(15)+�������ּ�(130)
                byte[] shop_info_byte = rpyMsg.get("shop_info").getBytes();
                excTrace = 1;
                // shop_name(notice2) : ������ ��(����)
                shop_name            = rpyMsg.get("notice2").trim();
                excTrace = 2;
                // entr_numb(notice1) : ����ڹ�ȣ(xxxyyzzzzz) -> xxx-yy-zzzzz
                String entr_numb_raw = rpyMsg.get("notice1").trim();
                entr_numb			 = String.format("%s-%s-%s", entr_numb_raw.substring(0, 3),			    // xxx
                                                                 entr_numb_raw.substring(3, 5),			    // yy
                                                                 entr_numb_raw.substring(5, 10)).trim();	// zzzzz
                excTrace = 3;
                // telephone(shop_info(50-65)) : ������ ��ȭ��ȣ
                telephone		= new String(Arrays.copyOfRange(shop_info_byte, 50, 65)).trim();
                // date_time(deal_date) : �ŷ��Ͻ�(yymmddhhMMss) -> 20yy-mm-dd hh:MM:ss
                String date_time_raw = rpyMsg.get("deal_date").trim();
                date_time			 = String.format("20%s-%s-%s %s:%s:%s", date_time_raw.substring(0, 2),		// yy
                                                                            date_time_raw.substring(2, 4),   	// mm
                                                                            date_time_raw.substring(4, 6),   	// dd
                                                                            date_time_raw.substring(6, 8),   	// hh
                                                                            date_time_raw.substring(8, 10),  	// MM
                                                                            date_time_raw.substring(10, 12));	// ss
                excTrace = 4;
                // currency_code : ��ȭ�ڵ� (410:KRW, 840:USD)
                currency_code   = rpyMsg.get("reserved").trim();
                excTrace = 5;
                // partnerId
                partnerId           = (rpyMsg.get("working_key_index") + rpyMsg.get("working_key")).trim();
                excTrace = 6;
                // privateKey
                privateKey          = rpyMsg.get("ksnet_reserved").trim();
                excTrace = 7;
                // sec_mch_id
                sec_mch_id          = rpyMsg.get("shop_numb").trim();
                excTrace = 8;
                // sec_mch_industry
                sec_mch_industry    = rpyMsg.get("comp_info").trim();
                excTrace = 9;
                // approval_numb
                approval_numb       = rpyMsg.get("approval_numb").trim();
                excTrace = 10;
                // status
                status              = rpyMsg.get("status");
                excTrace = 11;

                // �ʿ� �־� ��ȯ
                rtMap = new HashMap<String, String>();
                rtMap.put("shop_name",          shop_name);
                rtMap.put("entr_numb",          entr_numb);
                rtMap.put("telephone",          telephone);
                rtMap.put("date_time",          date_time);
                rtMap.put("currency_code",      currency_code);
                rtMap.put("partnerId",          partnerId);
                rtMap.put("privateKey",         privateKey);
                rtMap.put("sec_mch_id",         sec_mch_id);
                rtMap.put("sec_mch_industry",   sec_mch_industry);
                rtMap.put("approval_numb",      approval_numb);
                rtMap.put("status",             status);
            }
            // += [�ŷ��뺸 ����] ==============================================================================================+
            else if (msgAttr == IpgMsg.RpyPayNotifyAttr) {
                String date_time    = null; // �뺸 �Ͻ�
                String message1     = null; // �޽���1
                String message2     = null; // �޽���2
                String status       = null; // �����ڵ�

                // date_time : �뺸 �Ͻ�(yymmddhhMMss)
                String date_time_raw = rpyMsg.get("deal_date").trim();
                date_time			 = String.format("20%s-%s-%s %s:%s:%s", date_time_raw.substring(0, 2),		// yy
                                                                            date_time_raw.substring(2, 4),   	// mm
                                                                            date_time_raw.substring(4, 6),   	// dd
                                                                            date_time_raw.substring(6, 8),   	// hh
                                                                            date_time_raw.substring(8, 10),  	// MM
                                                                            date_time_raw.substring(10, 12));	// ss
                excTrace = 1;
                // message1, message2
                message1             = rpyMsg.get("message1").trim();
                message2             = rpyMsg.get("message2").trim();
                excTrace = 2;
                // status
                status               = rpyMsg.get("status");
                excTrace = 3;

                // �ʿ� �־� ��ȯ
                rtMap = new HashMap<String, String>();
                rtMap.put("date_time",  date_time);
                rtMap.put("message1",   message1);
                rtMap.put("message2",   message2);
                rtMap.put("status",     status);
            }
            // += [�ļ��� ������ �����Ӽ�] =======================================================================================+
            else {
                log(Logger.ERROR, "Undefined 'msgAttr' in parser! (MsgName: '" + rpyMsg.getMsgName() +  "', msgAttr: '" + msgAttr + "')");
                return null;
            }
        }
        catch (Exception e) {
            log(Logger.ERROR, "Exception! (MsgName: '" + rpyMsg.getMsgName() +  "', " + "ExcTrace:'" + excTrace + "')", e);
            return null;
        }

        return rtMap;
    }

    // ���ø�
    /*private Map<String, String> parse_xxx_Vx() {
        Map<String, String> rtMap = null;
        Map<String, IpgMsg.MsgAttr> msgAttr = rpyMsg.getMsgAttrMap();
        int excTrace = 0;

        try {
            // += [�������� ��ȸ ����] ==========================================================================================+
            if (msgAttr == IpgMsg.RpyShopInfoMsgAttr) {
                // ...
            }
            // += [���ι�ȣ ���� ����] ==========================================================================================+
            else if (msgAttr == IpgMsg.RpyArrovalNumMsgAttr) {
                // ...
            }
            // += [�ŷ��뺸 ����] ==============================================================================================+
            else if (msgAttr == IpgMsg.RpyPayNotifyAttr) {
                // ...
            }
            // += [�ļ��� ������ �����Ӽ�] =======================================================================================+
            else {
                log(Logger.ERROR, "Undefined 'msgAttr' in parser! (MsgName: '" + rpyMsg.getMsgName() +  "', msgAttr: '" + msgAttr + "')");
                return null;
            }
        }
        catch (Exception e) {
            log(Logger.ERROR, "Exception! (MsgName: '" + rpyMsg.getMsgName() +  "', " + "ExcTrace:'" + excTrace + "')", e);
            return null;
        }

        return rtMap;
    }*/
}