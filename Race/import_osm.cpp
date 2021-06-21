#define _USE_MATH_DEFINES

#include "import_osm.h"
#include "cJSON.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdlib.h>

using namespace std;

node::node(double _x, double _y, string _name) : x(_x), y(_y), name(_name) {}

adjazenz::adjazenz(double _w, int _t) : w(_w), t(_t) {}


void graph::out_ascii() {
	map<string, node>::iterator n;
	map<string, adjazenz>::iterator a;
	for (n = this->n.begin(); n != this->n.end(); n++) {
		cout << "Knoten " << n->first << ": ";
		cout << n->second.x << "/" << n->second.y << "/" << n->second.name << endl;
		cout << "Adjazenzen:" << endl;
		for (a = e[n->first].begin(); a != e[n->first].end(); a++) {
			cout << "\t" << a->first << ": w=" << a->second.w << ", t=" << a->second.t << endl;
		}
	}
}


//Berechnung der Entfernung zwischen zwei geografischen Punkten, hier node a und node b
double graph::Aufgabe1__harversine(node a, node b) {
	//Gegeben seien zwei Punkte A und B auf der Kugel durch den Breitengrad (LAT/x/Phi) und Längengrad(LON/y/Lambda):

	double LatA = a.x;
	double LatB = b.x;
	double LonA = a.y;
	double LonB = b.y;

	//Winkel umrechnen in Bogenmass
	double LonAP = (LonA / 360) * 2 * M_PI;
	double LonBP = (LonB / 360) * 2 * M_PI;
	double LatAP = (LatA / 360) * 2 * M_PI;
	double LatBP = (LatB / 360) * 2 * M_PI;

	//Erdradius in km
	double R = 6378.137;

	//Formel harversine zur Errechnung des Abstandes
	double Abstand = (sin((LatBP - LatAP) / 2) * sin((LatBP - LatAP) / 2) + cos(LatAP) * cos(LatBP) * sin((LonBP - LonAP) / 2) * sin((LonBP - LonAP) / 2));
	Abstand = 2 * R * asin(sqrt(Abstand));

	return Abstand;
}

