#include "anxietyDisorder.h"
#include <iostream>
#include <fstream>

void anxietyDisorder::generateEpisode(double* parameters, bool* physicakActivity, int* bullyData, std::vector<adverse_events> adverseEvents, std::array<SNUsage, numberOfAgeGroups> SNUse, std::vector<psyEpisode>& psyDisorder)
{
	// Compute individual sensitivity
	double anxietyProbability[numberOfAgeGroups] = {};
	rnd.normal(numberOfAgeGroups, anxietyProbability, p[(parameters[0] == 0 ? pBaselineGAD_male : pBaselineGAD_female)], 1);

	// Parameters = - Ln of OR
	double adversites_ors[] = { p[pParental_psychopathology_GAD_OR], p[pPhysical_abuse_GAD_OR], p[pEmotional_abuse_GAD_OR], p[pSexual_abuse_GAD_OR],
		p[pNeglect_GAD_OR], p[pBullying_GAD_OR], p[pAdversities2_GAD_OR], p[pAdversities3_GAD_OR], p[pAdversities4p_GAD_OR] };
	double SN_ors[] = { p[pSNNumber2_GAD_OR], p[pSNNumber3_GAD_OR], p[pSNNumber5_GAD_OR], p[pSNDuration30_GAD_OR], p[pSNDuration60_GAD_OR], p[pSNDuration120_GAD_OR]}; // SN
	double PA_OR = p[pPA_GAD_OR];
	double bullyOR = p[pBullying_GAD_OR];

	/* First compute the baseline risk based on adverseEvents ********/
	double risk_modifier(0);

	// Add COVID
	bool impactedByCOVID(rnd() < 0.50);

	// Add Demography effect
	if (parameters[0]==0) risk_modifier += p[pMale_GAD_OR];

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

	risk_modifier += ((number_of_adversities > 1 ? adversites_ors[6] : 0) + (number_of_adversities > 2 ? adversites_ors[7] : 0) + (number_of_adversities > 3 ? adversites_ors[8] : 0));

	double cte_risk = risk_modifier;

	/*std::ofstream afile;
    afile.open("C:\\Users\\HenriLeleu\\Downloads\\test.csv", std::ios::app);*/

	/* Run each year, break at first episode, following episode will be based on duration and risk of reccurrence */
	for (int i = 0; i < numberOfAgeGroups; i++)
	{
		// Reset constante value
		risk_modifier = cte_risk;

		// Add baseline
		risk_modifier += anxietyProbability[i];
		
		// Add PA
		risk_modifier += physicakActivity[i] ? PA_OR : 0;

		// Add Bully
		risk_modifier += (bullyData[i] == 1) ? bullyOR : 0;

		// Add SN
		// - Frequency
		double freq(SNUse[i].n);
		if (freq > 2 && freq <= 4)  risk_modifier += SN_ors[0];
		else if (freq > 4 && freq <= 6)  risk_modifier += SN_ors[1];
		else if (freq > 6)  risk_modifier += SN_ors[2];

		// - Time
		double durr(SNUse[i].time * 60);
		if (freq > 30 && freq <= 60)  risk_modifier += SN_ors[3];
		else if (freq > 60 && freq <= 120)  risk_modifier += SN_ors[4];
		else if (freq > 120)  risk_modifier += SN_ors[5];

		// COVID !
		/*double year(10 + i + parameters[1]);
		if (impactedByCOVID && (year >= 2020.0 && year < 2022.0))
		{
			risk_modifier += p[pCOVID_GAD_OR];
		}*/

		// Estimate risk
		risk_modifier = 1 / (1 + exp(risk_modifier));

		//afile << risk_modifier << ",";

 		if (rnd() < risk_modifier) {
			double start(static_cast<double>(i));
			double end(start + 1);

			psyDisorder.push_back({ psychatric_disorder::anxiety, start,end });

		}

	}

	/*afile << std::endl;
	afile.close();*/

}
