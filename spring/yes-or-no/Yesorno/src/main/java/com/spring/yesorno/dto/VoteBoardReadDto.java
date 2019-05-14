package com.spring.yesorno.dto;

import java.util.Date;

/** ��ǥ �Խñ� �б� ���� */
public class VoteBoardReadDto extends VoteBoardListDto {

	private boolean readerIsWriter;			// �� �ۼ��� Ȯ��
	private String writerMemberNickname;	// ��ǥ �Խñ� �ۼ��� �г���
	private Date voteBoardWrittenDate;		// ��ǥ �Խñ� �ۼ���
	private int voteBoardViewCnt;			// ��ǥ �Խñ� ��ȸ��
	private String voteBoardContent;		// ��ǥ �Խñ� ����
	private Date voteEndDate;				// ��ǥ ���� ����
	private int voteEndCnt;					// ��ǥ ���� ��ǥ��
	private int voteAgreePercent;			// ��ǥ ���� �����

	public VoteBoardReadDto(VoteBoardListDto dto) {
		this.voteBoardId = dto.voteBoardId;
		this.voteBoardTitle = dto.voteBoardTitle;
		this.voteBoardImageURL = dto.voteBoardImageURL;
		this.voteAgreeCnt = dto.voteAgreeCnt;
		this.voteDisagreeCnt = dto.voteDisagreeCnt;
		this.voteBoardState = dto.voteBoardState;
	}
	
	public boolean isReaderIsWriter() {
		return readerIsWriter;
	}

	public void setReaderIsWriter(boolean readerIsWriter) {
		this.readerIsWriter = readerIsWriter;
	}

	public String getWriterMemberNickname() {
		return writerMemberNickname;
	}
	
	public void setWriterMemberNickname(String writerMemberNickname) {
		this.writerMemberNickname = writerMemberNickname;
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
	
	public String getVoteBoardContent() {
		return voteBoardContent;
	}
	
	public void setVoteBoardContent(String voteBoardContent) {
		this.voteBoardContent = voteBoardContent;
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

	public int getVoteAgreePercent() {
		return voteAgreePercent;
	}

	public void setVoteAgreePercent(int voteAgreePercent) {
		this.voteAgreePercent = voteAgreePercent;
	}
}
