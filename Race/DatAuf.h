#ifndef _VECTOR_H		// Brauchen wir Vector.h nur hier?
#define _VECTOR_H

#include <iostream>
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
		vector <double> Get1Coordinate(int);

	};

	static double GetDistanceMeters(const double& longitude1, const double& latitude1, const double& longitude2, const double& latitude2); //GetDistance(node node1, node node2)

}




#endif
