#pragma once

#include <iostream>
#include "Race.h"
#include "DatAuf.h"

using namespace std;


class SplineCatmullRom
{
public:
	double SplineKnots[4][3];
	double InterpolKnot[3];
	double t = nan("");
	double CoeffValue[4];
	void InterpolKnotReset();
	void SplineKnotsReset();
	void CalcCoeffValue();
	void CalcInterpolKnot(double t);
};


// brauchen wir nicht

