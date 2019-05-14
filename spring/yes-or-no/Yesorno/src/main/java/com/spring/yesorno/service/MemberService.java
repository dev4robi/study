package com.spring.yesorno.service;

import java.util.Date;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.ui.Model;
import org.springframework.validation.Errors;

import com.spring.yesorno.command.MemberInfoChangeCmd;
import com.spring.yesorno.command.MemberLoginCmd;
import com.spring.yesorno.command.MemberRegistrationCmd;
import com.spring.yesorno.dao.MemberDao;
import com.spring.yesorno.dto.MemberDto;
import com.spring.yesorno.exception.MemberException;
import com.spring.yesorno.util.JwtMemberAuth;

public class MemberService {

	@Autowired private MemberDao memberDao;
	@Autowired private JwtMemberAuth jwtMemberAuth;
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	// ȸ����ū ����(��Ű)
	public void generateMemberToken(String memberToken, HttpServletResponse response) {
		Cookie memberCookie = new Cookie("memberToken", memberToken);
		memberCookie.setPath("/");
		memberCookie.setComment("memberToken");
		memberCookie.setMaxAge((int)JwtMemberAuth.TokenExpireMinute);
		response.addCookie(memberCookie);
	}
	
	// ȸ����ū ����(��Ű)
	public void removeMemberToken(HttpServletResponse response) {
		Cookie memberCookie = new Cookie("memberToken", null);
		memberCookie.setPath("/");
		memberCookie.setComment("memberToken");
		memberCookie.setMaxAge(0);
		response.addCookie(memberCookie);
	}
	
