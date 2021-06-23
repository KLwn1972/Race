#pragma once

#include <iostream>
#include "DatAuf_SplineCatmullRom.h"
#include "Race.h"


using namespace std;

namespace DatAuf {


	class CalcDatAuf {
	public:
		vector<node> nodes;
		SplineCatmullRom SplineSegment;
		void DataProcessing();
		void InsertAdditionalNodes();
		void CopyNodesToSplineKnots(int NodeItem);
		node GetInterpolKnot();
		node GetNode(int NodeItem);
		void InsertOneAdditionalNode(int NodeItem, int NumberAdditionalNodes, node NewNode);
		void UpdateNodeIDProperty(int NodeItem, int NumberAdditionalNodes);
		void CalcDistanceToAllNextNode();
		void CalcRadiusGradientData();
		double GetDistanceMeters2D(node node1, node node2);
		void GetTestData();
		bool isLoop();
	private:
		void CalcHorizontalCurveRad(int index);
		void CalcVerticalCurveRad(int index);
		void CalcGradientPercentage(int index);
		double CalcForwardDerivativeFD(node NodeCurrent, node NodeForward);
		double CalcBackwardDerivativeFD(node NodeCurrent, node NodeBackward);
		void InsertOneNodeRecursiv(node Node1, node Node2, SplineCatmullRom SplineSegment);
		double Get1Coordinate(int);

	};

	static double GetDistanceMeters3D(node node1, node node2);
	static double deg2rad(double grad);
	//static double rad2deg(double rad);
}

