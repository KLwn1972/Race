#pragma once
#include <cstring>
#include <iostream>

using namespace std;

class ElevationCalculator {
public:
	static double getElevationFromSRTM_SIRCdata(const double &longitude, const double &latitude);
	static double getElevationFromSRTM_XSARdata(const double &longitude, const double &latitude);
};
