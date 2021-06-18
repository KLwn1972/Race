#include "Soll_Fahrtbestimmung.h"
#include <map>
#include <string>
#include <iostream>
#include <vector> 
#include <math.h>
#include "race.h"
//#include <queue> 
//#include <set> 
//#include <utility> 

using namespace std;

double const g = 9.81;
double const rho = 1.121;

//Soll_Fahrtbestimmung::Soll_Fahrtbestimmung(double _g) : g(_g) {}

void Soll_Fahrtbestimmung::set_node(node k){

}

double Soll_Fahrtbestimmung::F_normal(double V) {
    cout << "F_normal" << M * g * cos(atan(elevation / 100)) - F_kuppe(V) << "\n";
    return M * g * cos(atan(elevation/100)) - F_kuppe(V);
}
double Soll_Fahrtbestimmung::F_kuppe(double V) {
    cout << "F_kuppe" << M * V * V / verticalCurveRadius << "\n";
    return M * V * V / verticalCurveRadius;
}
double Soll_Fahrtbestimmung::F_quer(double V) {
    cout << "F_kurve" << M * V * V / horizontalCurveRadius << "\n";
    return M * V * V / horizontalCurveRadius;
}

//Widerstandskräfte
double Soll_Fahrtbestimmung::F_luft(double V) {
    cout << "F_luft" << 0.5 * rho * cw * A_stirn * V * V << "\n";
	return 0.5 * rho * cw * A_stirn * V * V;
}
double Soll_Fahrtbestimmung::F_gewicht() {
    cout << "F_gewicht" << M * g * sin(atan(elevation / 100)) << "\n";
	return M * g * sin(atan(elevation / 100));
}
double Soll_Fahrtbestimmung::F_roll(double V) {
    cout << "F_roll" << fr * F_normal(V) << "\n";
    return fr * F_normal(V);
}
double Soll_Fahrtbestimmung::F_fahrwiderstand(double V) {
    cout << "F_fahrwiderstand" << F_luft(V) + F_gewicht() + F_roll(V) << "\n";
	return F_luft(V) + F_gewicht() + F_roll(V);
}


double Soll_Fahrtbestimmung::F_haft_laengs(double V) {
    cout << "F_haft_laengs" << sqrt((mue_haft * F_normal(V)) * (mue_haft * F_normal(V)) - F_quer(V) * F_quer(V)) << "\n";
	return sqrt((mue_haft * F_normal(V)) * (mue_haft * F_normal(V)) - F_quer(V) * F_quer(V));
  //return sqrt(pow(mue_haft*F_normal, 2) - pow(F_quer, 2));
}

double Soll_Fahrtbestimmung::V_haft_quer_max() { //Getestet OK
    cout << "V_haft_quer_max" << sqrt(mue_haft * g * cos(atan(elevation / 100)) / ((mue_haft / verticalCurveRadius) + (1 / horizontalCurveRadius))) << "\n";
	return sqrt(mue_haft * g * cos(atan(elevation/100))/((mue_haft/verticalCurveRadius) + (1/horizontalCurveRadius)));
}

double Soll_Fahrtbestimmung::F_antrieb_max(double V) {
        /* NOTE: xs MUST be sorted */
        double pi = 2 * asin(1);
        double n = V / R_wheel / t_ratio * 60 / (2*pi);
        static const double xs[] = { 0, 2000, 4000, 5000, 6000, 8000, 10000, 12000, 14000 };
        static const double ys[] = { 400, 400, 400, 400, 350, 257, 225, 140, 90, 50 };

        /* number of elements in the array */
        static const int count = sizeof(xs) / sizeof(xs[0]);

        int i;
        double dx, dy;

        if (n <= xs[0]) {
            /* x is less than the minimum element
             * handle error here if you want */
            cout << "F_antrieb_max" << ys[0] / R_wheel / t_ratio<< "\n";
            cout << "Drehmoment" << ys[0] << "\n";
            return ys[0] / R_wheel / t_ratio; /* return minimum element */
        }

        if (n >= xs[count - 1]) {
            cout << "F_antrieb_max" << ys[count - 1] / R_wheel / t_ratio << "\n";
            cout << "Drehmoment" << ys[count - 1] << "\n";
            return ys[count - 1] / R_wheel / t_ratio; /* return maximum */
        }

        /* find i, such that xs[i] <= x < xs[i+1] */
        for (i = 0; i < count - 1; i++) {
            if (xs[i + 1] > n) {
                break;
            }
        }

        /* interpolate */
        dx = xs[i + 1] - xs[i];
        dy = ys[i + 1] - ys[i];
        cout << "F_antrieb_max" << (ys[i] + (n - xs[i]) * dy / dx) / R_wheel / t_ratio << "\n";
        cout << "Drehmoment" << (ys[i] + (n - xs[i]) * dy / dx) << "\n";
        return (ys[i] + (n - xs[i]) * dy / dx) / R_wheel / t_ratio;
    }


double Soll_Fahrtbestimmung::V_max() { 

    for (int Vel = 0; Vel <= int(V_max_Fzg / 3.6); Vel++) {

        //17.06.21 //funktioniert erst wenn kein Null Radius zugelassen is !!!
        cout << "Diff_" << min(F_haft_laengs(Vel), F_antrieb_max(Vel)) - F_fahrwiderstand(Vel) << "\n";
        if (Vel > V_haft_quer_max() || F_normal(Vel) < 0) {
            return Vel--;
        }
        else if (min(F_haft_laengs(Vel), F_antrieb_max(Vel)) - F_fahrwiderstand(Vel) < 0) {
            return Vel--;
        }    
    }
    return V_max_Fzg;
}


void Soll_Fahrtbestimmung::SpeedLimit_route(vector<node> &Strecke) {  //Methode getestet OK bis auf V_max.
    
    vector<node>::iterator n;
    for (n = Strecke.begin(); n != Strecke.end(); n++) {
        double* array = (*n).get_node_Properties();
        elevation = array[0];
        horizontalCurveRadius = array[1];
        verticalCurveRadius = array[2];
        speedLimit = V_max();
        (*n).set_speedLimit(speedLimit); //Test Code fuer V_max_strecke: n->speedLimit = 60;
        cout << "Soll_Fahrtbestimmung speedLimit" << speedLimit << "\n";
    }
}