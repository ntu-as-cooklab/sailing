#include "cfsr.h"
#include <iostream>
#include <string>
#include <fstream>

int main()
{
	int ncid = openCFSR(CFSR_U, 1979, 1);
	closeCFSR(ncid);

	return 0;
}
