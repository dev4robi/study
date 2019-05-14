package com.spring.yesorno.dto;

import java.util.Date;

/** ��ǥ �����Ȳ DB���� */
public class VoteProgressDto {

	private int voteProgressId;				// INT				PK,AI		��ǥ ���� ��ȣ
	private int voteBoardId;				// INT 				FK(ODC),NN	��ǥ ���� �Խ��� ��ȣ
	private int voterMemberId;				// INT				FK(ODR),NN	��ǥ�� ��ȣ
	private int voteResult;					// INT				NN			��ǥ ��� (+1:����, -1�ݴ�)
	private Date voteDate;					// DATETIME			NN			��ǥ ����
	
	public int getVoteProgressId() {
		return voteProgressId;
	}
	
	public void setVoteProgressId(int voteProgressId) {
		this.voteProgressId = voteProgressId;
	}
	
	public int getVoteBoardId() {
		return voteBoardId;
	}
	
	public void setVoteBoardId(int voteBoardId) {
		this.voteBoardId = voteBoardId;
	}
	
	public int getVoterMemberId() {
		return voterMemberId;
	}
	
	public void setVoterMemberId(int voterMemberId) {
		this.voterMemberId = voterMemberId;
	}

	public int getVoteResult() {
		return voteResult;
	}

	public void setVoteResult(int voteResult) {
		this.voteResult = voteResult;
	}

	public Date getVoteDate() {
		return voteDate;
	}
	
	public void setVoteDate(Date voteDate) {
		this.voteDate = voteDate;
	}
}
