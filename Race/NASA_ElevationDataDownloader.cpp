#pragma once
#include <cstring>
#include <iostream>
#include <fstream> 
#include <sstream>

#define CURL_STATICLIB
//#include <curl\curl.h>
#include "NASA_ElevationDataDownloader.h"

using namespace std;

string FileDownloader::createDownloadURL(int longitude, int latitude) {
	//return "http://e4ftl01.cr.usgs.gov/MEASURES/SRTMGL1.003/2000.02.11/N48E009.SRTMGL1.hgt.zip";
	return "https://e4ftl01.cr.usgs.gov/MEASURES/SRTMGL1.003/2000.02.11/N48E009.SRTMGL1.hgt.zip";
}


void FileDownloader::downloadFile(string downloadurl, string targetpath) {
	std::cout << "URL: \t" << downloadurl << std::endl;
	std::cout << "Ziepfad: \t" << targetpath << std::endl;
	/*
	downloadurl = "https://curl.se/libcurl/c/curl_easy_perform.html";
	stringstream curlTargetStream;
	CURL* ptr_curl;
	CURLcode curl_result;
	
	//Test Zielpfad
	string ausgabedatei = targetpath + "test1.txt";
	ofstream ausgabeDateiStream(ausgabedatei, ios_base::out | ios_base::trunc);


	ptr_curl = curl_easy_init();
	if (ptr_curl) {
		//URL setzen
		curl_easy_setopt(ptr_curl, CURLOPT_URL, downloadurl.c_str());
		//Verbose Mode
		//curl_easy_setopt(ptr_curl, CURLOPT_VERBOSE, 1L);
		//Activate Progress Meter
		curl_easy_setopt(ptr_curl, CURLOPT_NOPROGRESS, 0);
		
		//Login credentials
		curl_easy_setopt(ptr_curl, CURLOPT_USERNAME, "MMarks");
		curl_easy_setopt(ptr_curl, CURLOPT_PASSWORD, "MtMProjekt2");
		//curl_easy_setopt(ptr_curl, CURLOPT_USERPWD, "MMarks:MtMProjekt2");
		
		curl_easy_setopt(ptr_curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(ptr_curl, CURLOPT_MAXREDIRS, 1L);

		//Ausgabe Einrichten
		curl_easy_setopt(ptr_curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(ptr_curl, CURLOPT_WRITEDATA, &curlTargetStream);

		//Der eigentliche Download
		curl_result = curl_easy_perform(ptr_curl);
		ausgabeDateiStream << curlTargetStream.str();

		//Was passiert bei Fehlern?
		if (curl_result != CURLE_OK) {
			cout << "Curl Fehler";
		}

		//Aufräumen danach
		curl_easy_cleanup(ptr_curl);
	}
	ausgabeDateiStream.close();
	*/
}




void FileDownloader::downloadMultipleFile(int longitude_start, int longitude_end, int latitude_start, int latitude_end) {
	for (int i = longitude_start; i < longitude_end; i++) {
		for (int j = latitude_start; j < latitude_end; j++) {
			cout << "Longitude: " << i << "\t Latitude: " << j << endl;
			cout << createDownloadURL(i, j) << endl;
		}
	}
}


void FileDownloader::downloadGermany() {
	downloadMultipleFile(5, 16, 47, 56);
}




size_t FileDownloader::write_data(void* ptr, size_t size, size_t nmemb, void* stream) {
	string data((const char*)ptr, (size_t)size * nmemb);
	*((stringstream*)stream) << data << endl;
	return size * nmemb;
}
