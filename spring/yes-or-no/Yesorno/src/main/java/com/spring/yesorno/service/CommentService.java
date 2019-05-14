package com.spring.yesorno.service;

import java.util.Date;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dao.CommentDao;
import com.spring.yesorno.dto.CommentDto;
import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.util.JwtMemberAuth;

public class CommentService {
	
	@Autowired JwtMemberAuth jwtMemberAuth;
	@Autowired CommentDao commentDao;
	
	Log log = LogFactory.getLog(CommentService.class); 
	
	public boolean writeComment(String memberToken, int boardId, int baseCommentId, int stepNumber, String commentContent) {
		boolean svcResult = false;
		
		try {
			// ���� ���� �˻�
			if (commentContent == null) {
				return false; // ���� ����
			}
			
			int contentLength = commentContent.length(); 
			if (contentLength < 2 || contentLength > 256) {
				return false; // ���� ���� ����
			}
			
			// ȸ����ū ���� 
			MemberDto writeMemberDto = null;
			if ((writeMemberDto = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				return false; // ȸ����ū ���� ����
			}
			
			// ��� DTO ����
			CommentDto writeCommentDto = new CommentDto();
			writeCommentDto.setCommentId(0);
			writeCommentDto.setCommentOwnedBoardId(boardId);
			writeCommentDto.setCommentWriterMemberId(writeMemberDto.getMemberId());
			writeCommentDto.setCommentWrittenDate(new Date());
			writeCommentDto.setCommentContent(commentContent);
			writeCommentDto.setCommentBaseCnt(baseCommentId);
			writeCommentDto.setCommentStepCnt(stepNumber);
			
			// ��� ����
			if (commentDao.insertComment(writeCommentDto) != 1) {
				return false; // ��� ���� ����
			} else {
				svcResult = true;
			}
			
		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		}

		return svcResult;
	}
}
