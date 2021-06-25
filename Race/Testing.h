#pragma once
#ifndef TESTINGDEF
#define TESTINGDEF

using namespace std;
#include <string>
#include "race.h"

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
	vector<Testing> Testvektor = vector<Testing>();
	void TestSollfahrtbestimmung(vector<node>& nodes);
	void TestDatenAufbereitung(vector<node>& nodes);
	void TestDatenbeschaffung(vector<node>& nodes);
	void TestFahrphysik(vector<node>& nodes);
	void PreTestFahrphysik(vector <node>& nodes);

private:
	node StartNode = node();
};

#endif