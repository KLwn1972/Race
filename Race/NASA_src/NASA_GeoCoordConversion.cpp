/*
* Team NASA Datendownload SRTM - Petra Stedile, Manuel Marks
* Zweck der Datei: Implementierung der Umrechnungsroutinenen für Koordinaten.
* Funktionen werden in den Download- /Auslese-Routinen verwendet --> Dateiname und Position des Datensatzes.
*/


#include "NASA_GeoCoordConversion.h"
#include "NASA_constants.h"
#include"../Race.h"

//namespace NASA {


	int GeoCoordConversion::getDeg_From_WGS84Decimal(double decimal) {
		return (int)decimal;
	}

	double GeoCoordConversion::getMin_From_WGS84Decimal(double decimal) {
		return decimal * 60;
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

//}
