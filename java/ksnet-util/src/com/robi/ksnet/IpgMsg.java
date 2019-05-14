package com.robi.ksnet;

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;

public class IpgMsg {

    // Ŭ���� ��� //
    public static final String IPG_CHARSET = "EUC-KR";

    // ���� �Ӽ��� ��� //
    public static final Map<String, MsgAttr> ReqArrovalNumMsgAttr;      // ���ι�ȣ ���� ��û �Ӽ��� (�������� + Pay���� + ���ι�ȣ)
    public static final Map<String, MsgAttr> RpyArrovalNumMsgAttr;      // ���ι�ȣ ���� ���� �Ӽ���
    public static final Map<String, MsgAttr> ReqShopInfoMsgAttr;        // �������� ��ȸ ��û �Ӽ��� (�������� + Pay����)
    public static final Map<String, MsgAttr> RpyShopInfoMsgAttr;        // �������� ��ȸ ���� �Ӽ���
    public static final Map<String, MsgAttr> ReqPayNotifyAttr;          // �������� �뺸���� ��û �Ӽ���
    public static final Map<String, MsgAttr> RpyPayNotifyAttr;          // �������� �뺸���� ���� �Ӽ���
    // ���⿡ ���ο� �Ӽ� �߰�...

    // ���� �Ӽ��� ��� �ʱ�ȭ //
    static {
        // ���ι�ȣ ���� ��û �Ӽ��� (�������� + Pay���� + ���ι�ȣ)
        ReqArrovalNumMsgAttr = new HashMap<String, MsgAttr>();
        ReqArrovalNumMsgAttr.put("length"             , new MsgAttr(0,    "length",               0,      4,      '9',  null,               "��������: �����ʵ� ���� ��������"));
        ReqArrovalNumMsgAttr.put("stx"                , new MsgAttr(1,    "stx",                  4,      1,      'X',  new byte[]{0x02},   "������: STX(0x02)"));
        ReqArrovalNumMsgAttr.put("deal_type"          , new MsgAttr(2,    "deal_type",            5,      2,      'X',  "BI".getBytes(),    "�ŷ�����: BI(����/�˸����� ���ι�ȣ����)"));
        ReqArrovalNumMsgAttr.put("terminal_div_numb"  , new MsgAttr(3,    "terminal_div_numb",    7,      10,     'X',  null,               "�ܸ����ȣ: DPT0F00000"));
        ReqArrovalNumMsgAttr.put("comp_info"          , new MsgAttr(4,    "comp_info",            17,     4,      'X',  null,               "��ü����: null"));
        ReqArrovalNumMsgAttr.put("msg_numb"           , new MsgAttr(5,    "msg_numb",             21,     12,     '9',  null,               "������ȣ: null"));
        ReqArrovalNumMsgAttr.put("pos_entry_mode"     , new MsgAttr(6,    "pos_entry_mode",       33,     1,      'X',  "Q".getBytes(),     "PosEntryMode: 'Q'(QR����)"));
        ReqArrovalNumMsgAttr.put("track2"             , new MsgAttr(7,    "track2",               34,     37,     'X',  null,               "TrackII: null"));
        ReqArrovalNumMsgAttr.put("fs"                 , new MsgAttr(8,    "fs",                   71,     1,      'X',  new byte[]{0x1C},   "FS: 0x1C"));
        ReqArrovalNumMsgAttr.put("approval_dist"      , new MsgAttr(9,    "approval_dist",        72,     2,      'X',  "MI".getBytes(),    "��ȸ����: MI"));
        ReqArrovalNumMsgAttr.put("total_amount"       , new MsgAttr(10,   "total_amount",         74,     9,      '9',  null,               "�ѱݾ�: �ѱݾ�(�޷������� ��� 100 = 1$)"));
        ReqArrovalNumMsgAttr.put("cancle_amount"      , new MsgAttr(11,   "cancle_amount",        83,     9,      '9',  null,               "�κ���ұݾ�: �κ���ұݾ�(�޷������� ��� 100 = 1$)"));
        ReqArrovalNumMsgAttr.put("vat_amount"         , new MsgAttr(12,   "vat_amount",           92,     9,      '9',  null,               "����(�ΰ���): ����(�ΰ���)(�޷������� ��� 100 = 1$)"));
        ReqArrovalNumMsgAttr.put("tax_amount"         , new MsgAttr(13,   "tax_amount",           101,    9,      '9',  null,               "�����ݾ�: �����ݾ�(�޷������� ��� 100 = 1$)"));
        ReqArrovalNumMsgAttr.put("working_key_index"  , new MsgAttr(14,   "working_key_index",    110,    2,      'X',  "AA".getBytes(),    "WorkingKeyIndex: AA"));
        ReqArrovalNumMsgAttr.put("password"           , new MsgAttr(15,   "password",             112,    16,     'X',  null,               "��й�ȣ: null"));
        ReqArrovalNumMsgAttr.put("approval_numb"      , new MsgAttr(16,   "approval_numb",        128,    12,     'X',  null,               "���ŷ����ι�ȣ: null"));
        ReqArrovalNumMsgAttr.put("approval_date"      , new MsgAttr(17,   "approval_date",        140,    6,      'X',  null,               "���ŷ���������(YYMMDD): null"));
        ReqArrovalNumMsgAttr.put("user_info"          , new MsgAttr(18,   "user_info",            146,    13,     'X',  null,               "���������: null"));
        ReqArrovalNumMsgAttr.put("shop_id"            , new MsgAttr(19,   "shop_id",              159,    2,      'X',  null,               "������ID: null"));
        ReqArrovalNumMsgAttr.put("shop_reserved"      , new MsgAttr(20,   "shop_reserved",        161,    30,     'X',  null,               "����������ʵ�: null"));
        ReqArrovalNumMsgAttr.put("point_dist"         , new MsgAttr(21,   "point_dist",           191,    4,      'X',  null,               "����Ʈ����: ���̻� ����(����WX/�˸�:AL)"));
        ReqArrovalNumMsgAttr.put("ksnet_reserved"     , new MsgAttr(22,   "ksnet_reserved",       195,    20,     'X',  null,               "KSNET_Reserved: null"));
        ReqArrovalNumMsgAttr.put("dongul_dist"        , new MsgAttr(23,   "dongul_dist",          215,    1,      'X',  "N".getBytes(),     "���۱���: 'N' (���� �ŷ� �ƴ�)"));
        ReqArrovalNumMsgAttr.put("pay_div_type"       , new MsgAttr(24,   "pay_div_type",         216,    1,      'X',  "S".getBytes(),     "��ü����: 'S' (����Ʈ�� ����)"));
        ReqArrovalNumMsgAttr.put("telecom_dist"       , new MsgAttr(25,   "telecom_dist",         217,    1,      'X',  null,               "��Ż籸��: null"));
        ReqArrovalNumMsgAttr.put("credit_card_type"   , new MsgAttr(26,   "credit_card_type",     218,    1,      'X',  null,               "�ſ�ī������: null"));
        ReqArrovalNumMsgAttr.put("transaction_type"   , new MsgAttr(27,   "transaction_type",     219,    1,      'X',  null,               "�ŷ�����: null"));
        ReqArrovalNumMsgAttr.put("use_sign"           , new MsgAttr(28,   "use_sign",             220,    1,      'X',  "N".getBytes(),     "���ڼ������� : 'N'"));
        ReqArrovalNumMsgAttr.put("platform_reserved"  , new MsgAttr(29,   "platform_reserved",    221,    200,    'X',  null,               "����/�˸�/�߰�����: null"));
        ReqArrovalNumMsgAttr.put("etx"                , new MsgAttr(30,   "etx",                  421,    1,      'X',  new byte[]{0x03},   "ETX: 0x03"));
        ReqArrovalNumMsgAttr.put("cr"                 , new MsgAttr(31,   "cr",                   422,    1,      'X',  new byte[]{0x0D},   "CR: 0x0D"));

        // ���ι�ȣ ���� ���� �Ӽ���
        RpyArrovalNumMsgAttr = new HashMap<String, MsgAttr>();
        RpyArrovalNumMsgAttr.put("length"             , new MsgAttr(0,    "length",               0,      4,      '9',  null,               "��������: �����ʵ� ���� ��������"));
        RpyArrovalNumMsgAttr.put("stx"                , new MsgAttr(1,    "stx",                  4,      1,      'X',  null,               "STX: 0x02"));
        RpyArrovalNumMsgAttr.put("deal_type"          , new MsgAttr(2,    "deal_type",            5,      2,      'X',  null,               "�ŷ�����: BJ(����/�˸�����)"));
        RpyArrovalNumMsgAttr.put("terminal_div_numb"  , new MsgAttr(3,    "terminal_div_numb",    7,      10,     'X',  null,               "�ܸ����ȣ: null"));
        RpyArrovalNumMsgAttr.put("comp_info"          , new MsgAttr(4,    "comp_info",            17,     4,      'X',  null,               "��ü����: ����(goods_id(4)), �˸�(sec_mch_industry(4))"));
        RpyArrovalNumMsgAttr.put("msg_numb"           , new MsgAttr(5,    "msg_numb",             21,     12,     '9',  null,               "������ȣ: null"));
        RpyArrovalNumMsgAttr.put("status"             , new MsgAttr(6,    "status",               33,     1,      'X',  null,               "Status: 'O'(����), 'X'(����)"));
        RpyArrovalNumMsgAttr.put("deal_date"          , new MsgAttr(7,    "deal_date",            34,     12,     'X',  null,               "�ŷ��Ͻ�: YYMMDDhhmmss"));
        RpyArrovalNumMsgAttr.put("card_type"          , new MsgAttr(8,    "card_type",            46,     1,      'X',  null,               "ī��Type: null"));
        RpyArrovalNumMsgAttr.put("message1"           , new MsgAttr(9,    "message1",             47,     16,     'X',  null,               "�޽���1: ���ν�(ī��������) / ������(�����޽���)"));
        RpyArrovalNumMsgAttr.put("message2"           , new MsgAttr(10,   "message2",             63,     16,     'X',  null,               "�޽���2: ���ν�(OK) / ������(�����޽���)"));
        RpyArrovalNumMsgAttr.put("approval_numb"      , new MsgAttr(11,   "approval_numb",        79,     12,     'X',  null,               "���ι�ȣ: ������ ä�� ������ȣ(���ڸ�6) or �����ڵ�(4)"));
        RpyArrovalNumMsgAttr.put("shop_numb"          , new MsgAttr(12,   "shop_numb",            91,     15,     'X',  null,               "��������ȣ: ����(sub_mch_id), �˸�(sec_mch_id)"));
        RpyArrovalNumMsgAttr.put("make_comp_gove_code", new MsgAttr(13,   "make_comp_gove_code",  106,    2,      'X',  null,               "�߱޻��ڵ�: ī��� �ڵ�(Table����)"));
        RpyArrovalNumMsgAttr.put("card_type_name"     , new MsgAttr(14,   "card_type_name",       108,    16,     'X',  null,               "ī��������: null"));
        RpyArrovalNumMsgAttr.put("purc_comp_gove_code", new MsgAttr(15,   "purc_comp_gove_code",  124,    2,      'X',  null,               "���Ի��ڵ�: null"));
        RpyArrovalNumMsgAttr.put("purc_comp_gove_name", new MsgAttr(16,   "purc_comp_gove_name",  126,    16,     'X',  null,               "���Ի��: null"));
        RpyArrovalNumMsgAttr.put("working_key_index"  , new MsgAttr(17,   "working_key_index",    142,    2,      'X',  null,               "WorkingKeyIndex: ����(AppID), �˸�(ParterID)"));
        RpyArrovalNumMsgAttr.put("working_key"        , new MsgAttr(18,   "working_key",          144,    16,     'X',  null,               "WorkingKey: ����(AppID), �˸�(ParterID)"));
        RpyArrovalNumMsgAttr.put("spare_point"        , new MsgAttr(19,   "spare_point",          160,    9,      '9',  null,               "��������Ʈ: ȯ������ 1USD���� KRW�ݾ�(7.2) or All '0'�� ��� KRW����"));
        RpyArrovalNumMsgAttr.put("point1"             , new MsgAttr(20,   "point1",               169,    9,      '9',  null,               "����Ʈ1: null"));
        RpyArrovalNumMsgAttr.put("point2"             , new MsgAttr(21,   "point2",               178,    9,      '9',  null,               "����Ʈ2: null"));
        RpyArrovalNumMsgAttr.put("point3"             , new MsgAttr(22,   "point3",               187,    9,      'X',  null,               "����Ʈ3: null"));
        RpyArrovalNumMsgAttr.put("notice1"            , new MsgAttr(23,   "notice1",              196,    20,     'X',  null,               "Notice1: ����(10:����ڹ�ȣ+10:mch_id), �˸�(10:����ڹ�ȣ)"));
        RpyArrovalNumMsgAttr.put("notice2"            , new MsgAttr(24,   "notice2",              216,    40,     'X',  null,               "Notice2: �������� (����)"));
        RpyArrovalNumMsgAttr.put("transaction_type"   , new MsgAttr(25,   "transaction_type",     256,    1,      'X',  null,               "�ŷ�����: 0(ȯ����ȸ����), 1(����ȯ��), 2(����ȯ��)"));
        RpyArrovalNumMsgAttr.put("reserved"           , new MsgAttr(26,   "reserved",             257,    5,      'X',  null,               "Reserved: ��ȭ�ڵ�(3)"));
        RpyArrovalNumMsgAttr.put("ksnet_reserved"     , new MsgAttr(27,   "ksnet_reserved",       262,    40,     'X',  null,               "KSNET_Reserved: ����(32:API_Key), �˸�(32:PrivateKey)"));
        RpyArrovalNumMsgAttr.put("shop_info"          , new MsgAttr(28,   "shop_info",            302,    242,    'X',  null,               "���������� : ����: ��������(50)+����������ó(15)+����������ȣ(15)+�������ּ�(130)+{����:APP_Secret(32)}"));
        RpyArrovalNumMsgAttr.put("etx"                , new MsgAttr(29,   "etx",                  544,    1,      'X',  null,               "ETX: 0x03"));
        RpyArrovalNumMsgAttr.put("cr"                 , new MsgAttr(30,   "cr",                   545,    1,      'X',  null,               "CR: 0x0D"));

        // �������� ��ȸ ��û �Ӽ���
        ReqShopInfoMsgAttr = new HashMap<String, MsgAttr>();
        ReqShopInfoMsgAttr.put("length"             , new MsgAttr(0,    "length",               0,      4,      '9',    null,               "��������: �����ʵ� ���� ��������"));
        ReqShopInfoMsgAttr.put("stx"                , new MsgAttr(1,    "stx",                  4,      1,      'X',    new byte[]{0x02},   "������: STX(0x02)"));
        ReqShopInfoMsgAttr.put("deal_type"          , new MsgAttr(2,    "deal_type",            5,      2,      'X',    "BI".getBytes(),    "�ŷ�����: BI(����/�˸����� ������ȸ)"));
        ReqShopInfoMsgAttr.put("terminal_div_numb"  , new MsgAttr(3,    "terminal_div_numb",    7,      10,     'X',    null,               "�ܸ����ȣ: DPT0F00000"));
        ReqShopInfoMsgAttr.put("comp_info"          , new MsgAttr(4,    "comp_info",            17,     4,      'X',    null,               "��ü����: null"));
        ReqShopInfoMsgAttr.put("msg_numb"           , new MsgAttr(5,    "msg_numb",             21,     12,     '9',    null,               "������ȣ: null"));
        ReqShopInfoMsgAttr.put("pos_entry_mode"     , new MsgAttr(6,    "pos_entry_mode",       33,     1,      'X',    "Q".getBytes(),     "PosEntryMode: 'Q'(QR����)"));
        ReqShopInfoMsgAttr.put("track2"             , new MsgAttr(7,    "track2",               34,     37,     'X',    null,               "TrackII: null"));
        ReqShopInfoMsgAttr.put("fs"                 , new MsgAttr(8,    "fs",                   71,     1,      'X',    new byte[]{0x1C},   "FS: 0x1C"));
        ReqShopInfoMsgAttr.put("approval_dist"      , new MsgAttr(9,    "approval_dist",        72,     2,      'X',    "KI".getBytes(),    "��ȸ����: KI"));
        ReqShopInfoMsgAttr.put("total_amount"       , new MsgAttr(10,   "total_amount",         74,     9,      '9',    null,               "�ѱݾ�: null"));
        ReqShopInfoMsgAttr.put("cancle_amount"      , new MsgAttr(11,   "cancle_amount",        83,     9,      '9',    null,               "�κ���ұݾ�: null"));
        ReqShopInfoMsgAttr.put("vat_amount"         , new MsgAttr(12,   "vat_amount",           92,     9,      '9',    null,               "����(�ΰ���): null"));
        ReqShopInfoMsgAttr.put("tax_amount"         , new MsgAttr(13,   "tax_amount",           101,    9,      '9',    null,               "�����ݾ�: null"));
        ReqShopInfoMsgAttr.put("working_key_index"  , new MsgAttr(14,   "working_key_index",    110,    2,      'X',    "AA".getBytes(),    "WorkingKeyIndex: AA"));
        ReqShopInfoMsgAttr.put("password"           , new MsgAttr(15,   "password",             112,    16,     'X',    null,               "��й�ȣ: null"));
        ReqShopInfoMsgAttr.put("approval_numb"      , new MsgAttr(16,   "approval_numb",        128,    12,     'X',    null,               "���ŷ����ι�ȣ: null"));
        ReqShopInfoMsgAttr.put("approval_date"      , new MsgAttr(17,   "approval_date",        140,    6,      'X',    null,               "���ŷ���������(YYMMDD): null"));
        ReqShopInfoMsgAttr.put("user_info"          , new MsgAttr(18,   "user_info",            146,    13,     'X',    null,               "���������: null"));
        ReqShopInfoMsgAttr.put("shop_id"            , new MsgAttr(19,   "shop_id",              159,    2,      'X',    null,               "������ID: null"));
        ReqShopInfoMsgAttr.put("shop_reserved"      , new MsgAttr(20,   "shop_reserved",        161,    30,     'X',    null,               "����������ʵ�: null"));
        ReqShopInfoMsgAttr.put("point_dist"         , new MsgAttr(21,   "point_dist",           191,    4,      'X',    null,               "����Ʈ����: ���̻� ����(����WX/�˸�:AL)"));
        ReqShopInfoMsgAttr.put("ksnet_reserved"     , new MsgAttr(22,   "ksnet_reserved",       195,    20,     'X',    null,               "KSNET_Reserved: null"));
        ReqShopInfoMsgAttr.put("dongul_dist"        , new MsgAttr(23,   "dongul_dist",          215,    1,      'X',    "N".getBytes(),     "���۱���: 'N' (���� �ŷ� �ƴ�)"));
        ReqShopInfoMsgAttr.put("pay_div_type"       , new MsgAttr(24,   "pay_div_type",         216,    1,      'X',    "S".getBytes(),     "��ü����: 'S' (����Ʈ�� ����)"));
        ReqShopInfoMsgAttr.put("telecom_dist"       , new MsgAttr(25,   "telecom_dist",         217,    1,      'X',    null,               "��Ż籸��: null"));
        ReqShopInfoMsgAttr.put("credit_card_type"   , new MsgAttr(26,   "credit_card_type",     218,    1,      'X',    null,               "�ſ�ī������: null"));
        ReqShopInfoMsgAttr.put("transaction_type"   , new MsgAttr(27,   "transaction_type",     219,    1,      'X',    null,               "�ŷ�����: null"));
        ReqShopInfoMsgAttr.put("use_sign"           , new MsgAttr(28,   "use_sign",             220,    1,      'X',    "N".getBytes(),     "���ڼ������� : 'N'"));
        ReqShopInfoMsgAttr.put("platform_reserved"  , new MsgAttr(29,   "platform_reserved",    221,    200,    'X',    null,               "����/�˸�/�߰�����: null"));
        ReqShopInfoMsgAttr.put("etx"                , new MsgAttr(30,   "etx",                  421,    1,      'X',    new byte[]{0x03},   "ETX: 0x03"));
        ReqShopInfoMsgAttr.put("cr"                 , new MsgAttr(31,   "cr",                   422,    1,      'X',    new byte[]{0x0D},   "CR: 0x0D"));

        // �������� ��ȸ ���� �Ӽ���
        RpyShopInfoMsgAttr = new HashMap<String, MsgAttr>();
        RpyShopInfoMsgAttr.put("length"             , new MsgAttr(0,    "length",               0,      4,      '9',  null,               "��������: �����ʵ� ���� ��������"));
        RpyShopInfoMsgAttr.put("stx"                , new MsgAttr(1,    "stx",                  4,      1,      'X',  null,               "STX: 0x02"));
        RpyShopInfoMsgAttr.put("deal_type"          , new MsgAttr(2,    "deal_type",            5,      2,      'X',  null,               "�ŷ�����: BJ(����/�˸�����)"));
        RpyShopInfoMsgAttr.put("terminal_div_numb"  , new MsgAttr(3,    "terminal_div_numb",    7,      10,     'X',  null,               "�ܸ����ȣ: null"));
        RpyShopInfoMsgAttr.put("comp_info"          , new MsgAttr(4,    "comp_info",            17,     4,      'X',  null,               "��ü����: ����(goods_id(4)), �˸�(sec_mch_industry(4))"));
        RpyShopInfoMsgAttr.put("msg_numb"           , new MsgAttr(5,    "msg_numb",             21,     12,     '9',  null,               "������ȣ: null"));
        RpyShopInfoMsgAttr.put("status"             , new MsgAttr(6,    "status",               33,     1,      'X',  null,               "Status: 'O'(����), 'X'(����)"));
        RpyShopInfoMsgAttr.put("deal_date"          , new MsgAttr(7,    "deal_date",            34,     12,     'X',  null,               "�ŷ��Ͻ�: YYMMDDhhmmss"));
        RpyShopInfoMsgAttr.put("card_type"          , new MsgAttr(8,    "card_type",            46,     1,      'X',  null,               "ī��Type: null"));
        RpyShopInfoMsgAttr.put("message1"           , new MsgAttr(9,    "message1",             47,     16,     'X',  null,               "�޽���1: ���ν�(ī��������) / ������(�����޽���)"));
        RpyShopInfoMsgAttr.put("message2"           , new MsgAttr(10,   "message2",             63,     16,     'X',  null,               "�޽���2: ���ν�(OK) / ������(�����޽���)"));
        RpyShopInfoMsgAttr.put("approval_numb"      , new MsgAttr(11,   "approval_numb",        79,     12,     'X',  null,               "���ι�ȣ: ������ ä�� ������ȣ(���ڸ�6) or �����ڵ�(4)"));
        RpyShopInfoMsgAttr.put("shop_numb"          , new MsgAttr(12,   "shop_numb",            91,     15,     'X',  null,               "��������ȣ: ����(sub_mch_id), �˸�(sec_mch_id)"));
        RpyShopInfoMsgAttr.put("make_comp_gove_code", new MsgAttr(13,   "make_comp_gove_code",  106,    2,      'X',  null,               "�߱޻��ڵ�: ī��� �ڵ�(Table����)"));
        RpyShopInfoMsgAttr.put("card_type_name"     , new MsgAttr(14,   "card_type_name",       108,    16,     'X',  null,               "ī��������: null"));
        RpyShopInfoMsgAttr.put("purc_comp_gove_code", new MsgAttr(15,   "purc_comp_gove_code",  124,    2,      'X',  null,               "���Ի��ڵ�: null"));
        RpyShopInfoMsgAttr.put("purc_comp_gove_name", new MsgAttr(16,   "purc_comp_gove_name",  126,    16,     'X',  null,               "���Ի��: null"));
        RpyShopInfoMsgAttr.put("working_key_index"  , new MsgAttr(17,   "working_key_index",    142,    2,      'X',  null,               "WorkingKeyIndex: ����(AppID), �˸�(ParterID)"));
        RpyShopInfoMsgAttr.put("working_key"        , new MsgAttr(18,   "working_key",          144,    16,     'X',  null,               "WorkingKey: ����(AppID), �˸�(ParterID)"));
        RpyShopInfoMsgAttr.put("spare_point"        , new MsgAttr(19,   "spare_point",          160,    9,      '9',  null,               "��������Ʈ: ȯ������ 1USD���� KRW�ݾ�(7.2) or All '0'�� ��� KRW����"));
        RpyShopInfoMsgAttr.put("point1"             , new MsgAttr(20,   "point1",               169,    9,      '9',  null,               "����Ʈ1: null"));
        RpyShopInfoMsgAttr.put("point2"             , new MsgAttr(21,   "point2",               178,    9,      '9',  null,               "����Ʈ2: null"));
        RpyShopInfoMsgAttr.put("point3"             , new MsgAttr(22,   "point3",               187,    9,      'X',  null,               "����Ʈ3: null"));
        RpyShopInfoMsgAttr.put("notice1"            , new MsgAttr(23,   "notice1",              196,    20,     'X',  null,               "Notice1: ����(10:����ڹ�ȣ+10:mch_id), �˸�(10:����ڹ�ȣ)"));
        RpyShopInfoMsgAttr.put("notice2"            , new MsgAttr(24,   "notice2",              216,    40,     'X',  null,               "Notice2: �������� (����)"));
        RpyShopInfoMsgAttr.put("transaction_type"   , new MsgAttr(25,   "transaction_type",     256,    1,      'X',  null,               "�ŷ�����: 0(ȯ����ȸ����), 1(����ȯ��), 2(����ȯ��)"));
        RpyShopInfoMsgAttr.put("reserved"           , new MsgAttr(26,   "reserved",             257,    5,      'X',  null,               "Reserved: ��ȭ�ڵ�(3)"));
        RpyShopInfoMsgAttr.put("ksnet_reserved"     , new MsgAttr(27,   "ksnet_reserved",       262,    40,     'X',  null,               "KSNET_Reserved: ����(32:API_Key), �˸�(32:PrivateKey)"));
        RpyShopInfoMsgAttr.put("shop_info"          , new MsgAttr(28,   "shop_info",            302,    242,    'X',  null,               "���������� : ����: ��������(50)+����������ó(15)+����������ȣ(15)+�������ּ�(130)+{����:APP_Secret(32)}"));
        RpyShopInfoMsgAttr.put("etx"                , new MsgAttr(29,   "etx",                  544,    1,      'X',  null,               "ETX: 0x03"));
        RpyShopInfoMsgAttr.put("cr"                 , new MsgAttr(30,   "cr",                   545,    1,      'X',  null,               "CR: 0x0D"));

        // �������� �뺸���� ��û �Ӽ���
        ReqPayNotifyAttr = new HashMap<String, MsgAttr>();
        ReqPayNotifyAttr.put("length"               , new MsgAttr(0,    "length",               0,      4,      '9',    null,               "��������: �����ʵ� ���� ��������"));
        ReqPayNotifyAttr.put("stx"                  , new MsgAttr(1,    "stx",                  4,      1,      'X',    new byte[]{0x02},   "������: STX(0x02)"));
        ReqPayNotifyAttr.put("deal_type"            , new MsgAttr(2,    "deal_type",            5,      2,      'X',    "BI".getBytes(),    "�ŷ�����: BI(����/�˸����� �ŷ��뺸)"));
        ReqPayNotifyAttr.put("terminal_div_numb"    , new MsgAttr(3,    "terminal_div_numb",    7,      10,     'X',    null,               "�ܸ����ȣ: DPT0F00000"));
        ReqPayNotifyAttr.put("comp_info"            , new MsgAttr(4,    "comp_info",            17,     4,      'X',    null,               "��ü����: null"));
        ReqPayNotifyAttr.put("msg_numb"             , new MsgAttr(5,    "msg_numb",             21,     12,     '9',    null,               "������ȣ: null"));
        ReqPayNotifyAttr.put("pos_entry_mode"       , new MsgAttr(6,    "pos_entry_mode",       33,     1,      'X',    "Q".getBytes(),     "PosEntryMode: 'Q'(QR����)"));
        ReqPayNotifyAttr.put("track2"               , new MsgAttr(7,    "track2",               34,     37,     'X',    null,               "TrackII: null"));
        ReqPayNotifyAttr.put("fs"                   , new MsgAttr(8,    "fs",                   71,     1,      'X',    new byte[]{0x1C},   "FS: 0x1C"));
        ReqPayNotifyAttr.put("approval_dist"        , new MsgAttr(9,    "approval_dist",        72,     2,      'X',    "NI".getBytes(),    "��ȸ����: NI"));
        ReqPayNotifyAttr.put("total_amount"         , new MsgAttr(10,   "total_amount",         74,     9,      '9',    null,               "�ѱݾ�: �ѱݾ�(�޷������� ��� 100 = 1$)"));
        ReqPayNotifyAttr.put("cancle_amount"        , new MsgAttr(11,   "cancle_amount",        83,     9,      '9',    null,               "�κ���ұݾ�: �κ���ұݾ�(�޷������� ��� 100 = 1$)"));
        ReqPayNotifyAttr.put("tax_free_amount"      , new MsgAttr(12,   "tax_free_amount",      92,     9,      '9',    null,               "����(�ΰ���): ����(�ΰ���)(�޷������� ��� 100 = 1$)"));
        ReqPayNotifyAttr.put("tax_amount"           , new MsgAttr(13,   "tax_amount",           101,    9,      '9',    null,               "�����ݾ�: �����ݾ�(�޷������� ��� 100 = 1$)"));
        ReqPayNotifyAttr.put("working_key_index"    , new MsgAttr(14,   "working_key_index",    110,    2,      'X',    "AA".getBytes(),    "WorkingKeyIndex: AA"));
        ReqPayNotifyAttr.put("password"             , new MsgAttr(15,   "password",             112,    16,     'X',    null,               "��й�ȣ: �߱������ð�(14:yyyymmddhhMMss)"));
        ReqPayNotifyAttr.put("approval_numb"        , new MsgAttr(16,   "approval_numb",        128,    12,     'X',    null,               "���ŷ����ι�ȣ: 00000000000000K00hhseq"));
        ReqPayNotifyAttr.put("approval_date"        , new MsgAttr(17,   "approval_date",        140,    6,      'X',    null,               "���ŷ���������: �ѱ��ð�(YYMMDD)"));
        ReqPayNotifyAttr.put("user_info"            , new MsgAttr(18,   "user_info",            146,    13,     'X',    null,               "���������: null"));
        ReqPayNotifyAttr.put("shop_id"              , new MsgAttr(19,   "shop_id",              159,    2,      'X',    null,               "������ID: null"));
        ReqPayNotifyAttr.put("shop_reserved"        , new MsgAttr(20,   "shop_reserved",        161,    30,     'X',    null,               "����������ʵ�: ���������ȭ�ڵ�(3)+��������ݾ�(7)+�ΰ���(9)(�޷������� ��� 100 = 1$)"));
        ReqPayNotifyAttr.put("point_dist"           , new MsgAttr(21,   "point_dist",           191,    4,      'X',    null,               "����Ʈ����: ���̻� ����(����WX/�˸�:AL)"));
        ReqPayNotifyAttr.put("ksnet_reserved"       , new MsgAttr(22,   "ksnet_reserved",       195,    20,     'X',    null,               "KSNET_Reserved: �˸�:PartnerID"));
        ReqPayNotifyAttr.put("dongul_dist"          , new MsgAttr(23,   "dongul_dist",          215,    1,      'X',    "N".getBytes(),     "���۱���: 'N' (���� �ŷ� �ƴ�)"));
        ReqPayNotifyAttr.put("pay_div_type"         , new MsgAttr(24,   "pay_div_type",         216,    1,      'X',    "S".getBytes(),     "��ü����: 'S' (����Ʈ�� ����)"));
        ReqPayNotifyAttr.put("telecom_dist"         , new MsgAttr(25,   "telecom_dist",         217,    1,      'X',    null,               "��Ż籸��: null"));
        ReqPayNotifyAttr.put("credit_card_type"     , new MsgAttr(26,   "credit_card_type",     218,    1,      'X',    null,               "�ſ�ī������: null"));
        ReqPayNotifyAttr.put("transaction_type"     , new MsgAttr(27,   "transaction_type",     219,    1,      'X',    null,               "�ŷ�����: null"));
        ReqPayNotifyAttr.put("use_sign"             , new MsgAttr(28,   "use_sign",             220,    1,      'X',    "N".getBytes(),     "���ڼ������� : 'N'"));
        ReqPayNotifyAttr.put("platform_reserved"    , new MsgAttr(29,   "platform_reserved",    221,    200,    'X',    null,               "��ê/�˸�/�߰�����: �����ڵ�(128)+CNY��û���ݾ�(10.2)+�÷�����ŷ���ȣ(32)"));
        ReqPayNotifyAttr.put("etx"                  , new MsgAttr(30,   "etx",                  421,    1,      'X',    new byte[]{0x03},   "ETX: 0x03"));
        ReqPayNotifyAttr.put("cr"                   , new MsgAttr(31,   "cr",                   422,    1,      'X',    new byte[]{0x0D},   "CR: 0x0D"));

        // �������� �뺸���� ���� �Ӽ���
        RpyPayNotifyAttr = new HashMap<String, MsgAttr>();
        RpyPayNotifyAttr.put("length"               , new MsgAttr(0,    "length",               0,      4,      '9',    null,               "��������: �����ʵ� ���� ��������"));
        RpyPayNotifyAttr.put("stx"                  , new MsgAttr(1,    "stx",                  4,      1,      'X',    null,               "STX: 0x02"));
        RpyPayNotifyAttr.put("deal_type"            , new MsgAttr(2,    "deal_type",            5,      2,      'X',    null,               "�ŷ�����: BJ(����/�˸����� �ŷ��뺸)"));
        RpyPayNotifyAttr.put("terminal_div_numb"    , new MsgAttr(3,    "terminal_div_numb",    7,      10,     'X',    null,               "�ܸ����ȣ: null"));
        RpyPayNotifyAttr.put("comp_info"            , new MsgAttr(4,    "comp_info",            17,     4,      'X',    null,               "��ü����: null"));
        RpyPayNotifyAttr.put("msg_numb"             , new MsgAttr(5,    "msg_numb",             21,     12,     '9',    null,               "������ȣ: null"));
        RpyPayNotifyAttr.put("status"               , new MsgAttr(6,    "status",               33,     1,      'X',    null,               "Status: 'O'(����), 'X'(����)"));
        RpyPayNotifyAttr.put("deal_date"            , new MsgAttr(7,    "deal_date",            34,     12,     'X',    null,               "�ŷ��Ͻ�: YYMMDDhhmmss"));
        RpyPayNotifyAttr.put("card_type"            , new MsgAttr(8,    "card_type",            46,     1,      'X',    null,               "ī��Type: null"));
        RpyPayNotifyAttr.put("message1"             , new MsgAttr(9,    "message1",             47,     16,     'X',    null,               "�޽���1: ���ν�(ī��������) / ������(�����޽���)"));
        RpyPayNotifyAttr.put("message2"             , new MsgAttr(10,   "message2",             63,     16,     'X',    null,               "�޽���2: ���ν�(OK) / ������(�����޽���)"));
        RpyPayNotifyAttr.put("approval_numb"        , new MsgAttr(11,   "approval_numb",        79,     12,     'X',    null,               "���ι�ȣ: ������ ��� �����ڵ�(4)"));
        RpyPayNotifyAttr.put("shop_numb"            , new MsgAttr(12,   "shop_numb",            91,     15,     'X',    null,               "��������ȣ: ������ ��ȣ"));
        RpyPayNotifyAttr.put("make_comp_gove_code"  , new MsgAttr(13,   "make_comp_gove_code",  106,    2,      'X',    null,               "�߱޻��ڵ�: ī��� �ڵ�(Table����)"));
        RpyPayNotifyAttr.put("card_type_name"       , new MsgAttr(14,   "card_type_name",       108,    16,     'X',    null,               "ī��������: �߱޻� ī��������"));
        RpyPayNotifyAttr.put("purc_comp_gove_code"  , new MsgAttr(15,   "purc_comp_gove_code",  124,    2,      'X',    null,               "���Ի��ڵ�: null"));
        RpyPayNotifyAttr.put("purc_comp_gove_name"  , new MsgAttr(16,   "purc_comp_gove_name",  126,    16,     'X',    null,               "���Ի��: null"));
        RpyPayNotifyAttr.put("working_key_index"    , new MsgAttr(17,   "working_key_index",    142,    2,      'X',    null,               "WorkingKeyIndex: nul"));
        RpyPayNotifyAttr.put("working_key"          , new MsgAttr(18,   "working_key",          144,    16,     'X',    null,               "WorkingKey: null"));
        RpyPayNotifyAttr.put("spare_point"          , new MsgAttr(19,   "spare_point",          160,    9,      '9',    null,               "��������Ʈ: null"));
        RpyPayNotifyAttr.put("point1"               , new MsgAttr(20,   "point1",               169,    9,      '9',    null,               "����Ʈ1: null"));
        RpyPayNotifyAttr.put("point2"               , new MsgAttr(21,   "point2",               178,    9,      '9',    null,               "����Ʈ2: null"));
        RpyPayNotifyAttr.put("point3"               , new MsgAttr(22,   "point3",               187,    9,      'X',    null,               "����Ʈ3: null"));
        RpyPayNotifyAttr.put("notice1"              , new MsgAttr(23,   "notice1",              196,    20,     'X',    null,               "Notice1: null"));
        RpyPayNotifyAttr.put("notice2"              , new MsgAttr(24,   "notice2",              216,    40,     'X',    null,               "Notice2: null"));
        RpyPayNotifyAttr.put("transaction_type"     , new MsgAttr(25,   "transaction_type",     256,    1,      'X',    null,               "�ŷ�����: 'N'(����)"));
        RpyPayNotifyAttr.put("reserved"             , new MsgAttr(26,   "reserved",             257,    5,      'X',    null,               "Reserved: null"));
        RpyPayNotifyAttr.put("ksnet_reserved"       , new MsgAttr(27,   "ksnet_reserved",       262,    40,     'X',    null,               "KSNET_Reserved: null"));
        RpyPayNotifyAttr.put("etx"                  , new MsgAttr(28,   "etx",                  302,    1,      'X',    null,               "ETX: 0x03"));
        RpyPayNotifyAttr.put("cr"                   , new MsgAttr(29,   "cr",                   303,    1,      'X',    null,               "CR: 0x0D"));

        // ���⿡ ���ο� �Ӽ��� �ʱ�ȭ �߰�...
        // ...
    }

