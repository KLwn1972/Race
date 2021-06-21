#include <iostream>
#include "DatAuf.h"
#include "Race.h"
#include "DatAuf_SplineCatmullRom.h"

using namespace std;


void DatAuf::CalcDatAuf::DataProcessing() {

	cout << "DatAuf: DataProcessing aufgerufen" << endl;

	// Check and insert if necessary additional knots
	this->InsertAdditionalNodes();

	// Calculate Data for SOLL-Fahrtbestimmmung 
	this->CalcRadiusGradientData();

}

void DatAuf::CalcDatAuf::InsertAdditionalNodes() {
	cout << "DatAuf: InsertAdditionalKnots aufgerufen" << endl;

	double DistanceTwoNodes = 0.0;
	SplineCatmullRom SplineSegment;
	SplineSegment.SplineKnotsReset();
	SplineSegment.CalcInterpolKnot(0.65);


}

void DatAuf::CalcDatAuf::CalcRadiusGradientData() {

	int MaxNumberNodes = this->nodes.size();

	for (int index = 0; index < MaxNumberNodes; index++) {

		this->CalcHorizontalCurveRad(index);
		this->CalcVerticalCurveRad(index);
		this->CalcGradientPercentage(index);

	}

	// Dummy-Funktion
	CalcHorizontalCurveRad(5);
	CalcVerticalCurveRad(5);
	CalcGradientPercentage(5);
	cout << "DatAuf: CalcRadiusGradientData-Funktion wurde aufgerufen." << endl;

	return;
}

vector<double> DatAuf::CalcDatAuf::Get1Coordinate(int index) {
	// Dummy-Funktion
	vector<double> oneNode;
	oneNode.push_back(nodes[index].longitude);
	oneNode.push_back(nodes[index].latitude);
	oneNode.push_back(nodes[index].elevation);
	cout << "DatAuf: GetCoordinates-Funktion wurde aufgerufen." << endl;
	return oneNode;
}


double DatAuf::GetDistanceMeters(const double& longitude1, const double& latitude1, const double& longitude2, const double& latitude2) {	//GetDistance(node node1, node node2)
	// Dummy-Funktion
	cout << "DatAuf: GetDistanceMeters-Funktion wurde aufgerufen." << endl;
	// Dummy-Funktion
	cout << "DatAuf: GetDistanceMeters-Funktion wurde aufgerufen." << endl;
	double earth_flattening = 1 / 298.257223563;
	double equatorial_radius_km = 6378.137;
	double F = (latitude1 + latitude2) / 2;
	double G = (latitude1 - latitude2) / 2;
	double l = (longitude1 - longitude2) / 2;
	//double S =



	return 2;
}

void DatAuf::CalcDatAuf::CalcHorizontalCurveRad(int index) {
	cout << "DatAuf: CalcHorizontalCurveRad2-Funktion wurde aufgerufen." << endl;

	this->nodes[index].horizontalCurveRadius = index;

	return;
}

void DatAuf::CalcDatAuf::CalcVerticalCurveRad(int index) {
	cout << "DatAuf: CalcVerticalCurveRad2-Funktion wurde aufgerufen." << endl;

	this->nodes[index].verticalCurveRadius = index * 2.0;

	return;
}

void DatAuf::CalcDatAuf::CalcGradientPercentage(int index) {
	cout << "DatAuf: CalcGradientPercentage2-Funktion wurde aufgerufen." << endl;

	this->nodes[index].gradient = index * 3.0;

	return;
}
