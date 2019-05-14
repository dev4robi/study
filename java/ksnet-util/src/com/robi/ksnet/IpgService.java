package com.robi.ksnet;

import java.util.Date;
import java.text.SimpleDateFormat;
import java.net.InetSocketAddress;

public class IpgService {

    // Ŭ���� ���� ��� //
    private static final Logger _logger = Logger.getInstance();
    private static final SimpleDateFormat DATE_FMT = new SimpleDateFormat("yyMMddHHmmss");

    public static String getCurTime() {
        return DATE_FMT.format(new Date());
    }

    // �������� ��ȸ
    public static boolean shopInfoService(IpgRelayServer.Client cli, IpgMsg reqMsg, IpgMsg rpyMsg) {
        String cliAddrStr = cli.getSocAddr().toString();
        boolean svcResult = false;

        _logger.log(Logger.INFO, String.format("Client(%s):shopInfoService() ����!", cliAddrStr));

        // ���� ���� ���� //
        String curTime = IpgService.getCurTime();

        // ������ ������ȸ ��û �Ӽ��� �ʱ�ȭ
        String req_terminal_div_numb = new String(reqMsg.get("terminal_div_numb"));
        String svc_partner_platform = "";
        
        if (req_terminal_div_numb.equals("DPT0F50057")) {
            // �˸�����
            svcResult =true;
            svc_partner_platform = "alipay";
        }
        else if (req_terminal_div_numb.equals("DPT0F50200")) {
            // ��������
            svcResult =true;
            svc_partner_platform = "wechatpay";
        }
        else {
            // �̵�ϴܸ���
            svcResult = false;
        }

        // ������ ������ȸ ���� �Ӽ��� ����
        if (svcResult) {
            rpyMsg.set("terminal_div_numb"  , req_terminal_div_numb.getBytes());
            rpyMsg.set("comp_info"          , reqMsg.get("comp_info").getBytes());
            rpyMsg.set("msg_numb"           , reqMsg.get("msg_numb").getBytes());
            rpyMsg.set("deal_date"          , curTime.getBytes());
            rpyMsg.set("card_type"          , new byte[] { (byte)'1' });
            rpyMsg.set("shop_numb"          , new String("��������ȣ(15)").getBytes());
            rpyMsg.set("make_comp_gove_code", new byte[] { (byte)'0', (byte)'5'} );
            rpyMsg.set("card_type_name"     , new String("05�� ������?").getBytes());
            rpyMsg.set("purc_comp_gove_code", new byte[] { (byte)'0', (byte)'5'} );
            rpyMsg.set("purc_comp_gove_name", new String("05�� ����ī��?").getBytes());
            rpyMsg.set("notice1"            , new String("TBOTW-Myeong-dong01S").getBytes());
            rpyMsg.set("transaction_type"   , new byte[] { (byte)'0' } );
            if (svc_partner_platform.equals("alipay")) {
                rpyMsg.set("notice2"            , new String("�˸����� API Ű (MAX:40����Ʈ)").getBytes());
                rpyMsg.set("ksnet_reserved"     , new String("�˸�:PT_ID+SMC_ID").getBytes());
            }
            else if (svc_partner_platform.equals("wechatpay")) {
                rpyMsg.set("notice2"            , new String("������ API Ű (MAX:40����Ʈ)").getBytes());
                rpyMsg.set("ksnet_reserved"     , new String("����:APP_ID+MCH_ID").getBytes());
            }
            rpyMsg.set("shop_info"          , new String("1208197322õ�����ϻ�ȸ-��1��                              0234206807     01012344321    ����Ư���� ������ �Ｚ�� 168-26 ������� 107�� 11 (�Ｚ��, ����Ÿ��) 5�� ȿ�ָ��� ��ǰ��").getBytes());
        }
        
        byte[] resultSym = new byte[1];
        byte[] resultMsg1, resultMsg2, approval_numb;
        
        if (svcResult) {
            resultSym[0] = (byte)'O';
            approval_numb = "C12345".getBytes();
            resultMsg1 = "ī��������".getBytes();
            resultMsg2 = "OK: ���ι�ȣ8��".getBytes();
        }
        else {
            resultSym[0] = (byte)'X';
            approval_numb = "C011".getBytes();
            resultMsg1 = "�ܸ����ȣ��,".getBytes();
            resultMsg2 = "��Ͼȵ��־���!".getBytes();
        }

        rpyMsg.set("status",   resultSym);
        rpyMsg.set("approval_numb", approval_numb);
        rpyMsg.set("message1", resultMsg1);
        rpyMsg.set("message2", resultMsg2);
        // ���� ���� ���� //

        _logger.log(Logger.INFO, String.format("Client(%s):shopInfoService() ����! (Result:%b)", cliAddrStr, svcResult));
        return true;
    }

