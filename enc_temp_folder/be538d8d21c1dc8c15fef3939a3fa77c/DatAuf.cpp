#pragma once

#include <iostream>
#include <vector>
#include "DatAuf.h"
#include "DatAuf_SplineCatmullRom.h"

using namespace std;



void DatAuf::CalcDatAuf::DataProcessing() {			//Ueberpruefung auf nan-Werte?

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
		cout << "DatAuf: InsertAdditionalNodes aufgerufen" << endl;
		/*
		int NodeItem = 0;
		int NodeItemCurrent = 0;
		int NodeItemNext = 0;
		int MaxNumberNodes = this->nodes.size();
		node Node1, Node2;
		double DistanceTwoNodes = 0.0;
		SplineCatmullRom SplineSegment;
		SplineSegment.SplineKnotsReset();

		// Schleife �ber alle "nodes"
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


			// MaxNumberNodes updaten nach Einf�gen weiterer "nodes"
			MaxNumberNodes = this->nodes.size();
		}
		SplineSegment.CalcInterpolKnot(0.65);
		*/

	}

	void DatAuf::CalcDatAuf::CalcRadiusGradientData() {

		int MaxNumberNodes = this->nodes.size();

		for (int index = 1; index < MaxNumberNodes; index++) {//0

			this->CalcHorizontalCurveRad(index);
			this->CalcVerticalCurveRad(index);
			this->CalcGradientPercentage(index);

		}

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
			double equatorial_radius_km = 6378.137;		// // get h�he von erstem Knoten:+ nodes[0].elevation: H�hen-Levelkorrektur?
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
		int MaxIndexNodes = this->nodes.size()-1;
		int preIndex = index - 1;
		int postIndex= index +1;

		if (this->isLoop()) {								// Bedingung: Letzter Knoten ist gleich erster Knoten ueberpruefen im OSM_Nord!
			if (index == 0) {preIndex = MaxIndexNodes-1;}
			if (index == MaxIndexNodes) {postIndex = 1;}
		}
		else {
			if (index == 0) { int preIndex = 0; }
			if (index == MaxIndexNodes) { int postIndex = MaxIndexNodes; }
		}

		double dis_PrePoint = GetDistanceMeters2D(nodes[preIndex], nodes[index]);
		double dis_PostPoint = GetDistanceMeters2D(nodes[index], nodes[postIndex]);
		double dis_PrePointSq = dis_PrePoint * dis_PrePoint;
		double dis_PostPointSq = dis_PostPoint * dis_PostPoint;

		// calculation of temporary nodes S1, S2
		node* P1_temp=new node;
		P1_temp->longitude = nodes[index].longitude;
		P1_temp->latitude = nodes[preIndex].latitude;
		node* P2_temp = new node;
		P2_temp->longitude = nodes[index].longitude;
		P2_temp->latitude = nodes[postIndex].latitude;

		double dis_P1temp = GetDistanceMeters2D(nodes[index], *P1_temp);
		double dis_P2temp = GetDistanceMeters2D(nodes[index], *P2_temp);
		delete P1_temp, P2_temp;

		double alpha1 = acos(dis_P1temp / dis_PrePoint);
		double alpha2 = acos(dis_P2temp / dis_PostPoint);
		double alpha = alpha1 + alpha2;
		double MulPrePost = dis_PrePoint * dis_PostPoint * cos(alpha);

		double radiusIndex= 0.5 * sqrt(dis_PrePointSq * dis_PostPointSq * (dis_PrePointSq + dis_PostPointSq - 2 * MulPrePost) / (dis_PrePointSq * dis_PostPointSq - MulPrePost * MulPrePost));

		if (radiusIndex < 10E-6 || radiusIndex > 10E6) {
			cout << "Warnung:  CalcHorizontalCurveRad limit....." << endl;						// Error Handling
		}
		this->nodes[index].horizontalCurveRadius = radiusIndex;

		cout << "DatAuf: CalcHorizontalCurveRad-Funktion wurde aufgerufen." << endl;
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
		double DistanceTwoNodes = GetDistanceMeters3D(Node1, Node2);

		if (DistanceTwoNodes > 1.0) {

		}

	}

	double DatAuf::deg2rad(double grad) {
		double rad = grad * 3.14159265358979 / 180;
		return rad;
	}

	bool DatAuf::CalcDatAuf::isLoop()
	{
		int MaxIndexNodes = this->nodes.size() - 1;
		return  (nodes[0].id == nodes[MaxIndexNodes].id);
	}

	void DatAuf::CalcDatAuf::GetTestData()
	{
		nodes[0].longitude = 6.945215;
		nodes[0].latitude = 50.33409;
		nodes[0].elevation = 500;
		nodes[0].id = "loop";

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

		nodes[8].longitude = nodes[0].longitude;
		nodes[8].latitude = nodes[0].latitude;
		nodes[8].elevation = nodes[0].elevation;
		nodes[8].id = nodes[0].id;

	}


