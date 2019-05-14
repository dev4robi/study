<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ include file="./include/fcex_include.jsp"%>
<%!	
	//--- [Concurrent var Begin] ---//
	//
	private Map<String, Long> fcexMap = new ConcurrentHashMap<String, Long>();		// Thread-safe HashMap for currency exchange data
	private Map<String, Long> fcexMapCpy = new ConcurrentHashMap<String, Long>();	// Thread-safe HashMap to work concurrency
	private boolean fcexUpdating = false;											// While updating fcexMap, value must be 'true' otherwise 'false'
	private boolean fcexCpyUpdating = false;										// While updating fcexCpyMap, value must be 'true' otherwise 'false'
	private long nextFcexMapUpdateableTime = 0;										// The time that next updateable
	//
	//--- [Concurrent var End] ---//

	// Update 'FCEX' (Foreign Currency EXchange hashmap)
    protected synchronized void updateFcex() {
		long curTime = System.currentTimeMillis();

        if (curTime > nextFcexMapUpdateableTime) {
			nextFcexMapUpdateableTime = curTime + UpdateTermMillis; // next updateable time is after 1-hour from curTime
			fcexUpdating = true;
			fcexMap.clear();
				
			new Thread(()->{ // execute main logic with thread to prevent concurrency bottleneck
				boolean updateSuccess = false;
				
				// [1] Request hana-bank FCEX data from server to update 'Map<String, Long> fcexMap'
				try {
					System.out.println(String.format("fcexMap update begin! (%d)", System.currentTimeMillis())); // test
					byte[] sendByte = KsCommonLib.makeSendByte("ufce2", null);
					ByteBuffer writeBuf = ByteBuffer.allocate(sendByte.length);
					
					writeBuf.put(sendByte);
					writeBuf.clear();
					
					InetSocketAddress svrAddr = new InetSocketAddress("127.0.0.1", 9999);
					SocketChannel cliSocChannel = SocketChannel.open(svrAddr);
					cliSocChannel.write(writeBuf);
					
					ByteBuffer readBuf = ByteBuffer.allocate(5096);
					byte[] readByte = null;
					int readLen = -1;
					
					if ((readLen = cliSocChannel.read(readBuf)) != -1) {
						readByte = new byte[readLen];
						System.arraycopy(readBuf.array(), 0, readByte, 0, readByte.length);
					}
					
					System.out.println(String.format("����:\n%s", new String(readByte)));
					byte[] recvByte = KsCommonLib.makeRecvByte(readByte);
					recvByte = KsCommonLib.hexString2ByteAry(new String(recvByte));
					fcexMap = (ConcurrentHashMap<String, Long>)KsCommonLib.byteAry2Obj(recvByte);
					
					updateSuccess = true; // only way for fcexMap updating 'success'
					cliSocChannel.close();
					System.out.println(String.format("fcexMap update complete! (%d)", System.currentTimeMillis())); // test
					System.out.println("fcexMap: " + fcexMap.toString()); // test
				}
				catch (Exception e) {
					e.printStackTrace();
					nextFcexMapUpdateableTime = 0; // init next update time when exception thrown
					return;
				}
				
				fcexUpdating = false;
				
				// [2] Update 'Map<String, Long> fcexMapCpy' using copy data of fcexMap
				if (updateSuccess) {
					System.out.println(String.format("fcexMapCpy update begin! (%d)", System.currentTimeMillis())); // test
					fcexCpyUpdating = true;
					fcexMapCpy.clear();

					for (Entry<String, Long> entry : fcexMap.entrySet()) { // fcexMap deep copy
						fcexMapCpy.put(entry.getKey(), new Long(entry.getValue().toString()));
					}
					
					fcexCpyUpdating = false;
					System.out.println(String.format("fcexMapCpy update complete! (%d)", System.currentTimeMillis())); // test
					System.out.println("fcexMapCpy: " + fcexMapCpy.toString()); // test
				}
			}).start(); 
		}

        return;
    }

	// fcexMap is ConcurrentHashMap(thread-safe) and this method does not change static value. so this method does not need 'synchronized' keyword
    protected Long getFcex(String key) throws Exception {
		if (fcexMap.size() > 0 && !fcexUpdating) {
			System.out.println("������ ��� sz:" + fcexMap.size()); // test
			return fcexMap.get(key);
		}
		else if (fcexMapCpy.size() > 0 && !fcexCpyUpdating) {
			System.out.println("ī�Ǹ� ��� sz:" + fcexMapCpy.size()); // test
			return fcexMapCpy.get(key);
		}
		else {
			System.out.println("throwException"); // test
			throw new Exception("ȯ�������� �������Դϴ�. ���� �� �ٽ� �õ����ּ���.");
		}
    }

	// Get codeSymbol from codename
    protected String getCurrencySymbolByCodename(String codename) {
        if (codename != null && codename.length() > 0) {
			for (int i = 0; i < CurrencyCodename.length; ++i) {
				if (codename.equals(CurrencyCodename[i])) {
					return CurrencyCodeSymbol[i];
				}
			}
		}

        return "";
    }
