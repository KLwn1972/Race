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
		int DataProcessing();
		void InsertAdditionalNodes();
		void CopyNodesToSplineKnots(size_t NodeItem);
		node GetInterpolKnot();
		node GetNode(size_t NodeItem);
		void InsertOneAdditionalNode(size_t NodeItem, size_t NumberAdditionalNodes, node NewNode);
		void UpdateNodeIDProperty(size_t NodeItem, size_t NumberAdditionalNodes);
		void CalcDistanceToAllNextNode();
		void CalcRadiusGradientData();
		double GetDistanceMeters2D(node node1, node node2);
		void GetTestData();
		bool isLoop();
		int retval = 0;
	private:
		void CalcHorizontalCurveRad(size_t index);
		void CalcVerticalCurveRad(size_t index);
		void CalcGradientPercentage(size_t index);
		double CalcForwardDerivativeFD(node NodeCurrent, node NodeForward);
		double CalcBackwardDerivativeFD(node NodeCurrent, node NodeBackward);
		void InsertOneNodeRecursiv(node Node1, node Node2, SplineCatmullRom SplineSegment);
		double Get1Coordinate(int);
	};

	static double GetDistanceMeters3D(node node1, node node2);
	static double deg2rad(double grad);
}

