#pragma once
#include "psy.h"

class SuicideAttemps :
	public psy
{
public:
	SuicideAttemps();
	void generateEpisode(std::vector<psyEpisode> &PsyDisorder);

private:
	void reparseEpisodes(bool* episodes, std::vector<psyEpisode> &source, psychatric_disorder);

};

