#pragma once
#include "psy.h"

class anxietyDisorder :
    public psy
{
public:

    void generateEpisode(double* parameters, bool* physicakActivity, int* bullyData, std::vector<adverse_events> adverseEvents, std::array<SNUsage, numberOfAgeGroups> SNUse, std::vector<psyEpisode>& psyDisorder);

};

