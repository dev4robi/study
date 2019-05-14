package com.spring.yesorno.exception;

@SuppressWarnings("serial")
public class MemberException extends RuntimeException {
	
	public enum EnumMemberExceptionType {
		ET_UNKNOWN,
		ET_TOKEN_VALIDATION_FAIL,					/** �ش� ȸ����ū ������ �������� �� */
		ET_DB_DATA_NOT_FOUND,						/** DB���� �ش� ȸ�� ������ ã�� ������ �� */
		ET_DB_UPDATE_FAIL,							/** DB�� ȸ������ ������Ʈ �������� �� */
		ET_WRONG_AUTHORITY_LEVEL,					/** ȸ�� ����� �ش� ����� �����ϱ⿡ �������� �� */
		ET_RETIRED_MEMBER,							/** Ż���� ȸ���� �� */
		ET_BANNED_MEMBER,							/** ��� ������ ȸ���� �� */
	}

	private EnumMemberExceptionType exceptionType;
	
	public MemberException(EnumMemberExceptionType exceptionType) {
		this.exceptionType = exceptionType;
	}

	public EnumMemberExceptionType getExceptionType() {
		return exceptionType;
	}

	public void setExceptionType(EnumMemberExceptionType exceptionType) {
		this.exceptionType = exceptionType;
	}
}
