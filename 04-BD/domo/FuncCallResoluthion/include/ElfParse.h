/*
 * ElfParse.h
 *
 *  Created on: 2018年12月26日
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


//定义一个结构，用于保存解析符号表得到的信息
struct funcInfo
{
	int address; //地址
	string name; //函数名
	int lineNum; //函数的入口地址在文件中的行号
};

//定义一个结构，用于保存函数调用信息
struct callInfo
{
	//funcInfo func[];//记录被调用的函数信息
	string name;    //记录调用者的名字
	int callNum;    //记录调用的行号
};

/**
 * 函数名： elfParseInit
 * 函数参数：filePath 文件路径
 * 函数功能：将elf格式的二进制文件，通过交叉编译工具链中的工具，
 * 		    生成反汇编和符号表，将文本保存到文件中。反汇编文件后缀为（.dump），符号表文件后缀为（.list）。
 * 返回值：
 * 		成功返回true。
 * 		失败返回false。
 */
bool elfParseInit(string);

/**
 * 函数名： symbolTableAnalysis
 * 函数参数：listFilePath 符号表文件路径
 * 函数功能：解析符号表文件信息
 * 返回值：
 * 		成功返回0。
 * 		失败返回-1。
 */
int symbolTableAnalysis(string listFilePath);

/**
 * 函数名： disassemblyAnalysis
 * 函数参数：listFilePath 反汇编文件路径
 * 函数功能：解析反汇编文件信息
 * 返回值：
 * 		成功返回0。
 * 		失败返回-1。
 */
int disassemblyAnalysis(string dumpfilepath);

#endif /* INCLUDE_ELFPARSE_H_ */
