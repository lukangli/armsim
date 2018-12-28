/*
 * main.cc
 *
 *  Created on: 2018Äê12ÔÂ26ÈÕ
 *      Author: LuKang
 */

#include <iostream>
#include <string>
#include "ElfParse.h"
using namespace std;
string filePath = "./test/start.out";


int main()
{
	elfParseInit(filePath);
	symbolTableAnalysis("./test/start.list");
	disassemblyAnalysis("./test/start.dump");
	return 0;
}
