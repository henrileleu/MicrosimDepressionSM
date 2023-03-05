#include "psy.h"
#include "vars.h";

psy::psy() : type(INT_MAX), start(0.0), end(0.0)
{}

psy::psy(int type, double start, double end) : type(type), start(start), end(end)
{}

bool psy::has(int _type, double _when)
{
	return (_type == type) && (_when >= start) && (_when < end);
}