#include "Testing.h"
#include<stdio.h>
#include <string>
#include <vector>

void ErrorLog::TestDatenAufbereitung(vector<node>& nodes) {
	int index = 0;
	Testing Test2add;
	Test2add.Aufgabe = "Datenaufbereitung";
	Test2add.Testname = "SteigungKleiner90Grad";
	Test2add.Ergebnisse = "Passed";
	size_t Sizevektor = nodes.size();
	while (index < Sizevektor - 1) {
		if (nodes[index].gradient > 90) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
	Test2add.Aufgabe = "Datenaufbereitung";
	Test2add.Testname = "SteigungWertebereich";
	Test2add.Ergebnisse = "Passed";
	index = 0;
	while (index < Sizevektor - 1) {
		if (nodes[index].gradient > 18 || nodes[index].gradient < -11) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
	Test2add.Aufgabe = "Datenaufbereitung";
	Test2add.Testname = "MinKurvenRadius";
	Test2add.Ergebnisse = "Passed";
	index = 0;
	while (index < Sizevektor - 1) {
		if (nodes[index].horizontalCurveRadius < 1) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
	Test2add.Aufgabe = "Datenaufbereitung";
	Test2add.Testname = "DistanceToNext";
	Test2add.Ergebnisse = "Passed";
	index = 0;
	while (index < Sizevektor - 1) {
		if (nodes[index].distanceToNext > 1) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
}

void ErrorLog::TestSollfahrtbestimmung(vector<node>& nodes) {
	int index = 0;
	Testing Test2add;
	Test2add.Aufgabe = "Sollfahrtbestimmung";
	Test2add.Testname = "Vmax kleiner Vmax_Limited";
	Test2add.Ergebnisse = "Passed";
	size_t Sizevektor = nodes.size();
	while (index < Sizevektor - 1) {
		if (nodes[index].speedIs > nodes[index].speedLimit) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
	index = 0;
	Test2add.Aufgabe = "Sollfahrtbestimmung";
	Test2add.Testname = "Nur positive Geschwindigkeit";
	Test2add.Ergebnisse = "Passed";
	while (index < Sizevektor - 1) {
		if (nodes[index].speedIs <0) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
	index = 0;
	/*Testing Test2add;
	Test2add.Aufgabe = "Sollfahrtbestimmung";
	Test2add.Testname = "alle NaN Ersetzt";
	Test2add.Ergebnisse = "Passed";
	while (index < Sizevektor - 1) {
		if (nodes[index].speedIs==nan ) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);*/
}