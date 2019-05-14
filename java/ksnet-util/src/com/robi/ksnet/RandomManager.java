package com.robi.ksnet;

import java.util.Random;

public class RandomManager {
	
	private static final Random RANDOM				= new Random();	// ���� �õ尪 ������ ���� ���� 1ȸ�� ����
	private static final String RANDOM_ALPHA		= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	private static final String RANDOM_NUMBER		= "0123456789";
	private static final String RANDOM_ALPHA_NUMBER = RANDOM_ALPHA + RANDOM_NUMBER;
	
	// �ɼǿ� ����(����, ����, ���� + ����) len������ ������ ���ڿ��� ��ȯ
	public static String genRandomStr(int len, String option) {
		StringBuilder sb = new StringBuilder();
		String op = null;
		
		if (option.equals("ALPHA")) {
			op = RANDOM_ALPHA;
		}
		else if (option.equals("NUMBER")) {
			op = RANDOM_NUMBER;
		}
		else if (option.equals("ALPHA_NUMBER")) {
			op = RANDOM_ALPHA_NUMBER;
		}

		// [ https://docs.oracle.com/javase/8/docs/api/index.html?java/util/Random.html ]
		// java.util.Random are thread safe. However, the concurrent use of the same java.util.Random instance
		// across threads may encounter contention and consequent poor performance.
		// -> Random Ŭ������ �⺻������ Thread-Safe����, ���� �����忡�� ���� ���� ���� ȿ������ ���� �� ����.
		
		int opLen = op.length();
		
		for (int i = 0; i < len; ++i) {
			sb.append(op.charAt(RANDOM.nextInt(opLen)));
		}
		
		return sb.toString();
	}
}