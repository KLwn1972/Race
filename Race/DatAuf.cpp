#pragma once

#include <iostream>
#include "DatAuf.h"
#include "DatAuf_SplineCatmullRom.h"

using namespace std;



void DatAuf::CalcDatAuf::DataProcessing() {

		cout << "DatAuf: DataProcessing aufgerufen" << endl;

		// Check and insert if necessary additional knots
		InsertAdditionalNodes();

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

	double DatAuf::CalcDatAuf::Get1Coordinate(int index) {
		// Dummy-Funktion
		vector<double> oneNode;
		oneNode.push_back(nodes[index].longitude);
		oneNode.push_back(nodes[index].latitude);
		oneNode.push_back(nodes[index].elevation);
		cout << "DatAuf: GetCoordinates-Funktion wurde aufgerufen." << endl;
		return 2;
	}


	double DatAuf::GetDistanceMeters(node node1, node node2) {
		cout << "DatAuf: GetDistanceMeters-Funktion wurde aufgerufen." << endl;
		// Dummy-Funktion
		cout << "DatAuf: GetDistanceMeters-Funktion wurde aufgerufen." << endl;
		double earth_flattening = 1 / 298.257223563;
		double equatorial_radius_km = 6378.137;		// H�henkorrektur?
		double F = (node1.latitude + node2.latitude) / 2;
		double G = (node1.latitude - node2.latitude) / 2;
		double l = (node1.longitude - node2.longitude) / 2;
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

	double DatAuf::CalcDatAuf::deg2rad(double grad) {
		double rad = grad * 3.14159265358979 / 180;
		return rad;
	}
