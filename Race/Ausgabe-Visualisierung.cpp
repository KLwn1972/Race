#include <iostream>
#include <vector>
#include "Race.h"
#include "tinyxml2.h"
#include "Ausgabe-Visualisierung.h"
#include <string>
#include <time.h>
#include <sstream>

using namespace std;
using namespace tinyxml2;

//function returns wether a given node is main node or not
bool is_main_node(string node_id) {
    if (node_id.find("_") == string::npos) return 1;
    else return 0;
}

// Function to create a date_string out of a starTime [s] and a raceTime [s]
string timeConversion(double raceTime, time_t startTime) {
    time_t rawtime;
    if (isnan(raceTime)) rawtime = (time_t)0 + startTime;
    else rawtime = (time_t)raceTime + startTime;
    struct tm timeinfo;
    localtime_s(&timeinfo, &rawtime);

    char buffer[21];
    strftime(buffer, 21, "%FT%TZ", &timeinfo);
    return buffer;
}

// Mainfunction of ausgabe_visualisierung
void ausgabe_visualisierung(vector <node>& track, string trackName) {
    XMLError fault_flag_gpx, fault_flag_kml;
    fault_flag_gpx = output_gpx(track, trackName);
    fault_flag_kml = output_kml(track, trackName);
    if (fault_flag_gpx != 0)
        cout << "Error while creating gpx-File. ErrorID: " << fault_flag_gpx << endl;
    if (fault_flag_kml != 0)
        cout << "Error while creating kml-File. ErrorID: " << fault_flag_kml << endl;
}

//Funktion creates a new element for a given XML-Document
void insertElementKML(tinyxml2::XMLDocument& xmlDoc, XMLElement* Element_parent, string elementName, string text) {
    XMLElement* Element_child = xmlDoc.NewElement(elementName.c_str());
    Element_child->SetText(text.c_str());
    Element_parent->InsertEndChild(Element_child);
}

//Funktion creates a new linestyle for a given XML-Document
void insertColorDefinitionKML(tinyxml2::XMLDocument& xmlDoc, XMLElement* Element_parent, string colorName, string colorCode) {
    XMLElement* Element_Style = xmlDoc.NewElement("Style");
    Element_Style->SetAttribute("id", colorName.c_str());
    Element_parent->InsertEndChild(Element_Style);

    XMLElement* Element_LineStyle = xmlDoc.NewElement("LineStyle");
    Element_Style->InsertEndChild(Element_LineStyle);
    insertElementKML(xmlDoc, Element_LineStyle, "color", colorCode.c_str());
    insertElementKML(xmlDoc, Element_LineStyle, "width", "12");
}

//Funktion gives back a colorcode
string generate_color_code(double act_value, double min_value, double max_value) {
    const int number_categories = 9;
    int  act_category;
    string color_code = "color0";
    act_category = (act_value - min_value) / ((max_value - min_value) / (number_categories - 1));
    switch (act_category) {
    case 0: color_code = "color0";
        break;
    case 1: color_code = "color1";
        break;
    case 2: color_code = "color2";
        break;
    case 3: color_code = "color3";
        break;
    case 4: color_code = "color4";
        break;
    case 5: color_code = "color5";
        break;
    case 6: color_code = "color6";
        break;
    case 7: color_code = "color7";
        break;
    case 8: color_code = "color8";
        break;
    }
    return color_code;
}
//Funktion gives back a colorcode
string generate_color_code2(double act_value, double min_value, double max_value) {
    long long int relative_value, color_number;
    stringstream stream;

    relative_value = act_value / (max_value - min_value) * 510;

    if (relative_value < 256) color_number = 4278255615 - (255 - relative_value) * 256;
    else                      color_number = 4278255615 - (relative_value - 255);

    stream << std::hex << color_number;
    std::string color_code(stream.str());
    return color_code;
}

//Erstellung KML Ausgabe
XMLError output_kml(vector <node>& track, string trackName) {
    time_t startTime;
    time(&startTime);
    string timestr;

    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LinkEndChild(xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));

    XMLNode* pRoot = xmlDoc.NewElement("kml");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement* Element_Document = xmlDoc.NewElement("Document");

    insertElementKML(xmlDoc, Element_Document, "name", trackName);
    vector<node>::iterator it = track.end() - 1;
    double v_min = get_min_speedIs(track);
    double v_max = get_max_speedIs(track); 
    string description = "race time = " + to_string(it->raceTime) 
        + " s\nmax. speed = " + to_string(v_max) + " m/s";
    insertElementKML(xmlDoc, Element_Document, "description", description);
    insertElementKML(xmlDoc, Element_Document, "visibility", "1");
    insertElementKML(xmlDoc, Element_Document, "open", "1");

    insertColorDefinitionKML(xmlDoc, Element_Document, "color0", "ff0000ff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color1", "ff0040ff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color2", "ff0080ff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color3", "ff00bfff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color4", "ff00ffff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color5", "ffbfff00");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color6", "ff80ff00");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color7", "ff40ff00");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color8", "ff00ff00");

    it = track.begin();
    //for (vector<node>::iterator it = track.begin(); it < track.end() - 1; ) {
    for (unsigned int i = 0; i < track.size() - 1; i++) {
        XMLElement* Element_Placemark = xmlDoc.NewElement("Placemark");
        Element_Document->InsertEndChild(Element_Placemark);

        insertElementKML(xmlDoc, Element_Placemark, "visibility", "1");
        insertElementKML(xmlDoc, Element_Placemark, "open", "0");
        //string color = "#color" + to_string(i % 9);  //Funktionsaufruf generate_color_code
        
        //string color = generate_color_code(it->speedIs, v_min, v_max);
        //insertElementKML(xmlDoc, Element_Placemark, "styleUrl", ("#"+color).c_str());

        string colorCode = generate_color_code2(it->speedIs, v_min, v_max);
        insertColorDefinitionKML(xmlDoc, Element_Document, it->id, colorCode);
        insertElementKML(xmlDoc, Element_Placemark, "styleUrl", ("#" + it->id).c_str());

        string name = "Track no. " + to_string(i);
        insertElementKML(xmlDoc, Element_Placemark, "name", name.c_str());
        description = createNotesKML(it);
        insertElementKML(xmlDoc, Element_Placemark, "description", description.c_str());

        XMLElement* Element_LineString = xmlDoc.NewElement("LineString");
        Element_Placemark->InsertEndChild(Element_LineString);

        insertElementKML(xmlDoc, Element_LineString, "extrude", "true");
        insertElementKML(xmlDoc, Element_LineString, "tessellate", "true");
        insertElementKML(xmlDoc, Element_LineString, "altitudeMode", "clampToGround");

        string coordinates = "";
        coordinates += to_string(it->longitude) + ","
            + to_string(it->latitude) + ","
            + to_string(it->elevation) + " ";
        it++;
        coordinates += to_string(it->longitude) + ","
            + to_string(it->latitude) + ","
            + to_string(it->elevation) + " ";
        insertElementKML(xmlDoc, Element_LineString, "coordinates", coordinates);
    }
    pRoot->InsertEndChild(Element_Document);
    xmlDoc.InsertEndChild(pRoot);

    XMLError eResult = xmlDoc.SaveFile((trackName+".kml").c_str());
    return eResult;
}

