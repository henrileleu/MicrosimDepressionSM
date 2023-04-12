#include "individual.h"
#include "vars.h"
#include "parameters.h"

individual::individual() : date_of_birth(0), male(true)
{}


individual::individual(double _year, bool _male) : date_of_birth(_year), male(_male)
{}

void individual::history()
{
	// Generate history for patients:
	// 1. Adverse events that affect whole childhood
	// 2. Adverse events that punctualy affect childhood
	// 3. Use adverse events to estimate psychiatric events
	generateAdverseEvents();
	generateAdverseEvents(0.0);
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

bool individual::isMale() const
{
	return male;
}

double individual::getDate_of_birth() const
{
	return date_of_birth;
}

std::string individual::outputAdverseEvent() const
{
	if (adverseEvents.empty()) return std::to_string(getDate_of_birth()) + separator + std::to_string(isMale() ? 1 : 2) + ";;;\n";
	std::string out;
	for (std::vector<adverse_events>::const_iterator i = adverseEvents.begin(); i != adverseEvents.end(); i++)
	{
		out += std::to_string(getDate_of_birth()) + separator + std::to_string(isMale()?1:2) + separator + i->outputAdverseEvent();
	}

	return out;
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

	// Generate corralated rnd values
	double r[50];
	double corr[] = {	1.00,0.40,0.40,0.60,0.50,0.60,0.50,0.00,
						0.00,1.00,0.30,0.30,0.00,0.00,0.00,0.00,
						0.00,0.00,1.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,1.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,1.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,1.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,1.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00
	};

	rnd.correlated_rnd(r, corr, adverse_event_n);

	for (int i = 0; i < adverse_events_list::Bullying; i++)
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

		// Test if probability
		if (*(r+i) < ae_probability)
		{
			adverseEvents.push_back(adverse_events(i, 10.0, 18.0));
		}
	}

	//delete(r);
	
}

void individual::generateAdverseEvents(double time)
{
	// Variables 
	double ae_probability, ae_frequency, ae_duration;
	
	// Generate corralated rnd values
	double r[8];
	double corr[] = {	1.00,0.20,0.20,0.20,0.20,0.20,0.20,0.20,
						0.00,1.00,0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,1.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,1.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,1.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,1.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,1.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,1.00
	};

	rnd.correlated_rnd(r, corr, 8);

	for (int i = adverse_events_list::Bullying; i < adverse_event_n; i++)
	{
		switch (i)
		{
		default:
			ae_probability = 0;
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Bullying:
			ae_probability = 0;
			ae_frequency = 1;
			ae_duration = 0.25;

			// Run from 10 to 18
			for (int j = 10; j < 19; j++)
			{
				double year(date_of_birth + j);
				if (year < 2004) ae_probability = 0.05;
				else ae_probability = 0.05 + (year - 2004) * (male ? 0.0043 : 0.0071);

				if (*(r+j-10) < ae_probability / 3.0)
				{
					adverseEvents.push_back(adverse_events(i, j, j + 1.0));
				}
			}

			break;

		case adverse_events_list::Social_media_use:
			ae_probability = 1;
			
			// Run from 10 to 18
			for (int j = 10; j < 19; j++)
			{
				double year(date_of_birth + j);
				ae_frequency = year > 2004 ? (0.6418 * (year-2004) + 4.6791) / 2 : 0;
				ae_duration = year > 2004 ? 60 * (0.0648 * (year - 2004)) + 1.2951 : 0;

				if (ae_frequency <= 0) continue;

				// Add frequency and duration as param 2 & 3 of adverse events 
				adverseEvents.push_back(adverse_events(i, j, std::max(0.0,rnd.normal(ae_frequency, ae_frequency / 2.0))));
				adverseEvents.push_back(adverse_events(i+1, j, std::max(0.0,rnd.normal(ae_duration, ae_duration / 4.0))));
			}
			break;
		}

		
	}
}
void individual::generatePsychiatricDisorder() 
{
	// Baseline risk
	double baseline(-2.442347035);
	
	// -Ln of OR
	double ors[5] = { -0.086177696, 0.127833372, -0.285178942, -0.039220713, -0.086177696};

	// Init
	double vect_caract[adverse_event_n+1] = {};


	for (double i = 10.0; i < 19.0; i++)
	{
		double freq(0);
		double durr(0);

		double number_of_adversities(0);
		double risk_modifier(0);
		for (std::vector<adverse_events>::const_iterator it = adverseEvents.begin();
			it != adverseEvents.end(); it++)
		{
			// different for type
			int t = it->getType();
			switch (t)
			{
			case Parental_psychopathology:
			case Physical_abuse:
			case Emotional_abuse:
			case Sexual_abuse:
			case Neglect:
				risk_modifier += ors[t];
				number_of_adversities += 1.0;
				break;
			case Bullying:
				if (it->getStart() >= i && it->getStart() < i + 1.0) risk_modifier += -0.322083499;
				break;
			case Social_media_use:
				if (it->getStart() < i || it->getStart() >= i + 1.0) continue;
				freq = it->getEnd();
				if (freq > 2 && freq <= 4)  risk_modifier += -0.451075619;
				else if (freq > 4 && freq <= 6)  risk_modifier += -0.770108222;
				else if (freq > 6)  risk_modifier += -1.124929597;
				break;
			case Social_media_use+1:
				if (it->getStart() < i || it->getStart() >= i + 1.0) continue;
				freq = it->getEnd();
				if (freq > 30 && freq <= 60)  risk_modifier += 0.051293294;
				else if (freq > 60 && freq <= 120)  risk_modifier += -0.277631737;
				else if (freq > 120)  risk_modifier += -0.182321557;
				break;

			default:
				break;

			}

		}

		// Add final modifiers
		risk_modifier += number_of_adversities * -0.148420005;
		if (male) risk_modifier += 0.235722334;

		// Add baserisk
		risk_modifier += -baseline;

		// Estimate risk
		risk_modifier = 1 / (1 + exp(risk_modifier));

		if (rnd() < risk_modifier) psyDisorders.push_back(psy(0, i, i + 1));


		
	}




}