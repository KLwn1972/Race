#include "NASA_GeoCoordConversion.h"


int GeoCoordConversion::getGrad_From_WGS84Decimal(double decimal) {
	return (int) decimal;
}

double GeoCoordConversion::getMin_From_WGS84Decimal(double decimal) {
	return decimal*60;
}

double GeoCoordConversion::getSeconds_From_WGS84Decimal(double decimal) {
	return decimal * 60 * 60;
}

double GeoCoordConversion::getDecimal_From_WGS84GradMinSec(int grad, int minutes, double seconds) {
	double decimal;
	decimal = (double)grad;
	decimal += (double)minutes / 60;
	decimal += (double)seconds / 3600;
	return decimal;
}