string createNotesKML(vector<node>::iterator it) {
    string notes = "<![CDATA[<br>node Id:" + it->id
        + "<br>longetude:" + to_string(it->longitude)
        + "<br>latitude:" + to_string(it->latitude)
        + "<br>elevation:" + to_string(it->elevation)
        + "<br>distanceToNext:" + to_string(it->distanceToNext)
        + "[m]<br>horizontalCurveRadius:" + to_string(it->horizontalCurveRadius)
        + "[m]<br>verticalCurveRadius:" + to_string(it->verticalCurveRadius)
        + "[m]<br>gradient:" + to_string(it->gradient)
        + "[%]<br>speed limit:" + to_string(it->speedLimit)
        + "[m/s]<br>current speed:" + to_string(it->speedIs)
        + "[m/s]<br>race time:" + to_string(it->raceTime)
        + "[s]]]>";
    return notes;
}
//get max SpeedIs
double get_max_speedIs(vector <node>& in_vector) {
    double max = in_vector.begin()->speedIs;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->speedIs >= max) max = it->speedIs;
    }
    return max;
}
//get min SpeedIs
double get_min_speedIs(vector <node>& in_vector) {
    double min = in_vector.begin()->speedIs;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->speedIs <= min) min = it->speedIs;
    }
    return min;
}

//Adds a node to the gpx-Document
void add_node_gpx(tinyxml2::XMLDocument* xmlDoc, node* node_to_add, XMLElement* parent_element, time_t starttime) {
    XMLElement* Element_trkpt = xmlDoc->NewElement("trkpt");
    XMLElement* Element_elevation = xmlDoc->NewElement("ele");
    XMLElement* Element_time = xmlDoc->NewElement("time");
    string timestr;
    Element_trkpt = xmlDoc->NewElement("trkpt");
    Element_trkpt->SetAttribute("lat", node_to_add->latitude);
    Element_trkpt->SetAttribute("lon", node_to_add->longitude);
    Element_elevation = xmlDoc->NewElement("ele");
    Element_elevation->SetText(node_to_add->elevation);
    Element_trkpt->InsertEndChild(Element_elevation);
    Element_time = xmlDoc->NewElement("time");
    Element_time->SetText(timeConversion(node_to_add->raceTime, starttime).c_str());
    Element_trkpt->InsertEndChild(Element_time);
    parent_element->InsertEndChild(Element_trkpt);
}

//Creates the gpx Document of the Racetrack
XMLError output_gpx(vector <node>& track, string trackName) {
    time_t startTime;
    time(&startTime);
    string timestr;

    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LinkEndChild(xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));
    XMLNode* pRoot = xmlDoc.NewElement("gpx");

    xmlDoc.InsertFirstChild(pRoot);

    XMLElement* Element_trk = xmlDoc.NewElement("trk");
    XMLElement* pElement2 = xmlDoc.NewElement("name");
    pElement2->SetText(trackName.c_str());
    Element_trk->InsertEndChild(pElement2);
    pElement2 = xmlDoc.NewElement("number");
    pElement2->SetText("1");
    Element_trk->InsertEndChild(pElement2);
    pElement2 = xmlDoc.NewElement("trkseg");
    XMLElement* Element_trkpt = xmlDoc.NewElement("trkpt");
    XMLElement* Element_elevation = xmlDoc.NewElement("ele");
    XMLElement* Element_time = xmlDoc.NewElement("time");


    //insert Trackpoints to gpx
    for (unsigned int i = 0; i < track.size(); i++) {
        add_node_gpx(&xmlDoc, &track[i], pElement2, startTime);
    }
    Element_trk->InsertEndChild(pElement2);
    pRoot->InsertEndChild(Element_trk);
    xmlDoc.InsertEndChild(pRoot);


    XMLError eResult = xmlDoc.SaveFile((trackName+".gpx").c_str());
    return eResult;
}
