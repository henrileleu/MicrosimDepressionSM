#include "individual.h"

individual::individual() : date_of_birth(0), male(true), SNUse({}), physicalActivityProbability(0.0), physicalActivityData(), bullyData(), age_of_death(), psyDisorder(), depressionProbabilites(), 
chronicCondition(), obesity(), overweight(), tabaco(), alcohol(), cannabis()
{}


individual::individual(double _year, bool _male) : date_of_birth(_year), male(_male), SNUse({}), physicalActivityProbability(rnd()), physicalActivityData(), bullyData(), psyDisorder(),
chronicCondition(), obesity(), overweight(), tabaco(), alcohol(), cannabis()
{
	double ln_hr((_year - 2019) * p[pBackgroundMortalityGompertz_year] + male * p[pBackgroundMortalityGompertz_sex]);
	age_of_death = date_of_birth + rnd.gompertz(p[pBackgroundMortalityGompertz_a], p[pBackgroundMortalityGompertz_b], exp(ln_hr));
	rnd.normal(numberOfAgeGroups, depressionProbabilites, p[pBaselineDepressionRisk], 1);
}

void individual::history()
{
	// Generate history for patients:

	// 1. Adverse events that affect whole childhood
	generateAdverseEvents();

	// 2. Physical Activity
	physicalActivity PA = physicalActivity();
	PA.generatePhysicalActivity(physicalActivityData, date_of_birth, male, physicalActivityProbability); // True = Has PA > Guidelines

	// 3. Adverse events that punctualy affect childhood (only bullying for now)
	generateAdverseEvents(0.0);

	// 3bis. Chronic Physical Condition, Obesity, Substance Use
	 
	
	// 4. SN Use
	socialNetwork SNClass = socialNetwork();
	if (Options_ReverseCausation == false && noSN == false) SNUse = SNClass.getSNUsage(static_cast<int>(date_of_birth), male, physicalActivityProbability);

	// 5. Use adverse events to estimate psychiatric events
	generatePsychiatricDisorder();

	// 6. SN Use based on Dep Risk for Reverse Causation
	if (Options_ReverseCausation == true) SNUse = SNClass.SNUsageFromDepression(static_cast<int>(date_of_birth), male, depressionProbabilites);
}

std::string individual::outputAdverseEvent() const
{
	/*if (adverseEvents.empty()) return std::to_string(date_of_birth) + separator + std::to_string(male ? 1 : 2) + ";;;\n";
	std::string out;
	for (std::vector<adverse_events>::const_iterator i = adverseEvents.begin(); i != adverseEvents.end(); i++)
	{
		out += std::to_string(date_of_birth) + separator + std::to_string(male?1:2) + separator + i->outputAdverseEvent() + "\n";
	}

	return out;
	*/
	return std::string();

}

void individual::outputAll(long id, std::array < char[outputBufferSize], 4>& buffer) const
{
	std::array<std::string, 4> output = {};

	// Output the data in 3 separate strings that will be put in 3 separate tables with the id as a key
	// - Demographics
	// - Adverse Events
	// - Social Media Use
	// - Psy disorder

	// Demographics
	sprintf_s(buffer[0], "%x;%.0f;%.2f;%i\n", id, date_of_birth, age_of_death, male ? 1 : 2);

	

	// Adverse Events
	bool first(true);
	char* b(buffer[1]);
	size_t n(0);
	for (std::vector<adverse_events>::const_iterator i = adverseEvents.begin(); i != adverseEvents.end(); i++)
	{
		aes AE(i->outputAdverseEvent());
		n += sprintf_s(b+n, outputBufferSize-n, "%x;%d;%.0f;%.0f\n", first?id:0, AE.type, AE.start, AE.end);
		first = false;
	}
	// PA
	// Cleaning up by merging similar year;
	bool t(physicalActivityData[0]);
	int start(0);
	int end(0);
	for (int i = 0; i < numberOfAgeGroups; i++)
	{
		if (t != physicalActivityData[i + 1] || i == (numberOfAgeGroups - 1))
		{
			n += sprintf_s(b + n, outputBufferSize - n, "%x;%d;%d;%d\n", first ? id : 0, t ? 101 : 100, 10 + start, 10 + end);
			start = i + 1; end = i + 1; t = physicalActivityData[i + 1];
			first = false;
		}
		else
		{
			end = i + 1;
		}
  	}
	// Bully
	t = (bullyData[0]==1);
	start = 0;
	end = 0;
	for (int i = 0; i < numberOfAgeGroups; i++)
	{
		if (t != (bullyData[i + 1]==1) || i == (numberOfAgeGroups - 1))
		{
			n += sprintf_s(b + n, outputBufferSize - n, "%x;%d;%d;%d\n", first ? id : 0, t ? 201 : 200, 10 + start, 10 + end);
			start = i + 1; end = i + 1; t = bullyData[i + 1];
			first = false;
		}
		else
		{
			end = i + 1;
		}
	}
	
	
	// SN
	first  = true;
	b = buffer[2];
	n = 0;
	for (int i = 0; i < numberOfAgeGroups; i++)
	{
		n += sprintf_s(b + n, outputBufferSize - n, "%x;%d;%d;%.2f\n", first ? id : 0, i + 10, SNUse[i].n, SNUse[i].time);
		first = false;
	}
	
	
	// Psy Disorder
	first = true;
	b = buffer[3];
	n = 0; 
	for (std::vector<psyEpisode>::const_iterator i = psyDisorder.begin(); i != psyDisorder.end(); i++)
	{
		n += sprintf_s(b + n, outputBufferSize - n, "%x;%d;%.2f;%.2f\n", first ? id : 0, i->type, 10 + i->start, 10 + i->end);
		first = false;
	}
	
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
	double r[5];
	double corr[] = {	1.00,0.40,0.80,0.10,0.10,
						0.00,1.00,0.70,0.10,0.10,
						0.00,0.00,1.00,0.10,0.10,
						0.00,0.00,0.00,1.00,0.10,
						0.00,0.00,0.00,0.00,1.00		
	};

	rnd.correlated_rnd(r, corr, adverse_event_n);

	// Fixed events for adversities
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
			ae_probability = p[pParental_psychopathology_Pr];
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Physical_abuse:
			ae_probability = p[pPhysical_abuse_Pr];
			ae_frequency = 0;
			ae_duration = 0;
			break;
		
		case adverse_events_list::Emotional_abuse:
			ae_probability = p[pEmotional_abuse_Pr];
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Sexual_abuse:
			ae_probability = p[pSexual_abuse_Pr];
			ae_frequency = 0;
			ae_duration = 0;
			break;

		case adverse_events_list::Neglect:
			ae_probability = p[pNeglect_Pr];
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
	double ae_probability(male ? p[pBullyingMale] : p[pBullyingFemale]);
	
	// Generate corralated rnd values, physical activity is not correlated
	double r[9];
	double corr[] = {	1.00,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20,
						0.00,1.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,1.00,0.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,1.00,0.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,1.00,0.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,1.00,0.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,1.00,0.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,1.00,0.00,
						0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,1.00
	};

	rnd.correlated_rnd(r, corr, 9);

	// Includes changes over time in bullying
	for (int j = 10; j < 19; j++)
	{
		//double year(date_of_birth + j);
		
		if (*(r + j - 10) < ae_probability)
		{
			bullyData[j - 10] = 1;
		}
	}
}

