#pragma once

#include <iostream>
#include "DatAuf.h"
#include "DatAuf_SplineCatmullRom.h"

using namespace std;



void DatAuf::CalcDatAuf::DataProcessing() {			//überprüfung auf nan-Werte?

		cout << "DatAuf: DataProcessing aufgerufen" << endl;
		// Get better Test-Data
		GetTestData();
		GetDistanceMeters2D(nodes[0], nodes[1]);
		GetDistanceMeters3D(nodes[0], nodes[1]);

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

	double DatAuf::CalcDatAuf::Get1Coordinate(int index) {
		// Dummy-Funktion
		vector<double> oneNode;
		oneNode.push_back(nodes[index].longitude);
		oneNode.push_back(nodes[index].latitude);
		oneNode.push_back(nodes[index].elevation);
		cout << "DatAuf: GetCoordinates-Funktion wurde aufgerufen." << endl;
		return 2;
	}


	double DatAuf::CalcDatAuf::GetDistanceMeters2D(node node1, node node2) {
		double distance2D;
		if (node1.latitude == node2.latitude && node1.longitude == node2.longitude) {
			distance2D = 0;
		}
		else {

			double earth_flattening = 1 / 298.257223563;
			double equatorial_radius_km = 6378.137;		// // get höhe von erstem Knoten:+ nodes[0].elevation: Höhen-Levelkorrektur?
			// intermin values: 
			double F = deg2rad((node1.latitude + node2.latitude) / 2);
			double G = deg2rad((node1.latitude - node2.latitude) / 2);
			double L = deg2rad((node1.longitude - node2.longitude) / 2);
			double S = sin(G) * sin(G) * cos(L) * cos(L) + cos(F) * cos(F) * sin(L) * sin(L);
			double C = cos(G) * cos(G) * cos(L) * cos(L) + sin(F) * sin(F) * sin(L) * sin(L);
			double w = atan(sqrt(S / C));
			double D = 2 * w * equatorial_radius_km;
			double T = sqrt(S * C) / w;
			double H1 = (3 * T - 1) / (2 * C);
			double H2 = (3 * T + 1) / (2 * S);

			distance2D = (D * (1 + earth_flattening * H1 * sin(F) * sin(F) * cos(G) * cos(G) - earth_flattening * H2 * cos(F) * cos(F) * sin(G) * sin(G)))*1000;
		}
		cout << "DatAuf: GetDistanceMeters2D-Funktion wurde aufgerufen. Die Distanz betraegt: " << distance2D<< " Meter." <<endl;	
		return distance2D;
	}



	double DatAuf::GetDistanceMeters3D(node node1, node node2) {
		double distance3D;
		if (node1.latitude == node2.latitude && node1.longitude == node2.longitude && node1.elevation == node2.elevation) {
			distance3D = 0;
		}
		else {
			CalcDatAuf TwoNodes;
		    double distance2D=TwoNodes.GetDistanceMeters2D(node1, node2);
			double DiffElevation = (node2.elevation - node1.elevation);
			distance3D = sqrt(distance2D * distance2D + DiffElevation * DiffElevation);
		}
		cout << "DatAuf: GetDistanceMeters3D-Funktion wurde aufgerufen. Die Distanz betraegt: " << distance3D << " Meter." <<endl;
		return distance3D;
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

	double DatAuf::deg2rad(double grad) {
		double rad = grad * 3.14159265358979 / 180;
		return rad;
	}


	void DatAuf::CalcDatAuf::GetTestData()
	{
		nodes[0].longitude = 6.945215;
		nodes[0].latitude = 50.33409;
		nodes[0].elevation = 500;

		nodes[1].longitude = 6.971891;
		nodes[1].latitude = 50.34831;
		nodes[1].elevation = 333;

		nodes[2].longitude = 6.994314;
		nodes[2].latitude = 50.360358;
		nodes[2].elevation = 222;

		nodes[3].longitude = 6.995172;
		nodes[3].latitude = 50.376866;
		nodes[3].elevation = 148;

		nodes[4].longitude = 6.992038;
		nodes[4].latitude = 50.373581;
		nodes[4].elevation = 98;

		nodes[5].longitude = 6.985426;
		nodes[5].latitude = 50.371774;
		nodes[5].elevation = 66;

		nodes[6].longitude = 6.989161;
		nodes[6].latitude = 50.374539;
		nodes[6].elevation = 99;

		nodes[7].longitude = 6.980189;
		nodes[7].latitude = 50.372267;
		nodes[7].elevation = 148;

		nodes[8].longitude = nodes[1].longitude;
		nodes[8].latitude = nodes[1].latitude;
		nodes[8].elevation = nodes[1].elevation;

	}