#pragma once
#include "vlsRandGenerator.h"

/* For simplicity order of adverse events here should be the same as order in the adverse event list enum */

// List of variables, order should be the one of the parameters
enum vars_list : size_t
{
	// Background Mortality
	pBackgroundMortalityGompertz_a,
	pBackgroundMortalityGompertz_b,
	pBackgroundMortalityGompertz_year,
	pBackgroundMortalityGompertz_sex,

	// Odds ration of Adversities with Depression
	pParental_psychopathology_OR,
	pPhysical_abuse_OR,
	pEmotional_abuse_OR,
	pSexual_abuse_OR,
	pNeglect_OR,
	pCorporal_image_OR,
	pSleep_OR,
	pParental_psychopathology_prob,
	pParental_psychopathology_freq,
	pBaselineDepressionRisk,

	// Probabilities of adversities
	pParental_psychopathology_Pr,
	pPhysical_abuse_Pr,
	pEmotional_abuse_Pr,
	pSexual_abuse_Pr,
	pNeglect_Pr,

	// Depression Related parameters
	pDurrationDepression_a,
	pDurrationDepression_b,
	pDepressionRelapseNoRisk,
	pTimeToDepressionRelapse,
	pReverseCausationDepIncreaseRate,

	// Correlation between social use and other factors
	pCorrSNUseMentalState,
	pCorrSNUsePhysicalActivity,

	// Options
	pOptions_ReverseCausation,

	// Should always be the last one
	TechNumberOfItems

};

static const size_t NumberOfParameters = TechNumberOfItems;

// CSV Separator
static std::string separator = ";";

// Global variable
const static int numberOfAgeGroups = 9;
#define AGEGRVAR

// Options
extern bool lowerPA;
extern bool Options_ReverseCausation;
extern bool noSN;