package com.robi.ksnet;

import java.util.HashMap;

public class ServletCommonVar {
    // ���� ���뺯�� //
    public String _wtId, _wtPw;						    // ����ū, ��ū��ȣ
	public String _reqURI;								// ������ URI
	public HashMap<String, String> _reqMap, _rpyMap;	// reqeust, response�� ���� HashMap
	public boolean _jspResult;							// ���� ���

    public ServletCommonVar() {
        this._wtId = null;
        this._wtPw = null;
        this._reqURI = null;
        this._reqMap = null;
        this._rpyMap = null;
        this._jspResult = false;
    }
}