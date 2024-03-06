#pragma once
#include <vector>
#include "vars.h"
#include "vlsRandGenerator.h"
#include "parameters.h"
#include "adverse_events.h"
#include "socialNetwork.h"

enum class psychatric_disorder
{
	depression,
	anxiety,
	suicide
};

struct psyEpisode
{
	psychatric_disorder type;
	double start;
	double end;
};

class psy
{
public:

	psy();
	void generateEpisode(indCararcteristics& parameters, double* physicakActivity, int * bullyData, std::vector<adverse_events> adverseEvents, std::array<SNUsage, numberOfAgeGroups> SNUse, std::vector<psyEpisode> &psyDisorder);

};