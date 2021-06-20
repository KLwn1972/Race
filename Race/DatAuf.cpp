#include <iostream>
#include "DatAuf.h"
#include "Race.h"

using namespace std;



void CalcDatAuf::DataProcessing() {
	// Dummy-Funktion
	/*
	double distance = this->GetDistanceMeters(1, 2, 3, 4);
	cout << "DatAuf: GetDistanceMeters-Funktion wurde aufgerufen, Distanz: " << distance << endl;	
	vector<double> Test_coordinates = this->GetCoordinates(3);
	//CatmullRomSpline();
	*/
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
	return 2;
}


