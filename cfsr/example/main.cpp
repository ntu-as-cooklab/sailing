#include "cfsr.h"
#include <iostream>
#include <string>
#include <fstream>

int main()
{
	int ncid = openCFSR(CFSR_OU, 1979, 1);
	printf("%f\n", bilinearOUV(ncid, 2, 0, -20));
	closeCFSR(ncid);

	return 0;
}
