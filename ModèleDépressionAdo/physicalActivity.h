#pragma once
#include "vars.h"
#include "parameters.h"

#ifndef AGEGRVAR
const static int numberOfAgeGroups = 9;
#define AGEGRVAR
#endif

class physicalActivity
{
public:
	physicalActivity();
	void generatePhysicalActivity(bool * r, double yearOfBirth, bool male, double physicalActivityProbability);
};

