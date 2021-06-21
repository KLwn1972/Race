//#ifndef _VECTOR_H		// Brauchen wir Vector.h nur hier?
//#define _VECTOR_H

#include <iostream>
#include "DatAuf_SplineCatmullRom.h"
#include "Race.h"


using namespace std;

namespace DatAuf {


	class CalcDatAuf {
	public:
		vector<node> nodes;
		void DataProcessing();
		void InsertAdditionalNodes();
		void CalcRadiusGradientData();

	private:
		void CalcHorizontalCurveRad(int index);
		void CalcVerticalCurveRad(int index);
		void CalcGradientPercentage(int index);
		void InsertOneNodeRecursiv(node Node1, node Node2, SplineCatmullRom SplineSegment);
		double Get1Coordinate(int);
		double deg2rad(double grad);


	};

	static double GetDistanceMeters(node node1, node node2);

}




//#endif
