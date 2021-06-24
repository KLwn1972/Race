/*
* Team NASA Datendownload SRTM - Petra Stedile, Manuel Marks
* Zweck der Datei: Definition der Umrechnungsroutinenen für Koordinaten.
* Umsetzung als Klassenroutinen einer Hilfsklassenroutine
*/

#pragma once

//namespace NASA {

	//Goal:
	//Conversion in WGS84 Formats Decimal and Degree Minutes Seconds
	//Reine Umrechnungsmethoden --> Klassenmethoden --> Konstruktor gelöscht
	class GeoCoordConversion
	{
	public:
		GeoCoordConversion() = delete;
		static int getDeg_From_WGS84Decimal(double decimal);
		static double getMin_From_WGS84Decimal(double decimal);
		static double getSeconds_From_WGS84Decimal(double decimal);
		static double getDecimal_From_WGS84GradMinSec(int grad, int minutes, double seconds);
	};

//}