#include "Soll_Fahrtbestimmung.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include "race.h"
#include "Simulation/SimulationEnvironment.h"
//#include <queue>
//#include <set>
//#include <utility>

using namespace std;

using namespace Simulation;

//Soll_Fahrtbestimmung::Soll_Fahrtbestimmung(double _g) : g(_g) {}
//
//Soll_Fahrtbestimmung::Soll_Fahrtbestimmung(Vehicle& vehicle, SimulationEnvironment& environment)
//{
//	this->vehicle = vehicle;
//	this->environment = environment;
//}

void Soll_Fahrtbestimmung::setVehicle(Vehicle* vehicle)
{
	this->vehicle = vehicle;
}

void Soll_Fahrtbestimmung::setEnvironment(SimulationEnvironment* environment)
{
	this->environment = environment;
}

//void Soll_Fahrtbestimmung::set_node(node k) {
//}

double Soll_Fahrtbestimmung::F_normal(double V) {
	cout << "F_normal" << vehicle->Mass * GRAVITATIONALCONSTANT * cos(atan(gradient / 100)) - F_kuppe(V) << "\n";
	return vehicle->Mass * GRAVITATIONALCONSTANT * cos(atan(gradient / 100)) - F_kuppe(V);
}
double Soll_Fahrtbestimmung::F_kuppe(double V) {
	cout << "F_kuppe" << vehicle->Mass * V * V / verticalCurveRadius << "\n";
	return vehicle->Mass * V * V / verticalCurveRadius;
}
double Soll_Fahrtbestimmung::F_quer(double V) {
	cout << "F_kurve" << vehicle->Mass * V * V / horizontalCurveRadius << "\n";
	return vehicle->Mass * V * V / horizontalCurveRadius;
}

//Widerstandskräfte
double Soll_Fahrtbestimmung::F_luft(double V) {
	cout << "F_luft" << 0.5 * environment->calcAirDensity(500) * vehicle->DragCoefficient * vehicle->FrontalArea * V * V << "\n";
	return 0.5 * environment->calcAirDensity(500) * vehicle->DragCoefficient * vehicle->FrontalArea * V * V;
}
double Soll_Fahrtbestimmung::F_gewicht() {
	cout << "F_gewicht" << vehicle->Mass * GRAVITATIONALCONSTANT * sin(atan(gradient / 100)) << "\n";
	return vehicle->Mass * GRAVITATIONALCONSTANT * sin(atan(gradient / 100));
}
double Soll_Fahrtbestimmung::F_roll(double V) {
	cout << "F_roll" << environment->getRollingResistanceCoefficient() * F_normal(V) << "\n";
	return environment->getRollingResistanceCoefficient() * F_normal(V);
}
double Soll_Fahrtbestimmung::F_fahrwiderstand(double V) {
	cout << "F_fahrwiderstand" << F_luft(V) + F_gewicht() + F_roll(V) << "\n";
	return F_luft(V) + F_gewicht() + F_roll(V);
}

double Soll_Fahrtbestimmung::F_haft_laengs(double V) {
	cout << "F_haft_laengs" << sqrt((environment->calcFrictionCoefficient(V) * F_normal(V)) * (environment->calcFrictionCoefficient(V) * F_normal(V)) - F_quer(V) * F_quer(V)) << "\n";
	return sqrt((environment->calcFrictionCoefficient(V) * F_normal(V)) * (environment->calcFrictionCoefficient(V) * F_normal(V)) - F_quer(V) * F_quer(V));
	//return sqrt(pow(FrictionCoefficient*F_normal, 2) - pow(F_quer, 2));
}

double Soll_Fahrtbestimmung::V_haft_quer_max() { //Getestet OK
	cout << "V_haft_quer_max" << sqrt(environment->calcFrictionCoefficient(60 * KMH2MS) * GRAVITATIONALCONSTANT * cos(atan(gradient / 100)) / ((environment->calcFrictionCoefficient(60 * KMH2MS) / verticalCurveRadius) + (1 / horizontalCurveRadius))) << "\n";
	return sqrt(environment->calcFrictionCoefficient(60 * KMH2MS) * GRAVITATIONALCONSTANT * cos(atan(gradient / 100)) / ((environment->calcFrictionCoefficient(60 * KMH2MS) / verticalCurveRadius) + (1 / horizontalCurveRadius)));
}

