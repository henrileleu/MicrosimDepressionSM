#pragma once
#include "psy.h"

class moodDisorder :
    public psy
{
public:
    
    void generateEpisode(indCararcteristics& parameters, bool* physicakActivity, int* bullyData, std::vector<adverse_events> adverseEvents, std::array<SNUsage, numberOfAgeGroups> SNUse, std::vector<psyEpisode>& psyDisorder, double * depressionProbabilites);
    
    void subsequentDepressionEpisodes(std::vector<psyEpisode>& results);
};

