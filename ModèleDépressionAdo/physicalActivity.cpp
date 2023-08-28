#include "physicalActivity.h"

physicalActivity::physicalActivity()
{
}

void physicalActivity::generatePhysicalActivity(bool * r, double yearOfBirth, bool male, double physicalActivityProbability)
{
	// For now, PA has been stable for the last 10-years (Verdot, 2020), sensitivity analysis will be implemented to test an alternative assumption
	// Using Estaban data 2014-2016, 60 min per day

	// Set a single probability here, we'll assume that they are active & non active adolescents
	// physicalActivityProbability

	// For now starts at 10

	for (int j = 0; j < numberOfAgeGroups; j++)
	{
		int y(static_cast<int>(yearOfBirth) + 10 + j);
		double reduction(1);

		// COVID 
		if (y == 2020 || y == 2021) reduction *= p[pPA_COVIDReduction]; // 50% reduction during COVID

		// Add reducing trend, starting in 2002
		double intervals(y - 2002);
		if (lowerPA && y > 2002)
		{
			reduction *= pow(p[pPA_Decrease], intervals);
		}

		if (j < 14)
		{
			if (physicalActivityProbability < ((male ? p[pPA_Freq10Male] : p[pPA_Freq10Female]) * reduction) ) r[j] = true;
			continue;
		}
		if (physicalActivityProbability < ((male ? p[pPA_Freq14Male] : p[pPA_Freq14Female]) * reduction) ) r[j] = true;
	}
}