%>

<%
	request.setCharacterEncoding("EUC-KR");
		
	// [1] When this page called, automatically update FCEX table every 'UpdateTermMillis'
	updateFcex();

	// [2] Calculate exchange result by 'ajax'
	request.setCharacterEncoding("EUC-KR");

	String errorMsg = "";
	String wonPriceParam   = request.getParameter("wonPrice");
	String excPriceParam   = null;
	String excTypeParam    = request.getParameter("excType");
	String roundOpsParam   = request.getParameter("roundOps");
	boolean doExcWork      = true;
	long wonPrice = 0;
	long excPrice = 0;

	try {
		// [2-1] form parameter validation check
		double dbWonPrice = 0.00;
		if (wonPriceParam == null) {
			doExcWork = false;
		}
		else if (((wonPriceParam = wonPriceParam.replaceAll("[^0-9//.//-]", "")).length()) == 0) {
			errorMsg += " [�ùٸ� ��ȭ���� �Է�] ";
			doExcWork = false;
		}
		else if ((dbWonPrice = Double.parseDouble(wonPriceParam)) <= 0.00) {
			errorMsg += " [0�� �̻��� ��ȭ���� �Է�] ";
			doExcWork = false;
		}
		else if (dbWonPrice > (double)Integer.MAX_VALUE) {
			errorMsg += " [�Ѱ� �ݾ� �ʰ�] ";
			doExcWork = false;
		}
		else {
			wonPrice = Long.parseLong(wonPriceParam);
		}
		
		if (excTypeParam == null) {
			excTypeParam = CurrencyCodename[0];
		}
		else {
			boolean excContain = false;
			for (int i = 0; i < CurrencyCodename.length; ++i) {
				if (excTypeParam.equals(CurrencyCodename[i])) {
					excContain = true;
					break; 
				}
			}
			
			if (!excContain) {
				errorMsg += String.format(" [������ ��ȭ����(%s)] ", excTypeParam);
				doExcWork = false;
			}
		}
		
		if (roundOpsParam == null) {
			roundOpsParam = RoundOptions[0];
			doExcWork = false;
		}
		else {
			boolean roundOpsContain = false;
			for (int i = 0; i < RoundOptions.length; ++i) {
				if (roundOpsParam.equals(RoundOptions[i])) {
					roundOpsContain = true;
					break; 
				}
			}
			
			if (!roundOpsContain) {
				errorMsg += String.format(" [���� �ɼ� ����(%s)] ", roundOpsParam);
				doExcWork = false;
			}
		}
		
		// [2-2] Exchange work
		if (doExcWork) {
			long excVal = getFcex(excTypeParam);
			
			excPrice = wonPrice * 100 / excVal;
			
			System.out.println(String.format("�Է�:%d / ȯ��:%d / �ݾ�:%d", wonPrice, excVal, excPrice)); // test
			
			if (roundOpsParam.equals(RoundOptions[0])) { // round up
				if (excPrice % excVal != 0) { ++excPrice; System.out.println("�ø� ����"); }
			}
			else if (roundOpsParam.equals(RoundOptions[1])) { // half round up
				if (excPrice % excVal >= 50) { ++excPrice; System.out.println("�ݿø� ����"); }
			}
			else if (roundOpsParam.equals(RoundOptions[2])) { // round down
				// round down will set default by divide(/) operator
			}
		}
	}
	catch (Exception e) {
		e.printStackTrace();
		errorMsg += String.format(" [%s] ", e.getMessage());
		doExcWork = false;
	}
	finally {
		// [3] Update html UI
		wonPriceParam = String.format("%d", wonPrice);
		excPriceParam = String.format("%d%s", excPrice, getCurrencySymbolByCodename(excTypeParam));		
	}
%>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
    <!-- Bootstrap -->
    <link href="/resources/css/bootstrap.min.css" rel="stylesheet">
    <title>KSnet FCEX Service (iframe)</title>
