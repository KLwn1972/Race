#pragma once
using namespace std;
#include <string>
#include "race.h"


//Parameter abhängig von der Teststrecke, hier Nordschleife-Nürburgring


class Testing
{
public:
	string Aufgabe = "";
	string Testname = "";
	string Ergebnisse = "";
};

class ErrorLog
{
public:
	vector<Testing> Testvektor=vector<Testing>();
	void TestSollfahrtbestimmung(vector<node>& nodes);
	void TestDatenAufbereitung(vector<node>& nodes);
	void TestDatenbeschaffung(vector<node>& nodes);
	void TestFahrphysik(vector<node>& nodes);
};