#pragma once
#include "adverse_events.h"
#include "psy.h"


class individual
{
public:
	individual();
	individual(double year, bool male);
	void history();
	bool hasAdverseEvent(int type, double when);
	bool hasPsychiatricDisorder(int type, double when);

private:
	void generateAdverseEvents();
	void generateAdverseEvents(double year); // year - 2019
	void generatePsychiatricDisorder();

	double date_of_birth;
	bool male;
	std::vector<adverse_events> adverseEvents;
	std::vector<psy> psyDisorders;

};