    // �������� ��ȸ
    public static boolean approvalInfoService(IpgRelayServer.Client cli, IpgMsg reqMsg, IpgMsg rpyMsg) {
        String cliAddrStr = cli.getSocAddr().toString();
        boolean svcResult = false;

        _logger.log(Logger.INFO, String.format("Client(%s):approvalInfoService() ����!", cliAddrStr));

        // ���� ���� ���� //
        String curTime = IpgService.getCurTime();

        // ������ ������ȸ ��û �Ӽ��� �ʱ�ȭ
        String req_terminal_div_numb = new String(reqMsg.get("terminal_div_numb"));
        String svc_partner_platform = "";
        
        if (req_terminal_div_numb.equals("DPT0F50057")) {
            // �˸�����
            svcResult =true;
            svc_partner_platform = "alipay";
        }
        else if (req_terminal_div_numb.equals("DPT0F50200")) {
            // ��������
            svcResult =true;
            svc_partner_platform = "wechatpay";
        }
        else {
            // �̵�ϴܸ���
            svcResult = false;
        }

        // �������� ��ȸ ���� �Ӽ��� ����
        if (svcResult) {
            rpyMsg.set("terminal_div_numb"  , req_terminal_div_numb.getBytes());
            rpyMsg.set("comp_info"          , reqMsg.get("comp_info").getBytes());
            rpyMsg.set("msg_numb"           , reqMsg.get("msg_numb").getBytes());
            rpyMsg.set("deal_date"          , curTime.getBytes());
            rpyMsg.set("card_type"          , new byte[] { (byte)'1' });
            rpyMsg.set("shop_numb"          , new String("��������ȣ(15)").getBytes());
            rpyMsg.set("make_comp_gove_code", new byte[] { (byte)'0', (byte)'5'} );
            rpyMsg.set("card_type_name"     , new String("05�� ������?").getBytes());
            rpyMsg.set("purc_comp_gove_code", new byte[] { (byte)'0', (byte)'5'} );
            rpyMsg.set("purc_comp_gove_name", new String("05�� ����ī��?").getBytes());
            rpyMsg.set("point1"             , new String("7654321").getBytes());
            rpyMsg.set("point2"             , new String("1234567").getBytes());
            rpyMsg.set("point3"             , new String("KRW").getBytes());
            rpyMsg.set("notice1"            , new String("TBOTW-Myeong-dong01S").getBytes());
            rpyMsg.set("transaction_type"   , new byte[] { (byte)'0' } );
            if (svc_partner_platform.equals("alipay")) {
                rpyMsg.set("ksnet_reserved"     , new String("CNY123456700").getBytes());
            }
            else if (svc_partner_platform.equals("wechatpay")) {
                rpyMsg.set("ksnet_reserved"     , new String("����:APP_ID+MCH_ID").getBytes());
            }
        }
        
        byte[] resultSym = new byte[1];
        byte[] resultMsg1, resultMsg2, approval_numb;
        
        if (svcResult) {
            resultSym[0] = (byte)'O';
            approval_numb = "C12345".getBytes();
            resultMsg1 = "ī��������".getBytes();
            resultMsg2 = "OK: ���ι�ȣ8��".getBytes();
        }
        else {
            resultSym[0] = (byte)'X';
            approval_numb = "C011".getBytes();
            resultMsg1 = "�ܸ����ȣ��,".getBytes();
            resultMsg2 = "��Ͼȵ��־���!".getBytes();
        }

        rpyMsg.set("status", resultSym);
        rpyMsg.set("approval_numb", approval_numb);
        rpyMsg.set("message1", resultMsg1);
        rpyMsg.set("message2", resultMsg2);
        // ���� ���� ���� //

        _logger.log(Logger.INFO, String.format("Client(%s):approvalInfoService() ����! (Result:%b)", cliAddrStr, svcResult));
        return true;
    }

