package com.spring.yesorno.dao;

import java.util.ArrayList;

import org.mybatis.spring.SqlSessionTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.VoteBoardDto;

public class VoteBoardDao implements IVoteBoardDao{

	@Autowired private SqlSessionTemplate sqlSession;
	
	// �Խñ� ���� ��ȸ
	public int selectVoteBoardRowCount() throws DataAccessException {
		IVoteBoardDao voteBoardDao = sqlSession.getMapper(IVoteBoardDao.class);
		return voteBoardDao.selectVoteBoardRowCount();
	}
	
	// �Խñ� ��ȣ�� ��ȸ
	public VoteBoardDto selectVoteBoard(int boardId) throws DataAccessException {
		IVoteBoardDao voteBoardDao = sqlSession.getMapper(IVoteBoardDao.class);
		return voteBoardDao.selectVoteBoard(boardId);
	}
	
	// �Խñ� �������� ��ȸ
	public ArrayList<VoteBoardDto> selectVoteBoardList(int begin, int dataPerPage) throws DataAccessException {
		IVoteBoardDao voteBoardDao = sqlSession.getMapper(IVoteBoardDao.class);
		return voteBoardDao.selectVoteBoardList(begin, dataPerPage);
	}
	
	// �Խñ� �߰�
	public int insertVoteBoard(VoteBoardDto voteBoardDto) throws DataAccessException {
		IVoteBoardDao voteBoardDao = sqlSession.getMapper(IVoteBoardDao.class);
		return voteBoardDao.insertVoteBoard(voteBoardDto);
	}
	
	// �Խñ� �������� ����
	public int updateVoteBoardContent(int boardId, String modifiedContent) {
		IVoteBoardDao voteBoardDao = sqlSession.getMapper(IVoteBoardDao.class);
		return voteBoardDao.updateVoteBoardContent(boardId, modifiedContent);
	}
	
	// �Խñ� ��ȣ�� ����
	public int deleteVoteBoard(int boardId) throws DataAccessException {
		IVoteBoardDao voteBoardDao = sqlSession.getMapper(IVoteBoardDao.class);
		return voteBoardDao.deleteVoteBoard(boardId);
	}
}
