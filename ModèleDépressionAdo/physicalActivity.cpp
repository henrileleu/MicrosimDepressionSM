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
		if (y == 2020 || y == 2021) reduction *= 0.5; // 50% reduction during COVID

		// Add reducing trend, starting in 2014
		double intervals(y - 2014);
		if (lowerPA && y > 2014)
		{
			reduction *= pow(0.930888, intervals);
		}

		if (j < 14)
		{
			if (physicalActivityProbability < ((male ? 0.337 : 0.202) * reduction) ) r[j] = true;
			continue;
		}
		if (physicalActivityProbability < ((male ? 0.401 : 0.157) * reduction) ) r[j] = true;
	}
}
