#include "socialNetwork.h"
#include <iostream>
#include <fstream>

socialNetwork::socialNetwork()
{}

const std::array<SNUsage, numberOfAgeGroups> socialNetwork::getSNUsage(int yearOfBirth, bool male, double physicalActivityProbability)
{
	

	// Check conditions for fill the array
	//	Last position is age 18 i.e. 2004 - yearOfBirth >= 18
	if (2004 - yearOfBirth >= 10 + numberOfAgeGroups) return std::array<SNUsage, numberOfAgeGroups>({});

	// First generates Rnd
	double SNProbability[numberOfSN];
	SNProbabilityEstimate(SNProbability, physicalActivityProbability, p[pCorrSNUsePhysicalActivity]);

	return generateSNUse(yearOfBirth, male, SNProbability, true);

}
const std::array<SNUsage, numberOfAgeGroups> socialNetwork::SNUsageFromDepression(int yearOfBirth, bool male, double* priorProbabilities)  // priorProbabilities should be the size of numberOfAgeGroups
{

	double SNProbability[numberOfSN * numberOfAgeGroups + numberOfAgeGroups]; // +1 to accomodate the priors in the end

	// Generate the SN probabilities from the priors
	for (int i = 0; i < numberOfAgeGroups; i++)
	{
		double rowProbability[numberOfSN];
		SNProbabilityEstimate(rowProbability, priorProbabilities[i], -0.20);

		// Fill
		for (int j = 0; j < numberOfSN; j++) SNProbability[j + i * numberOfSN] = rowProbability[j];
	}

	// Add after the last age group the priors
	//for (int j = 0; j < numberOfAgeGroups; j++) SNProbability[numberOfSN * numberOfAgeGroups + j] = (priorProbabilities[j] > 0.5 ? 1 : 0);
	
	/*std::ofstream afile;
	afile.open("C:\\Users\\HenriLeleu\\Downloads\\test.csv", std::ios::app);
	for (int i = 0; i < numberOfAgeGroups; i++)
	{
		afile << priorProbabilities[i] << ",";
		//for (int j = 0; j < numberOfSN; j++) afile << SNProbability[j + i * numberOfSN] << ",";
		afile << SNProbability[i * numberOfSN] << ",";

	}

	afile << std::endl;
	
	afile.close();
	*/

	return generateSNUse(yearOfBirth, male, SNProbability, false);
}


void socialNetwork::SNProbabilityEstimate(double* SNProbability, double physicalActivityProbability, double Corr)
{
	// Initialize a n+1 array
	double tempSNProbability[numberOfSN + 1] = {};
	double CorrValues[1] = { physicalActivityProbability };

	// Generate Correlation Array
	double snCorr(p[pCorrSN]);
	double a(Corr);
	double SNProbabilityCorrelation[] =
	{
		1.0,	a,		a,		a,		a,		a,		a,
		0.0,	1.0,	snCorr,	snCorr,	snCorr,	snCorr,	snCorr,
		0.0,	0.0,	1.0,	snCorr,	snCorr,	snCorr,	snCorr,
		0.0,	0.0,	0.0,	1.0,	snCorr,	snCorr,	snCorr,
		0.0,	0.0,	0.0,	0.0,	1.0,	snCorr,	snCorr,
		0.0,	0.0,	0.0,	0.0,	0.0,	1.0,	snCorr,
		0.0,	0.0,	0.0,	0.0,	0.0,	0.0,	1.0

	};

	rnd.correlated_rnd(tempSNProbability, SNProbabilityCorrelation, numberOfSN + 1, CorrValues, 1);

	// Fill the final array, skip the n first elements to were used for correlation
	for (int i = 0; i < numberOfSN; ++i)
	{
		SNProbability[i] = tempSNProbability[i + 1];

	}

	/*std::ofstream afile;
	afile.open("C:\\Users\\HenriLeleu\\Downloads\\test.csv", std::ios::app);
	afile << physicalActivityProbability << ",";
	for (int i = 0; i < numberOfSN; ++i) afile << SNProbability[i] << ",";
	afile << std::endl;
	*/

	return;
}

const std::array < SNUsage, numberOfAgeGroups> socialNetwork::generateSNUse(int yearOfBirth, bool male, double* SNProbability, bool cteTimeProbabilities)
{
	std::array<SNUsage, numberOfAgeGroups> results({});

	double corrDepression(p[pReverseCausationSNIncrease]);

	// Loop thougth all SN
	for (int SN = 0; SN < numberOfSN; SN++)
	{
		if (noAddictiveSN && (SN == 2 || SN == 4 || SN == 5)) continue;

		double timeOnSN[numberOfAgeGroups] = {};
		if (cteTimeProbabilities) timeOnSN[0] = rnd.gamma(6.25, timeSpentOnSN[SN])*0.9;
		else  timeOnSN[0] = rnd.gamma(6.25, timeSpentOnSN[SN] * corrDepression);
		

		//Loop through all positions in the array
		for (size_t i = 0; i < numberOfAgeGroups; i++)
		{
			size_t index(SN + (cteTimeProbabilities ? 0 : (i * numberOfSN))); // If CteTime the Pr array is a numberOfSN long if CteTime is false, the a new time each numberOfSN rows
			// Set values
			size_t age(i + 10);
			size_t year(yearOfBirth + age);
			if (year < 2005) continue;

			if (year > 2022) year = 17; // Max condition, after 2021 assumes that use is constant
			else year = year - 2005;

			// Check if use
			double pr(SNProbability[index]);
			bool isUsing(pr < *(getProportionUsers(age, male) + year + SN * 27));

			// If use, then set times
			results[i].n += (isUsing ? 1 : 0);
			results[i].time += (isUsing ? timeOnSN[0] : 0);
		}

	}
	
	// Limit to 1h per day max
	if (Limit1h)
	{
		for (int SN = 0; SN < numberOfSN; SN++)
		{
			for (size_t i = 0; i < numberOfAgeGroups; i++)
			{
				results[i].time = (results[i].time > 1.0 ? 1.0 : results[i].time);
			}
		}
	}

	// Limit to 1h per day max
	if (intervention)
	{
		for (int SN = 0; SN < numberOfSN; SN++)
		{
			for (size_t i = 0; i < numberOfAgeGroups; i++)
			{
				size_t age(i + 10);
				size_t year(yearOfBirth + age);
				if (year >= 2010 && results[i].time > 1.0) results[i].time -= 0.5;

			}
		}
	}

	return results;
}

const double * socialNetwork::getProportionUsers(size_t age, bool male)
{
	if (age >= 16)
	{
		return (male ? &proportionUsers1625male[0][0] : &proportionUsers1625female[0][0]);
	}
	else return (male ? &proportionUsers1114male[0][0] : &proportionUsers1114female[0][0]);
}