    // ���� �ɹ� ���� //
    private String  msgName;                    // ���� �̸�
    private byte[]  msgStream;                  // ���� ����Ʈ��Ʈ��
    private Map<String, MsgAttr> msgAttrMap;    // ���� �Ӽ������� ���� ��
    
    private String traceId; // ���� ID
    private Logger logger;  // �ΰ�
    
    // ���� �ɹ� �Լ� //
    public IpgMsg(byte[] _msgStream, Map<String, MsgAttr> _msgAttrMap, String traceId, Logger logger) {
        this.msgStream = _msgStream;
        this.msgAttrMap = _msgAttrMap;
        this.traceId = traceId;
        this.logger = logger;

        // �Ӽ��� �̸� �ʱ�ȭ
        if (_msgAttrMap == ReqArrovalNumMsgAttr) {
            this.msgName = "ReqAprrovalNumMsg";
        }
        else if (_msgAttrMap == RpyArrovalNumMsgAttr) {
            this.msgName = "RpyAprrovalNumMsg";
        }
        else if (_msgAttrMap == ReqShopInfoMsgAttr) {
            this.msgName = "ReqShopInfoMsg";
        }
        else if (_msgAttrMap == RpyShopInfoMsgAttr) {
            this.msgName = "RpyShopInfoMsg";
        }
        else if (_msgAttrMap == ReqPayNotifyAttr) {
            this.msgName = "ReqPayNotify";
        }
        else if (_msgAttrMap == RpyPayNotifyAttr) {
            this.msgName = "RpyPayNotify";
        }
        else {
            this.msgName = "undefined_attr_msg";
        }

        // �Ӽ����� �°� �ʱ�ȭ ���� ����
        if (this.msgStream == null && _msgAttrMap != null) {
            // ���� �Ӽ��� �о�ͼ� ���� ����
            int szMsg = 0;

            for (String attrKey : _msgAttrMap.keySet()) {
                MsgAttr attr = _msgAttrMap.get(attrKey);
                szMsg += attr.length;
            }

            // ���� ����Ʈ��Ʈ�� �ʱ�ȭ
            genStreamAndInit(szMsg, _msgAttrMap);
            set("length", String.format("%04d", szMsg));
        }
    }