	// ȸ�� ���� Ȯ��
	public boolean checkMemberGrade(int memberId, MemberDto.EnumMemberGradeId memberGradeOver) {
		boolean svcResult = false;
		MemberDto checkMember = new MemberDto();
		checkMember.setMemberId(memberId);
		
		if ((checkMember = memberDao.memberSelect(checkMember)) != null) { // ȸ������ ����
			if (memberGradeOver.getValue() <= checkMember.getMemberGradeId()) { // ��û ���� �̻�
				svcResult = true;
			} else { // ��û ���� ����
				svcResult = false;
			}
		} else { // ȸ������ ����
			svcResult = false;
		}
		
		return svcResult;
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
	
	// ȸ�� ����
	public boolean memberRegistration(MemberRegistrationCmd cmd, Errors errors) {
		boolean svcResult = false;

		try {
			MemberDto registerMember = new MemberDto();
			registerMember.setMemberEmail(cmd.getMemberEmail());								// �̸���
			registerMember.setMemberNickname(cmd.getMemberNickname());							// �г���
			registerMember.setMemberGradeId(MemberDto.EnumMemberGradeId.MG_NORMAL.getValue());	// ���
			registerMember.setMemberJoinDate(new Date());										// ������
			svcResult = (memberDao.memberInsert(registerMember) == 1 ? true : false);
		} catch (DataAccessException e) {
			if (e instanceof DuplicateKeyException) { // �̸��� Ȥ�� �г��� �ߺ� 
				String errorMsg = e.getMessage();
				if (errorMsg.contains("for key 'MEMBER_EMAIL'")) { // �̸��� �ߺ�
					errors.rejectValue("memberEmail", "error.alreadyUsed");
				} else { // �г��� �ߺ�
					errors.rejectValue("memberNickname", "error.alreadyUsed");
				}
			}

			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	// ȸ�� Ż��
	public boolean memberDeregistration(String memberToken, HttpServletResponse response) throws MemberException {
		boolean svcResult = false;
		MemberDto deregistrationMember = null;

		try {
			// ȸ����ū ����
			if ((deregistrationMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
			
			// ȸ������ ��ȸ
			if ((deregistrationMember = memberDao.memberSelect(deregistrationMember)) != null) {
				deregistrationMember.setMemberGradeId(MemberDto.EnumMemberGradeId.MG_RETIRED.getValue());
				deregistrationMember.setMemberLastLoginDate(new Date());
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}

			// ȸ��Ż�� ������Ʈ
			if (memberDao.memberDeregistrationUpdate(deregistrationMember) == 1) {
				removeMemberToken(response);
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_UPDATE_FAIL);
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	// ȸ�� �α���
	public boolean memberLogin(MemberLoginCmd cmd, HttpServletResponse response, Errors errors) throws MemberException {
		boolean svcResult = false;

		try {
			MemberDto loginMember = new MemberDto();
			loginMember.setMemberEmail(cmd.getMemberEmail());
		
			// ȸ������ ��ȸ
			if ((loginMember = memberDao.memberSelect(loginMember)) != null ? true : false) {} 
			else {
				svcResult = false;
				errors.rejectValue("memberEmail", "error.login.memberEmail");
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}
			
			// ���� ��� Ȯ��
			if (loginMember.getMemberGradeId() != MemberDto.EnumMemberGradeId.MG_RETIRED.getValue()) {}			
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_BANNED_MEMBER);
			}
			
			if (loginMember.getMemberGradeId() != MemberDto.EnumMemberGradeId.MG_BANNED.getValue()) {
				loginMember.setMemberLastLoginDate(new Date());
			}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_BANNED_MEMBER);
			}

			// �α��� ��� ������Ʈ
			if (memberDao.memberLoginUpdate(loginMember) == 1) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_UPDATE_FAIL);
			}
			
			// �ɹ���ū ����
			String memberToken = null;
			if ((memberToken = jwtMemberAuth.createMemberToken(loginMember)) != null) {
				generateMemberToken(memberToken, response);
				svcResult = true;
			} else {
				errors.rejectValue("memberEmail", "error.token.generate");
				return false;
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	// ȸ�� �α׾ƿ�
	public boolean memberLogout(String memberToken, HttpServletResponse response) {
		boolean svcResult = false;

		// ȸ����ū ����
		if (jwtMemberAuth.authMemberToken(memberToken)) {
			removeMemberToken(response);
			svcResult = true;
		} else {
			svcResult = false;
			throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
		}

		return svcResult;
	}
	
	// ȸ������ ����
	public boolean memberInfoChange(String memberToken, MemberInfoChangeCmd cmd, HttpServletResponse response, Errors errors) {		
		boolean svcResult = false;
		MemberDto infoChangeMember = null;

		try {
			// ȸ����ū ����
			if ((infoChangeMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {
				infoChangeMember.setMemberNickname(cmd.getMemberNickname());
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
				
			// ȸ������ ����
			if (memberDao.memberInfoUpdate(infoChangeMember) == 1) {
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_UPDATE_FAIL);
			}
			
			// �ɹ���ū ����
			memberToken = null;
			if ((memberToken = jwtMemberAuth.createMemberToken(infoChangeMember)) != null) {
				generateMemberToken(memberToken, response);
				svcResult = true;
			} else {
				errors.rejectValue("memberEmail", "error.token.generate");
				return false;
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			if (e instanceof DuplicateKeyException) { // �ߺ� �г���
				errors.rejectValue("memberNickname", "error.alreadyUsed");
			}

			svcResult = false;
		}

		return svcResult;
	}

	//////////	//////////	//////////	//////////	//////////	//////////	//////////

	// ȸ������ ���� ������
	public boolean memberInfoChangePage(String memberToken, MemberInfoChangeCmd cmd, Errors errors) {		
		boolean svcResult = false;
		MemberDto infoChangeMember = null;

		try {
			// ȸ������ ����
			if ((infoChangeMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
			
			// ȸ������ ��ȸ
			if ((infoChangeMember = memberDao.memberSelect(infoChangeMember)) != null) {
				cmd.setMemberEmail(infoChangeMember.getMemberEmail());
				cmd.setMemberNickname(infoChangeMember.getMemberNickname());
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	public boolean memberDeregistrationPage(String memberToken, Model model) {
		boolean svcResult = false;
		MemberDto deregistMember = new MemberDto();
		
		try {
			// ȸ����ū ����
			if ((deregistMember = jwtMemberAuth.getMemberFromToken(memberToken)) != null) {}
			else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_TOKEN_VALIDATION_FAIL);
			}
			
			// ȸ������ ��ȸ
			if ((deregistMember = memberDao.memberSelect(deregistMember)) != null) {
				model.addAttribute("deregistrationMember", deregistMember);
				svcResult = true;
			} else {
				svcResult = false;
				throw new MemberException(MemberException.EnumMemberExceptionType.ET_DB_DATA_NOT_FOUND);
			}
		} catch (DataAccessException e) {
			e.printStackTrace();
			svcResult = false;
		}

		return svcResult;
	}
	
	//////////	//////////	//////////	//////////	//////////	//////////	//////////
}
