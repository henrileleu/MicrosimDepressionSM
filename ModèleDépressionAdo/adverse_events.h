#pragma once

#include <vector>
#include <array>
#include <string>
#include "vars.h"
#include "parameters.h"

enum adverse_events_list : int
{
	Parental_psychopathology,
	Physical_abuse,  
	Emotional_abuse,
	Sexual_abuse,
	Neglect,
	Bullying,
};

const int adverse_event_n = Bullying;

struct aes
{
	int type;
	double start;
	double end;
};

class adverse_events
{
public:
	adverse_events();
	adverse_events(int type, double start, double end);
	adverse_events(int type, double start, double end, double frequence, double duration);

	bool has(int type, double when) const;
	double returnOR(double when) const;
	int getType() const;
	double getStart() const;
	double getEnd() const;
	double getFrequence() const;
	double getDuration() const;

	aes outputAdverseEvent() const;

private:
	int type;
	double start, end, frequence, duration;

};

