#include "cfsr.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

int main()
{
	SetConsoleOutputCP(950);

	printf(
	"\n"
	"*************** csv->CFSR檔案轉換程式 ***************\n"
	"                  2016/5/16\n"
	"                 En Shih (石恩)\n"
	"\n\n");

	printf("檔案轉換中，請稍候... ...\n\n");

	for (int i=1; i<=12; i++)
	{
		convertCFSR(CFSR_OU, i);
		convertCFSR(CFSR_OV, i);
		convertCFSR(CFSR_U, i);
		convertCFSR(CFSR_V, i);
	}

	printf("檔案轉換完畢\n");

	return 0;
}
