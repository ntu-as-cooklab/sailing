#include "cfsr.h"
#include <iostream>
#include <string>
#include <fstream>

int main()
{
	int ncid = openCFSR(CFSR_OU, 1979, 1);
	printf("%d", getOUV(ncid, 1, 12, 23));
	closeCFSR(ncid);

	return 0;
}
