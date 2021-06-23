#ifndef _VECTOR_H
#define _VECTOR_H

#include <iostream>
#include "DatAuf.h"
#include "Race.h"
#include "SplineCatmullRom.h"

using namespace std;

void SplineCatmullRom::CalcCoeffValue() {
	
	double t = this->t;

	this->CoeffValue[0] = -t * t * t + 2 * t * t - t;
	this->CoeffValue[1] = 3 * t * t * t - 5 * t * t + 2;
	this->CoeffValue[2] = -3 * t * t * t + 4 * t * t + t;
	this->CoeffValue[3] = t * t * t - t * t;
}

void SplineCatmullRom::CalcInterpolKnot(double t){
	//vector<node> NewKnot;
	this->t = t;
	this->InterpolKnotReset();

	this->CalcCoeffValue();

	// Begin: Temp values for Test
	this->InterpolKnot[0] = 1;
	this->InterpolKnot[1] = 2;
	this->InterpolKnot[2] = 3;
	// End: Temp Values for Test

	for (int i = 0;i < 4;i++) {
		this->InterpolKnot[0] += this->SplineKnots[i][0] * this->CoeffValue[i];
		this->InterpolKnot[1] += this->SplineKnots[i][1] * this->CoeffValue[i];
		this->InterpolKnot[2] += this->SplineKnots[i][2] * this->CoeffValue[i];
	}

	this->InterpolKnot[0] *= 0.5;
	this->InterpolKnot[1] *= 0.5;
	this->InterpolKnot[2] *= 0.5;


	return;
}

void SplineCatmullRom::InterpolKnotReset() {
	this->InterpolKnot[0] = 0.0;
	this->InterpolKnot[1] = 0.0;
	this->InterpolKnot[2] = 0.0;

	return;
}

void SplineCatmullRom::SplineKnotsReset() {
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 3;j++) {
			this->SplineKnots[i][j] = 0.0;
		}
	}

	return;
}

#endif