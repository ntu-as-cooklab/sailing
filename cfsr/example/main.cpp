#include "cfsr.h"
#include <iostream>
#include <string>
#include <fstream>

int main()
{
	for (int i=1; i<=12; i++)
	{
		convertCFSR(CFSR_OU, i);
		convertCFSR(CFSR_OV, i);
		convertCFSR(CFSR_U, i);
		convertCFSR(CFSR_V, i);
	}

	return 0;
}
