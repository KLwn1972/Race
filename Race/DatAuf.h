// /////////////////////////////////////////////////////////////////////////
// Team Datenaufbereitung: Andreas Jakobi, Andreas Maier, Kathrin Gerhard //
// ///////////////////////////////////////////////////////////////////////// 

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
		int retval = 0;

		int DataProcessing();
		double GetDistanceMeters2D(node node1, node node2);


	private:
		// Node handling
		node GetInterpolKnot();
		node GetNode(size_t NodeItem);
		void InsertAdditionalNodes();
		void CopyNodesToSplineKnots(size_t NodeItem);
		void InsertOneAdditionalNode(size_t NodeItem, size_t NumberAdditionalNodes, node NewNode);
		void UpdateNodeIDProperty(size_t NodeItem, size_t NumberAdditionalNodes);
		void CalcDistanceToAllNextNode();
		double CalcForwardDerivativeFD(node NodeCurrent, node NodeForward);
		double CalcBackwardDerivativeFD(node NodeCurrent, node NodeBackward);

		//Caluclation street characteristica
		void CalcRadiusGradientData();
		void CalcHorizontalCurveRadviaXYcoordinates(size_t index);
		void CalcVerticalCurveRad(size_t index);		
		void CalcGradientPercentage(size_t index);
		double deg2rad(double grad);
		bool isLoop();
	};

	static double GetDistanceMeters3D(node node1, node node2);
}

