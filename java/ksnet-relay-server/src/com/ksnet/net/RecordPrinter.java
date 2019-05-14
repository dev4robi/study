package com.ksnet.net;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import com.ksnet.util.*;

public class RecordPrinter {
	
	private Record record;
	
	// ������
	public RecordPrinter(Record record) {
		this.record = record;
	}
	
	public void setRecord(Record record) {
		this.record = record;
	}
	
	// ��ĺ� ����Լ�
	// �ϳ�����_������ü_ǥ����
	private void print_hana_record() {
		int attrSize = record.getAttrMap().size();
		Attribute[] attrAry = new Attribute[attrSize];
		
		for (Map.Entry<String, Attribute> entry : record.getAttrMap().entrySet()) {
			Attribute attr = entry.getValue();
			int number = attr.getNumber();
			
			attrAry[number - 1] = attr;
		}
	
		System.out.println(String.format("%s\t%-40s\t%s\t%s\t%-20s", "����", "�̸�(�ڵ��)", "�����ε���", "����Ʈ����", "���簪"));
		for (Attribute attr : attrAry) {
			System.out.println(String.format("%3d\t%-40s\t%4d\t\t%4d\t\t%-20s", attr.getNumber(), (attr.getName() + String.format("(%s)", attr.getCodeName())), attr.getBeginIndex(), attr.getByteLength(), new String(attr.getValue())));
		}		
	}

	public void print(Record record) {
		setRecord(record);
		print();
	}
	
	public void print() {
		String recordType = record.getTypeName();
		
		// �ϳ�����_������ü_ǥ����_�����ͺ�_����� ��� ���
		if (recordType.equals("HanaRecordClient")) {
			String recordSubType = record.getSubTypeName();
			
			if (recordSubType.equals("Head")) {
				System.out.println("=[ǥ����]=================================================================================");
				print_hana_record();
				System.out.println("=================================================================================[ǥ����]=");
			}
			else if (recordSubType.equals("Data")) {
				System.out.println("=[�����ͺ�]================================================================================");
				print_hana_record();
				System.out.println("================================================================================[�����ͺ�]=");
			}
			else if (recordSubType.equals("Tail")) {
				System.out.println("=[�����]=================================================================================");
				print_hana_record();
				System.out.println("=================================================================================[�����]=");
			}			
		}
		// ��ȭ �߹�ŷ ��� ���
		else if (recordType.equals("HanaRecordServer")) {
			print_hana_record();
		}
		else if (false) {
			// ���⿡ ���ο� ���Ÿ�� �߰�...
		}
		else {
			System.out.println(this.getClass().getName());
			Logger.logln(Logger.LogType.LT_ERR, "�������� �ʴ� ���� ����. (" + recordType + ")");
			return;
		}
	}
}