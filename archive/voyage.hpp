struct Voyage
{
	/* Parameters */

	float 	altitude 		= 2;
	float 	windlimit 		= 8;
	int 	sailopenhours 	= 12;
	float 	alpha 			= 0.11;				// parameter for wind profile power law

	/* Status */

	bool 	sailopen 		= true;
	UV 		dir				= 0;
};
