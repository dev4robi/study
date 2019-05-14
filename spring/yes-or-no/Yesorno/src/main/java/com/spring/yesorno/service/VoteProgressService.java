package com.spring.yesorno.service;

import java.util.ArrayList;
import java.util.Date;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;

import com.spring.yesorno.dao.VoteProgressDao;
import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.dto.VoteProgressDto;
import com.spring.yesorno.dto.VoteResultDto;
import com.spring.yesorno.util.JwtMemberAuth;

public class VoteProgressService {

	@Autowired private MemberService memberService;
	@Autowired private VoteProgressDao voteProgressDao;
	
	@Autowired private JwtMemberAuth jwtMemberAuth;
	
	private Log log = LogFactory.getLog(VoteProgressService.class);
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////	
	
	private int calcVoteAgreePercent(int voteAgreeCnt, int voteDisagreeCnt) {
		return (int)(voteAgreeCnt * 100.0f / (voteAgreeCnt + voteDisagreeCnt));
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	public boolean vote(String memberToken, int boardId, String agreeOrDisagree, VoteResultDto result) {
		boolean svcResult = false;
		boolean isAgree = agreeOrDisagree.equals("agree");
		
		try {
			// ȸ�� ��ū �˻�
			MemberDto voteMember = null;
			if ((voteMember = jwtMemberAuth.getMemberFromToken(memberToken)) == null) {
				result.setVoteResult("needLogin");
				return false;
			}
			
			// ȸ�� ��� �˻�
			int voteMemberId = voteMember.getMemberId();
			if (!memberService.checkMemberGrade(voteMemberId, MemberDto.EnumMemberGradeId.MG_NORMAL)) { // ȸ�� ��� �̴�
				result.setVoteResult("memberGradeError");
				return false;
			}
			
			// �ߺ���ǥ �˻� �� ��ǥ ���ݰ��� Ȯ��
			ArrayList<Integer> voteBoardIdList = new ArrayList<Integer>();
			voteBoardIdList.add(boardId);
			ArrayList<VoteProgressDto> voteProgressList = voteProgressDao.selectVoteProgressList(voteBoardIdList);			

			boolean voteDuplicated = false;
			int voteAgreeCnt = 0, voteDisagreeCnt = 0;
			for (VoteProgressDto voteProgressDto : voteProgressList) {
				if (voteMemberId == voteProgressDto.getVoterMemberId()) {
					voteDuplicated = true;
				}
				
				int voteResult = voteProgressDto.getVoteResult();
				if (voteResult > 0) {
					++voteAgreeCnt;
				} else if (voteResult < 0) {
					++voteDisagreeCnt;
				}
			}

			if (voteDuplicated) { // �ߺ���ǥ �ź�
				result.setVoteResult("alreadyVote");
				return false;
			}
			
			// ��ǥ��� ����
			VoteProgressDto voteProgressDto = new VoteProgressDto();
			voteProgressDto.setVoteProgressId(0);
			voteProgressDto.setVoteBoardId(boardId);
			voteProgressDto.setVoteResult((isAgree ? 1 : -1));
			voteProgressDto.setVoterMemberId(voteMember.getMemberId());
			voteProgressDto.setVoteDate(new Date());
			
			if (voteProgressDao.insertVoteProgress(voteProgressDto) == 1) {
				result.setVoteAgreeCnt(isAgree ? ++voteAgreeCnt : voteAgreeCnt);
				result.setVoteDisagreeCnt(isAgree ? voteDisagreeCnt : ++voteDisagreeCnt);
				result.setVoteAgreePercent(calcVoteAgreePercent(voteAgreeCnt, voteDisagreeCnt));
				svcResult = true;
			}
			else {
				svcResult = false;
			}

		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		} 

		return svcResult;
	}
	
	// �ش� �Խñ��� ��ǥ ��� ��ȯ
	public boolean getVoteResult(int boardId, VoteResultDto result) {
		boolean svcResult = false;
		ArrayList<Integer> voteBoardIdList = new ArrayList<Integer>();
		voteBoardIdList.add(boardId);
		
		try {
			ArrayList<VoteProgressDto> voteProgressList = voteProgressDao.selectVoteProgressList(voteBoardIdList);
			
			// ��ǥ ���� ����
			int voteAgreeCnt = 0, voteDisagreeCnt = 0;		
			for (VoteProgressDto voteProgressDto : voteProgressList) {
				int voteResult = voteProgressDto.getVoteResult();
				
				if (voteResult > 0) {
					++voteAgreeCnt;
				} else if (voteResult < 0) {
					++voteDisagreeCnt;
				}
			}
			
			// ��� ��� ��ȯ
			result.setVoteAgreeCnt(voteAgreeCnt);
			result.setVoteDisagreeCnt(voteDisagreeCnt);
			result.setVoteAgreePercent(calcVoteAgreePercent(voteAgreeCnt, voteDisagreeCnt));
			svcResult = true;
			
		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		}

		return svcResult;
	}

	//////////	//////////	//////////	//////////	//////////	//////////	//////////
}