</head>
<body>
	<c:set var="CurrencyCodename" value="<%=CurrencyCodename%>"/>
	<c:set var="CurrencyCodeSymbol" value="<%=CurrencyCodeSymbol%>"/>
	<c:set var="RoundOptions" value="<%=RoundOptions%>"/>
	<c:set var="RoundOptionCodename" value="<%=RoundOptionSymbol%>"/>
	<c:set var="wonPrice" value="<%=wonPriceParam%>"/>
	<c:set var="excPrice" value="<%=excPriceParam%>"/>
	<c:set var="excType" value="<%=excTypeParam%>"/>
	<c:set var="roundOps" value="<%=roundOpsParam%>"/>
	<c:set var="errorMsg" value="<%=errorMsg%>"/>
	<!-- ��¿� â -->
	<div class="d-flex justify-content-center align-items-center" style="height: 500px;">
		<table>
			<!-- Ÿ��Ʋ -->
			<tr>
				<td colspan="3" align="center"><br><h1>KSnet ȯ�� ��ȯ</h1><br></td>
			</tr>
			<tr>
				<!-- ��ȭ ���� -->
				<td align="left"><div class="form-group">
					<label for="wonPrice">��ȭ ���� (��)</label>
					<input type="text" style="text-align: right;" class="form-control" name="wonPrice" id="wonPrice" max="2147483647" value="${wonPrice}" readonly/>
				</div></td>
				<!-- ��ȭ ���� �޺��ڽ� -->
				<td align="center"><div class="form-group">
					<label for="excType">��ȭ ����</label>
					<select class="form-control" name="excType" id="excType" disabled>
						<c:forEach var="i" begin="0" end="${fn:length(CurrencyCodename) - 1}" step="1">
							<c:set var="codename" value="${CurrencyCodename[i]}"/>
							<c:set var="codeSymbol" value="${CurrencyCodeSymbol[i]}"/>
							<option value="${codename}" <c:choose>
								<c:when test="${empty excType}">
									<c:if test="${i == 0}">selected</c:if>
								</c:when>
								<c:otherwise>
									<c:if test="${excType == CurrencyCodename[i]}">selected</c:if>
								</c:otherwise>
							</c:choose>>${codename}(${codeSymbol})</option>
						</c:forEach>
					</select>
				</div></td>
				<!-- ��ȭ ȯ����� -->
				<td align="right"><div class="form-group">
					<label for="excPrice">��ȭ ����</label>
					<input type="text" style="text-align: right;" class="form-control" name="excPrice" id="excPrice" value="${excPrice}" readonly>
				</div></td>
			</tr>
			<!-- ȯ�� ��ȯ ��ư -->
			<tr>
				<td colspan="3" align="center"><input type="button" class="btn btn-warning btn-block" value="�ٸ� �ݾ� �����ϱ�" onclick="showInputPage()"></td>
			</tr>
			<!-- �ݿø� üũ�ڽ� -->
			<tr>
				<td colspan="3" align="right" style="font-size: 12px;">
					<div class="radio">
						<c:set var="RoundOptionCodename" value="${['�ø�', '�ݿø�', '����']}"/>
						��� ����� (
						<c:forEach var="i" begin="0" end="${fn:length(RoundOptions) - 1}" step="1">
							<c:set var="roundOpsVal" value="${RoundOptions[i]}"/>
							<label><input type="radio" name="roundOps" value="${roundOpsVal}" disabled <c:choose>
								<c:when test="${empty roundOps}">
									<c:if test="${i == 0}">checked</c:if>
								</c:when>
								<c:otherwise>
									<c:if test="${roundOps == roundOpsVal}">checked</c:if>
								</c:otherwise>
							</c:choose>>${RoundOptionCodename[i]}</option>
						</c:forEach>)
					</div>
				</td>
			</tr>
			<!-- ���� �޽���! -->
			<tr id="errorMsgRow" <c:if test="${empty errorMsg}">style="display: none;"</c:if>><td colspan="3"><div class="alert alert-danger">
				<strong>����!</strong> <span id="errorMsg">${errorMsg}</span>
			</div></td></tr>
		</table>
	</div>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="/resources/js/jquery331.js"></script>
    <!-- Include all compiled plugins (below), or include individual files as needed -->
    <script src="/resources/js/bootstrap.min.js"></script>
	<!-- JavaScript for iframe -->
	<script src="js/iframe.js"></script>
</body>
</html>