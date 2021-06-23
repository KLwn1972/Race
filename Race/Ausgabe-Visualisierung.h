#pragma once
#include "Race.h";

bool is_main_node(string);
void ausgabe_visualisierung(vector <node>&, string);
string timeConversion(double, time_t);
void insertElementKML(tinyxml2::XMLDocument&, XMLElement*, string, string);
void insertColorDefinitionKML(tinyxml2::XMLDocument&, XMLElement*, string, string);
string generate_color_code(double, double, double);
string generate_color_code2(double, double, double);
XMLError output_kml(vector<node>&, string);
void add_node_gpx(tinyxml2::XMLDocument*, node*, XMLElement*, time_t);
XMLError output_gpx(vector<node>&, string);
string createNotesKML(vector<node>::iterator);
double get_max_speedIs(vector <node>&);
double get_min_speedIs(vector <node>&);
