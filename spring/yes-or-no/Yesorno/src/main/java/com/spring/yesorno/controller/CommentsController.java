package com.spring.yesorno.controller;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.CookieValue;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.spring.yesorno.service.CommentService;
import com.spring.yesorno.util.JsonParser;

@Controller
public class CommentsController {

	@Autowired private JsonParser jsonParser;
	@Autowired private CommentService commentService;
	
	Log log = LogFactory.getLog(CommentsController.class);
	
	// ��� �ۼ�
	// [C]POST: boards/voteboards/{boardId}/comments
	@RequestMapping(value = "/boards/voteboards/{boardId}/comments", method = RequestMethod.POST)
	public @ResponseBody String writeComment(@CookieValue(value = "memberToken", required = false) String memberToken, @PathVariable("boardId") int boardId, @RequestBody String jsonData) {
		final String resultOk = "success";
		final String resultErr = "error";
		String resultString = resultErr;

		try {
			String commentContent = (String)jsonParser.mapFromJsonString(jsonData).get("comment");	
			if (commentService.writeComment(memberToken, boardId, 0, 0, commentContent)) {
				resultString = resultOk;
				return "{\"result\":\"success\",\"comment\":\"" + commentContent + "\"}"; // oft ���� �۵��ϴ°��� Ȯ��. �̷������� ��ȯ����...
			} else {
				resultString = resultErr;
			}

		} catch (Exception e) {
			log.debug(e.getMessage());
		}
	
		return resultString;
	}
	
	// ��� ��ȸ
	@RequestMapping(value = "/boards/voteboards/{boardId}/comments", method = RequestMethod.GET)
	public @ResponseBody String selectComment(@PathVariable("boardId") int boardId) {
		return ""; // ���� oft Ȯ�� �� ������� ����
	}
}
