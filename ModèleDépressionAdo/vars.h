#pragma once
#include "parameters.h"

/* For simplicity order of adverse events here should be the same as order in the adverse event list enum */

// List of variables, order should be the one of the parameters
enum vars_list : int
{
	pParental_psychopathology_OR,
	pPhysical_abuse_OR,
	pEmotional_abuse_OR,
	pSexual_abuse_OR,
	pNeglect_OR,
	pCorporal_image_OR,
	pSleep_OR,
	pParental_psychopathology_prob,
	pParental_psychopathology_freq,
	








};

// Global variable
extern parameters<double> p;