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
	//Soll_Fahrtbestimmung(Vehicle& vehicle, SimulationEnvironment& environment);
	void setVehicle(Vehicle* vehicle);
	void setEnvironment(SimulationEnvironment* environment);

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
	double EngineTorque_v_CUR[10] = { 0, 2000, 4000, 5000, 6000, 7000, 8000, 10000, 12000, 14000 }; //umwalden in Km/h
	double EngineTorque_trq_CUR[10] = { 400, 400, 400, 400, 350, 257, 225, 140, 90, 50 };
	double Mass = 2000; //vom Json;
	double V_max_Fzg = 350; //vom Json (Umwandeln in m/s) aus V_CUR (letzte Komponente des ArraEngineTorque_trq_CUR)
	double R_wheel = 0.533; //später nicht mehr nötig, wenn Drehmoment vs Geschwindigkeit CUR benutzt wird.
	double t_ratio = 0.125; //später nicht mehr nötig, wenn Drehmoment vs Geschwindigkeit CUR benutzt wird.
	double RollingResistanceCoefficient = 0.02; //fr aus Json;
	double DragCoefficient = 0.26; //cw aus Json;
	double FrontalArea = 2;
	double FrictionCoefficient = 0.1; //Klären mit Gruppe 6 wie die Kurve abzufragen !

	vector<node> n;
	Vehicle* vehicle = new Vehicle();
	SimulationEnvironment* environment = new SimulationEnvironment();

	double const Airdensity = 1.121;
};
