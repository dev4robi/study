package com.spring.yesorno.dto;

import java.util.Date;

public class CommentDto {

	private int commentId;				// INT			PK,AI		��� ��ȣ
	private int commentOwnedBoardId;	// INT			FK(ODC),NN	��� �ۼ� �Խñ� ��ȣ
	private int commentWriterMemberId;	// INT			FK(ODR),NN	��� �ۼ��� ��ȣ
	private Date commentWrittenDate;	// DATETIME		NN			��� �ۼ���
	private String commentContent;		// VARCHAR(256)	NN			��� ����
	private int commentBaseCnt;			// INT			NN			������ ���۹�ȣ  
	private int commentStepCnt;			// INT			NN			���� ����
	
	public int getCommentId() {
		return commentId;
	}
	
	public void setCommentId(int commentId) {
		this.commentId = commentId;
	}
	
	public int getCommentOwnedBoardId() {
		return commentOwnedBoardId;
	}
	
	public void setCommentOwnedBoardId(int commentOwnedBoardId) {
		this.commentOwnedBoardId = commentOwnedBoardId;
	}
	
	public int getCommentWriterMemberId() {
		return commentWriterMemberId;
	}
	
	public void setCommentWriterMemberId(int commentWriterMemberId) {
		this.commentWriterMemberId = commentWriterMemberId;
	}
	
	public Date getCommentWrittenDate() {
		return commentWrittenDate;
	}
	
	public void setCommentWrittenDate(Date commentWrittenDate) {
		this.commentWrittenDate = commentWrittenDate;
	}
	
	public String getCommentContent() {
		return commentContent;
	}
	
	public void setCommentContent(String commentContent) {
		this.commentContent = commentContent;
	}
	
	public int getCommentBaseCnt() {
		return commentBaseCnt;
	}
	
	public void setCommentBaseCnt(int commentBaseCnt) {
		this.commentBaseCnt = commentBaseCnt;
	}
	
	public int getCommentStepCnt() {
		return commentStepCnt;
	}
	
	public void setCommentStepCnt(int commentStepCnt) {
		this.commentStepCnt = commentStepCnt;
	}
}
