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

	// OR for Suicide Attempts
	pSuicideAttemps_Depression_OR,
	pSuicideAttemps_GAD_OR,
	pSuicideAttemps_DepressionGAD_OR,
	pSuicideAttemps_Baseline_MaleOR,
	pSuicideAttemps_Baseline_FemaleOR,

	// Odds ratio of Adversities with GAD
	pParental_psychopathology_GAD_OR,
	pPhysical_abuse_GAD_OR,
	pEmotional_abuse_GAD_OR,
	pSexual_abuse_GAD_OR,
	pNeglect_GAD_OR,
	pBullying_GAD_OR,
	pAdversities2_GAD_OR,
	pAdversities3_GAD_OR,
	pAdversities4p_GAD_OR,
	pSNNumber2_GAD_OR,
	pSNNumber3_GAD_OR,
	pSNNumber5_GAD_OR,
	pSNDuration30_GAD_OR,
	pSNDuration60_GAD_OR,
	pSNDuration120_GAD_OR,
	pPA_GAD_OR,
	pMale_GAD_OR,
	pCOVID_GAD_OR,
	pBaselineGAD_male,
	pBaselineGAD_female,

	// Odds ratio of Adversities with Depression
	pParental_psychopathology_OR,
	pPhysical_abuse_OR,
	pEmotional_abuse_OR,
	pSexual_abuse_OR,
	pNeglect_OR,
	pBullying_OR,
	pAdversities2_OR,
	pAdversities3_OR,
	pAdversities4p_OR,
	pSNNumber2_OR,
	pSNNumber3_OR,
	pSNNumber5_OR,
	pSNDuration30_OR,
	pSNDuration60_OR,
	pSNDuration120_OR,
	pPA_OR,
	pMale_OR,
	pCOVID_Dep_OR,

	pParental_psychopathology_prob,
	pParental_psychopathology_freq,
	pBaselineDepressionRisk,

	// Physical Activity
	pPA_Freq10Male,
	pPA_Freq10Female,
	pPA_Freq14Male,
	pPA_Freq14Female,
	pPA_Freq10Male_alt,
	pPA_Freq10Female_alt,
	pPA_Freq14Male_alt,
	pPA_Freq14Female_alt,
	pPA_Decrease,
	pPA_COVIDReduction,

	// Physical Chronic Conditions
	pObesityMale_a,
	pObesityMale_b,
	pObesityFemale_a,
	pObesityFemale_b,
	pChronicConditionMale_a,
	pChronicConditionMale_b,
	pChronicConditionFemale_a,
	pChronicConditionFemale_b,
	pOverweightMale_a,
	pOverweightMale_b,
	pOverweightFemale_a,
	pOverweightFemale_b,
	pAlcoholMale_a,
	pAlcoholMale_b,
	pAlcoholFemale_a,
	pAlcoholFemale_b,
	pTabacoMale_a,
	pTabacoMale_b,
	pTabacoFemale_a,
	pTabacoFemale_b,
	pCannabisMale_a,
	pCannabisMale_b,
	pCannabisFemale_a,
	pCannabisFemale_b,
	pCannabis_Depression_OR,
	pTabaco_Depression_OR,
	pAlcohol_Depression_OR,
	pOverweight_Depression_OR,
	pObesity_DepressionMale_OR,
	pObesity_DepressionFemale_OR,
	pChronicCondition_Depression_OR,

	// Probabilities of adversities
	pParental_psychopathology_Pr,
	pPhysical_abuse_Pr,
	pEmotional_abuse_Pr,
	pSexual_abuse_Pr,
	pNeglect_Pr,
	pBullyingMale,
	pBullyingFemale,
	
	// Depression Related parameters
	pDurrationDepression_a,
	pDurrationDepression_b,
	pDepressionRelapseNoRisk,
	pTimeToDepressionRelapse,
	pReverseCausationDepIncreaseRate,
	pReverseCausationSNIncrease,

	// Correlation between social use and other factors
	pCorrSNUseMentalState,
	pCorrSNUsePhysicalActivity,
	pCorrSN,

	// Time spent on SN
	pTimeSpentFacebook,
	pTimeSpentTwitter,
	pTimeSpentSnapshat,
	pTimeSpentWhatsapp,
	pTimeSpentInstagram,
	pTimeSpentTikTok,

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

struct indCararcteristics {
	bool male;
	double date_of_birth;
	bool chronicCondition[numberOfAgeGroups];
	bool obesity[numberOfAgeGroups];
	bool overweight[numberOfAgeGroups];
	bool tabaco[numberOfAgeGroups];
	bool alcohol[numberOfAgeGroups];
	bool cannabis[numberOfAgeGroups];
};

#define AGEGRVAR
const static size_t outputBufferSize = 1000;

// Options
extern bool lowerPA;
extern bool Options_ReverseCausation;
extern bool noSN;
extern bool noAddictiveSN;
extern bool USVersion;
extern bool Limit1h;
extern bool intervention;
extern bool individual_intervention;
extern std::string userFolder;