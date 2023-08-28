#include "adverse_events.h"

adverse_events::adverse_events(): type(INT_MAX), start(0.0), end(0.0), frequence(0.0), duration(0.0)
{}

adverse_events::adverse_events(int type, double start, double end) : type(type), start(start), end(end), frequence(0.0), duration(0.0)
{}

adverse_events::adverse_events(int type, double start, double end, double frequence, double duration) : type(type), start(start), end(end), frequence(frequence), duration(duration)
{}

bool adverse_events::has(int _type, double _when) const
{
	return (_type == type) && (_when >= start) && (_when < end);
}

double adverse_events::returnOR(double _when) const
{
	return (_when >= start) && (_when < end) ? p[vars_list::pParental_psychopathology_OR + type] : 1;
}

int adverse_events::getType() const
{
	return type;
}

double adverse_events::getStart() const
{
	return start;
}

double adverse_events::getEnd() const
{
	return end;
}

double adverse_events::getFrequence() const
{
	return frequence;
}

double adverse_events::getDuration() const
{
	return duration;
}

aes adverse_events::outputAdverseEvent() const
{
	return aes({ type, start, end });
}
