#pragma once
#include <iostream>
#include "DatAuf.h"
using namespace std;




double CalcDatAuf::GetDistanceMeters(const double& longitude1, const double& latitude1, const double& longitude2, const double& latitude2) {

	return 2;
}


void CalcDatAuf::DataProcessing() {
	double distance = this->GetDistanceMeters(1, 2, 3, 4);
	cout << "Distanz: " << distance << endl;

}