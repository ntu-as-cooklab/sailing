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
			  		 months = 12;//((endYear-startYear)*12 + (endMonth-startMonth+1));

	int ouid[months], ovid[months], auid[months], avid[months];

public:

	UV OUV(Date date, LatLon pos);
	UV AUV(Date date, LatLon pos);

	CfsrReader()
	{
		for (int i=0; i<12; i++)
		ouid[i]
		= ovid[i]
		= auid[i]
		= avid[i] = 0;
	}

	~CfsrReader()
	{
		for (int i=0; i<12; i++)
		{
			closeCFSR(ouid[i]);
			closeCFSR(ovid[i]);
			closeCFSR(auid[i]);
			closeCFSR(avid[i]);
		}
	}
};

#endif