    // ���� ��Ʈ�� �Ҵ� �� �Ӽ���('9', 'X', ...) ���� ä���
    private void genStreamAndInit(int szMsg, Map<String, MsgAttr> msgAttrMap) {
        this.msgStream = new byte[szMsg];

        for (String key : msgAttrMap.keySet()) {
            MsgAttr attr = msgAttrMap.get(key);
            set(key, attr.defaults);
        }
    }

    // ���� �Ӽ����� ���ڿ��� ����
    public boolean set(String key, String newStrStream) {
        byte[] newStream = null;

        if (newStrStream != null) {
            newStream = newStrStream.getBytes();
        }

        return this.set(key, newStream);
    }

    // ���� �Ӽ����� ����Ʈ�� ����
    public boolean set(String key, byte[] newStream) {
        boolean rtResult = false;

        if (this.msgStream == null || key == null) {
            return false;
        }

        try {
            final MsgAttr msgAttr = msgAttrMap.get(key);

            if (msgAttr == null) {
                log(Logger.ERROR, traceId, String.format("[%s] Ű�� '%s'�� ã�� �� ����!", this.msgName, key));
                return false;
            }

            if (newStream == null) {
                newStream = msgAttr.defaults; // defaults �� null�� �� ����
            }

            byte typeFiller = '?';
            int type9_Offset = 0;

            if (msgAttr.type == 'X') {
                typeFiller = ' ';
            }
            else if (msgAttr.type == '9') {
                typeFiller = '0';
            }
            else {
                log(Logger.WARNING, traceId, String.format("[%s] �����ǵ� �Ӽ� Ÿ�� '%c'!", this.msgName, msgAttr.type));
            }

            // �ϴ� filler������ ä��
            Arrays.fill(this.msgStream, msgAttr.offset, msgAttr.offset + msgAttr.length, typeFiller);

            // ������ ���� �� �� ���� ���
            if (newStream != null) {
                int msgLoseLen = newStream.length - msgAttr.length;
                
                if (msgLoseLen > 0) {
                    String loseMsg = new String(Arrays.copyOfRange(newStream, msgAttr.length, msgAttr.length + msgLoseLen));
                    log(Logger.ERROR, traceId, String.format("[%s] Ű�� '%s'�� ����Ʈ ���� �ʰ�, [%s](%dbyte)���� �ջ� �߻�!", this.msgName, key, loseMsg, msgLoseLen));
                }

                if (msgAttr.type == '9') {
                    type9_Offset = Math.min(msgAttr.length - newStream.length, 0);
                }

                int offset = msgAttr.offset + type9_Offset;
                System.arraycopy(newStream, 0, this.msgStream, offset, Math.min(newStream.length, msgAttr.length));
            }

            rtResult = true;
        }
        catch (Exception e) {
            log(Logger.ERROR, traceId, "Exception!", e);
        }
        finally {
            return rtResult;
        }
    }

