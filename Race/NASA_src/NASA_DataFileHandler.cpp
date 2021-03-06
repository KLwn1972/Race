/*
* Team NASA Datendownload SRTM - Petra Stedile, Manuel Marks
* Zweck der Datei: Implementierung der Datenroutinen: Download, Unzip, L?schen und Hilfsroutinen
*/



#pragma once 
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <sstream>
#include <codecvt>
#include <regex>
#include <windows.h>
#include "../Race.h"

#ifdef CURL_ON
#include <curl\curl.h>
#endif // 


#include "NASA_DataFileHandler.h"
#include "NASA_constants.h"

using namespace std;
//namespace NASA {

	///////////////////////////////////////////////////////////////////
	//Funktion aus Aufgabe: H?heninformation f?r ganz Deutschland herunterladen 
	//Funktion l?uft als Standalone - Klassenfunktion
	//Funktion nicht notwendig f?r Rennsimulation, da Download automatisiert
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
		int error = 0;
		for (int long_i = longitude_start; long_i < longitude_end; long_i++) {
			for (int lat_i = latitude_start; lat_i < latitude_end; lat_i++) {
				cout << "Download Longitude: " << long_i << "\t Latitude: " << lat_i << endl;
				error = downloadNASAFile(long_i, lat_i);
				if (!error) {
					unzipNASAZipfile(long_i, lat_i);
				}
				deleteNASAZipfile(long_i, lat_i);
			}
		}
		return;
	}

	///////////////////////////////////////////////////////////////////
	//Basisfunktion f?r Download 
	//Nutzt CURL
	///////////////////////////////////////////////////////////////////
	int NASADataFileHandler::downloadNASAFile(int longitude, int latitude) {

		int ret = 0;
		string targetfile = createFilenamefromLongLat(longitude, latitude) + ".zip";
		string downloadurl = createDownloadURL(longitude, latitude);
		string ausgabedatei = createDownloadZielpfadFromCurrentPath() + targetfile;
		string cookiedatei = createDownloadZielpfadFromCurrentPath() + "NASA_cookies.txt";
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
#ifdef DEBUG
				//curl_easy_setopt(ptr_curl, CURLOPT_VERBOSE, 1L);					//Verbose Mode
				curl_easy_setopt(ptr_curl, CURLOPT_NOPROGRESS, 0);					//Activate Progress Meter
#endif
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
#ifdef DEBUG
					cerr << "Curl Error in NASADataFileHandler::downloadNASAFile() " << endl;
#endif
					ret = -1;
				}
			}
			curl_easy_cleanup(ptr_curl);		//Aufr?umen danach
#endif //CURL_ON
#ifndef CURL_ON
			cerr << "CURL_OFF: " << downloadurl << " cannot be downloaded.\nError in NASADataFileHandler::downloadNASAFile() --> Activate CURL in Race.h" << endl << endl;
			ret = -1;
#endif // !CURL_ON

		}
		else {
			cerr << "Filestream Error in NASADataFileHandler::downloadNASAFile() " << endl;
			ret = -1;
		}
		ausgabeDateiStream.close();
		return ret;
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion zum Entpacken
	///////////////////////////////////////////////////////////////////
	void NASADataFileHandler::unzipNASAZipfile(int longitude, int latitude) {
#ifdef CURL_ON

		//Entpacken mit sytemcall von 7Zip

#ifdef USE_WINDOWS
		string zipfile = createDownloadZielpfadFromCurrentPath() + createFilenamefromLongLat(longitude, latitude) + ".zip";
		string zipcall = createDownloadZielpfadFromCurrentPath() + "7z.exe e ";
		string ziptarget = " -aoa -o" + createDownloadZielpfadFromCurrentPath() +" -y >NUL";
#endif //USE_WINDOWS

#ifndef USE_WINDOWS   //Linux System
		string zipfile = createDownloadZielpfadFromCurrentPath() + createFilenamefromLongLat(longitude, latitude) + ".zip";
		string zipcall = " LINUX 7 ZIP CALL!!! "  ; //Auf Linux-System korrigieren / Testen
		string ziptarget = " -aoa -o" + createDownloadZielpfadFromCurrentPath();
#endif //!USE_WINDOWS
		std::cout << "7Zip Extract: \t" << zipfile << std::endl;

#ifdef DEBUG
		cout << "----------------Unzip----------------------------" << endl;
		cout << zipcall << zipfile << ziptarget << endl;
#endif
		system((zipcall + zipfile + ziptarget).c_str());
#endif //CURL_ON

#ifndef CURL_ON
		cerr << "CURL_OFF: Unnecessary / unintended function call NASADataFileHandler::unzipAndDeleteNASAFile() ";
#endif // !CURL_ON


		return;
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion zum Entpacken
	///////////////////////////////////////////////////////////////////
	void NASADataFileHandler::deleteNASAZipfile(int longitude, int latitude) {
#ifdef CURL_ON
#ifdef USE_WINDOWS
		string zipfile = NASADataFileHandler::createDownloadZielpfadFromCurrentPath() + createFilenamefromLongLat(longitude, latitude) + ".zip ";
#endif //USE_WINDOWS
#ifndef USE_WINDOWS   //Linux System
		string zipfile = NASADataFileHandler::createDownloadZielpfadFromCurrentPath() + createFilenamefromLongLat(longitude, latitude) + ".zip ";
#endif //!USE_WINDOWS
		cout << "Deleting File" << zipfile << endl;
		remove(zipfile.c_str());

#endif //CURL_ON
#ifndef CURL_ON
		cerr << "CURL_OFF: Unnecessary / unintended function call of NASADataFileHandler::unzipAndDeleteNASAFile() ";
#endif // !CURL_ON


		return;
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion: Verhindert unn?tige Downloads
	///////////////////////////////////////////////////////////////////
	bool NASADataFileHandler::checkLongitudeLatitudeinNEquadrant(int longitude, int latitude) {
		if (longitude > longitude_max || longitude < longitude_min || latitude > latitude_max || latitude < latitude_min) {
			return false;
		}
		else
			return true;
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion f?r die Erstellung der Datenfiles
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
	//Hilfsfunktion f?r Zusammensetzung des vollst?ndigen Downloadlinks
	///////////////////////////////////////////////////////////////////
	string NASADataFileHandler::createDownloadURL(int longitude, int latitude) {
		return nasa_url_base + createFilenamefromLongLat(longitude, latitude) + nasa_url_addon;
	}

	///////////////////////////////////////////////////////////////////
	//Hilfsfunktion f?r R?ckgabe Zielpfad fuer Download
	///////////////////////////////////////////////////////////////////
	string NASADataFileHandler::createDownloadZielpfadFromCurrentPath() {
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string current_dir = converter.to_bytes(std::wstring(buffer).substr(0, pos));
		std::string download_dir_bslsh = std::regex_replace(current_dir, std::regex(nasa_project_compilefolder), nasa_relative_download_zielpfad);
		std::string download_dir_slash = download_dir_bslsh;
		size_t start_pos = 0;
		while ((start_pos = download_dir_slash.find("\\", start_pos)) != std::string::npos) {
			download_dir_slash.replace(start_pos, 1, "/");
		}
		return download_dir_slash;
	}

	///////////////////////////////////////////////////////////////////
	//CURL callback Funktion: Realisierung mit c++ Bordmitteln: Filestream
	///////////////////////////////////////////////////////////////////
	size_t NASADataFileHandler::write_data_callback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
		((ofstream*)stream)->write((const char*)ptr, size * nmemb);
		return size * nmemb;
	}

//}