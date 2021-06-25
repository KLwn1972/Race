/************************************************************************************************
Projekt 2
Aufgabe 5: Soll_Fahrtbestimmung
Francesco Fusiello, Verena Fuchs und Daniel Kucher
************************************************************************************************/

#include "Soll_Fahrtbestimmung.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include "race.h"
#include "Simulation/SimulationEnvironment.h"

using namespace std;
using namespace Simulation;

void Soll_Fahrtbestimmung::setVehicle(Vehicle* vehicle)
{
	this->vehicle = vehicle;
}

void Soll_Fahrtbestimmung::setEnvironment(SimulationEnvironment* environment)
{
	this->environment = environment;
}

double Soll_Fahrtbestimmung::F_normal(double V) {
	return vehicle->Mass * GRAVITATIONALCONSTANT * cos(atan(gradient / 100)) - F_kuppe(V);
}

double Soll_Fahrtbestimmung::F_kuppe(double V) {
	return vehicle->Mass * V * V / verticalCurveRadius;
}

double Soll_Fahrtbestimmung::F_quer(double V) {
	return vehicle->Mass * V * V / horizontalCurveRadius;
}

double Soll_Fahrtbestimmung::F_luft(double V) {
	return 0.5 * environment->calcAirDensity(500) * vehicle->DragCoefficient * vehicle->FrontalArea * V * V;
}

double Soll_Fahrtbestimmung::F_gewicht() {
	return vehicle->Mass * GRAVITATIONALCONSTANT * sin(atan(gradient / 100));
}

double Soll_Fahrtbestimmung::F_roll(double V) {
	return environment->getRollingResistanceCoefficient() * F_normal(V);
}

double Soll_Fahrtbestimmung::F_fahrwiderstand(double V) {
	return F_luft(V) + F_gewicht() + F_roll(V);
}

double Soll_Fahrtbestimmung::F_haft_laengs(double V) {
	return sqrt((environment->calcFrictionCoefficient(V) * F_normal(V)) * (environment->calcFrictionCoefficient(V) * F_normal(V)) - F_quer(V) * F_quer(V));
}

double Soll_Fahrtbestimmung::V_haft_quer_max() {
	return sqrt(environment->calcFrictionCoefficient(60 * KMH2MS) * GRAVITATIONALCONSTANT * cos(atan(gradient / 100)) / ((environment->calcFrictionCoefficient(60 * KMH2MS) / verticalCurveRadius) + (1 / horizontalCurveRadius)));
}

double Soll_Fahrtbestimmung::F_antrieb_max(double V) {
	return vehicle->interpolateEngineTorqueFromVelocity(V) / (vehicle->WheelSize) * vehicle->FinalDriveRatio;
}

double Soll_Fahrtbestimmung::V_max() {
	for (int Vel = 0; Vel <= int(vehicle->VMaxLimited); Vel++) {
		if (Vel > V_haft_quer_max() || F_normal(Vel) < 0) {
			return (Vel - 1 < 0) ? 0 : (--Vel);
		}
		else if (min(F_haft_laengs(Vel), F_antrieb_max(Vel)) - F_fahrwiderstand(Vel) < 0) {
			return (Vel - 1 < 0) ? 0 : (--Vel);
		}
	}
	return vehicle->VMaxLimited;
}

void Soll_Fahrtbestimmung::SpeedLimit_route(vector<node>& Strecke) {
	for (node& n : Strecke) {
		gradient = n.gradient;
		horizontalCurveRadius = n.horizontalCurveRadius;
		verticalCurveRadius = n.verticalCurveRadius;
		n.speedLimit = V_max();
	}
}