    // ���� �Ӽ����� ���ڿ��� ��ȯ
    public String get(String key) {
        String rtStr = null;

        if (this.msgStream == null || key == null) {
            return null;
        }

        try {
            final MsgAttr msgAttr = msgAttrMap.get(key);

            if (msgAttr == null) {
                log(Logger.ERROR, traceId, "msgAttr == null");
                return null;
            }

            int bgnIdx = msgAttr.offset, endIdx = msgAttr.offset + msgAttr.length;

            if (bgnIdx > endIdx) {
                log(Logger.ERROR, traceId, String.format("'%s'������ �Ӽ���'%s'�� offset(%d)�� length(%d)�˻�!", this.msgName, key, msgAttr.offset, msgAttr.length));             
                return null;
            }
            
            byte[] keyStream = Arrays.copyOfRange(this.msgStream, msgAttr.offset, msgAttr.offset + msgAttr.length);

            // �̽������� ���� ��ü
            for (int i = 0; i < keyStream.length; ++i) {
                if (keyStream[i] > (byte)0xFF && keyStream[i] < (byte)0x20) { // 0x00 ~ 0x1F ������ ���� ���� (�����÷� ����)
                    keyStream[i] = (byte)0x3F; // '?'
                }
            }

            rtStr = new String(keyStream, IPG_CHARSET);
        }
        catch (Exception e) {
            log(Logger.ERROR, traceId, "Exception!", e);
        }
        finally {
            return rtStr;
        }
    }

