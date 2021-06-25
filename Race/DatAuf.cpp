#pragma once

#include <iostream>
#include <string>
#include "DatAuf.h"

using namespace std;

int DatAuf::CalcDatAuf::DataProcessing() {
	// Check and insert additional knots if necessary
	cout << "DatAuf: Insert nodes..." << endl;
	this->InsertAdditionalNodes();
	cout << "DatAuf: Done." << endl;

	// Calculate Data for SOLL-Fahrtbestimmmung
	cout << "DatAuf: Calculation of vertical and horizontal radius and gradient..." << endl;
	this->CalcRadiusGradientData();
	cout << "DatAuf: Done." << endl;

	return this->retval;
}

void DatAuf::CalcDatAuf::InsertAdditionalNodes() {
	node NewNode, PrevNode;
	std::vector<node>::iterator NewNodeItemInsert;
	size_t NodeItem = 0;
	size_t NodeItemInsert = 0;
	size_t MaxNumberNodes = this->nodes.size();
	size_t NumberAdditionalNodes;
	size_t InsertMode = 0;
	int RefinementIterator = 0;
	double DistanceTwoNodes = 0.0;
	double Delta_t, t_previous, t_current;

	// Set Insertmode for Node insertion:
	//  Case 0: Equidistant in parameter t of the Spline
	//  Case 1: Adaptation of t depending on calculated distance, target distance: <1.0
	InsertMode = 1;

	while (NodeItem < MaxNumberNodes - 1) {
		CopyNodesToSplineKnots(NodeItem);

		nodes[NodeItem].distanceToNext = GetDistanceMeters3D(nodes[NodeItem], nodes[NodeItem + 1]);
		if (nodes[NodeItem].distanceToNext > 1.0) {
			switch (InsertMode) {
			case 0:
				NumberAdditionalNodes = int(nodes[NodeItem].distanceToNext);
				Delta_t = 1.0 / NumberAdditionalNodes;
				for (int i = 1; i < NumberAdditionalNodes; i++) {
					SplineSegment.CalcInterpolKnot(i * Delta_t);
					NewNode = GetInterpolKnot();
					InsertOneAdditionalNode(NodeItem, i, NewNode);
				}
				// Zaehler mit eingefuegten Punkten ergaenzen, "-1", da ohne einfuegen der Zaehler automatisch um 1 erhoeht wird
				NodeItem += NumberAdditionalNodes - 1;

				break;
			case 1:
				Delta_t = 1.0 / int(nodes[NodeItem].distanceToNext);
				NumberAdditionalNodes = 0;
				PrevNode = nodes[NodeItem];
				t_previous = 0.0;
				t_current = t_previous + Delta_t;

				while (t_current <= 1.0) {
					RefinementIterator += 1;

					SplineSegment.CalcInterpolKnot(t_current);
					NewNode = GetInterpolKnot();
					DistanceTwoNodes = GetDistanceMeters3D(PrevNode, NewNode);
					if (DistanceTwoNodes > 1.0) {
						Delta_t = Delta_t * 0.9;
						t_current = t_previous + Delta_t;
					}
					else if (DistanceTwoNodes < 0.98) {
						Delta_t = Delta_t * 1.05;
						t_current = t_previous + Delta_t;
					}
					else {
						//Insert Additional Node
						NumberAdditionalNodes += 1;
						InsertOneAdditionalNode(NodeItem, NumberAdditionalNodes, NewNode);
						//Preparation next iteration step
						PrevNode = NewNode;
						t_previous = t_current;
						t_current = t_current + Delta_t;
						if (t_current >= 1.0) {
#ifdef DEBUG
							cout << "Grenzfall t_current ist aufgetreten, Iteration: " << RefinementIterator << " NodeItem: " << NodeItem + NumberAdditionalNodes << endl;
#endif
							SplineSegment.CalcInterpolKnot(1.0);
							NewNode = GetInterpolKnot();
							DistanceTwoNodes = GetDistanceMeters3D(PrevNode, NewNode);
							if (DistanceTwoNodes > 1.0) {
#ifdef DEBUG
								cout << "Warnung:  InsertAdditionalNode insert last node....." << endl;
#endif
								t_current = 0.5 * (1.0 + t_previous);
								RefinementIterator += 1;
								SplineSegment.CalcInterpolKnot(t_current);
								NewNode = GetInterpolKnot();
								DistanceTwoNodes = GetDistanceMeters3D(PrevNode, NewNode);
								NumberAdditionalNodes += 1;
								InsertOneAdditionalNode(NodeItem, NumberAdditionalNodes, NewNode);

								break;
							}
						}
					}
				}
				NodeItem += NumberAdditionalNodes;

				break;
			}
		}
		//Increment NodeItem
		NodeItem += 1;
		// Update MaxNumberNodes after insertion
		MaxNumberNodes = this->nodes.size();
	}
#ifdef DEBUG
	cout << "Iteratorwert = " << RefinementIterator << endl;
#endif
	CalcDistanceToAllNextNode();
}