    // �������� �뺸
    public static boolean payNotifyService(IpgRelayServer.Client cli, IpgMsg reqMsg, IpgMsg rpyMsg) {
        String cliAddrStr = cli.getSocAddr().toString();
        boolean svcResult = false;

        _logger.log(Logger.INFO, String.format("Client(%s):payNotifyService() ����!", cliAddrStr));

        // ���� ���� ���� //
        String curTime = IpgService.getCurTime();

        // �������� �뺸 ���� �Ӽ��� �ʱ�ȭ
        String req_terminal_div_numb = new String(reqMsg.get("terminal_div_numb"));
        String svc_partner_platform = "";
        
        if (req_terminal_div_numb.equals("DPT0F50057")) {
            // �˸�����
            svcResult =true;
            svc_partner_platform = "alipay";
        }
        else if (req_terminal_div_numb.equals("DPT0F50200")) {
            // ��������
            svcResult =true;
            svc_partner_platform = "wechatpay";
        }
        else {
            // �̵�ϴܸ���
            svcResult = false;
        }

        // �������� �뺸 ���� �Ӽ��� ����
        if (svcResult) {
            rpyMsg.set("terminal_div_numb"  , req_terminal_div_numb.getBytes());
            rpyMsg.set("comp_info"          , reqMsg.get("comp_info").getBytes());
            rpyMsg.set("msg_numb"           , reqMsg.get("msg_numb").getBytes());
            rpyMsg.set("deal_date"          , curTime.getBytes());
            rpyMsg.set("card_type"          , new byte[] { (byte)'1' });
            rpyMsg.set("shop_numb"          , new String("��������ȣ(15)").getBytes());
            rpyMsg.set("make_comp_gove_code", new byte[] { (byte)'0', (byte)'5'} );
            rpyMsg.set("card_type_name"     , new String("05�� ������?").getBytes());
            rpyMsg.set("purc_comp_gove_code", new byte[] { (byte)'0', (byte)'5'} );
            rpyMsg.set("purc_comp_gove_name", new String("05�� ����ī��?").getBytes());
            rpyMsg.set("transaction_type"   , new byte[] { (byte)'0' } );
        }
        
        byte[] resultSym = new byte[1];
        byte[] resultMsg1, resultMsg2, approval_numb;
        
        if (svcResult) {
            resultSym[0] = (byte)'O';
            resultMsg1 = "ī��������".getBytes();
            resultMsg2 = "OK: ".getBytes();
            approval_numb = "C12345".getBytes();
        }
        else {
            resultSym[0] = (byte)'X';
            resultMsg1 = "�ܸ����ȣ��,".getBytes();
            resultMsg2 = "��Ͼȵ��־���!".getBytes();
            approval_numb = "C011".getBytes();
        }

        rpyMsg.set("status",   resultSym);
        rpyMsg.set("message1", resultMsg1);
        rpyMsg.set("message2", resultMsg2);
        rpyMsg.set("approval_numb", approval_numb);
        // ���� ���� ���� //

        _logger.log(Logger.INFO, String.format("Client(%s):payNotifyService() ����! (Result:%b)", cliAddrStr, svcResult));
        return true;
    }

}