    public byte[] getMsgStream() {
        return this.msgStream;
    }

    public void setMsgStream(byte[] msgStream) {
        this.msgStream = msgStream;
    }

    public String getMsgName() {
        return this.msgName;
    }

    public Map<String, MsgAttr> getMsgAttrMap() {
        return this.msgAttrMap;
    }

    // ��� �Ӽ����� ���ڿ��� ��ȯ
    public String getAllAttrStr() {
        List<MsgAttr> attrList = new LinkedList<MsgAttr>(this.msgAttrMap.values());
        StringBuilder sb = new StringBuilder();

        Collections.sort(attrList);

        sb.append('[').append(this.msgName).append("':'").append(attrList.size()).append(']');

        for (MsgAttr msgAttr : attrList) {            
            sb.append("('").append(msgAttr.name).append("':'").append(this.get(msgAttr.name)).append("')");
        }

        return sb.toString();
    }

    // ��Ʈ���� ���ڿ��� ��ȯ
    public String getMsgStreamToStr() {
        byte[] byteStreamCopy = Arrays.copyOfRange(this.msgStream, 0, this.msgStream.length);

        for (int i = 0; i < byteStreamCopy.length; ++i) {
            byte b = byteStreamCopy[i];

            if (b > (byte)0xFF && b < (byte)0x20) {
                byteStreamCopy[i] = (byte)0x3F; // '?'
            }
        }

        return new String(byteStreamCopy);
    }

    public String getMsgStreamToHexaStr() {
        StringBuilder hexaSb = new StringBuilder();

        for (byte b : this.msgStream) {
            hexaSb.append(String.format("%02X ", b));
        }

        return hexaSb.toString();
    }

    // IpgMsg �α׻���
    private void log(int logType, String traceId, Object... logObjs) {
        if (logger != null) {
           logger.traceLog(logType, traceId, "[IpgMsg]", logObjs);
        }
	}

    // ���� �Ӽ��� �������� Ŭ���� //
    public static class MsgAttr implements Comparable<MsgAttr> {

        public int      number;     // ����
        public String   name;       // �׸��
        public int      offset;     // ����Ʈ ������
        public int      length;     // ����Ʈ ����
        public char     type;       // �Ӽ�
        public byte[]   defaults;    // �⺻��
        public String   desc;       // ����

        public MsgAttr(int _number, String _name, int _offset, int _length, char _type, byte[] _defaults, String _desc) {
            number      = _number;
            name        = _name;
            offset      = _offset;
            length      = _length;
            type        = _type;
            defaults    = _defaults;
            desc        = _desc;
        }

        @Override
        public int compareTo(MsgAttr msgAttr) {
            return (this.number - msgAttr.number); // ��������
        }
    }
}