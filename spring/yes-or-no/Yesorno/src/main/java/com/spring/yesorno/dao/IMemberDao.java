package com.spring.yesorno.dao;

import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.MemberDto;

public interface IMemberDao {

	public String selectMemberNickname(int memberId) throws DataAccessException;			// ȸ�� �г��� ��ȸ
	public MemberDto memberSelect(MemberDto memberDto) throws DataAccessException;			// ȸ��  ��ȸ
	public int memberInsert(MemberDto memberDto) throws DataAccessException; 				// ȸ�� ����
	public int memberDeregistrationUpdate(MemberDto memberDto) throws DataAccessException;	// ȸ�� Ż�� ���� ������Ʈ
	public int memberInfoUpdate(MemberDto memberDto) throws DataAccessException;			// ȸ������ ����
	public int memberLoginUpdate(MemberDto memberDto) throws DataAccessException;			// ȸ�� �α��� ���� ������Ʈ
}
