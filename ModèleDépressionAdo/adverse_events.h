#pragma once

#include <vector>
#include <array>


enum adverse_events_list : int
{
	Parental_psychopathology,
	Physical_abuse,  // Include bullying, cyberbullying
	Emotional_abuse, // Include bullying, cyberbullying
	Sexual_abuse,
	Neglect,
	Social_media_use,
};

const int adverse_event_n = 6;

class adverse_events
{
public:
	adverse_events();
	adverse_events(int type, double start, double end);
	bool has(int type, double when);
	double returnOR(double when);

private:
	int type;
	double start, end;
};

