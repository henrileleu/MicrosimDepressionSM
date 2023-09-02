#include "moodDisorder.h"
#include <iostream>
#include <fstream>

void moodDisorder::generateEpisode(double* parameters, bool* physicakActivity, int* bullyData, std::vector<adverse_events> adverseEvents, std::array<SNUsage, numberOfAgeGroups> SNUse, std::vector<psyEpisode>& psyDisorder, double * depressionProbabilites)
{

	std::vector<psyEpisode> results;

	// Parameters = - Ln of OR
	double adversites_ors[] = { p[pParental_psychopathology_OR], p[pPhysical_abuse_OR], p[pEmotional_abuse_OR], 
		p[pSexual_abuse_OR], p[pNeglect_OR], p[pBullying_OR], 
		p[pAdversities2_OR], p[pAdversities3_OR], p[pAdversities4p_OR]};

	double SN_ors[] = { p[pSNNumber2_OR], p[pSNNumber3_OR], p[pSNNumber5_OR], p[pSNDuration30_OR], p[pSNDuration60_OR], p[pSNDuration120_OR]}; // SN
	double PA_OR = p[pPA_OR];
	double bullyOR = p[pBullying_OR];

	/* First compute the baseline risk based on adverseEvents ********/
	double risk_modifier(0);

	// Add Demography effect
	if (parameters[0] == 0.0) risk_modifier += p[pMale_OR];
			 
	// Add COVID
	bool impactedByCOVID(rnd() < 0.20);

	// Add Adversities
	double number_of_adversities(0);
	
	for (std::vector<adverse_events>::const_iterator it = adverseEvents.begin();
		it != adverseEvents.end(); it++)
	{
		// different for type
		int t = it->getType();
		risk_modifier += adversites_ors[t];
		number_of_adversities += 1.0;
	}

	risk_modifier += ( (number_of_adversities > 1 ? adversites_ors[6] : 0) + (number_of_adversities > 2 ? adversites_ors[7] : 0) + (number_of_adversities > 3 ? adversites_ors[8] : 0));

	double cte_risk = risk_modifier;

	//std::ofstream afile;
	//afile.open("C:\\Users\\HenriLeleu\\Downloads\\test.csv", std::ios::app);

	/* Run each year, break at first episode, following episode will be based on duration and risk of reccurrence */
	for (int i = 0; i < numberOfAgeGroups; i++)
	{
		double year(10 + i + parameters[1]);

		// Reset constante value
		risk_modifier = cte_risk;

		// For reverse causation, this will be used to increase the depression
		if (Options_ReverseCausation)
		{
			if (year > 2019) depressionProbabilites[i] += 10.0 * p[pReverseCausationDepIncreaseRate];
			else if (year > 2009) depressionProbabilites[i] += (year - 2009.0) * p[pReverseCausationDepIncreaseRate];
		}

		// Add baseline
		risk_modifier += depressionProbabilites[i] * (1.0 - (parameters[0] == 0 ? -0.20 : 0.08));

		// Add PA
		risk_modifier += physicakActivity[i] ? PA_OR : 0;

		// Add Bully
		risk_modifier += (bullyData[i]==1) ? bullyOR : 0;

		double durr(SNUse[i].time * 60);
		double freq(SNUse[i].n);

		if (individual_intervention)
		{
			double estimated_risk(1 / (1 + exp(risk_modifier)));
			if (estimated_risk > 0.35)
			{
				durr = 0;
				freq = 0;
			}
		}
		
		// Add SN
		// - Frequency

		if (freq > 2 && freq <= 4)  risk_modifier += SN_ors[0];
		else if (freq > 4 && freq <= 6)  risk_modifier += SN_ors[1];
		else if (freq > 6)  risk_modifier += SN_ors[2];

		// - Time
		if (durr > 30 && durr <= 60)  risk_modifier += SN_ors[3];
		else if (durr > 60 && durr <= 120)  risk_modifier += SN_ors[4];
		else if (durr > 120)  risk_modifier += SN_ors[5];
		
		// COVID !
		if (impactedByCOVID && (year == 2020.0 || (year == 2021.0 && rnd() < 0.5)))
		{
			risk_modifier += p[pCOVID_Dep_OR];
		}
		

		// Estimate risk
		risk_modifier = 1 / (1 + exp(risk_modifier));

		//afile << risk_modifier << ",";

		if (risk_modifier > 0.5) {
			double start(static_cast<double>(i) + rnd());
			double end(start + rnd.gamma(p[pDurrationDepression_a], p[pDurrationDepression_b]) / 12);

			psyDisorder.push_back({ psychatric_disorder::depression, start, end});

		}

		
		// Override the probability value, with the individual risk to reuse for SN in the reverse causation assumption
		depressionProbabilites[i] = risk_modifier;

	}

	//afile << std::endl;
	//afile.close();

	subsequentDepressionEpisodes(psyDisorder);

}

	void moodDisorder::subsequentDepressionEpisodes(std::vector<psyEpisode>& results)
	{
		double noRelapse(p[pDepressionRelapseNoRisk]);

		if (rnd() < noRelapse || results.empty()) return;

		double start = results.back().start;
		double end = results.back().end;
		while (start < 90)
		{
			start = end + rnd.weibull(1.0, pTimeToDepressionRelapse);
			end = start + rnd.gamma(p[pDurrationDepression_a], p[pDurrationDepression_b]) / 12;

			results.push_back({ psychatric_disorder::depression, start, end });

			if (rnd() < noRelapse) return;
		}

		return;
	}
