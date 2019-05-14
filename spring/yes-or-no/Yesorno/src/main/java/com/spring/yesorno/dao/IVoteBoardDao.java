package com.spring.yesorno.dao;

import java.util.ArrayList;

import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dto.VoteBoardDto;

public interface IVoteBoardDao {

	public int selectVoteBoardRowCount() throws DataAccessException; // �Խñ� ���� ��ȸ
	public VoteBoardDto selectVoteBoard(int boardId) throws DataAccessException; // �Խñ� ��ȣ�� ��ȸ
	public ArrayList<VoteBoardDto> selectVoteBoardList(int begin, int dataPerPage) throws DataAccessException; // �Խñ� (begin)~(begin+dataPerPage) ��ȸ
	public int insertVoteBoard(VoteBoardDto voteBoardDto) throws DataAccessException; // �Խñ� �߰�
	public int updateVoteBoardContent(int boardId, String modifiedContent) throws DataAccessException; // �Խñ� �������� ����
	public int deleteVoteBoard(int boardId) throws DataAccessException; // �Խñ� ��ȣ�� ����
}
