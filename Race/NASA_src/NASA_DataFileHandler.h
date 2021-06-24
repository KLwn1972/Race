/*
* Team NASA Datendownload SRTM - Petra Stedile, Manuel Marks
* Zweck der Datei: Definition der Datenroutinen: Download, Unzip, Löschen und Hilfsroutinen in einer Klasse NASADataFileHandler
* Teilweise als Klassenroutinen
*/


#pragma once
#include <string>

using namespace std;
//namespace NASA {



	class NASADataFileHandler { 
	public:

		void downloadAreaElevationInfo_NASA_SIRC_between(int longitude_start, int longitude_end, int latitude_start, int latitude_end);
		void downloadElevationDataofGermany_NASA_SIRC();
		string createFilenamefromLongLat(int longitude, int latitude);
		static bool checkLongitudeLatitudeinNEquadrant(int longitude, int latitude);
		static string createDownloadZielpfadFromCurrentPath();

	private:

		//returns 0 in case of success, returns -1 in case of curl error
		int downloadNASAFile(int longitude, int latitude);

		void unzipNASAZipfile(int longitude, int latitude);
		void deleteNASAZipfile(int longitude, int latitude);
		string createDownloadURL(int longitude, int latitude);
		static size_t write_data_callback(void* ptr, size_t size, size_t nmemb, FILE* stream);
		//Nicht vorhandene Daten bei NASA resultieren in 1 KB zip Dateien im Pfad

	};

//}



