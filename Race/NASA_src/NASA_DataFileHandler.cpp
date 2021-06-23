#pragma once 
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <sstream>
#include "../Race.h"

#ifdef CURL_ON
#include <curl\curl.h>
#endif // 


#include "NASA_DataFileHandler.h"
#include "NASA_constants.h"

using namespace std;
//namespace NASA {

	///////////////////////////////////////////////////////////////////
	//Funktion aus Aufgabe: Höheninformation für ganz Deutschland herunterladen 
	//Funktion läuft als Standalone - Klassenfunktion
	//Funktion nicht notwendig für Rennsimulation, da Download automatisiert
	///////////////////////////////////////////////////////////////////
	void NASADataFileHandler::downloadElevationDataofGermany_NASA_SIRC() {
		downloadAreaElevationInfo_NASA_SIRC_between(5, 16, 47, 56);
		return;
	}

	///////////////////////////////////////////////////////////////////
	//Download laed bis zu Max Breiten- und Laengengrad, da Referencepunkt in SuedWest Ecke --> "<" der _end Werte
	//Klassenfunktion Standalone
	///////////////////////////////////////////////////////////////////
	void NASADataFileHandler::downloadAreaElevationInfo_NASA_SIRC_between(int longitude_start, int longitude_end, int latitude_start, int latitude_end) {
		for (int long_i = longitude_start; long_i < longitude_end; long_i++) {
			for (int lat_i = latitude_start; lat_i < latitude_end; lat_i++) {
				cout << "Dowload Longitude: " << long_i << "\t Latitude: " << lat_i << endl;
				downloadNASAFile(long_i, lat_i);
				unzipAndDeleteNASAZipfile(long_i, lat_i);
			}
		}
		return;
	}

	///////////////////////////////////////////////////////////////////
	//Basisfunktion für Download 
	//Nutzt CURL
	///////////////////////////////////////////////////////////////////
	void NASADataFileHandler::downloadNASAFile(int longitude, int latitude) {
		string targetfile = createFilenamefromLongLat(longitude, latitude) + ".zip";
		string downloadurl = createDownloadURL(longitude, latitude);
		string ausgabedatei = nasa_download_zielpfad + targetfile;
		string cookiedatei = nasa_download_zielpfad + "NASA_cookies.txt";
#ifdef DEBUG
		std::cout << "Download to: \t" << ausgabedatei << std::endl;
		std::cout << "URL Source: \t" << downloadurl << std::endl;
#endif
		//Filestreams
		ofstream ausgabeDateiStream(ausgabedatei, ios_base::out | ios_base::binary | ios_base::trunc);

		if (ausgabeDateiStream.good()) {
#ifdef CURL_ON
			// CURL initialization
			CURL* ptr_curl;
			CURLcode curl_result;
			ptr_curl = curl_easy_init();

			if (ptr_curl) {
				curl_easy_setopt(ptr_curl, CURLOPT_URL, downloadurl.c_str());		//URL setzen
				//curl_easy_setopt(ptr_curl, CURLOPT_VERBOSE, 1L);					//Verbose Mode
				curl_easy_setopt(ptr_curl, CURLOPT_NOPROGRESS, 0);					//Activate Progress Meter
				curl_easy_setopt(ptr_curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);			//Login credentials
				curl_easy_setopt(ptr_curl, CURLOPT_USERPWD, nasa_usr_namepwd.c_str());	//Login credentials
				curl_easy_setopt(ptr_curl, CURLOPT_FOLLOWLOCATION, 1L);				//Redirects aktivieren
				curl_easy_setopt(ptr_curl, CURLOPT_UNRESTRICTED_AUTH, 1L);			//Redirect fuer Weiterleitung aktivieren
				curl_easy_setopt(ptr_curl, CURLOPT_MAXREDIRS, 5L);					//Redirects max
				curl_easy_setopt(ptr_curl, CURLOPT_WRITEFUNCTION, write_data_callback);//Ausgaben Einrichten
				curl_easy_setopt(ptr_curl, CURLOPT_COOKIEJAR, cookiedatei.c_str());	//Unterschied Cookiejar cookiefile lesen
				curl_easy_setopt(ptr_curl, CURLOPT_WRITEDATA, &ausgabeDateiStream);

				curl_result = curl_easy_perform(ptr_curl);							//Der eigentliche Download

				//Error Handling
				if (curl_result != CURLE_OK) {
					cerr << "Curl Error in NASADataFileHandler::downloadNASAFile " << endl;
				}
			}
			curl_easy_cleanup(ptr_curl);		//Aufräumen danach
#endif //CURL_ON
#ifndef CURL_ON
			cerr << "CURL_OFF: " << downloadurl << " cannot be downloaded.\nError in ASADataFileHandler::downloadNASAFile() --> Activate CURL in Race.h" << endl << endl;
#endif // !CURL_ON

		}
		else {
			cerr << "Filestream Error in NASADataFileHandler::downloadNASAFile() " << endl;
		}
		ausgabeDateiStream.close();

	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion zum Aufräumen
	///////////////////////////////////////////////////////////////////
	void NASADataFileHandler::unzipAndDeleteNASAZipfile(int longitude, int latitude) {
#ifdef CURL_ON

		//Entpacken mit sytemcall von 7Zip

#ifdef USE_WINDOWS
		string zipfile = nasa_download_zielpfad + createFilenamefromLongLat(longitude, latitude) + ".zip";
		string zipcall = nasa_download_zielpfad + "7z.exe e ";
		string ziptarget = " -aoa -o" + nasa_download_zielpfad +" -y >NUL";
		string zipfile_del_call = "del \"" + nasa_download_zielpfad_win + createFilenamefromLongLat(longitude, latitude) + ".zip \"";
#endif //USE_WINDOWS

#ifndef USE_WINDOWS   //Linux System
		string zipfile = nasa_download_zielpfad + createFilenamefromLongLat(longitude, latitude) + ".zip";
		string zipcall = " LINUX 7 ZIP CALL!!! "  ; //Auf Linux-System korrigieren / Testen
		string ziptarget = " -aoa -o" + nasa_download_zielpfad;
		string zipfile_del_call = "LINUX RM XYZ.ZIP CALL"; //Auf Linux-System korrigieren / Testen
#endif //!USE_WINDOWS
		std::cout << "7Zip Extract: \t" << zipfile << std::endl;

#ifdef DEBUG
		cout << "----------------Unzip----------------------------" << endl;
		cout << zipcall << zipfile << ziptarget << endl;
		cout << (zipfile_del_call.c_str()) << endl;
#endif
		system((zipcall + zipfile + ziptarget).c_str());
		system(zipfile_del_call.c_str());
		cout << "Delete: " << zipfile  << endl << endl;

#endif //CURL_ON


#ifndef CURL_ON
		cerr << "CURL_OFF: Unnecessary / unintended function call NASADataFileHandler::unzipAndDeleteNASAFile();
#endif // !CURL_ON


		return;
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion: Verhindert unnötige Downloads
	///////////////////////////////////////////////////////////////////
	bool NASADataFileHandler::checkLongitudeLatitudeinNEquadrant(int longitude, int latitude) {
		if (longitude > longitude_max || longitude < longitude_min || latitude > latitude_max || latitude < latitude_min) {
			return false;
		}
		else
			return true;
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion für die Erstellung der Datenfiles
	///////////////////////////////////////////////////////////////////
	string NASADataFileHandler::createFilenamefromLongLat(int longitude, int latitude) {
		if (!checkLongitudeLatitudeinNEquadrant(longitude, latitude)) {
			cerr << "Coordinates (" << longitude << ", " << latitude << " out of available regions.\nDummy file name created" << endl;
			return "FehlerinCreateFilenamefromLongLat.txt";
		}
		stringstream name;
		name << setfill('0') << "N" << setw(2) << latitude << setw(0) << "E" << setw(3) << longitude;
		return name.str();
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion für Zusammensetzung des vollständigen Downloadlinks
	///////////////////////////////////////////////////////////////////
	string NASADataFileHandler::createDownloadURL(int longitude, int latitude) {
		return nasa_url_base + createFilenamefromLongLat(longitude, latitude) + nasa_url_addon;
	}

	///////////////////////////////////////////////////////////////////
	//CURL callback Funktion: Realisierung mit c++ Bordmitteln: Filestream
	///////////////////////////////////////////////////////////////////
	size_t NASADataFileHandler::write_data_callback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
		((ofstream*)stream)->write((const char*)ptr, size * nmemb);
		return size * nmemb;
	}

//}