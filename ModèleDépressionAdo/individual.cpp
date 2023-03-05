#include "individual.h"
#include "vars.h"
#include "parameters.h"

individual::individual() : date_of_birth(0), male(true)
{}


individual::individual(double _year, bool _male) : date_of_birth(_year), male(_male)
{}

void individual::history()
{
	generateAdverseEvents();
	generatePsychiatricDisorder();
}

bool individual::hasAdverseEvent(int type, double when)
{
	for (std::vector<adverse_events>::iterator i = adverseEvents.begin(); i < adverseEvents.end(); i++)
	{
		if (i->has(type, when)) return true;
	}

	return false;
}

bool individual::hasPsychiatricDisorder(int type, double when)
{
	for (std::vector<psy>::iterator i = psyDisorders.begin(); i < psyDisorders.end(); i++)
	{
		if (i->has(type, when)) return true;
	}

	return false;
}

/* Function to generate the adverse events during childhood 
	- First calculate the probability of at least one of each (period dependant for the newer ones)
	- Then calculate the frequency of each
	- Then caculate when and duration
*/
void individual::generateAdverseEvents() 
{
	// Variables 
	double ae_probability, ae_frequency, ae_duration;
	double time(2019 - date_of_birth);

	for (int i = 0; i < adverse_event_n; i++)
	{
		switch(i)
		{
		default:
			ae_probability = 0;
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Parental_psychopathology:
			ae_probability = 0.437;
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Physical_abuse:
			ae_probability = 0.172;
			ae_frequency = 0;
			ae_duration = 0;
			break;
		
		case adverse_events_list::Emotional_abuse:
			ae_probability = 0.347;
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Sexual_abuse:
			ae_probability = 0.044;
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Neglect:
			ae_probability = 0.16;
			ae_frequency = 0;
			ae_duration = 0;
			break;

		}
	}
}

void individual::generateAdverseEvents(double time)
{
	// Variables 
	double ae_probability, ae_frequency, ae_duration;

	for (int i = 0; i < adverse_event_n; i++)
	{
		switch (i)
		{
		default:
			ae_probability = 0;
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Emotional_abuse:
			ae_probability = 0;
			ae_frequency = 2;
			ae_duration = 0.25;

			// Add trend for bullying, we will assume the increase is tied to cyberbullying
			if (time > 0) ae_probability += 0.0758 * time * time - 0.0593 * time;

			break;

		case adverse_events_list::Social_media_use:
			ae_probability = 1;
			ae_frequency = time > -6 ? (0.6418 * time + 4.6791) / 2 : 0;
			ae_duration = time > -6 ? 60 * (0.0648 * time) + 1.2951 : 0;
			break;
		}
	}
}
void individual::generatePsychiatricDisorder() {}