void individual::generatePsychiatricDisorder() 
{
	indCararcteristics param = { male, date_of_birth };

	// Mood
	moodDisorder generator_a;
	generator_a.generateEpisode(param, physicalActivityData, bullyData, adverseEvents, SNUse, psyDisorder, depressionProbabilites);

	// Anxiety
	anxietyDisorder generator_b;
	generator_b.generateEpisode(param, physicalActivityData, bullyData, adverseEvents, SNUse, psyDisorder);

	// Mood
	SuicideAttemps generator_c;
	generator_c.generateEpisode(param, psyDisorder);

}

void individual::generateOtherFactors()
{
	// Assumes full independance for simplicity at this stage
	// Fill the arrays for age 10 to 18
	
	double EventProbability(0.0);
	double basetime(date_of_birth+8); // Model starts at age 10, data is for age 12 (3e)

	double prob(rnd());

	// Chronic Pysical Condition
	double a(male ? p[pChronicConditionMale_a] :p[pChronicConditionFemale_a]);
	double b(male ? p[pChronicConditionMale_b] :p[pChronicConditionFemale_b]);
	for (int j = 0; j < numberOfAgeGroups; j++)
	{
		EventProbability = a * (basetime + j) + b;
		chronicCondition[j] = (EventProbability > prob);
	}

	// Obesity & Overweight; Same prob
	prob = rnd();
	a = (male ? p[pObesityMale_a] : p[pObesityFemale_a]);
	b = (male ? p[pObesityMale_b] : p[pObesityFemale_b]);
	double a1(male ? p[pOverweightMale_a] : p[pOverweightFemale_a]);
	double b1(male ? p[pOverweightMale_a] : p[pOverweightFemale_a]);
	for (int j = 0; j < numberOfAgeGroups; j++)
	{
		double p2(prob);
				
		EventProbability = a * (basetime + j) + b;
		obesity[j] = EventProbability > p2;

		p2 -= EventProbability;
		EventProbability = a1 * (basetime + j) + b1;
		overweight[j] = (EventProbability > p2);
	}

	// Alcohol
	prob = rnd();
	a = (male ? p[pAlcoholMale_a] : p[pAlcoholFemale_a]);
	b = (male ? p[pAlcoholMale_b] : p[pAlcoholFemale_b]);
	std::vector<bool> Alcohol;
	for (int j = 0; j < numberOfAgeGroups; j++)
	{
		EventProbability = a * (basetime + j) + b;
		alcohol[j] = (EventProbability > prob);
	}


	prob = rnd();
	a = (male ? p[pTabacoMale_a] : p[pTabacoFemale_a]);
	b = (male ? p[pTabacoMale_b] : p[pTabacoFemale_b]);
	std::vector<bool> Tabaco;
	for (int j = 0; j < numberOfAgeGroups; j++)
	{
		EventProbability = a * (basetime + j) + b;
		tabaco[j] = (EventProbability > prob);
	}

	prob = rnd();
	a = (male ? p[pCannabisMale_a] : p[pCannabisFemale_a]);
	b = (male ? p[pCannabisMale_b] : p[pCannabisFemale_b]);
	std::vector<bool> Cannabis;
	for (int j = 0; j < numberOfAgeGroups; j++)
	{
		EventProbability = a * (basetime + j) + b;
		cannabis[j] = (EventProbability > prob);
	}
}
