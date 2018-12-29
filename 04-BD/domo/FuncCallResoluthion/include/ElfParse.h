/*
 * ElfParse.h
 *
 *  Created on: 2018��12��26��
 *      Author: LuKang
 */

#ifndef INCLUDE_ELFPARSE_H_
#define INCLUDE_ELFPARSE_H_
#include <iostream>
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
using namespace std;


//����һ���ṹ�����ڱ���������ű�õ�����Ϣ
struct funcInfo
{
	int address; //��ַ
	string name; //������
	int lineNum; //��������ڵ�ַ���ļ��е��к�
};

//����һ���ṹ�����ڱ��溯��������Ϣ
struct callInfo
{
	//funcInfo func[];//��¼�����õĺ�����Ϣ
	string name;    //��¼�����ߵ�����
	int callNum;    //��¼���õ��к�
};

/**
 * �������� elfParseInit
 * ����������filePath �ļ�·��
 * �������ܣ���elf��ʽ�Ķ������ļ���ͨ��������빤�����еĹ��ߣ�
 * 		    ���ɷ����ͷ��ű����ı����浽�ļ��С�������ļ���׺Ϊ��.dump�������ű��ļ���׺Ϊ��.list����
 * ����ֵ��
 * 		�ɹ�����true��
 * 		ʧ�ܷ���false��
 */
bool elfParseInit(string);

/**
 * �������� symbolTableAnalysis
 * ����������listFilePath ���ű��ļ�·��
 * �������ܣ��������ű��ļ���Ϣ
 * ����ֵ��
 * 		�ɹ�����0��
 * 		ʧ�ܷ���-1��
 */
int symbolTableAnalysis(string listFilePath);

/**
 * �������� disassemblyAnalysis
 * ����������listFilePath ������ļ�·��
 * �������ܣ�����������ļ���Ϣ
 * ����ֵ��
 * 		�ɹ�����0��
 * 		ʧ�ܷ���-1��
 */
int disassemblyAnalysis(string dumpfilepath);

#endif /* INCLUDE_ELFPARSE_H_ */
