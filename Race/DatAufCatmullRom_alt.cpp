/*
#include <iostream>
#include "DatAuf.h"
#include "Race.h"


void SplineDatAuf::DataProcessingCatmullRomSpline() {
	// Dummy-Funktion
	double distance = this->GetDistanceMeters(1, 2, 3, 4);
	cout << "DatAuf: Distanz: " << distance << endl;
	vector<double> testCoordinate = this->Get1Coordinate(3);
	vector<double> testCoordinates = this->Get4Coordinates(3);
	double gradient = CalcGradient(1);
	double curvature = CalcCurvature(1);
	insertPoint(1);
}

vector<double> SplineDatAuf::Get4Coordinates(int index) {
	// Dummy-Funktion
	vector<double> fourCoordinates;
	fourCoordinates.push_back(nodes[index].longitude);
	fourCoordinates.push_back(nodes[index].latitude);
	fourCoordinates.push_back(nodes[index].elevation);
	cout << "DatAuf: Get4Coordinates-Funktion wurde aufgerufen." << endl;
	return fourCoordinates;
}

double SplineDatAuf::CalcGradient(int index) {
	// Dummy-Funktion
	cout << "DatAuf: CalcGradient-Funktion wurde aufgerufen." << endl;
	return 2;
}

double SplineDatAuf::CalcCurvature(int index) {
	// Dummy-Funktion
	cout << "DatAuf: CalcCurvature-Funktion wurde aufgerufen." << endl;
	return 2;
}

void SplineDatAuf::insertPoint(int index_prePoint) {
	// Dummy-Funktion
	cout << "DatAuf: InsertPoint-Funktion wurde aufgerufen." << endl;
}
*/