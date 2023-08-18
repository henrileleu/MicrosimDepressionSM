#include "anxietyDisorder.h"

void anxietyDisorder::generateEpisode(double* parameters, bool* physicakActivity, int* bullyData, std::vector<adverse_events> adverseEvents, std::array<SNUsage, numberOfAgeGroups> SNUse, std::vector<psyEpisode>& psyDisorder)
{
	// Use a single risk that reflects individual sensitivity
	//double anxietyProbability(rnd());

	// Parameters = - Ln of OR
	double adversites_ors[] = { -0.0676586484738149, 0.116533816255952, -0.113328685307003, 0.105360515657826, -0.0198026272961797, -0.46373401623214, -0.048790164169432, 0.0618754037180875, 0.27443684570176 };
	double SN_ors[] = { -0.357674444271816, -0.636576829071551, -1.18478998490916, 0.0725706928348354, -0.131028262406404, -0.307484699747961 }; // SN
	double PA_OR = 0.301105;
	double bullyOR = -0.46373;

	/* First compute the baseline risk based on adverseEvents ********/
	double risk_modifier(0);

	// Baseline
	double baseline(-0.7);
	risk_modifier += -baseline;

	// Add COVID
	bool impactedByCOVID(rnd() < 0.50);

	// Add Demography effect
	if (parameters[0]==0) risk_modifier += 0.430783;

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

	/* Run each year, break at first episode, following episode will be based on duration and risk of reccurrence */
	for (int i = 0; i < 9; i++)
	{
		// Reset constante value
		risk_modifier = cte_risk;

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
		double year(10 + i + parameters[1]);
		if (impactedByCOVID && (year >= 2020.0 && year < 2022.0))
		{
			risk_modifier += -1.481605;
		}

		// Estimate risk
		risk_modifier = 1 / (1 + exp(risk_modifier));

 		if (rnd() < risk_modifier) {
			double start(static_cast<double>(i));
			double end(start + 1);

			psyDisorder.push_back({ psychatric_disorder::anxiety, start,end });

		}

	}

}
