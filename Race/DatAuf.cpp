#pragma once

#include <iostream>
#include <vector>
#include "DatAuf.h"
#include "DatAuf_SplineCatmullRom.h"

using namespace std;



void DatAuf::CalcDatAuf::DataProcessing() {			//Ueberpruefung auf nan-Werte?

		cout << "DatAuf: DataProcessing aufgerufen" << endl;

		GetTestData();
		GetDistanceMeters2D(nodes[0], nodes[1]);
		GetDistanceMeters3D(nodes[0], nodes[1]);

		// Check and insert if necessary additional knots
		this->InsertAdditionalNodes();

		// Calculate Data for SOLL-Fahrtbestimmmung 
		this->CalcRadiusGradientData();


	}


	void DatAuf::CalcDatAuf::InsertAdditionalNodes() {/*
		cout << "DatAuf: InsertAdditionalNodes aufgerufen" << endl;

		int NodeItem = 0;
		int NodeItemInsert;
		int NodeItemCurrent = 0;
		int NodeItemNext = 0;
		int MaxNumberNodes = this->nodes.size();
		int NumberAdditionalNodes;
		node Node1, Node2, NewNode;
		std::vector<node>::iterator NewNodeItemInsert;
		double DistanceTwoNodes = 0.0;
		double Delta_t;
		SplineCatmullRom SplineSegment;
		SplineSegment.SplineKnotsReset();

		// Schleife �ber alle "nodes"
		//Start bei Index "1" und Ende bei "MaxNumberNodes - 1" (Behandlung von Index "0" und "Max" wird danach gemacht 
		NodeItem = 1;
		MaxNumberNodes = MaxNumberNodes - 1;
		//SChleife fuer NICHT geschlossenen Kurs
		while (NodeItem < MaxNumberNodes - 2) {

		//Schleife fuer geschlossenen Kurs
		//while (NodeItem < MaxNumberNodes - 2) {
			NodeItemCurrent = NodeItem;
			NodeItemNext = NodeItem + 1;

			//Nodes-Info auf lokalen SplineSegment kopieren
			//for (int i = NodeItem - 1;i < NodeItem + 3;i++) {
			for (int i = 0;i < 4;i++) {
				SplineSegment.SplineKnots[i][0] = this->nodes[NodeItem-1+i].longitude;
				SplineSegment.SplineKnots[i][1] = this->nodes[NodeItem-1+i].latitude;
				SplineSegment.SplineKnots[i][2] = this->nodes[NodeItem-1+i].elevation;
			}
			
			Node1 = this->nodes[NodeItem];
			Node2 = this->nodes[NodeItem + 1];

			// Aufruf Knoteneinfügen rekursiv
			//InsertOneNodeRecursiv(Node1, Node2, SplineSegment);

			nodes[NodeItem].distanceToNext = GetDistanceMeters3D(Node1, Node2);

			if (nodes[NodeItem].distanceToNext > 1.0) {
				NumberAdditionalNodes = int(nodes[NodeItem].distanceToNext);

				Delta_t = 1.0 / NumberAdditionalNodes;

				for (int i = 1;i < NumberAdditionalNodes;i++) {
					//cout << "Eingefuegter Node #" << i << endl;
					SplineSegment.InterpolKnotReset();
					SplineSegment.CalcInterpolKnot(i * Delta_t);
					NewNode.longitude = SplineSegment.InterpolKnot[0];
					NewNode.latitude = SplineSegment.InterpolKnot[1];
					NewNode.elevation = SplineSegment.InterpolKnot[2];
					NewNodeItemInsert = nodes.begin() + NodeItem + i;
					this->nodes.insert(NewNodeItemInsert, NewNode);
				}

				// Zaehler mit eingefuegten Punkten ergaenzen, "-1", da ohne einfuegen der Zaehler automatisch um 1 erhoeht wird
				NodeItem += NumberAdditionalNodes-1;
			}
			//NodeItem hochzaehlen
			NodeItem += 1;
			// MaxNumberNodes updaten nach Einf�gen weiterer "nodes"
			MaxNumberNodes = this->nodes.size();
		}

		MaxNumberNodes = this->nodes.size();
		for (NodeItem = 0;NodeItem < MaxNumberNodes-1;NodeItem++) {
			Node1 = this->nodes[NodeItem];
			Node2 = this->nodes[NodeItem + 1];

			nodes[NodeItem].distanceToNext = GetDistanceMeters3D(Node1, Node2);
		}

		SplineSegment.CalcInterpolKnot(0.65);
*/
	}

	void DatAuf::CalcDatAuf::CalcRadiusGradientData() {

		int MaxNumberNodes = this->nodes.size();

		for (int index = 0; index < MaxNumberNodes; index++) {

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
			double equatorial_radius_km = 6378.137;		//  get Hoehe von erstem Knoten:+ nodes[0].elevation: Hoehen-Levelkorrektur?

			// intermediary values: 
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
		//cout << "DatAuf: GetDistanceMeters2D-Funktion wurde aufgerufen. Die Distanz betraegt: " << distance2D<< " Meter." <<endl;	
		return distance2D;
	}



	double DatAuf::GetDistanceMeters3D(node node1, node node2) {
		double distance3D;
		if (node1.latitude == node2.latitude && node1.longitude == node2.longitude && node1.elevation == node2.elevation) {
			distance3D = 0;
		}
		else {
		CalcDatAuf TwoNodes;
		double distance2D = TwoNodes.GetDistanceMeters2D(node1, node2);
		double DiffElevation = (node2.elevation - node1.elevation);
		distance3D = sqrt(distance2D * distance2D + DiffElevation * DiffElevation);
		}
		//cout << "DatAuf: GetDistanceMeters3D-Funktion wurde aufgerufen. Die Distanz betraegt: " << distance3D << " Meter." <<endl;
		return distance3D;
	}




	void DatAuf::CalcDatAuf::CalcHorizontalCurveRad(int index) {

		//--------------
																// bool wird jedes mal aufgerufen?!
		/*
		if (loop) {																			// Bedingung: Letzter Knoten ist gleich erster Knoten -> ueberpruefen im OSM_Nord!
			if (index == 0) { preIndex = MaxIndexNodes - 1; }
			if (index == MaxIndexNodes) { postIndex = 1; }
		}

		if (!loop && (index == 0 || index == MaxIndexNodes)) { // no loop & starting condition
			double radiusIndex = 10E6;
		}
		else if ((nodes[preIndex].elevation == nodes[index].elevation && nodes[preIndex].elevation == nodes[postIndex].elevation) || (nodes[preIndex].distanceToNext == 0 && nodes[index].distanceToNext == 0)) {  // straight line
			double radiusIndex = 10E6;
		}
		else {*/
		//-----------


		// define Index for 3 points																	// Prüfen was an Rändern passiert
		int MaxIndexNodes = this->nodes.size() - 1;
		int preIndex = index - 1;
		int postIndex = index + 1;

		bool loop = this->isLoop();

		if (loop) {																			// Bedingung: Letzter Knoten ist gleich erster Knoten -> ueberpruefen im OSM_Nord!
			if (index == 0) { preIndex = MaxIndexNodes - 1; }
			if (index == MaxIndexNodes) { postIndex = 1; }
		}																					// else nach if?

		if (!loop && (index == 0 || index == MaxIndexNodes)) { // no loop & starting condition
			double radiusIndex = 10E6;
		}
		else {
			// get distances and temporary help values
			double dis_PrePoint = this->GetDistanceMeters2D(nodes[preIndex], nodes[index]);
			double dis_PostPoint = this->GetDistanceMeters2D(nodes[index], nodes[postIndex]);
			double dis_PrePointSq = dis_PrePoint * dis_PrePoint;
			double dis_PostPointSq = dis_PostPoint * dis_PostPoint;

			// calculation of temporary nodes S1, S2
			node* P1_temp = new node;
			P1_temp->longitude = nodes[index].longitude;
			P1_temp->latitude = nodes[preIndex].latitude;
			node* P2_temp = new node;
			P2_temp->longitude = nodes[index].longitude;
			P2_temp->latitude = nodes[postIndex].latitude;

			double dis_P1temp = this->GetDistanceMeters2D(nodes[index], *P1_temp);
			double dis_P2temp = this->GetDistanceMeters2D(nodes[index], *P2_temp);
			delete P1_temp, P2_temp;

			double alpha1 = acos(dis_P1temp / dis_PrePoint);
			double alpha2 = acos(dis_P2temp / dis_PostPoint);
			double alpha = alpha1 + alpha2;
			double MulPrePost = dis_PrePoint * dis_PostPoint * cos(alpha);

			double denominator = (dis_PrePointSq * dis_PostPointSq - MulPrePost * MulPrePost);
			if (denominator > 0 || denominator < 0) {
				// radius at Index
				double radiusIndex = 0.5 * sqrt(dis_PrePointSq * dis_PostPointSq * (dis_PrePointSq + dis_PostPointSq - 2 * MulPrePost) / (dis_PrePointSq * dis_PostPointSq - MulPrePost * MulPrePost));

				// limitations
				if (radiusIndex < 10E-6) {
					this->nodes[index].horizontalCurveRadius = 10E-6;
					cout << "Warning: horizontal radius is smaller than 10E-6. Node: " << index << endl;										// Error Handling: offen
				}
				else if (radiusIndex > 10E6) {
					this->nodes[index].horizontalCurveRadius = 10E6;
					cout << "Warning: horizontal radius is larger than 10E6. Node: " << index << endl;
				}
				else {
					this->nodes[index].horizontalCurveRadius = radiusIndex;
				}
			}
			else {
				this->nodes[index].horizontalCurveRadius = 10E6;
			}

			// error
			if (this->nodes[index].horizontalCurveRadius == nan("")) {
				cout << "Error: Calculation of horizontal radius failed. Node: " << index << endl;
			}
		}
	}

	void DatAuf::CalcDatAuf::CalcVerticalCurveRad(int index) {
		double radiusIndex = 0;
		// define Index for 3 points														// Prüfen was an Rändern passiert
		int MaxIndexNodes = this->nodes.size() - 1;
		int preIndex = index - 1;//-1
		int postIndex = index + 1;

		bool loop = this->isLoop();															// bool wird jedes mal aufgerufen?!

		if (loop) {																			// Bedingung: Letzter Knoten ist gleich erster Knoten -> ueberpruefen im OSM_Nord!
			if (index == 0) { preIndex = MaxIndexNodes - 1; }
			if (index == MaxIndexNodes) { postIndex = 1; }
		}

		if (!loop && (index == 0 || index == MaxIndexNodes)) { // no loop & starting condition
			radiusIndex = 10E6;
		}
		else {
			// get distances and temporary help values
			double diff_PrePointX = (0 - nodes[preIndex].distanceToNext);
			double diff_PrePointY = (nodes[preIndex].elevation - nodes[index].elevation);
			double dis_PrePointSq = diff_PrePointX * diff_PrePointX + diff_PrePointY * diff_PrePointY;

			double diff_PostPointX = (nodes[preIndex].distanceToNext + nodes[index].distanceToNext) - nodes[preIndex].distanceToNext;
			double diff_PostPointY = (nodes[postIndex].elevation - nodes[index].elevation);
			double dis_PostPointSq = diff_PostPointX * diff_PostPointX + diff_PostPointY * diff_PostPointY;

			double MulPrePost = diff_PrePointX * diff_PostPointX + diff_PrePointY * diff_PostPointY;

			double denominator = (dis_PrePointSq * dis_PostPointSq - MulPrePost * MulPrePost);

			if (denominator > 0 || denominator < 0) {
				// radius at Index
				radiusIndex = 0.5 * sqrt(dis_PrePointSq * dis_PostPointSq * (dis_PrePointSq + dis_PostPointSq - 2 * MulPrePost) / denominator);
			
				// limitations	-> tertiärer Operator?			
				if (radiusIndex < 10E-6) {
					radiusIndex = 10E-6;
					cout << "Warning: horizontal radius is smaller than 10E-6. Node: " << index << endl;										// Error Handling: offen
				}
				else if (radiusIndex > 10E6) {
					radiusIndex = 10E6;
					cout << "Warning: horizontal radius is larger than 10E6. Node: " << index << endl;
				}
				// calculate sign 
				int sign_cross_ab = (diff_PrePointX * diff_PostPointY - diff_PostPointX - diff_PrePointY)>=0 ? 1 : -1;
				radiusIndex = sign_cross_ab * radiusIndex;
			}
			else {
				radiusIndex = 10E6;
			}
		}
	
		this->nodes[index].verticalCurveRadius = radiusIndex;

		// error
		if (this->nodes[index].verticalCurveRadius == nan("")) {
			cout << "Error: Calculation of horizontal radius failed. Node: " << index << endl;
		}
		
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


// TEST-Daten:
	void DatAuf::CalcDatAuf::GetTestData()
	{
		nodes[0].longitude = 6.945215;
		nodes[0].latitude = 50.33409;
		nodes[0].distanceToNext = 50;
		nodes[0].elevation = 500;
		nodes[0].id = "loop";

		nodes[1].longitude = 6.971891;
		nodes[1].latitude = 50.34831;
		nodes[1].distanceToNext = 50;
		nodes[1].elevation = 750;

		nodes[2].longitude = 6.994314;
		nodes[2].latitude = 50.360358;
		nodes[2].distanceToNext = 50;
		nodes[2].elevation = 222;

		nodes[3].longitude = 6.995172;
		nodes[3].latitude = 50.376866;
		nodes[3].distanceToNext = 50;
		nodes[3].elevation = 148;

		nodes[4].longitude = 6.992038;
		nodes[4].latitude = 50.373581;
		nodes[4].distanceToNext = 50;
		nodes[4].elevation = 150;

		nodes[5].longitude = 6.985426;
		nodes[5].latitude = 50.371774;
		nodes[5].elevation = 66;

		nodes[6].longitude = 6.989161;
		nodes[6].latitude = 50.374539;
		nodes[6].elevation = 99;

		nodes[7].longitude = 6.980189;
		nodes[7].latitude = 50.372267;
		nodes[7].elevation = 148;
		nodes[7].distanceToNext = 300;

		nodes[8].longitude = nodes[0].longitude;
		nodes[8].latitude = nodes[0].latitude;
		nodes[8].elevation = nodes[0].elevation;
		nodes[8].id = nodes[0].id;
		nodes[8].distanceToNext = nodes[0].distanceToNext;

	}