void DatAuf::CalcDatAuf::CopyNodesToSplineKnots(size_t NodeItem) {
	size_t MaxNumberNodes = this->nodes.size();

	if (NodeItem == 0) {
		if (this->nodes.front().id == this->nodes.back().id) {
			// Closed Curve
			SplineSegment.SplineKnots[0][0] = this->nodes[MaxNumberNodes - 2].longitude;
			SplineSegment.SplineKnots[0][1] = this->nodes[MaxNumberNodes - 2].latitude;
			SplineSegment.SplineKnots[0][2] = this->nodes[MaxNumberNodes - 2].elevation;
			for (int i = 1; i < 4; i++) {
				SplineSegment.SplineKnots[i][0] = this->nodes[NodeItem - 1 + i].longitude;
				SplineSegment.SplineKnots[i][1] = this->nodes[NodeItem - 1 + i].latitude;
				SplineSegment.SplineKnots[i][2] = this->nodes[NodeItem - 1 + i].elevation;
			}
		}
		else {
			// Open Curve
			SplineSegment.SplineKnots[0][0] = this->nodes[0].longitude;
			SplineSegment.SplineKnots[0][1] = this->nodes[0].latitude;
			SplineSegment.SplineKnots[0][2] = this->nodes[0].elevation;
			for (int i = 1; i < 4; i++) {
				SplineSegment.SplineKnots[i][0] = this->nodes[NodeItem - 1 + i].longitude;
				SplineSegment.SplineKnots[i][1] = this->nodes[NodeItem - 1 + i].latitude;
				SplineSegment.SplineKnots[i][2] = this->nodes[NodeItem - 1 + i].elevation;
			}
		}
	}
	else if (NodeItem == (MaxNumberNodes - 2)) {
		if (this->nodes.front().id == this->nodes.back().id) {
			// Closed Curve
			for (int i = 0; i < 3; i++) {
				SplineSegment.SplineKnots[i][0] = this->nodes[NodeItem - 1 + i].longitude;
				SplineSegment.SplineKnots[i][1] = this->nodes[NodeItem - 1 + i].latitude;
				SplineSegment.SplineKnots[i][2] = this->nodes[NodeItem - 1 + i].elevation;
			}
			SplineSegment.SplineKnots[3][0] = this->nodes[1].longitude;
			SplineSegment.SplineKnots[3][1] = this->nodes[1].latitude;
			SplineSegment.SplineKnots[3][2] = this->nodes[1].elevation;
		}
		else {
			// Open Curve
			for (int i = 0; i < 3; i++) {
				SplineSegment.SplineKnots[i][0] = this->nodes[NodeItem - 1 + i].longitude;
				SplineSegment.SplineKnots[i][1] = this->nodes[NodeItem - 1 + i].latitude;
				SplineSegment.SplineKnots[i][2] = this->nodes[NodeItem - 1 + i].elevation;
			}
			SplineSegment.SplineKnots[3][0] = this->nodes[MaxNumberNodes - 1].longitude;
			SplineSegment.SplineKnots[3][1] = this->nodes[MaxNumberNodes - 1].latitude;
			SplineSegment.SplineKnots[3][2] = this->nodes[MaxNumberNodes - 1].elevation;
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			SplineSegment.SplineKnots[i][0] = this->nodes[NodeItem - 1 + i].longitude;
			SplineSegment.SplineKnots[i][1] = this->nodes[NodeItem - 1 + i].latitude;
			SplineSegment.SplineKnots[i][2] = this->nodes[NodeItem - 1 + i].elevation;
		}
	}
}

