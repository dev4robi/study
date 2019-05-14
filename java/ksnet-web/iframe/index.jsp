<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ include file="./include/fcex_include.jsp"%>
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
	<!-- �Է¿� â -->
	<div id="inputPage" align="center">
		<div class="d-flex justify-content-center align-items-center" style="height:500px;">
			<form action="./fcex_result.jsp" method="GET" target="resultPage">
				<table>
					<!-- Ÿ��Ʋ -->
					<tr>
						<td colspan="3" align="center"><br><h1>KSnet ȯ�� ��ȯ</h1><br></td>
					</tr>
					<tr>
						<!-- ��ȭ ���� -->
						<td align="left"><div class="form-group">
							<label for="wonPrice">��ȭ ���� (��)</label>
							<input type="text" style="text-align: right;" class="form-control" name="wonPrice" id="wonPrice" max="2147483647" value="${wonPrice}"/>
						</div></td>
						<!-- ��ȭ ���� �޺��ڽ� -->
						<td align="center"><div class="form-group">
							<label for="excType">��ȭ ����</label>
							<select class="form-control" name="excType" id="excType">
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
						<td colspan="3" align="center"><input type="submit" class="btn btn-info btn-block" value="ȯ�� ��ȯ" onclick="showResultPage()"></td>
					</tr>
					<!-- �ݿø� üũ�ڽ� -->
					<tr>
						<td colspan="3" align="right" style="font-size: 12px;">
							<div class="radio">
								��� ����� (
								<c:forEach var="i" begin="0" end="${fn:length(RoundOptions) - 1}" step="1">
									<c:set var="roundOpsVal" value="${RoundOptions[i]}"/>
									<label><input type="radio" name="roundOps" value="${roundOpsVal}" <c:choose>
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
			</form>
		</div>
	</div>
	<!-- iframe ���â -->
	<iframe id="resultPage" name="resultPage" src="" width="100%" height="0" frameBorder="0">></iframe>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="/resources/js/jquery331.js"></script>
    <!-- Include all compiled plugins (below), or include individual files as needed -->
    <script src="/resources/js/bootstrap.min.js"></script>
	<!-- JavaScript for iframe -->
	<script src="js/iframe.js"></script>
</body>
</html>