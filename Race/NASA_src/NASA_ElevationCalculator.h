/*
* Team NASA Datendownload SRTM - Petra Stedile, Manuel Marks
* Zweck der Datei: Definition der HGT-Auswertung. Ggf. werden bei aktiviertem CURL HGT nachgeladen.
* Lösung als Klasse NASADataFileHandler mit Klassenroutine
*/

#pragma once
#include <cstring>
#include <iostream>

//Informationen aus HGT müssen auf Heap, zu gross fur Stack

//__int16 srtm_data[SRTM_SIZE][SRTM_SIZE] = { 0 };

using namespace std;
//namespace NASA {

	extern const string  nasa_download_zielpfad;
	extern const string  nasa_url_base;
	extern const string  nasa_url_addon; 

	class HGT_ElevationCalculator {
	public:
		static double getElevationFromSRTM_SIRCdata(const double& longitude, const double& latitude);

	private:
		static bool checkIfFileExists(string filename);
		static double readSingleElevationValueFromFile(double &longitude_deltasec, double &latitude_deltasec, string filename);

		//Für Debuggingzwecke behalten, Funktion liest vollständige HGT ein
		//static double readElevationFromEntireFile(double &longitude_deltasec, double &latitude_deltasec, string filename);

	};

//}