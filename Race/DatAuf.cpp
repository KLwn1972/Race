#pragma once

#include <iostream>
#include <vector>
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
		cout << "DatAuf: InsertAdditionalNodes aufgerufen" << endl;

		int NodeItem = 0;
		int NodeItemCurrent = 0;
		int NodeItemNext = 0;
		int MaxNumberNodes = this->nodes.size();
		node Node1, Node2;
		double DistanceTwoNodes = 0.0;
		SplineCatmullRom SplineSegment;
		SplineSegment.SplineKnotsReset();

		// Schleife über alle "nodes"
		//Start bei Index "1" und Ende bei "MaxNumberNodes - 1" (Behandlung von Index "0" und "Max" wird danach gemacht 
		NodeItem = 1;
		MaxNumberNodes = MaxNumberNodes - 1;

		while (NodeItem < MaxNumberNodes) {
			NodeItemCurrent = NodeItem;
			NodeItemNext = NodeItem + 1;

			//Nodes-Info auf lokalen SplineSegment kopieren
			for (int i = NodeItem - 1;i < NodeItem + 3;i++) {
				SplineSegment.SplineKnots[i][0] = this->nodes[i].longitude;
				SplineSegment.SplineKnots[i][1] = this->nodes[i].latitude;
				SplineSegment.SplineKnots[i][2] = this->nodes[i].elevation;
			}
			
			Node1 = this->nodes[NodeItem];
			Node2 = this->nodes[NodeItem + 1];

			InsertOneNodeRecursiv(Node1, Node2, SplineSegment);


			// MaxNumberNodes updaten nach Einfügen weiterer "nodes"
			MaxNumberNodes = this->nodes.size();
		}
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
		double equatorial_radius_km = 6378.137;		// Höhenkorrektur?
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

	void DatAuf::CalcDatAuf::InsertOneNodeRecursiv(node Node1, node Node2, SplineCatmullRom SplineSegment){
		cout << "DatAuf: InsertOneNodeRecursiv-Funktion wurde aufgerufen." << endl;
		double DistanceTwoNodes = GetDistanceMeters(Node1, Node2);

		if (DistanceTwoNodes > 1.0) {

		}

	}

	double DatAuf::CalcDatAuf::deg2rad(double grad) {
		double rad = grad * 3.14159265358979 / 180;
		return rad;
	}
