#pragma once
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include "race.h"

using namespace std;

class Soll_Fahrtbestimmung : public node
{
public:
	//Soll_Fahrtbestimmung(double);
	void set_node(node k);

	double F_quer(double V);
	double F_kuppe(double V);
	double F_normal(double V);


	double F_luft(double V);
	double F_gewicht();
	double F_roll(double V);
	double F_fahrwiderstand(double V);


	double F_haft_laengs(double V);

	double F_antrieb_max(double V);

	double V_max();

	double V_haft_quer_max();

	void SpeedLimit_route(vector<node>&);

private:
	double Drehmoment; //Kennlinie vom Json;
	double M = 2000; //vom Json;
	double V_max_Fzg = 350; //vom Json (Umwandeln in m/s)
	double R_wheel = 0.533;
	double t_ratio = 0.125;
	double fr = 0.02;
	double cw = 0.26;
	double A_stirn = 2;
	double mue_haft = 0.8;

	vector<node> n;
};

