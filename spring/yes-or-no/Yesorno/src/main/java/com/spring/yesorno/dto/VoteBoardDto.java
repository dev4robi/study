package com.spring.yesorno.dto;

import java.util.Date;

/** ��ǥ �Խñ� DB ���� */
public class VoteBoardDto {

	private int voteBoardId;				// INT				PK,AI		��ǥ�Խñ� ��ȣ
	private int writerMemberId;				// INT 				FK,NN		�ۼ��� ��ȣ
	private Date voteBoardWrittenDate;		// DATETIME			NN			��ǥ�Խñ� �ۼ���
	private int voteBoardViewCnt;			// INT				NN			��ǥ�Խñ� ��ȸ��
	private String voteBoardTitle;			// VARCHAR(64)		NN			��ǥ�Խñ� ����
	private String voteBoardContent;		// VARCHAR(256)		NN			��ǥ�Խñ� ����
	private String voteBoardImageURL;		// VARCHAR(128)					��ǥ�Խñ� ÷���̹��� URL
	private Date voteEndDate;				// DATETIME			NN			Ʃǥ ���� ����
	private int voteEndCnt;					// INT							�ִ� ��ǥ �ο�

	public void copy(VoteBoardDto voteBoardDto) {
		voteBoardId = voteBoardDto.voteBoardId;
		writerMemberId = voteBoardDto.writerMemberId;
		voteBoardWrittenDate = voteBoardDto.voteBoardWrittenDate;
		voteBoardViewCnt = voteBoardDto.voteBoardViewCnt;
		voteBoardTitle = voteBoardDto.voteBoardTitle;
		voteBoardContent = voteBoardDto.voteBoardContent;
		voteBoardImageURL = voteBoardDto.voteBoardImageURL;
		voteEndDate = voteBoardDto.voteEndDate;
		voteEndCnt = voteBoardDto.voteEndCnt;
	}

	public int getVoteBoardId() {
		return voteBoardId;
	}
	
	public void setVoteBoardId(int voteBoardId) {
		this.voteBoardId = voteBoardId;
	}
	
	public int getWriterMemberId() {
		return writerMemberId;
	}
	
	public void setWriterMemberId(int writerMemberId) {
		this.writerMemberId = writerMemberId;
	}
	
	public Date getVoteBoardWrittenDate() {
		return voteBoardWrittenDate;
	}
	
	public void setVoteBoardWrittenDate(Date voteBoardWrittenDate) {
		this.voteBoardWrittenDate = voteBoardWrittenDate;
	}
	
	public int getVoteBoardViewCnt() {
		return voteBoardViewCnt;
	}
	
	public void setVoteBoardViewCnt(int voteBoardViewCnt) {
		this.voteBoardViewCnt = voteBoardViewCnt;
	}
	
	public String getVoteBoardTitle() {
		return voteBoardTitle;
	}
	
	public void setVoteBoardTitle(String voteBoardTitle) {
		this.voteBoardTitle = voteBoardTitle;
	}
	
	public String getVoteBoardContent() {
		return voteBoardContent;
	}
	
	public void setVoteBoardContent(String voteBoardContent) {
		this.voteBoardContent = voteBoardContent;
	}
	
	public String getVoteBoardImageURL() {
		return voteBoardImageURL;
	}
	
	public void setVoteBoardImageURL(String voteBoardImageURL) {
		this.voteBoardImageURL = voteBoardImageURL;
	}
	
	public Date getVoteEndDate() {
		return voteEndDate;
	}
	
	public void setVoteEndDate(Date voteEndDate) {
		this.voteEndDate = voteEndDate;
	}
	
	public int getVoteEndCnt() {
		return voteEndCnt;
	}
	
	public void setVoteEndCnt(int voteEndCnt) {
		this.voteEndCnt = voteEndCnt;
	}
}
