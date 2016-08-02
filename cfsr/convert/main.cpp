#include "cfsr.h"
#include <iostream>
#include <string>
#include <fstream>
#ifdef _WIN32
	#include <windows.h>
#endif

int main()
{
	#ifdef _WIN32
		SetConsoleOutputCP(950);
	#endif

	printf(
	"\n"
	"*************** csv->CFSR�ɮ��ഫ�{�� ***************\n"
	"                  2016/5/16\n"
	"                 En Shih (�ۮ�)\n"
	"\n\n");

	printf("�ɮ��ഫ���A�еy��... ...\n\n");

	for (int i=1; i<=12; i++)
	{
		convertCFSR(CFSR_OU, i);
		convertCFSR(CFSR_OV, i);
		convertCFSR(CFSR_U, i);
		convertCFSR(CFSR_V, i);
	}

	printf("�ɮ��ഫ����\n");

	return 0;
}
