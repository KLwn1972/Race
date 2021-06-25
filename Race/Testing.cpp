#include "Testing.h"
#include<stdio.h>
#include <string>
#include <vector>

//Parameter Nordschleife

double MinRadiusTest = 33; //[m]
double MinGradientTest = -11; //[%]
double MaxGradientTest = 18; //[%]
double NeunzigGradSteigung = 5728; //[%] 89° da 90° undefiniert
string IDStartpunkt = "6535972824";
double LatitudeStartPunkt = 50.3451467;
double LongitudeStartPunkt = 6.9264464;
double MaxSpeedTesla = 261 /3.6;  //[m/s]

void ErrorLog::TestDatenbeschaffung(vector<node>& nodes) {
	Testing Test2add;
	Test2add.Aufgabe = "Datenbeschaffung";
	Test2add.Testname = "IdStartPunkt";
	Test2add.Ergebnisse = "Passed";
	if (nodes[1].id != IDStartpunkt) { //
		Test2add.Ergebnisse = "Failed";
	}
	Testvektor.push_back(Test2add);
	
	Test2add.Aufgabe = "Datenbeschaffung";
	Test2add.Testname = "KoordinatenStartPunkt";
	Test2add.Ergebnisse = "Passed";
	if (nodes[1].latitude != LatitudeStartPunkt || nodes[1].longitude != LongitudeStartPunkt) {
		Test2add.Ergebnisse = "Failed";
	}
	Testvektor.push_back(Test2add);
}

void ErrorLog::TestDatenAufbereitung(vector<node>& nodes) {
	int index = 0;
	Testing Test2add;
	Test2add.Aufgabe = "Datenaufbereitung";
	Test2add.Testname = "SteigungKleiner90Grad";
	Test2add.Ergebnisse = "Passed";
	size_t Sizevektor = nodes.size();
	while (index < Sizevektor - 1) {
		if (nodes[index].gradient > NeunzigGradSteigung) { //89° in %
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
		if (nodes[index].gradient > MaxGradientTest || nodes[index].gradient < MinGradientTest) { //Werte für Nordschleife
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
		if (nodes[index].horizontalCurveRadius < MinRadiusTest) {  //Angabe für Nordschleife ist >=33m
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
		if (nodes[index].speedIs < 0) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
}

void ErrorLog::TestFahrphysik(vector<node>& nodes) {
	Testing Test2add;
	Test2add.Aufgabe = "Fahrphysik";
	Test2add.Testname = "IdStartPunkt";
	Test2add.Ergebnisse = "Passed";
	if (nodes[1].id != IDStartpunkt) { //
		Test2add.Ergebnisse = "Failed";
	}
	Testvektor.push_back(Test2add);
	
	Test2add.Aufgabe = "Fahrphysik";
	Test2add.Testname = "KoordinatenStartPunkt";
	Test2add.Ergebnisse = "Passed";
	if (nodes[1].latitude != LatitudeStartPunkt || nodes[1].longitude != LongitudeStartPunkt) {
		Test2add.Ergebnisse = "Failed";
	}
	Testvektor.push_back(Test2add);
	
	Test2add.Aufgabe = "Fahrphysik";
	Test2add.Testname = "GeschwindigkeitStartPunkt";
	Test2add.Ergebnisse = "Passed";
	if (nodes[1].speedIs != 0) {
		Test2add.Ergebnisse = "Failed";
	}
	Testvektor.push_back(Test2add);
	
	int index = 0;
	Test2add.Aufgabe = "Fahrphysik";
	Test2add.Testname = "KeinNaNmehr";
	Test2add.Ergebnisse = "Passed";
	size_t Sizevektor = nodes.size();
	while (index < Sizevektor - 1) {
		if (nodes[index].longitude == nan("") ||
			nodes[index].latitude == nan("") ||
			nodes[index].elevation == nan("") ||
			nodes[index].distanceToNext == nan("") ||
			nodes[index].horizontalCurveRadius == nan("") ||
			nodes[index].verticalCurveRadius == nan("") ||
			nodes[index].gradient == nan("") ||
			nodes[index].speedLimit == nan("") ||
			nodes[index].speedIs == nan("") ||
			nodes[index].raceTime == nan("")) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
	
	index = 0;
	Test2add.Aufgabe = "Fahrphysik";
	Test2add.Testname = "MaxSpeedRealistisch";
	Test2add.Ergebnisse = "Passed";
	Sizevektor = nodes.size();
	while (index < Sizevektor - 1) {
		if (nodes[index].speedIs > MaxSpeedTesla) {
			Test2add.Ergebnisse = "Failed";
		}
		index++;
	}
	Testvektor.push_back(Test2add);
}