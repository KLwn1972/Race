#pragma once
#include <string>

using namespace std;




class NASADataFileHandler {
public:

	void downloadAreaElevationInfo_NASA_SIRC_between(int longitude_start, int longitude_end, int latitude_start, int latitude_end);
	void downloadElevationDataofGermany_NASA_SIRC();

private:

	void downloadNASAFile(int longitude, int latitude);
	void deleteFailedDownloadZips();
	void unzipNASAFile(int longitude, int latitude);
	
	bool checkLongitudeLatitudeinNEquadrant(int longitude, int latitude);
	string createFilenamefromLongLat(int longitude, int latitude);
	string createDownloadURL(int longitude, int latitude);
	static size_t write_data_callback(void* ptr, size_t size, size_t nmemb, FILE* stream);
	//Nicht vorhandene Daten bei NASA resultieren in 1 KB zip Dateien im Pfad

	//Noch unglücklich Lösung: Generelle Variablen als Klassenkonstanten
	//Dateimanagement 
	//const string download_zielpfad = "D:/MtM/M_Projekt2/ConceptNASAImport/Load/";
	const string download_zielpfad = "../Load/";
	const string nasa_url_base = "https://e4ftl01.cr.usgs.gov/MEASURES/SRTMGL1.003/2000.02.11/";
	const string nasa_url_addon = ".SRTMGL1.hgt.zip";
	//limits in den NASA Maps
	const int longitude_max = 59;
	const int longitude_min = 0;
	const int latitude_max = 179;
	const int latitude_min = 0;
};