node DatAuf::CalcDatAuf::GetInterpolKnot() {
	node Node;
	Node.longitude = SplineSegment.InterpolKnot[0];
	Node.latitude = SplineSegment.InterpolKnot[1];
	Node.elevation = SplineSegment.InterpolKnot[2];
	return Node;
}

void DatAuf::CalcDatAuf::InsertOneAdditionalNode(size_t NodeItem, size_t NumberAdditionalNodes, node NewNode) {
	std::vector<node>::iterator NewNodeItemInsert = this->nodes.begin() + NodeItem + NumberAdditionalNodes;
	this->nodes.insert(NewNodeItemInsert, NewNode);
	this->UpdateNodeIDProperty(NodeItem, NumberAdditionalNodes);
}

void DatAuf::CalcDatAuf::UpdateNodeIDProperty(size_t NodeItem, size_t NumberAdditionalNodes) {
	this->nodes[NodeItem + NumberAdditionalNodes].id = this->nodes[NodeItem].id;
	this->nodes[NodeItem + NumberAdditionalNodes].id += "_";
	this->nodes[NodeItem + NumberAdditionalNodes].id += to_string(NumberAdditionalNodes);
}

void DatAuf::CalcDatAuf::CalcDistanceToAllNextNode() {
	size_t MaxNumberNodes = this->nodes.size();
	size_t NodeItem;
	for (NodeItem = 0; NodeItem < MaxNumberNodes - 1; NodeItem++) {
		nodes[NodeItem].distanceToNext = GetDistanceMeters3D(this->nodes[NodeItem], this->nodes[NodeItem + 1]);
		if (nodes[NodeItem].distanceToNext >= 1.0) {
			// retval-value kept at incoming value because insertion method could create single value to violate distance requirement
			//this->retval = -1;
#ifdef DEBUG
			cout << "NodeItem: " << NodeItem << " distance bigger than 1m." << endl;
#endif
		}
	}
	//Special handling of last vector-element depending on type of circuit
	if (this->nodes.front().id == this->nodes.back().id) {
		this->nodes.back().distanceToNext = nodes.front().distanceToNext;
	}
	else {
		nodes.back().distanceToNext = 0.0;
	}
}

void DatAuf::CalcDatAuf::CalcRadiusGradientData() {
	size_t MaxNumberNodes = this->nodes.size();
	for (int index = 0; index < MaxNumberNodes; index++) {
		this->CalcHorizontalCurveRad(index);
		this->CalcVerticalCurveRad(index);
		this->CalcGradientPercentage(index);
	}
}

double DatAuf::CalcDatAuf::GetDistanceMeters2D(node node1, node node2) {
	double distance2D;
	if (node1.latitude == node2.latitude && node1.longitude == node2.longitude) {
		distance2D = 0;
	}
	else {
		double earth_flattening = 1 / 298.257223563;
		double equatorial_radius_km = 6378.137;

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

		distance2D = (D * (1 + earth_flattening * H1 * sin(F) * sin(F) * cos(G) * cos(G) - earth_flattening * H2 * cos(F) * cos(F) * sin(G) * sin(G))) * 1000;
	}
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
		double DiffElevation = floor((node2.elevation - node1.elevation) * 10E6 + .5) / 10E6;
		distance3D = sqrt(distance2D * distance2D + DiffElevation * DiffElevation);
	}
	return distance3D;
}

