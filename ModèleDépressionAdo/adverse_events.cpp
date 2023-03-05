#include "adverse_events.h"
#include "vars.h"

adverse_events::adverse_events(): type(INT_MAX), start(0.0), end(0.0)
{}

adverse_events::adverse_events(int type, double start, double end) : type(type), start(start), end(end)
{}

bool adverse_events::has(int _type, double _when)
{
	return (_type == type) && (_when >= start) && (_when < end);
}

double adverse_events::returnOR(double _when)
{
	return (_when >= start) && (_when < end) ? p[vars_list::pParental_psychopathology_OR + type] : 1;
}