#ifndef _VECTOR_H		// Brauchen wir Vector.h nur hier?
#define _VECTOR_H

#include <iostream>
#include "Race.h"


using namespace std;


class CalcDatAuf{
public:
	vector<node> nodes;
	void DataProcessing();
	void InsertAdditionalNodes();
	void CalcFahrtbestimmungData();
	void CalcHorizontalCurveRad2(int index);
	void CalcVerticalCurveRad2(int index);
	void CalcGradientPercentage2(int index);
	vector <double> Get1Coordinate(int);
	double GetDistanceMeters(const double&, const double&, const double&, const double&);
};

class SplineDatAuf:public CalcDatAuf {
public:
	// SplineDatAuf() {} -> Konstruktor ist wann nötig?
	void DataProcessingCatmullRomSpline();
private:
	vector<double> Get4Coordinates(int index);
	double CalcGradient(int index);
	double CalcCurvature(int index);
	void insertPoint(int index_prePoint);
};

class RadiusDatAuf:public CalcDatAuf {
public:
	void DataProcessingRadiusGradient();
private:
	double CalcHorizontalCurveRad(int index);
	double CalcVerticalCurveRad(int index);
	double CalcGradientPercentage(int index);
};




#endif