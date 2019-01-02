/*
 * ElfParse.cc
 *
 *  Created on: 2018��12��26��
 *      Author: LuKang
 */
#include "ElfParse.h"

/**
 * �������� elfParseInit
 * ����������filePath �ļ�·��
 * �������ܣ���elf��ʽ�Ķ������ļ���ͨ��������빤�����еĹ��ߣ�
 * 		    ���ɷ����ͷ��ű��ı������ı����浽�ļ��С�������ļ���׺Ϊ��.dump�������ű��ļ���׺Ϊ��.list����
 * ����ֵ��
 * 		�ɹ�����true��
 * 		ʧ�ܷ���false��
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
	//cmd����ƴ��
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
 * �������� symbolTableStringAnalysis
 * ����������oneLine ���ű��е�һ���ַ�
 * �������ܣ��������ű�һ���ַ��е���Ϣ
 * ����ֵ��
 * 		�ɹ�����0��
 * 		ʧ�ܷ���-1��
 * һ���ַ��ĸ�ʽ����:
 * 10: 00000009     0 FUNC    LOCAL  DEFAULT    1 start
 */
vector<funcInfo>funclist; //��������ĺ�����Ϣ������
int symbolTableStringAnalysis(string oneline)
{
	char funcline[255] = {0};
	char funcname[60];
	funcInfo * func = new funcInfo;
	strncpy(funcline,oneline.data(),sizeof(funcline));
	//�����ַ����м�������ַ�
	sscanf(funcline,"%*s %08x %*[^ ] %*[^ ] %*[^ ] "
			"%*[^ ] %*[^ ] %s",&(func->address),funcname);
	func->name=funcname;
	//��Ϊ���ű��к�����ַ��Ϣ�뷴����ļ��к�����ַ���һ
	//���ｫ��ַ��һ�洢�����������н���
	func->address = func->address - 1;
	//cout<<func->address<<" "<<func->name<<endl;
	funclist.push_back(*func);
	return 0;
}
/**
 * �������� symbolTableAnalysis
 * ����������listFilePath ���ű��ļ�·��
 * �������ܣ��������ű��ļ���Ϣ
 * ����ֵ��
 * 		�ɹ�����0��
 * 		ʧ�ܷ���-1��
 */
