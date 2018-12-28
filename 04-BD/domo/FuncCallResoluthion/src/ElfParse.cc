/*
 * ElfParse.cc
 *
 *  Created on: 2018年12月26日
 *      Author: LuKang
 */
#include "ElfParse.h"

/**
 * 函数名： elfParseInit
 * 函数参数：filePath 文件路径
 * 函数功能：将elf格式的二进制文件，通过交叉编译工具链中的工具，
 * 		    生成反汇编和符号表文本，将文本保存到文件中。反汇编文件后缀为（.dump），符号表文件后缀为（.list）。
 * 返回值：
 * 		成功返回true。
 * 		失败返回false。
 */
bool elfParseInit(string filePath)
{
	if(filePath.empty())
	{
		return false;
	}
	int ret = 0;
	char path[50] = {0};
	char fileName[20] = {0};
	string cmdObjdump = "arm-linux-gnueabihf-objdump -d ";
	string cmdReadelf = "arm-linux-gnueabihf-readelf -s ";
	_splitpath(filePath.data(),NULL,path,fileName,NULL);
	//cmd命令拼接
	cmdObjdump.append(filePath + " > " + path + fileName + ".dump");
	cmdReadelf.append(filePath + " > " + path + fileName + ".list");
	ret = system(cmdObjdump.data());
	if(-1 == ret)
	{
		cout<<"cmdObjdump error"<<endl;
		return false;
	}
	ret = system(cmdReadelf.data());
	if(-1 == ret)
	{
		cout<<"cmdReadelf error"<<endl;
		return false;
	}
	return true;
}

/**
 * 函数名： symbolTableStringAnalysis
 * 函数参数：oneLine 符号表中的一行字符
 * 函数功能：解析符号表一行字符中的信息
 * 返回值：
 * 		成功返回0。
 * 		失败返回-1。
 * 一行字符的格式如下:
 * 10: 00000009     0 FUNC    LOCAL  DEFAULT    1 start
 */
vector<struct funcInfo>funclist; //保存读到的函数信息的容器
int symbolTableStringAnalysis(string oneline)
{
	char funcline[255] = {0};
	char funcname[60];
	struct funcInfo * func = new funcInfo;
	strncpy(funcline,oneline.data(),sizeof(funcline));
	//跳过字符串中间的无用字符
	sscanf(funcline,"%*s %08x %*[^ ] %*[^ ] %*[^ ] "
			"%*[^ ] %*[^ ] %s",&(func->address),funcname);
	func->name=funcname;
	//因为符号表中函数地址信息与反汇编文件中函数地址相差一
	//这里将地址减一存储，方便后面进行解析
	func->address = func->address - 1;
	//cout<<func->address<<" "<<func->name<<endl;
	funclist.push_back(*func);
	return 0;
}
/**
 * 函数名： symbolTableAnalysis
 * 函数参数：listFilePath 符号表文件路径
 * 函数功能：解析符号表文件信息
 * 返回值：
 * 		成功返回0。
 * 		失败返回-1。
 */
int symbolTableAnalysis(string listFilePath)
{
	if(listFilePath.empty())
	{
		return -1;
	}
	int ret = 0;
	string::size_type position = 0; //find函数的返回标志
	char data[255]; //保存每次读到的字符串
	string funcStr; //保存读到的函数是函数的行
	ifstream symbolTableFile;
	symbolTableFile.open(listFilePath,ios::out);
	if(symbolTableFile.bad())
	{
		return -1;
	}
	//读取文件，跳过前三行
	for(int i = 0;i<3;i++)
	{
		symbolTableFile.getline(data,sizeof(data));
	}
	while(true)
	{
		//获取文本中的一行
		symbolTableFile.getline(data,sizeof(data));
		//cout<<data<<endl;
		if(0 == strlen(data))
		{
			break;
		}
		funcStr = data;
		memset(data,0,sizeof(data));
		//在读到的一行中查找FUNC字符串如果找到就进行解析
		position = funcStr.find("FUNC");
		if(position != funcStr.npos)
		{
			ret = symbolTableStringAnalysis(funcStr);
			if(-1 == ret)
			{
				cout<<"Symbol table parsing error"<<endl;
				return -1;
			}
		}
	}
	symbolTableFile.close();
	return 0;
}

