package com.spring.yesorno.dto;

/** ��ǥ ����� ���� */
public class VoteResultDto {

	private int voteAgreePercent;	// ���� �����
	private int voteAgreeCnt;		// ���� ��
	private int voteDisagreeCnt;	// �ݴ� ��
	private String voteResult;		// ��ǥ ���

	public int getVoteAgreePercent() {
		return voteAgreePercent;
	}

	public void setVoteAgreePercent(int voteAgreePercent) {
		this.voteAgreePercent = voteAgreePercent;
	}

	public int getVoteAgreeCnt() {
		return voteAgreeCnt;
	}

	public void setVoteAgreeCnt(int voteAgreeCnt) {
		this.voteAgreeCnt = voteAgreeCnt;
	}

	public int getVoteDisagreeCnt() {
		return voteDisagreeCnt;
	}

	public void setVoteDisagreeCnt(int voteDisagreeCnt) {
		this.voteDisagreeCnt = voteDisagreeCnt;
	}

	public String getVoteResult() {
		return voteResult;
	}

	public void setVoteResult(String voteResult) {
		this.voteResult = voteResult;
	}
}
