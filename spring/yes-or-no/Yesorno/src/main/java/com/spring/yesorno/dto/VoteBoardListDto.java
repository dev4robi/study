package com.spring.yesorno.dto;

/** ��ǥ �Խñ� ��� */
public class VoteBoardListDto {

	public enum EnumVoteBoardState {
		BS_UNKOWN,
		BS_PROGRESSING,			// ������
		BS_IMMINENT,			// �ӹ�
		BS_ENDED,				// ����
	}
	
	protected int voteBoardId;						// ��ǥ�Խñ� ��ȣ
	protected String voteBoardTitle;				// ��ǥ�Խñ� ����
	protected String voteBoardImageURL;				// ��ǥ�Խñ� �̹��� ���
	protected EnumVoteBoardState voteBoardState;	// ��ǥ�Խñ� ����
	protected int voteAgreeCnt;						// ��ǥ�Խñ� ���� ����
	protected int voteDisagreeCnt;					// ��ǥ�Խñ� �ݴ� ����

	public int getVoteBoardId() {
		return voteBoardId;
	}

	public void setVoteBoardId(int voteBoardId) {
		this.voteBoardId = voteBoardId;
	}

	public String getVoteBoardTitle() {
		return voteBoardTitle;
	}
	
	public void setVoteBoardTitle(String voteBoardTitle) {
		this.voteBoardTitle = voteBoardTitle;
	}
	
	public String getVoteBoardImageURL() {
		return voteBoardImageURL;
	}
	
	public void setVoteBoardImageURL(String voteBoardImageURL) {
		this.voteBoardImageURL = voteBoardImageURL;
	}
	
	public EnumVoteBoardState getVoteBoardState() {
		return voteBoardState;
	}
	
	public void setVoteBoardState(EnumVoteBoardState voteBoardState) {
		this.voteBoardState = voteBoardState;
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
}
