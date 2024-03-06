#pragma once
#include "vars.h"
#include "parameters.h"
#include "adverse_events.h"
#include "vlsRandGenerator.h"
#include "psy.h"
#include "socialNetwork.h"
#include "physicalActivity.h"
#include "moodDisorder.h"
#include "anxietyDisorder.h"
#include "SuicideAttemps.h"

class individual
{
public:
	individual();
	individual(double year, bool male);
	void history();
	
	// Output functions
	std::string outputAdverseEvent() const;
	void outputAll(long id, std::array < char[outputBufferSize], 4> &) const;

private:
	void generateAdverseEvents();
	void generateAdverseEvents(double year); // year - 2019
	void generatePsychiatricDisorder();
	void generateOtherFactors();

	double date_of_birth;
	double age_of_death;
	bool male;
	std::vector<adverse_events> adverseEvents;
	std::vector<psyEpisode> psyDisorder;
	std::array<SNUsage, numberOfAgeGroups> SNUse;
	bool chronicCondition[numberOfAgeGroups];
	bool obesity[numberOfAgeGroups];
	bool overweight[numberOfAgeGroups];
	bool tabaco[numberOfAgeGroups];
	bool alcohol[numberOfAgeGroups];
	bool cannabis[numberOfAgeGroups];
	bool physicalActivityData[numberOfAgeGroups];
	int bullyData[numberOfAgeGroups];

	// Physical Activity Probability used in correlation with SN Use
	double physicalActivityProbability;

	// Depression Pr used to estimate depression risk between 10 et 18, needs to be here to be used for reverse causation correlation
	double depressionProbabilites[numberOfAgeGroups];
};

