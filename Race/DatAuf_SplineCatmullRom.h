#pragma once

#include <iostream>
#include "Race.h"


using namespace std;


class SplineCatmullRom
{
public:
	double SplineKnots[4][3] = { 0 };
	//node SplineKnots[4];
	double InterpolKnot[3] = { 0 };
	double t = nan("");
	double CoeffValue[4] = { 0 };
	void InterpolKnotReset();
	void SplineKnotsReset();
	void CalcCoeffValue();
	void CalcInterpolKnot(double t);
};