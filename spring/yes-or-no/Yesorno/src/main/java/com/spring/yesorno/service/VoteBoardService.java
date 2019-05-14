package com.spring.yesorno.service;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.ui.Model;
import org.springframework.validation.Errors;
import org.springframework.web.multipart.MultipartFile;

import com.spring.yesorno.command.VoteBoardWriteCmd;
import com.spring.yesorno.dao.MemberDao;
import com.spring.yesorno.dao.VoteBoardDao;
import com.spring.yesorno.dao.VoteProgressDao;
import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.dto.VoteBoardDto;
import com.spring.yesorno.dto.VoteBoardListDto;
import com.spring.yesorno.dto.VoteBoardReadDto;
import com.spring.yesorno.dto.VoteProgressDto;
import com.spring.yesorno.exception.MemberException;
import com.spring.yesorno.util.JwtMemberAuth;

public class VoteBoardService {

	final String ImageRepoURL = "/resources/images";
	final String DefaultImageURL = "/resources/images/defaultImg.png";	// �⺻ ǥ�� �̹��� ���
	final int BoardPerPage;												// �� �������� ������ �Խñ� ����
	final long VoteBoardImminentMillisecond = 1000 * 60 * 60 * (24);	// �Խñ� �ӹ� ����: 24�ð� ��
	final float VoteBoardImminentVoterPercent = 0.95f;					// �Խñ� �ӹ� ����: 95% ��ǥ

	private Log log = LogFactory.getLog(VoteBoardService.class);

	@Autowired private MemberDao memberDao;
	@Autowired private VoteBoardDao voteBoardDao;
	@Autowired private VoteProgressDao voteProgressDao;
	@Autowired private JwtMemberAuth jwtMemberAuth;
	
