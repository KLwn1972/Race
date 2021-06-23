/*
#include <iostream>
#include "DatAuf.h"
#include "Race.h"

using namespace std;



void CalcDatAuf::DataProcessing() {
	// Dummy-Funktion
	
	cout << "DatAuf: DataProcessing aufgerufen" << endl;

	// Check and insert if necessary additional knots
	this->InsertAdditionalNodes();

	// Calculate Data for SOLL-Fahrtbestimmmung 
	this->CalcFahrtbestimmungData();

}

vector<double> CalcDatAuf::Get1Coordinate(int index) {
	// Dummy-Funktion
	vector<double> oneNode;
	oneNode.push_back(nodes[index].longitude);
	oneNode.push_back(nodes[index].latitude);
	oneNode.push_back(nodes[index].elevation);
	cout << "DatAuf: GetCoordinates-Funktion wurde aufgerufen."<<endl;
	return oneNode;
}


double CalcDatAuf::GetDistanceMeters(const double& longitude1, const double& latitude1, const double& longitude2, const double& latitude2) {	// double reicht aus oder? -> nodes[index].longitude
	// Dummy-Funktion
	cout << "DatAuf: GetDistanceMeters-Funktion wurde aufgerufen." << endl;
	double earth_flattening = 1 / 298.257223563;
	double equatorial_radius_km = 6378.137;
	double F = (latitude1 + latitude2) / 2;
	double G = (latitude1 - latitude2) / 2;
	double l = (longitude1 - longitude2) / 2;
	double S= 



	return 2;
}


double CalcDatAuf::deg2rad(double) {

}


*/