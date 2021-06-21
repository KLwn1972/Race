#include <cstring>
#include <iostream>
#include "NASA_ElevationCalculator.h"

double ElevationCalculator::getElevationFromSRTM_SIRCdata(const double &longitude, const double &latitude) {
	cout << "Long: " << longitude << "\t Lat: " << latitude << endl;
	return 1.23456;
}

double ElevationCalculator::getElevationFromSRTM_XSARdata(const double &longitude, const double &latitude) {
	cout << "Long: " << longitude << "\t Lat: " << latitude << endl;
	return 9.87654;
}