	// ������
	public VoteBoardService(int dataPerPage) {
		BoardPerPage = dataPerPage;
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	// ��ǥ �Խñ�, ��ǥ �����Ȳ�� �����۾��� �������� �����ϴ� �Լ�
	private ArrayList<VoteBoardListDto> ConcatenateVoteBoardListDto(HttpServletRequest request, ArrayList<VoteBoardDto> voteBoardDtoList) {
		ArrayList<VoteBoardListDto> rtVoteBoardList = new ArrayList<VoteBoardListDto>(); // ��ȯ���� ���� �迭
		ArrayList<Integer> boardIdList = new ArrayList<Integer>(); // �Ű����� ��ǥ �Խñ۵��� ID�� ��� �迭

 		for (VoteBoardDto voteBoard : voteBoardDtoList) {
 			int boardId = voteBoard.getVoteBoardId();
 			if (!boardIdList.contains(boardId)) {
 				boardIdList.add(boardId);
 			}
 		}

 		ArrayList<VoteProgressDto> voteProgressList = null;	// ������ �Խñ۵��� ��ǥ ����Ʈ
 		voteProgressList = voteProgressDao.selectVoteProgressList(boardIdList);

 		// �Խñ� ����Ʈ DTO ����
 		for (VoteBoardDto voteBoard : voteBoardDtoList) {
 			VoteBoardListDto boardDto = new VoteBoardListDto();	// Ư�� �Խñ� DTO, voteBoardDtoList�� �߰��Ͽ� ��ȯ
 			
 			// �Խñ� �⺻�׸� �ʱ�ȭ
 			boardDto.setVoteBoardTitle(voteBoard.getVoteBoardTitle());
 			String voteBoardImgURL = voteBoard.getVoteBoardImageURL();
 			String realPath = request.getSession().getServletContext().getRealPath("");
 			if (voteBoardImgURL == null || voteBoardImgURL.length() == 0) {
 				voteBoardImgURL = (realPath + DefaultImageURL);
 			} else {
 				voteBoardImgURL = (realPath + voteBoardImgURL);
 				// ���� ���� Ȯ��
 				File imgFile = new File(voteBoardImgURL);
 				if (!imgFile.exists()) {
 					voteBoardImgURL = (realPath + DefaultImageURL);
 				}
 			}

 			boardDto.setVoteBoardId(voteBoard.getVoteBoardId());
 			boardDto.setVoteBoardImageURL(voteBoardImgURL);
			boardDto.setVoteAgreeCnt(0);
			boardDto.setVoteDisagreeCnt(0);
			boardDto.setVoteBoardState(VoteBoardListDto.EnumVoteBoardState.BS_PROGRESSING);

 			// �Խñ� ��ǥ ��� ����
				int totalVoteCnt = 0;

 			if (voteProgressList != null) {
 				int agreeCnt = 0, disagreeCnt = 0;
 				
 				for (VoteProgressDto voteProgress : voteProgressList) {
 					if (voteBoard.getVoteBoardId() == voteProgress.getVoteBoardId() ) {
	 					if (voteProgress.getVoteResult() > 0) {
	 						++agreeCnt;
	 					} else if (voteProgress.getVoteResult() < 0) {
	 						++disagreeCnt;
	 					} else {
	 						++totalVoteCnt;
	 					}
 					}
	 			}
 				// ��ǥ ��� ����
 				boardDto.setVoteAgreeCnt(agreeCnt);
 				boardDto.setVoteDisagreeCnt(disagreeCnt);
 				totalVoteCnt = agreeCnt + disagreeCnt;
 			}
 			
 			// �Խñ� ���� ����
 			final long endDate = voteBoard.getVoteEndDate().getTime();
 			final long today = new Date().getTime();
 			final long timeLeft = endDate - today;
 			final int voteLimit = voteBoard.getVoteEndCnt();
 			final int voteImmerseCnt = (int)(VoteBoardImminentVoterPercent * voteLimit);

 			if (timeLeft <= 0 || totalVoteCnt >= voteLimit) { // ��ǥ ����
 				boardDto.setVoteBoardState(VoteBoardListDto.EnumVoteBoardState.BS_ENDED);
 			} else if (timeLeft <= VoteBoardImminentMillisecond || totalVoteCnt >= voteImmerseCnt) { // ���� �ӹ� 
 				boardDto.setVoteBoardState(VoteBoardListDto.EnumVoteBoardState.BS_IMMINENT);
 			} else { // ������
 				boardDto.setVoteBoardState(VoteBoardListDto.EnumVoteBoardState.BS_PROGRESSING);
 			}
 			
 			rtVoteBoardList.add(boardDto);
 		}
		
		return rtVoteBoardList;
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	// ��ǥ �Խñ� ������
	public boolean voteBoardListing(HttpServletRequest request, int page, Model model) {
		boolean svcResult = true;
		ArrayList<VoteBoardListDto> voteBoardDtoList = null;
		
		try {
			// �Խñ� ����Ʈ ��������
			ArrayList<VoteBoardDto> voteBoardList = null;
			int begin = (page - 1) * BoardPerPage; // ���� �Խñ� ��ġ
	
	 		if ((voteBoardList = voteBoardDao.selectVoteBoardList(begin, BoardPerPage)) == null) {
	 			return false;
	 		}

	 		// ��ǥ ����� ��ǥ �Խñ� ���� ����
	 		voteBoardDtoList = ConcatenateVoteBoardListDto(request, voteBoardList);	 		
		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		}

		model.addAttribute("voteBoardList", voteBoardDtoList);

		return svcResult;
	}

	// ��ǥ �Խñ� ����
	public boolean voteBoardWrite(String memberToken, HttpServletRequest request, VoteBoardWriteCmd cmd, Errors errors) {
		boolean svcResult = false;
		MemberDto writeMemberDto = null;
		VoteBoardDto writeVoteBoardDto = new VoteBoardDto();
		
		try {
			// ȸ����ū ����
			if ((writeMemberDto = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}

			// �Խñ� �߰�
			MultipartFile thumbnailFile = cmd.getVoteBoardThumbnailImage();
			Date curDate = new Date();
			Calendar calendar = new GregorianCalendar();
			calendar.setTime(curDate);
			String todayFolder = null;
			String imgName = null;
			String imageURLforDB = DefaultImageURL;
			String imageURLforFolder = DefaultImageURL;
			
			if (thumbnailFile != null && !thumbnailFile.isEmpty()) {
				String realPath = request.getSession().getServletContext().getRealPath("");
				todayFolder = String.format("%s/%d/%d/%d", ImageRepoURL, calendar.get(Calendar.YEAR), (calendar.get(Calendar.MONTH) + 1), calendar.get(Calendar.DATE));
				imgName = String.format("/%d_%d_%s", curDate.getTime(), writeMemberDto.getMemberId(), cmd.getVoteBoardThumbnailImage().getOriginalFilename());
				imageURLforDB = (todayFolder + imgName);
				imageURLforFolder = (realPath + todayFolder);
			}

			writeVoteBoardDto.setVoteBoardId(0);
			writeVoteBoardDto.setWriterMemberId(writeMemberDto.getMemberId());
			writeVoteBoardDto.setVoteBoardWrittenDate(curDate);
			writeVoteBoardDto.setVoteBoardViewCnt(0);
			writeVoteBoardDto.setVoteBoardTitle(cmd.getVoteBoardTitle());
			writeVoteBoardDto.setVoteBoardContent(cmd.getVoteBoardContent());
			writeVoteBoardDto.setVoteBoardImageURL(imageURLforDB);
			writeVoteBoardDto.setVoteEndCnt(cmd.getVoteEndCnt());
			//writeVoteBoardDto.setVoteEndDate(cmd.getVoteEndDate());
			writeVoteBoardDto.setVoteEndDate(curDate); // test
			
			if (voteBoardDao.insertVoteBoard(writeVoteBoardDto) != 1) {
				return false; // �Խñ� �߰� ����
			} else {
				svcResult = true;
			}
			
			// �̹������� ����
			if (thumbnailFile != null && !thumbnailFile.isEmpty()) {
				File imgFolder = new File(imageURLforFolder);
				imgFolder.mkdirs();
				File imgFile = new File(imageURLforFolder + imgName);
				thumbnailFile.transferTo(imgFile);
				svcResult = true;
			}
		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		} catch (IOException ioe) {
			log.debug(ioe.getMessage());
			svcResult = false;			
		}
		
		return svcResult;
	}
	
	// Ư�� �Խñ� ��ȸ
	public boolean voteBoardReading(String memberToken, HttpServletRequest request, int boardId, Model model) {
		boolean svcResult = false;
		VoteBoardReadDto voteBoardReadDto = null;
		VoteBoardDto voteBoardDto = null;
		MemberDto readMemberDto = null;
		int readMemberId = 0;
		
		try {
			// ȸ����ū ���� - �Խñ� �д»�� IDȮ��
			if ((readMemberDto = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {
				readMemberId = readMemberDto.getMemberId();
			}
			
			// �ش� �Խñ� DB��ȸ
			if ((voteBoardDto = voteBoardDao.selectVoteBoard(boardId)) == null) {
				// �Խñ��� ã�� �� ����
				svcResult = false;
				// throws...
			}
			
			// �ش� �Խñ� �ۼ��� �г��� DB��ȸ 
			int writerMemberId = voteBoardDto.getWriterMemberId();
			String writerMemberNickname = null;
			if ((writerMemberNickname = memberDao.selectMemberNickname(writerMemberId)) == null) {
				writerMemberNickname = ("UnknownMemberId:" + writerMemberId);
			}
			
			// �Խñ۰� ��ǥ ��� �����۾� ����
			ArrayList<VoteBoardDto> voteBoardDtoList = new ArrayList<VoteBoardDto>();
			voteBoardDtoList.add(voteBoardDto);
			voteBoardReadDto = new VoteBoardReadDto(ConcatenateVoteBoardListDto(request, voteBoardDtoList).get(0));

			// �Խñ� Dtoä���ֱ�
			voteBoardReadDto.setReaderIsWriter(readMemberId == voteBoardDto.getWriterMemberId()); // (�Խñ� �д� ��� == �Խñ� �ۼ���) Ȯ��
			voteBoardReadDto.setWriterMemberNickname(writerMemberNickname);
			voteBoardReadDto.setVoteBoardWrittenDate(voteBoardDto.getVoteBoardWrittenDate());
			voteBoardReadDto.setVoteBoardViewCnt(voteBoardDto.getVoteBoardViewCnt());
			voteBoardReadDto.setVoteBoardContent(voteBoardDto.getVoteBoardContent());
			voteBoardReadDto.setVoteEndDate(voteBoardDto.getVoteEndDate());
			voteBoardReadDto.setVoteEndCnt(voteBoardDto.getVoteEndCnt());
			voteBoardReadDto.setVoteAgreePercent((int)(voteBoardReadDto.getVoteAgreeCnt() * 100.0f / (voteBoardReadDto.getVoteAgreeCnt() + voteBoardReadDto.getVoteDisagreeCnt())));

			model.addAttribute("voteBoardRead", voteBoardReadDto);
			svcResult = true;
		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		}
		
		return svcResult; 
	}

	// ��ǥ �Խñ� ����
	public boolean voteBoardDelete(String memberToken, int boardId) {
		boolean svcResult = false;
		VoteBoardDto voteBoardDto = null;
		MemberDto readMemberDto = null;
		int readMemberId = 0;
		
		try {
			// ȸ����ū ���� - �Խñ� �д»�� IDȮ��
			if ((readMemberDto = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {
				readMemberId = readMemberDto.getMemberId();
			} else {
				return false; // ȸ����ū ���� ����
			}
			
			// �ش� �Խñ� DB��ȸ
			if ((voteBoardDto = voteBoardDao.selectVoteBoard(boardId)) == null) {
				return false; // �Խñ��� ã�� �� ����
			}
			
			// �ۼ��� ���ο��� Ȯ��
			if (readMemberId != voteBoardDto.getWriterMemberId()) {
				return false; // �ۼ��� ������ �ƴ�
			}
			
			// �Խñ� ����
			if (voteBoardDao.deleteVoteBoard(boardId) != 1) {
				return false; // �Խñ� ���� ����
			} else {
				svcResult = true; // �Խñ� ���� ����
			}
			
		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		}
		
		return svcResult;
	}

	// ��ǥ �Խñ� ���� ����
	public boolean voteBoardModifyContent(String memberToken, int boardId, String modifiedContent) {
		boolean svcResult = false;
		VoteBoardDto voteBoardDto = null;
		MemberDto readMemberDto = null;
		int readMemberId = 0;

		try {
			// ���� ���� �˻�
			if (modifiedContent == null) {
				return false; // ���� ����
			}
			
			int contentLength = modifiedContent.length(); 
			if (contentLength < 2 || contentLength > 256) {
				return false; // ������ �ʹ� ��
			}

			// ȸ����ū ���� - �Խñ� �д»�� IDȮ��
			if ((readMemberDto = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {
				readMemberId = readMemberDto.getMemberId();
			} else {
				return false; // ȸ����ū ���� ����
			}
			
			// �ش� �Խñ� DB��ȸ
			if ((voteBoardDto = voteBoardDao.selectVoteBoard(boardId)) == null) {
				return false; // �Խñ��� ã�� �� ����
			}
			
			// �ۼ��� ���ο��� Ȯ��
			if (readMemberId != voteBoardDto.getWriterMemberId()) {
				return false; // �ۼ��� ������ �ƴ�
			}
						
			// �Խñ� ���� ����
			if (voteBoardDao.updateVoteBoardContent(boardId, modifiedContent) != 1) {
				return false; // �Խñ� ���� ���� ����
			} else {
				svcResult = true; // �Խñ� ������� ����
			}

		} catch (DataAccessException e) {
			log.debug(e.getMessage());
			svcResult = false;
		}
		
		return svcResult;
	}
}
