struct Voyage
{
	static const constexpr char* DATASET[3] = {"AVG", "ALL", "SIN"};
	static const constexpr char* MODE[5] = {"DRFT", "WIND", "DEST", "DIRN", "RAND"};

	std::string name;
	std::string project;

	/* Parameters */

	int 	dataset			= SIN;
	int 	mode 			= DEST;

	float 	altitude 		= 2;
	float 	windlimit 		= 8;
	int 	sailopenhours 	= 12;
	float 	alpha 			= 0.11;				// parameter for wind profile power law

	/* Status */

	bool 	sailopen 		= true;
	UV 		dir				= 0;
};
