package com.spring.yesorno.validator;

import org.springframework.validation.Errors;
import org.springframework.validation.ValidationUtils;
import org.springframework.validation.Validator;
import org.springframework.web.multipart.MultipartFile;

import com.spring.yesorno.command.VoteBoardWriteCmd;

public class VoteBoardWriteValidator implements Validator {

	@Override
	public boolean supports(Class<?> clazz) {
		return VoteBoardWriteValidator.class.isAssignableFrom(clazz);
	}

	@Override
	public void validate(Object target, Errors errors) {
		final Integer[] TitleLength = {2, 64};
		final Integer[] ContentLength = {2, 256};
		final Integer[] FileNameLength = {5, 128};
		final String[] ImgFileExt = {"jpg", "jpeg", "png", "bmp"};
		final long ThumbnailSizeLimit = 1048576; // 1MB
		VoteBoardWriteCmd cmd = (VoteBoardWriteCmd)target;
		final String boardTitle = cmd.getVoteBoardTitle();
		final String boardContent = cmd.getVoteBoardContent();
		

		// Ÿ��Ʋ �� üũ
		int errorSizeBefore = errors.getAllErrors().size();
		ValidationUtils.rejectIfEmptyOrWhitespace(errors, "voteBoardTitle", "error.required");
		int errorSizeAfter = errors.getAllErrors().size();
		if (errorSizeBefore != errorSizeAfter) {}
		// Ÿ��Ʋ ���� (2~64)
		else if (boardTitle.length() < TitleLength[0] || boardTitle.length() > TitleLength[1]) {
			errors.rejectValue("voteBoardTitle", "error.length", new String[]{TitleLength[0].toString(), TitleLength[1].toString()}, null);
		}
		// Ÿ��Ʋ ���
		else {}
		
		// ������ �� üũ
		errorSizeBefore = errors.getAllErrors().size();
		ValidationUtils.rejectIfEmptyOrWhitespace(errors, "voteBoardContent", "error.required");
		errorSizeAfter = errors.getAllErrors().size();
		if (errorSizeBefore != errorSizeAfter) {}
		// ������ ���� (2~256)
		else if (boardContent.length() < ContentLength[0] || boardContent.length() > ContentLength[1]) {
			errors.rejectValue("voteBoardTitle", "error.length", new String[]{ContentLength[0].toString(), ContentLength[1].toString()}, null);
		}
		// ������ ���
		else {}
		
		// �̹��� üũ
		MultipartFile img = cmd.getVoteBoardThumbnailImage();
		if (img != null && img.getSize() > 0) {
			String fileName = img.getOriginalFilename();
			int fileNameLength = fileName.length();

			// ���ϸ� ���� üũ
			if (fileNameLength < FileNameLength[0] || fileNameLength > FileNameLength[1]) {
				errors.rejectValue("voteBoardThumbnailImage", "fileNameLength.length",
								   new String[]{FileNameLength[0].toString(), FileNameLength[1].toString()}, null);
			}
			
			// ���� Ȯ���� üũ
			final String fileExt = fileName.substring(fileName.lastIndexOf(".") + 1, fileNameLength);
			boolean extResult = false; 
			
			for (String ext : ImgFileExt) {
				if (fileExt.equals(ext)) {
					extResult = true;
					break;
				}
			}
			
			if (!extResult) {
				errors.rejectValue("voteBoardThumbnailImage", "error.fileExtNotSupported");
			}

			// ����  ũ�� üũ
			if (img.getSize() > ThumbnailSizeLimit) {
				errors.rejectValue("voteBoardThumbnailImage", "error.fileSizeOver");
			}
		}
		
		// ���� �ο�, ���� üũ
		// ...
	}
}