int symbolTableAnalysis(string listFilePath)
{
	if(listFilePath.empty())
	{
		return -1;
	}
	int ret = 0;
	string::size_type position = 0; //find�����ķ��ر�־
	char data[255]; //����ÿ�ζ������ַ���
	string funcStr; //��������ĺ����Ǻ�������
	ifstream symbolTableFile;
	symbolTableFile.open(listFilePath,ios::out);
	if(symbolTableFile.bad())
	{
		return -1;
	}
	//��ȡ�ļ�������ǰ����
	for(int i = 0;i<3;i++)
	{
		symbolTableFile.getline(data,sizeof(data));
	}
	while(true)
	{
		//��ȡ�ı��е�һ��
		symbolTableFile.getline(data,sizeof(data));
		//cout<<data<<endl;
		if(0 == strlen(data))
		{
			break;
		}
		funcStr = data;
		memset(data,0,sizeof(data));
		//�ڶ�����һ���в���FUNC�ַ�������ҵ��ͽ��н���
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
map<string,vector<string>>mainFunc;
vector<string>* callFunc;
/**
 * �����������ж������ַ����Ƿ��������еĺ���
 * ���õ����ַ�����ŵ�map��
 */
void traversTheContainer(string &str,int& line)
{
	int i = 0;
	int j = 0;
	int ret = 0;
	string info;
	map<string,vector<string>>::iterator iter;
	vector<string>::iterator pIter;
	for(i = 0; i < (int)funclist.size(); i++)
	{
		ret = strcmp(funclist[i].name.data(),str.data());
		//������Ҫ���һ���ַ����пգ�ȥ������Ĳ��Ǻ������õĴ�
		if((0 == ret) && (0 != strlen(str.data())))
		{
			//��������ѡ����һ�������ڵ��õĺ���
			for(j = 0;j < (int)funclist.size(); j++)
			{
				//�����к�ȷ�������ı߽�
				if(funclist[j].lineNum < line && funclist[j+1].lineNum > line)
				{
					//�������������������õĺ����б���ֵ
					iter = mainFunc.find(funclist[j].name);
					if(iter == mainFunc.end())
					{
						//��ǰmap��û�д˺���ʱ����newһ���˺����ĵ����б�
						callFunc = new vector<string>;
						callFunc->push_back(funclist[i].name);
						//�����������͵��ú����б����map
						mainFunc.insert(map<string,vector<string>>::value_type(funclist[j].name,*callFunc));
					}
					else
					{
						//map���д���������ʱ���Ȳ��ҵ��ú����б������޴˺���
						pIter = find(iter->second.begin(),iter->second.end(),funclist[i].name);
						//���û�������
						if(pIter == iter->second.end())
						{
							iter->second.push_back(funclist[i].name);
						}
					}
				}
			}
		}
	}
}

void funcCallPrint()
{
	map<string,vector<string>>::iterator pIter;
	for(pIter = mainFunc.begin(); pIter != mainFunc.end();pIter++)
	{
		cout<<"������:"<<pIter->first<<endl;
		for(int i = 0;i < (int)pIter->second.size();i++)
		{
			cout<<"    "<<pIter->second[i]<<endl;
		}
	}
}
/**
 * �ж��ַ������Ƿ�����תָ��
 */
int strMatch(string &str, int& line)
{
	int i = 0;
	string::size_type position = 0; //find�����ķ��ر�־
	char jumpInstruc[2][10] = {"bl","b.n"}; //��תָ������
	for(i = 0; i < 2; i++)
	{
		position = str.find(jumpInstruc[i]);
		if(position != str.npos)
		{
			//�������תָ���
			return 0;
		}
	}
	return -1;
}
/**
 * �������� jumpDetection
 * ����������filePath ��Ҫ�����ļ���
 * �������ܣ����ļ����ҳ�����������䲢����
 * ����ֵ��
 * 		�ɹ�����0��
 * 		ʧ�ܷ���-1��
 */
int jumpDetection(ifstream& file)
{
	if(file.bad())
	{
		cout<<"fail to open the file"<<endl;
		return -1;
	}
	//��¼�к�
	int record = 0;
	int ret = 0;
	char data[255] = {0};
	string str;
	while(true)
	{
		//��ȡһ�н��н���
		file.getline(data,sizeof(data));
		str = data;
		memset(data,0,sizeof(data));
		ret = strMatch(str,record);
		if(0 == ret)
		{
			/*һ�к���������Ϣ���£�
			 * ��Ҫ�õ�<>�еĺ�����
			 *      282:	f000 fba5 	bl	9d0 <printk>
			 * �������£���sscanf
			 */
			sscanf(str.data(),"%*[^<]<%[^>]",data);
			str = data;
			traversTheContainer(str,record);
		}
		if(file.eof())
		{
			break;
		}
		record++;
	}
	return 0;
}

//�������еĺ�����ַ���бȽϣ���sort�����ıȽϹ���
bool cmpAddr(const funcInfo &a, const funcInfo &b)
{
	return a.address < b.address;
}

/**
 * �������� lineNumResolu
 * ����������file ��Ҫ�������ļ���
 * �������ܣ����ļ����ҳ��������ڵ��н��к���Ϣ���浽������
 * ����ֵ��
 * 		�ɹ�����0��
 * 		ʧ�ܷ���-1��
 */
int lineNumResolu(ifstream& file)
{
	if(file.bad())
	{
		return -1;
	}

	int  i = 0;
	int  count = 0;
	//��������¼ÿ���������к�
	char data[255] = {0};
	//����Ƿ��ҵ�����
	char funcLine[100] = {0};
	string::size_type position = 0; //find�����ķ��ر�־
	//�������еĺ�����Ϣ���պ�����ַ��������
	sort(funclist.begin(),funclist.end(),cmpAddr);
	while(true)
	{
		file.getline(data,sizeof(data));
		string str(data);
		/*
		 * FIXME :����Ϊ���Ż����ܣ�ֻ�Ժ����ĵ�ַ��Ϣ����ƥ����ܻ��bug��Ҫ���ڽ��
		 * ��bug��ԭ�����п��ܻ�����������������������ĳ�������ĵ�ַ��Ϣһ��
		 */
		sprintf(funcLine,"%08x",funclist[i].address);
		//sprintf(funcLine,"%08x <%s>:",funclist[i].address,funclist[i].name.data());
		position = str.find(funcLine);
		if(position != str.npos)
		{
			//���к���Ϣ���浽������
			funclist[i].lineNum = count;
			i++;
		}
		count++;
		if(i == (int)funclist.size())
		{
			break;
		}
	}
	//���һ��
	funcInfo * func = new funcInfo;
	func->lineNum = 4525;
	funclist.push_back(*func);
//#define debug
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
 * �������� disassemblyAnalysis
 * ����������dumpFilePath ������ļ�·��
 * �������ܣ�����������ļ���Ϣ
 * ����ֵ��
 * 		�ɹ�����0��
 * 		ʧ�ܷ���-1��
 */
int disassemblyAnalysis(string dumpFilePath)
{
	if(dumpFilePath.empty())
	{
		return -1;
	}
	int ret = 0;
	char data[255] = {0}; //����һ�е�����
	string::size_type position = 0; //find�����ķ��ر�־
	ifstream disassemblyFile;
	streampos pos; //��¼�ļ�ָ���λ��
	if(disassemblyFile.bad())
	{
		return -1;
	}
	disassemblyFile.open(dumpFilePath,ios::out);
	//ѭ����ȡ�ļ��е�����
	while(true)
	{
		//��ȡһ��
		disassemblyFile.getline(data,sizeof(data));
		if(disassemblyFile.eof())
		{
			break;
		}
		//����ʹ��find����
		string flag = data;
		//����ı��Σ�����⵽�ı��ξʹӿ�ʼ����
		position = flag.find("section .text:");
		if(position != flag.npos)
		{
			//��¼text�ο�ʼ��λ��
			pos = (int)disassemblyFile.tellg() - 13;
			break;
		}
	}
	//�����к�
	ret = lineNumResolu(disassemblyFile);
	if(-1 == ret)
	{
		cout<<"Line number resolution failed"<<endl;
		return -1;
	}
	//���ļ�ָ�븴λ��.text�ο�ʼ��λ��
	disassemblyFile.seekg(pos);
	//����������ת
	ret = jumpDetection(disassemblyFile);
	if(-1 == ret)
	{
		cout<<"Jump parsing failed"<<endl;
		return -1;
	}
	//��ӡ���
	funcCallPrint();
	return 0;
}
