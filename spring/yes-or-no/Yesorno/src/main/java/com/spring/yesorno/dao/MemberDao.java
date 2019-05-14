package com.spring.yesorno.dao;

import org.mybatis.spring.SqlSessionTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.MemberDto;

public class MemberDao implements IMemberDao {

	@Autowired private SqlSessionTemplate sqlSession;

	// ȸ�� �г��� ��ȸ
	public String selectMemberNickname(int memberId) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.selectMemberNickname(memberId);
	}
	
	// ȸ�� ��ȸ
	public MemberDto memberSelect(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberSelect(memberDto);
	}
	
	// ȸ�� ����
	public int memberInsert(MemberDto memberDto) throws DataAccessException { 			
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberInsert(memberDto);
	}
	
	// ȸ�� Ż�� ���� ������Ʈ
	public int memberDeregistrationUpdate(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberDeregistrationUpdate(memberDto);
	}
	
	// ȸ������ ����
	public int memberInfoUpdate(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberInfoUpdate(memberDto);
	}
	
	// ȸ�� �α��� ���� ������Ʈ
	public int memberLoginUpdate(MemberDto memberDto) throws DataAccessException {
		IMemberDao memberDao = sqlSession.getMapper(IMemberDao.class);
		return memberDao.memberLoginUpdate(memberDto);
	}
}