/**
 * 函数名： jumpDetection
 * 函数参数：filePath 所要解析文件流
 * 函数功能：在文件中找出函数调用语句并解析
 * 返回值：
 * 		成功返回0。
 * 		失败返回-1。
 */
int jumpDetection(ifstream& file)
{
	if(file.bad())
	{
		cout<<"fail to open the file"<<endl;
		return -1;
	}
	return 0;
}

//对容器中的函数地址进行比较，是sort函数的比较规则
bool cmpAddr(const struct funcInfo &a, const struct funcInfo &b)
{
	return a.address < b.address;
}

/**
 * 函数名： lineNumResolu
 * 函数参数：file 所要解析的文件流
 * 函数功能：在文件中找出函数所在的行将行号信息保存到容器中
 * 返回值：
 * 		成功返回0。
 * 		失败返回-1。
 */
int lineNumResolu(ifstream& file)
{
	if(file.bad())
	{
		return -1;
	}

	int  i = 0;
	int  count = 0;
	//计数器记录每个函数的行号
	char data[255] = {0};
	//标记是否找到函数
	char funcLine[100] = {0};
	string::size_type position = 0; //find函数的返回标志
	//对容器中的函数信息按照函数地址进行排序
	sort(funclist.begin(),funclist.end(),cmpAddr);
	while(true)
	{
		file.getline(data,sizeof(data));
		string str(data);
		/*
		 * FIXME :这里为了优化性能，只对函数的地址信息进行匹配可能会出bug需要后期解决
		 * 出bug的原因函数中可能会后立即数或者其他的数与某个函数的地址信息一致
		 */
		sprintf(funcLine,"%08x",funclist[i].address);
		//sprintf(funcLine,"%08x <%s>:",funclist[i].address,funclist[i].name.data());
		position = str.find(funcLine);
		if(position != str.npos)
		{
			//将行号信息保存到容器中
			funclist[i].lineNum = count;
			i++;
		}
		count++;
		if(i == (int)funclist.size())
		{
			break;
		}
	}
#define debug
#if defined(debug)
	char arr1[40];
	sort(funclist.begin(),funclist.end(),cmpAddr);
	for(int i = 0;i < (int)funclist.size();i++)
	{
		sprintf(arr1,"%d %08x <%s>:",funclist[i].lineNum,funclist[i].address,funclist[i].name.data());
		cout<<arr1<<endl;
	}
#endif
	return 0;
}

/**
 * 函数名： disassemblyAnalysis
 * 函数参数：dumpFilePath 反汇编文件路径
 * 函数功能：解析反汇编文件信息
 * 返回值：
 * 		成功返回0。
 * 		失败返回-1。
 */
int disassemblyAnalysis(string dumpFilePath)
{
	if(dumpFilePath.empty())
	{
		return -1;
	}
	int ret = 0;
	char data[255] = {0}; //缓存一行的数据
	string::size_type position = 0; //find函数的返回标志
	ifstream disassemblyFile;
	streampos pos; //记录文件指针的位置
	if(disassemblyFile.bad())
	{
		return -1;
	}
	disassemblyFile.open(dumpFilePath,ios::out);
	//循环读取文件中的内容
	while(true)
	{
		//读取一行
		disassemblyFile.getline(data,sizeof(data));
		if(disassemblyFile.eof())
		{
			break;
		}
		//方便使用find方法
		string flag = data;
		//检测文本段，当检测到文本段就从开始解析
		position = flag.find("section .text:");
		if(position != flag.npos)
		{
			//记录text段开始的位置
			pos = disassemblyFile.tellg();
			break;
		}
	}
	//解析行号
	ret = lineNumResolu(disassemblyFile);
	if(-1 == ret)
	{
		cout<<"Line number resolution failed"<<endl;
		return -1;
	}
	//将文件指针复位到text段开始的位置
	disassemblyFile.seekg(pos);
	//解析函数跳转
	ret = jumpDetection(disassemblyFile);
	if(-1 == ret)
	{
		cout<<"Jump parsing failed"<<endl;
		return -1;
	}
	return 0;
}