void DatAuf::CalcDatAuf::CalcHorizontalCurveRad(size_t index) {
	double radiusIndex = 0;
	double maxRadius = 10E6;
	double minRadius = 10E-6;
	// define Index for 3 points
	size_t MaxIndexNodes = this->nodes.size() - 1;
	size_t preIndex = index - 1;
	size_t postIndex = index + 1;

	bool loop = this->isLoop();

	if (loop && (index == 0)) {
		preIndex = MaxIndexNodes - 1;
	}
	else if (loop && (index == MaxIndexNodes)) {
		postIndex = 1;
	}
	else if (!loop && (index == 0 || index == MaxIndexNodes)) {
		radiusIndex = maxRadius;
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

		double denominator = floor((dis_PrePointSq * dis_PostPointSq - MulPrePost * MulPrePost) * maxRadius + .5) / maxRadius;
		double nominator = floor((dis_PrePointSq * dis_PostPointSq * (dis_PrePointSq + dis_PostPointSq - 2 * MulPrePost)) * maxRadius + .5) / maxRadius;
		if ((denominator > 0 && nominator > 0) || (denominator < 0 && nominator < 0)) {
			// radius at Index
			radiusIndex = 0.5 * sqrt(nominator / denominator);
			// limitations
			if (radiusIndex < minRadius) {
				this->nodes[index].horizontalCurveRadius = minRadius;
#ifdef DEBUG
				cout << "Warning: horizontal radius is smaller than " << minRadius << ". Node: " << index << endl;
#endif
			}
			else if (radiusIndex > maxRadius) {
				this->nodes[index].horizontalCurveRadius = maxRadius;
#ifdef DEBUG
				cout << "Warning: horizontal radius is larger than " << maxRadius << ". Node: " << index << endl;
#endif
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
			this->retval = -1;
		}
	}
}

void DatAuf::CalcDatAuf::CalcVerticalCurveRad(size_t index) {
	double radiusIndex = 0;
	double maxRadius = 10E6;
	double minRadius = 10E-6;
	// define Index for 3 points
	size_t MaxIndexNodes = this->nodes.size() - 1;
	size_t preIndex = index - 1;
	size_t postIndex = index + 1;

	bool loop = this->isLoop();
	if (loop && index == 0) {
		preIndex = MaxIndexNodes - 1;
	}
	else if (loop && index == MaxIndexNodes) {
		postIndex = 1;
	}
	else if (!loop && (index == 0 || index == MaxIndexNodes)) {
		radiusIndex = maxRadius;
	}
	else {
		// get distances and temporary help values
		double diff_PrePointX = floor((0 - nodes[preIndex].distanceToNext) * maxRadius + .5) / maxRadius;
		double diff_PrePointY = floor((nodes[preIndex].elevation - nodes[index].elevation) * maxRadius + .5) / maxRadius;
		double dis_PrePointSq = diff_PrePointX * diff_PrePointX + diff_PrePointY * diff_PrePointY;

		double diff_PostPointX = floor(((nodes[preIndex].distanceToNext + nodes[index].distanceToNext) - nodes[preIndex].distanceToNext) * maxRadius + .5) / maxRadius;
		double diff_PostPointY = floor((nodes[postIndex].elevation - nodes[index].elevation) * maxRadius + .5) / maxRadius;
		double dis_PostPointSq = diff_PostPointX * diff_PostPointX + diff_PostPointY * diff_PostPointY;

		double MulPrePost = diff_PrePointX * diff_PostPointX + diff_PrePointY * diff_PostPointY;

		double denominator = floor((dis_PrePointSq * dis_PostPointSq - MulPrePost * MulPrePost) * maxRadius + .5) / maxRadius;
		double nominator = floor((dis_PrePointSq * dis_PostPointSq * (dis_PrePointSq + dis_PostPointSq - 2 * MulPrePost)) * maxRadius + .5) / maxRadius;
		if ((denominator > 0 && nominator > 0) || (denominator < 0 && nominator < 0)) {
			// radius at Index
			radiusIndex = 0.5 * sqrt((nominator / denominator));

			// limitations
			if (radiusIndex < minRadius) {
#ifdef DEBUG
				cout << "Warning: vertical radius is smaller than " << minRadius << ". Node: " << index << endl;
#endif
				radiusIndex = minRadius;
			}
			else if (radiusIndex > maxRadius) {
#ifdef DEBUG
				cout << "Warning: vertical radius is larger than " << maxRadius << ". Node: " << index << endl;
#endif
				radiusIndex = maxRadius;
			}

			// calculate sign
			int sign_cross_ab = (diff_PrePointX * diff_PostPointY - diff_PostPointX - diff_PrePointY) >= 0 ? 1 : -1;
			radiusIndex = sign_cross_ab * radiusIndex;
		}
		else {
			radiusIndex = maxRadius;
		}
	}

	this->nodes[index].verticalCurveRadius = radiusIndex;

	// error
	if (this->nodes[index].verticalCurveRadius == nan("")) {
		cout << "Error: Calculation of vertical radius failed. Node: " << index << endl;
		this->retval = -1;
	}
}

void DatAuf::CalcDatAuf::CalcGradientPercentage(size_t index) {
	node NodeCurrent, NodeForward, NodeBackward;
	size_t MaxNumberNodes = this->nodes.size();
	if (index == 0) {
		if (nodes.front().id == nodes.back().id) {
			// Close
			NodeCurrent = GetNode(0);
			NodeBackward = GetNode(MaxNumberNodes - 2);
			NodeForward = GetNode(1);
		}
		else {
			// Open
			NodeCurrent = GetNode(0);
			NodeForward = GetNode(1);
			// Correction in "elevation" for Backward-node in order to use general formula for Finite Differences
			NodeBackward = GetNode(1);
			NodeBackward.elevation = -NodeForward.elevation + 2 * NodeCurrent.elevation;
		}
	}
	else if (index == (MaxNumberNodes - 1)) {
		if (nodes.front().id == nodes.back().id) {
			// Close
			NodeCurrent = GetNode(index);
			NodeForward = GetNode(1);
			NodeBackward = GetNode(index - 1);
		}
		else {
			// Open
			NodeCurrent = GetNode(index);
			NodeBackward = GetNode(index - 1);
			// Correction in "elevation" in order to use general formula for Finite Differences
			NodeForward = GetNode(index - 1);
			NodeForward.elevation = -NodeBackward.elevation + 2 * NodeCurrent.elevation;
		}
	}
	else {
		NodeCurrent = GetNode(index);
		NodeBackward = GetNode(index - 1);
		NodeForward = GetNode(index + 1);
	}

	double ForwardGradientInMeter = CalcForwardDerivativeFD(NodeCurrent, NodeForward);
	double BackwardGradientInMeter = CalcBackwardDerivativeFD(NodeCurrent, NodeBackward);
	this->nodes[index].gradient = (ForwardGradientInMeter + BackwardGradientInMeter) / 2.0 * 100.0;

	// error
	if (this->nodes[index].gradient == nan("")) {
		cout << "Error: Calculation of gradient failed. Node: " << index << endl;
		this->retval = -1;
	}
	if (this->nodes[index].gradient < -100.0 || this->nodes[index].gradient > 100.0) {
		cout << "Error: Absolute value of calculated gradient bigger 100 percent:  Node: " << index << endl;
		this->retval = -1;
	}
}

double DatAuf::CalcDatAuf::CalcForwardDerivativeFD(node NodeCurrent, node NodeForward) {
	double DistanceTwoNodes = GetDistanceMeters3D(NodeCurrent, NodeForward);
	return (NodeForward.elevation - NodeCurrent.elevation) / DistanceTwoNodes;
}

double DatAuf::CalcDatAuf::CalcBackwardDerivativeFD(node NodeCurrent, node NodeBackward) {
	double DistanceTwoNodes = GetDistanceMeters3D(NodeCurrent, NodeBackward);
	return (NodeCurrent.elevation - NodeBackward.elevation) / DistanceTwoNodes;
}

double DatAuf::CalcDatAuf::deg2rad(double grad) {
	double rad = grad * 3.14159265358979 / 180;
	return rad;
}

bool DatAuf::CalcDatAuf::isLoop() {
	size_t MaxIndexNodes = this->nodes.size() - 1;
	return (nodes[0].id == nodes[MaxIndexNodes].id);
}

node DatAuf::CalcDatAuf::GetNode(size_t NodeItem) {
	return this->nodes[NodeItem];
}