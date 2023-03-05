#pragma once

enum class psychatric_disorder
{
	depression,
	suicide
};

class psy
{
public:
	psy();
	psy(int type, double start, double end);
	bool has(int type, double when);

private:
	int type;
	double start, end;
};