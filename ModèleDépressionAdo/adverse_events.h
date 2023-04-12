#pragma once

#include <vector>
#include <array>
#include <string>

enum adverse_events_list : int
{
	Parental_psychopathology,
	Physical_abuse,  
	Emotional_abuse,
	Sexual_abuse,
	Neglect,
	Bullying,
	Social_media_use,
};

const int adverse_event_n = 7;

class adverse_events
{
public:
	adverse_events();
	adverse_events(int type, double start, double end);

	bool has(int type, double when) const;
	double returnOR(double when) const;
	int getType() const;
	double getStart() const;
	double getEnd() const;
	std::string outputAdverseEvent() const;

private:
	int type;
	double start, end;

};

