#include "adverse_events.h"
#include "vars.h"

adverse_events::adverse_events(): type(INT_MAX), start(0.0), end(0.0)
{}

adverse_events::adverse_events(int type, double start, double end) : type(type), start(start), end(end)
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

std::string adverse_events::outputAdverseEvent() const
{
	return std::to_string(type) + separator + std::to_string(start) + separator + std::to_string(end) + "\n";
}