int graph::Aufgabe1__import_osm(string filepath) {


	double distance = 0;
	unsigned int i = 0, j = 0;
	string first = ""; string second = "";
	string str;
	string idstring;
	string wayidstring;
	string line;
	string file = filepath;
	ifstream t(filepath); //Import der .osm Datei

	cJSON* id = NULL;
	cJSON* lat = NULL;
	cJSON* lon = NULL;
	cJSON* name = NULL;
	cJSON* elements = NULL;
	cJSON* relation = NULL;
	cJSON* members = NULL;
	cJSON* nodes = NULL;
	cJSON* jsonobject = NULL;
	cJSON* type = NULL; 
	cJSON* ref = NULL;
	cJSON* tags = NULL;
	cJSON* waynode_ary = NULL;

	if (!t.is_open()) { cout << "Fehler: Kann nicht oeffnen!\n"; return 1; }

	//Textdatei lesen
	t.seekg(0, std::ios::end);
	t.seekg(0, std::ios::beg);
	str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>()); //Überführung des Dateiinhalts in String str
	t.close();

	//____________________________________________________________________________________________________________________________
	//Durchlaufen und speichern aller Wege in einer Relation
	cJSON* json_struct_complete = cJSON_Parse(str.c_str()); //Überführung des Strings str in cJSON Struktur json_struct_complete
	elements = cJSON_GetObjectItemCaseSensitive(json_struct_complete, "elements"); //elements: Array elements
	jsonobject = elements->child;
	members = cJSON_GetObjectItemCaseSensitive(jsonobject, "members"); //elements: Array elements
	jsonobject = members->child;
	vector<string> ways;
	//Schleife zum durchlaufen aller Wege in einer relation
	while (jsonobject != NULL) {
		ref = cJSON_GetObjectItemCaseSensitive(jsonobject, "ref");
		idstring = to_string(ref->valueint);	//schreiben in ein Vector o.ä.
		
		//Erzeugen von Wegen mit dazugehörigen Id's
		
		ways.push_back(idstring);
		
		
		jsonobject = jsonobject->next;
	}

	//____________________________________________________________________________________________________________________________
	//Durchlaufen und speichern aller Knoten in einem Weg
	cJSON* json_struct_complete = cJSON_Parse(str.c_str()); //Überführung des Strings str in cJSON Struktur json_struct_complete
	elements = cJSON_GetObjectItemCaseSensitive(json_struct_complete, "elements"); //elements: Array elements
	jsonobject = elements->child;
	nodes = cJSON_GetObjectItemCaseSensitive(jsonobject, "nodes"); //elements: Array elements
	jsonobject = nodes->child;
	
	vector<string> nodess;
	//Schleife zum durchlaufen aller Wege in einer relation
	while (jsonobject != NULL) {
		//ref = cJSON_GetObjectItemCaseSensitive(jsonobject, "");
		idstring = to_string(jsonobject->valueint);	//schreiben in ein Vector o.ä.
		
		//Erzeugen von Wegen mit dazugehörigen Id's
		nodess.push_back(idstring);
		
		jsonobject = jsonobject->next;
	}
	
	//____________________________________________________________________________________________________________________________
	//Speichern der Koordinaten eines Knotens
	cJSON* json_struct_complete = cJSON_Parse(str.c_str()); //Überführung des Strings str in cJSON Struktur json_struct_complete
	elements = cJSON_GetObjectItemCaseSensitive(json_struct_complete, "elements"); //elements: Array elements
	jsonobject = elements->child;

	vector<double> coord;
	//Schleife zum durchlaufen aller Wege in einer relation
		lat = cJSON_GetObjectItemCaseSensitive(jsonobject, "lat");
		lon = cJSON_GetObjectItemCaseSensitive(jsonobject, "lon");
		//Abspeichern der lat und lon in ein Vector...

		idstring = to_string(lat->valuedouble);
		double value = strtod(idstring.c_str(), NULL);
		coord.push_back(value);
		
		idstring = to_string(lon->valuedouble);
		double value2 = strtod(idstring.c_str(), NULL);
		coord.push_back(value2);
	


	//cJSON_ArrayForEach(jsonobject, elements) //#define cJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)
	//{
	//	type = cJSON_GetObjectItemCaseSensitive(jsonobject, "way");
	//	ref = cJSON_GetObjectItemCaseSensitive(jsonobject, "ref");

	//	if (strcmp(type->valuestring, "way") == 0)
	//	{
	//	//Kantenimport
	//		waynode_ary = cJSON_GetObjectItemCaseSensitive(jsonobject, "nodes");
	//		int ary_size = cJSON_GetArraySize(waynode_ary);
	//		for (j = 0; j < ary_size; j++) {
	//			id = cJSON_GetArrayItem(waynode_ary, j);
	//			wayidstring = to_string(id->valuedouble);
	//			wayidstring = wayidstring.substr(0, wayidstring.find(".", 0));
	//			second = wayidstring;

	//			if (j == 0) { first = wayidstring; }
	//			else {
	//				distance = Aufgabe1__harversine(n[first], n[second]);
	//				e[second][first] = distance;
	//				e[first][second] = distance;
	//			}
	//			first = second;
	//		}
	//	}

	//	//Knotenimport:
	//	else {
	//		y = cJSON_GetObjectItemCaseSensitive(jsonobject, "lat");
	//		x = cJSON_GetObjectItemCaseSensitive(jsonobject, "lon");
	//		tags = cJSON_GetObjectItemCaseSensitive(jsonobject, "tags");
	//		name = cJSON_GetObjectItemCaseSensitive(tags, "name");

	//		idstring = to_string(id->valuedouble);
	//		idstring = idstring.substr(0, idstring.find(".", 0));
	//		if (name != NULL) { n[idstring] = node(x->valuedouble, y->valuedouble, name->valuestring); }
	//		else { n[idstring] = node(x->valuedouble, y->valuedouble); }
	//	}
	//}

	/*
	//-----------------------------------------------------------------------------------------//

	Der folgende Teil ist vor der Verwendung von der cJSON Library entstanden. Um die Kanten
	ebenfalls mit cJSON Lib zu erzeugen, fehlte leider die Zeit. Code ist eine Mischung aus
	Variante 1: Stringverarbeitung und korrigierten cJSON Anteil.

	//-----------------------------------------------------------------------------------------//

	//Import der Adjazenzen:
	string filenew = filepath;
	ifstream _OSM(filenew);
	//_OSM.open(filenew, ios::in);
	if (!_OSM) { fprintf(stderr, "Fehler: Kann nicht oeffnen!\n"); return 1; }

	while (!_OSM.eof()) {
		getline(_OSM, line);
		i = 0;
		size_t pos = 0;
		size_t komma = 0;
		size_t begin = 0;

		if (size_t pos1 = line.find("nodes", pos) < UINT32_MAX) {
			do {
				getline(_OSM, line);
				komma = line.find(",", pos);
				begin = line.find_first_not_of(" ", pos);
				string id = line.substr(begin, komma - begin);
				second = id;

				if (i == 0) { first = id; }
				else {
					distance = Aufgabe1__harversine(n[first], n[second]);
					e[second][first] = distance;
					e[first][second] = distance;
				}
				first = second;
				++i;

			} while ((komma < UINT32_MAX));
		}
	}

	_OSM.close();
	*/

	cout << "\nOK\n" << endl;
	return 0;
}