double Soll_Fahrtbestimmung::F_antrieb_max(double V) {
	cout << "F_antrieb_max" << vehicle->interpolateEngineTorqueFromVelocity(V) / (vehicle->WheelSize * INCH2M) / vehicle->FinalDriveRatio << "\n";
	//cout << "Torque at wheel" << vehicle->interpolateEngineTorqueFromVelocity(V);
	cout << (vehicle->WheelSize * INCH2M);
	return vehicle->interpolateEngineTorqueFromVelocity(V) / (vehicle->WheelSize * INCH2M) / vehicle->FinalDriveRatio;

	///* NOTE: EngineTorque_v_CUR MUST be sorted */
	//double n = V / R_wheel / t_ratio * 60 / (2 * PI);
	///*static const double EngineTorque_v_CUR[] = { 0, 2000, 4000, 5000, 6000, 8000, 10000, 12000, 14000 };
	//static const double EngineTorque_trq_CUR[] = { 400, 400, 400, 400, 350, 257, 225, 140, 90, 50 };*/

	///* number of elements in the array */
	//static const int count = sizeof(EngineTorque_v_CUR) / sizeof(EngineTorque_v_CUR[0]);

	//int i;
	//double dx, dy;

	//if (n <= EngineTorque_v_CUR[0]) {
	//	/* x is less than the minimum element
	//	 * handle error here if you want */
	//	cout << "F_antrieb_max" << EngineTorque_trq_CUR[0] / R_wheel / t_ratio << "\n";
	//	cout << "Drehmoment" << EngineTorque_trq_CUR[0] << "\n";
	//	return EngineTorque_trq_CUR[0] / R_wheel / t_ratio; /* return minimum element */
	//}

	//if (n >= EngineTorque_v_CUR[count - 1]) {
	//	cout << "F_antrieb_max" << EngineTorque_trq_CUR[count - 1] / R_wheel / t_ratio << "\n";
	//	cout << "Drehmoment" << EngineTorque_trq_CUR[count - 1] << "\n";
	//	return EngineTorque_trq_CUR[count - 1] / R_wheel / t_ratio; /* return maximum */
	//}

	///* find i, such that EngineTorque_v_CUR[i] <= x < EngineTorque_v_CUR[i+1] */
	//for (i = 0; i < count - 1; i++) {
	//	if (EngineTorque_v_CUR[i + 1] > n) {
	//		break;
	//	}
	//}

	///* interpolate */
	//dx = EngineTorque_v_CUR[i + 1] - EngineTorque_v_CUR[i];
	//dy = EngineTorque_trq_CUR[i + 1] - EngineTorque_trq_CUR[i];
	//cout << "F_antrieb_max" << (EngineTorque_trq_CUR[i] + (n - EngineTorque_v_CUR[i]) * dy / dx) / R_wheel / t_ratio << "\n";
	//cout << "Drehmoment" << (EngineTorque_trq_CUR[i] + (n - EngineTorque_v_CUR[i]) * dy / dx) << "\n";
	//return (EngineTorque_trq_CUR[i] + (n - EngineTorque_v_CUR[i]) * dy / dx) / R_wheel / t_ratio;
}

double Soll_Fahrtbestimmung::V_max() {
	//double mass = this->vehicle->Mass;
	//this->vehicle->TorqueSpeedCurve->getY(10);
	for (int Vel = 0; Vel <= int(vehicle->VMaxLimited); Vel++) {
		cout << "*******************Vel" << Vel << "\n";
		//17.06.21 //funktioniert erst wenn kein Null Radius zugelassen is !!!
		cout << "Diff_" << min(F_haft_laengs(Vel), F_antrieb_max(Vel)) - F_fahrwiderstand(Vel) << "\n";
		if (Vel > V_haft_quer_max() || F_normal(Vel) < 0) {
			cout << "Soll_Fahrtbestimmung speedLimit" << ((Vel - 1 < 0) ? 0 : (--Vel)) << "\n";
			return (Vel - 1 < 0) ? 0 : (--Vel);
		}
		else if (min(F_haft_laengs(Vel), F_antrieb_max(Vel)) - F_fahrwiderstand(Vel) < 0) {
			cout << "Soll_Fahrtbestimmung speedLimit" << ((Vel - 1 < 0) ? 0 : (--Vel)) << "\n";
			return (Vel - 1 < 0) ? 0 : (--Vel);
		}
	}
	cout << "Soll_Fahrtbestimmung speedLimit" << vehicle->VMaxLimited << "\n";
	return vehicle->VMaxLimited;
}

void Soll_Fahrtbestimmung::SpeedLimit_route(vector<node>& Strecke) {  //Methode getestet OK bis auf V_max.
	for (node& n : Strecke)
	{
		gradient = n.gradient;
		horizontalCurveRadius = n.horizontalCurveRadius;
		verticalCurveRadius = n.verticalCurveRadius;
		n.speedLimit = V_max();
		cout << "Soll_Fahrtbestimmung speedLimit" << n.speedLimit << "\n";
	}
}