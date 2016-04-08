#ifndef CFSR_HPP
#define CFSR_HPP

#include "cfsr.h"
#include "date.hpp"
#include "latlon.hpp"
#include "uv.hpp"

class CfsrReader
{
	static const int startYear 	= 1979, startMonth = 1,
			  		 endYear	= 1979, endMonth   = 1,
			  		 months = ((endYear-startYear)*12 + (endMonth-startMonth+1));

	int ouid[months], ovid[months], auid[months], avid[months];

public:

	UV OUV(Date date, LatLon pos);
	UV AUV(Date date, LatLon pos);

	CfsrReader()
	{
		ouid[0]
		= ovid[0]
		= auid[0]
		= avid[0] = 0;
	}

	~CfsrReader()
	{
		closeCFSR(ouid[0]);
		closeCFSR(ovid[0]);
		closeCFSR(auid[0]);
		closeCFSR(avid[0]);
	}
};

#endif
