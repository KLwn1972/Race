#include <cstring>
#include <iostream>
#include <fstream> 
#include "../Race.h"
#include "NASA_constants.h"
#include "NASA_ElevationCalculator.h"
#include "NASA_DataFileHandler.h"
#include "NASA_GeoCoordConversion.h"


using namespace std;
//namespace NASA {

///////////////////////////////////////////////////////////////////////////////////
// Rückgabe:
//	- Höhe in Metern	wenn hgt file vorhanden
//	- 0,				wenn innerhalb möglicher SRTM Daten, aber File nicht vorhanden --> Meer
//	- -32768			falls Anfrage außerhalb NordOst-Quadrant
///////////////////////////////////////////////////////////////////////////////////

	double HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(const double& longitude, const double& latitude) {
		double elevation;

		if (NASADataFileHandler::checkLongitudeLatitudeinNEquadrant((int)longitude, (int)latitude) ) {

			//Vorkomma Koordinaten --> Dateiidentifikation
			int long_deg = GeoCoordConversion::getDeg_From_WGS84Decimal(longitude);
			int lat_deg = GeoCoordConversion::getDeg_From_WGS84Decimal(latitude);

			//gerundeter Nachkommateil in Sec für Position in Datei
			double delta_sec_long = GeoCoordConversion::getSeconds_From_WGS84Decimal(longitude - (double)long_deg);
			double delta_sec_lat = GeoCoordConversion::getSeconds_From_WGS84Decimal(latitude - (double)lat_deg);

			NASADataFileHandler filehandler;
			string sourcefilename = filehandler.createFilenamefromLongLat(long_deg, lat_deg) + ".hgt";

#ifdef DEBUG
			cout << "Lese Hoeheninfo [" << longitude << ", " << latitude << "] aus " << nasa_download_zielpfad + sourcefilename << endl;
#endif
			if (!checkIfFileExists(sourcefilename)) {
				cout << sourcefilename << " not available. Download starting ..." << endl;
#ifdef CURL_ON
				filehandler.downloadAreaElevationInfo_NASA_SIRC_between(long_deg, long_deg + 1, lat_deg, lat_deg + 1);
#endif
#ifndef CURL_ON
				cerr << "CURL_OFF: Missing " << sourcefilename << " cannot be downloaded. \nError in HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata() --> Activate CURL in Race.h" << endl << endl;
#endif // !CURL_ON
			}
			if (checkIfFileExists(sourcefilename)) { //Nachgezogener Download erfolgreich?
				elevation = readSingleElevationValueFromFile(delta_sec_long, delta_sec_lat, sourcefilename);
			}
			else { //File wirklich nicht verfuegbar  -- > Gebiet im Meer 
				elevation = 0.0;
			}
		}
		else {
			elevation = -32768.0;
			cerr << "Request outside of NE quadrant: Longitude - " << longitude << " Latitude - " << latitude << endl;
			cerr << "Limits: Longitude [" << longitude_min << "," << longitude_max << "], Latitiude[" << latitude_min << "," << latitude_max << "]" << endl; 
			cerr << "Returned INT_MIN: " << elevation << endl;
		}
		return elevation;
	}


	double HGT_ElevationCalculator::readSingleElevationValueFromFile(double& longitude_deltasec, double& latitude_deltasec, string filename) {
		double elevationvalue = -32768.0;
		// Position Zielwert in eingelesenem File bestimmen (Untere linke Ecke bestimmt Dateinamen, Werte in Matrix in Lattitude gedreht
		int pos_longitude = (int)(longitude_deltasec + 0.5);
		int pos_latitude = (int)((srtm_size - 1 - latitude_deltasec) + 0.5);

#ifdef DEBUG
		cout << "Longitude Dsec: " << longitude_deltasec << endl;
		cout << "Latitude Dsec: " << latitude_deltasec << endl;
		cout << "Longitude File Pos: " << pos_longitude << endl;
		cout << "Latitude File Pos: " << pos_latitude << endl;
		cout << "Reading single HGT data spot" << endl;
#endif // DEBUG

		fstream hgtfile;
		string zieldatei = nasa_download_zielpfad + filename;
		hgtfile.open(zieldatei.c_str(), ios_base::in | ios_base::binary);

		if (!hgtfile.good()) {
			cerr << "Error reading " << zieldatei << " in HGT_ElevationCalculator::readElevationFromFile(): " << endl;
			cerr << "Returning INT_MIN: " << elevationvalue << endl;
		}
		else {
			unsigned char buffer[2] = { 0 };
			size_t datapos = sizeof(buffer) * ((pos_latitude * srtm_size) + pos_longitude);
			hgtfile.seekg(datapos, ios_base::beg);
			hgtfile.read(reinterpret_cast<char*> (buffer), sizeof(buffer));
			elevationvalue = (double)((buffer[0] << 8) | buffer[1]);
		}
		hgtfile.close();
		return elevationvalue;
	}


	bool HGT_ElevationCalculator::checkIfFileExists(string filename) {
		fstream file;
		string zieldatei = nasa_download_zielpfad + filename;
		file.open(zieldatei, ios_base::in);
		if (file.good()) {
			file.close();
			return true;
		}
		else {
			file.close();
			return false;
		}
	}


	//Backup: Einlesen des kompletten Arrays für ggf. Debugging
	 
	// 	   	__int16 srtm_data[srtm_size][srtm_size] = { 0 }; //Platz auf dem Heap, zu groß für Stack
	// 
	//	double HGT_ElevationCalculator::readElevationFromEntireFile(double& longitude_deltasec, double& latitude_deltasec, string filename) {
	//		double elevationvalue = -32768.32768;
	//		// Position Zielwert in eingelesenem File bestimmen (Untere linke Ecke bestimmt Dateinamen, Werte in Matrix in Lattitude gedreht
	//		int pos_longitude = (int)(longitude_deltasec + 0.5);
	//		int pos_latitude = (int)((srtm_size - 1 - latitude_deltasec) + 0.5);
	//
	//#ifdef DEBUG
	//		cout << "Longitude Dsec: " << longitude_deltasec << endl;
	//		cout << "Latitude Dsec: " << latitude_deltasec << endl;
	//		cout << "Longitude File Pos: " << pos_longitude << endl;
	//		cout << "Latitude File Pos: " << pos_latitude << endl;
	//#endif // DEBUG
	//
	//		fstream hgtfile;
	//		string zieldatei = nasa_download_zielpfad + filename;
	//		hgtfile.open(zieldatei.c_str(), ios_base::in | ios_base::binary);
	//
	//		if (!hgtfile.good()) {
	//			cerr << "Error reading " << zieldatei << " in HGT_ElevationCalculator::readElevationFromFile(): " << endl;
	//			cerr << "Returning Default Value: " << elevationvalue << endl;
	//		}
	//		else {
	//			unsigned char buffer[2] = { 0 };
	//#ifdef DEBUG
	//			cout << "Reading full HGT data :" << endl;
	//#endif // DEBUG
	//			for (int r_y_lat = 0; r_y_lat < srtm_size; r_y_lat++) {
	//				for (int c_x_long = 0; c_x_long < srtm_size; c_x_long++) {
	//					if (!hgtfile.read(reinterpret_cast<char*>(buffer), sizeof(buffer)))
	//					{
	//						std::cout << "Error reading file " << zieldatei << " in HGT_ElevationCalculator::readElevationFromFile():" << std::endl;
	//						cerr << "Returning Default Value: " << elevationvalue << endl;
	//						return elevationvalue;
	//					}
	//					srtm_data[c_x_long][r_y_lat] = (buffer[0] << 8) | buffer[1];
	//				}
	//
	//			}
	//		}
	//		hgtfile.close();
	//		elevationvalue = (double)srtm_data[pos_longitude][pos_latitude];
	//		return elevationvalue;
	//	}


//}