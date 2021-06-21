//#pragma once 
//#include <string>
//#include <iostream>
//#include <iomanip>
//#include <fstream> 
//#include <sstream>
//
//#include <curl\curl.h>
//#include "NASA_DataFileHandler.h"
//
//using namespace std;
//
////Download laed bis zu Max Breiten und Laengengrad, da Referencepunkt in SuedWest Ecke --> "<" der _end Werte
//void NASADataFileHandler::downloadAreaElevationInfo_NASA_SIRC_between(int longitude_start, int longitude_end, int latitude_start, int latitude_end) {
//	for (int long_i = longitude_start; long_i < longitude_end; long_i++) {
//		for (int lat_i = latitude_start; lat_i < latitude_end; lat_i++) {
//			cout << "Loading file for Longitude: " << long_i << "\t Latitude: " << lat_i << endl;
//			downloadNASAFile(long_i, lat_i);
//		}
//	}
//	deleteFailedDownloadZips();
//	for (int long_i = longitude_start; long_i < longitude_end; long_i++) {
//		for (int lat_i = latitude_start; lat_i < latitude_end; lat_i++) {
//			unzipNASAFile(long_i, lat_i);
//		}
//	}
//	return;
//}
//
//
//void NASADataFileHandler::downloadElevationDataofGermany_NASA_SIRC() {
//	downloadAreaElevationInfo_NASA_SIRC_between(5, 16, 47, 56);
//	return;
//}
//
//
//void NASADataFileHandler::downloadNASAFile(int longitude, int latitude) {
//	string targetfile = createFilenamefromLongLat(longitude, latitude) + ".zip";
//	string downloadurl = createDownloadURL(longitude, latitude);
//	std::cout << "Download to: \t" << targetfile << std::endl;
//	std::cout << "URL Source: \t" << downloadurl << std::endl;
//
//	CURL* ptr_curl;
//	CURLcode curl_result;
//
//	string ausgabedatei = download_zielpfad + targetfile;
//	string cookiedatei = download_zielpfad + "NASA_cookies.txt";
//
//	cout << ausgabedatei << endl;
//
//	//Filestreams
//	ofstream ausgabeDateiStream(ausgabedatei, ios_base::out | ios_base::binary | ios_base::trunc);
//
//	ptr_curl = curl_easy_init();
//
//	if (ptr_curl) {
//		curl_easy_setopt(ptr_curl, CURLOPT_URL, downloadurl.c_str());		//URL setzen
//		//curl_easy_setopt(ptr_curl, CURLOPT_VERBOSE, 1L);					//Verbose Mode
//		curl_easy_setopt(ptr_curl, CURLOPT_NOPROGRESS, 0);					//Activate Progress Meter
//
//		curl_easy_setopt(ptr_curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);			//Login credentials
//		curl_easy_setopt(ptr_curl, CURLOPT_USERPWD, "MMarks:MtMProjekt2");	//Login credentials
//
//		curl_easy_setopt(ptr_curl, CURLOPT_FOLLOWLOCATION, 1L);				//Redirects aktivieren
//		curl_easy_setopt(ptr_curl, CURLOPT_UNRESTRICTED_AUTH, 1L);			//Redirect fuer Weiterleitung aktivieren
//		curl_easy_setopt(ptr_curl, CURLOPT_MAXREDIRS, 5L);					//Redirects max
//
//		//Ausgaben Einrichten
//		curl_easy_setopt(ptr_curl, CURLOPT_WRITEFUNCTION, write_data_callback);
//		curl_easy_setopt(ptr_curl, CURLOPT_COOKIEJAR, cookiedatei.c_str());	//Unterschied Cookiejar cookiefile lesen
//		curl_easy_setopt(ptr_curl, CURLOPT_WRITEDATA, &ausgabeDateiStream);
//
//		curl_result = curl_easy_perform(ptr_curl);							//Der eigentliche Download
//
//
//		//Error Handling
//
//		if (curl_result != CURLE_OK) {
//			cout << "Curl Fehler";
//		}
//
//		curl_easy_cleanup(ptr_curl);		//Aufräumen danach
//	}
//
//	ausgabeDateiStream.close();
//}
//
//
//void NASADataFileHandler::unzipNASAFile(int longitude, int latitude) {
//	cout << "Zip zip" << endl;
//	string targetfile = createFilenamefromLongLat(longitude, latitude) + ".zip";
//	std::cout << "Unip File: \t" << targetfile << std::endl;
//	std::cout << "Path: \t" << download_zielpfad << std::endl;
//	string zip_pfad = download_zielpfad + targetfile;
//
//	return;
//}
//
//
//void NASADataFileHandler::deleteFailedDownloadZips() {
//	cout << "Loeschen" << endl;
//	return;
//}
//
//
//bool NASADataFileHandler::checkLongitudeLatitudeinNEquadrant(int longitude, int latitude) {
//	if (longitude > longitude_max || longitude < longitude_min || latitude > latitude_max || latitude < latitude_min) {
//		return false;
//	}
//	else
//		return true;
//}
//
//
//string NASADataFileHandler::createFilenamefromLongLat(int longitude, int latitude) {
//	if  (!checkLongitudeLatitudeinNEquadrant(longitude, latitude)){
//		cerr << "Coordinates ("<< longitude <<", " << latitude <<" out of available regions.\nDummy file name created" << endl;
//		return "DummyDepp.txt";
//	}
//	stringstream name;
//	name << setfill('0')<< "N" << setw(2)  << latitude << setw(0) << "E" << setw(3) << longitude ;
//	return name.str();
//}
//
//
//string NASADataFileHandler::createDownloadURL(int longitude, int latitude) {
//	string url = nasa_url_base;
//	url += createFilenamefromLongLat(longitude, latitude);
//	url += nasa_url_addon;
//	//cout << "URL created " << url << endl;
//	return url;
//}
//
//
//size_t NASADataFileHandler::write_data_callback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
//	static int sum = 0;
//	sum += size * nmemb; 
//	//string data((const char*)ptr, (size_t)size * nmemb);
//	//*((ofstream*)stream) << data;
//	((ofstream*)stream)->write((const char*)ptr, size * nmemb);
//	//printf("%d\n", sum);
//	return size * nmemb;
//}
//
