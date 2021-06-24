/************************************************************************************************
Projekt 2
Aufgabe 5: Soll_Fahrtbestimmung
Francesco Fusiello, Verena Fuchs und Daniel Kucher
************************************************************************************************/

#pragma once
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include "race.h"
#include "Simulation/Vehicle.h"
#include "Simulation/SimulationEnvironment.h"

using namespace std;
using namespace Simulation;

class Soll_Fahrtbestimmung : public node
{
public:
	void setVehicle(Vehicle* vehicle);
	void setEnvironment(SimulationEnvironment* environment);

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
	vector<node> n;
	Vehicle* vehicle = new Vehicle();
	SimulationEnvironment* environment = new SimulationEnvironment();
};
