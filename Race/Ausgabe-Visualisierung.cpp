/*
Team Ausgabe_Visualisierung: Yihao Zhu, Bernhard Lauss


*/


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

// Mainfunction of ausgabe_visualisierung
void ausgabe_visualisierung(vector<node>& track, string trackName) {
    XMLError fault_flag_gpx, fault_flag_kml;
    if (track.size() > 1) {
        fault_flag_gpx = output_gpx(track, trackName,0);
        fault_flag_kml = output_kml(track, trackName,0);
        if (fault_flag_gpx != 0)
            cout << "Error while creating gpx-File. ErrorID: " << fault_flag_gpx << endl;
        else cout << "Outputfile: " << trackName << ".gpx succesfull created. "<<endl;
        if (fault_flag_kml != 0)
            cout << "Error while creating kml-File. ErrorID: " << fault_flag_kml << endl;
        else cout << "Outputfile: " << trackName << ".kml succesfull created. " << endl;
        
        fault_flag_gpx = output_gpx(track, trackName+"_reduced", 1);
        fault_flag_kml = output_kml(track, trackName+"_reduced", 1);
        if (fault_flag_gpx != 0)
            cout << "Error while creating gpx-File. ErrorID: " << fault_flag_gpx << endl;
        else cout << "Outputfile: " << trackName << "_reduced.gpx succesfull created. " << endl;
        if (fault_flag_kml != 0)
            cout << "Error while creating kml-File. ErrorID: " << fault_flag_kml << endl;
        else cout << "Outputfile: " << trackName << "_reduced.kml succesfull created. " << endl;
    
    
    }
    else cout << "Vector-Size <2, don't execute ausgabe_visualisierung" << endl;
}

//Creates the gpx Document of the Racetrack
XMLError output_gpx(vector<node>& track, string trackName, bool reduced_resolution) {
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
        if (is_main_node(track[i].id)||!reduced_resolution) {
            add_node_gpx(&xmlDoc, &track[i], pElement2, startTime, Element_trkpt, Element_elevation, Element_time);
        }
    }
    Element_trk->InsertEndChild(pElement2);
    pRoot->InsertEndChild(Element_trk);
    xmlDoc.InsertEndChild(pRoot);


    XMLError eResult = xmlDoc.SaveFile((trackName + ".gpx").c_str());
    return eResult;
}

//Adds a node to the gpx-Document
void add_node_gpx(tinyxml2::XMLDocument* xmlDoc, node* node_to_add, XMLElement* parent_element, time_t starttime, XMLElement* Element_trkpt, XMLElement* Element_elevation, XMLElement* Element_time) {
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

// Function to create a date_string out of a starTime [s] and a raceTime [s]
string timeConversion(double raceTime, time_t startTime) {
    time_t rawtime;
    if (isnan(raceTime) || raceTime == numeric_limits<double>::infinity()) rawtime = (time_t)0 + startTime;
    else rawtime = (time_t)raceTime + startTime;
    struct tm timeinfo;
    localtime_s(&timeinfo, &rawtime);

    char buffer[21];
    strftime(buffer, 21, "%FT%TZ", &timeinfo);
    return buffer;
}

//Erstellung KML Ausgabe
XMLError output_kml(vector<node>& track, string trackName, bool reduced_resolution) {
    //modeselector - Select Datatype for coloring the track: 0=elevation 1=horizontalCurveRadius 2=speedLimit 3=speedIs
    int mode_selector = 3;
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LinkEndChild(xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));

    XMLNode* pRoot = xmlDoc.NewElement("kml");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement* Element_Document = xmlDoc.NewElement("Document");

    insertElementKML(xmlDoc, Element_Document, "name", trackName);
    double value_min = get_min_value(track, mode_selector);
    double value_max = get_max_value(track, mode_selector);
    double v_max = get_max_speedIs(track);
    vector<node>::iterator it = track.end() - 1;
    string description = "race time = " + to_string(it->raceTime)
        + " s\nmax. speed = " + to_string(v_max) + " m/s";
    insertElementKML(xmlDoc, Element_Document, "description", description);
    insertElementKML(xmlDoc, Element_Document, "visibility", "1");
    insertElementKML(xmlDoc, Element_Document, "open", "1");

    it = track.begin();
    do{
        XMLElement* Element_Placemark = xmlDoc.NewElement("Placemark");
        Element_Document->InsertEndChild(Element_Placemark);

        insertElementKML(xmlDoc, Element_Placemark, "visibility", "1");
        insertElementKML(xmlDoc, Element_Placemark, "open", "0");

        string colorCode = generate_color_code(get_act_value(*it, mode_selector), value_min, value_max);
        insertColorDefinitionKML(xmlDoc, Element_Document, it->id, colorCode);
        insertElementKML(xmlDoc, Element_Placemark, "styleUrl", ("#" + it->id).c_str());

        string name = "node id: " + it->id;
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
        while (!is_main_node(it->id)&&reduced_resolution) {
            it++;
        }
        coordinates += to_string(it->longitude) + ","
            + to_string(it->latitude) + ","
            + to_string(it->elevation) + " ";
        insertElementKML(xmlDoc, Element_LineString, "coordinates", coordinates);
    } while (it != track.end() - 1);
    pRoot->InsertEndChild(Element_Document);
    xmlDoc.InsertEndChild(pRoot);

    XMLError eResult = xmlDoc.SaveFile((trackName + ".kml").c_str());
    return eResult;
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
    long long int relative_value, color_number;
    stringstream stream;

    relative_value = (long long int)((act_value - min_value) / (max_value - min_value) * 510);

    if (relative_value < 256) color_number = 4278255615 - (255 - relative_value) * 256;
    else                      color_number = 4278255615 - (relative_value - 255);

    stream << std::hex << color_number;
    std::string color_code(stream.str());
    return color_code;
}

string createNotesKML(vector<node>::iterator it) {
    string notes = "<![CDATA[<br>node Id:" + it->id
        + "<br>longetude:" + to_string(it->longitude)
        + "<br>latitude:" + to_string(it->latitude)
        + "<br>elevation:" + to_string(it->elevation)
        + "[m]<br>distanceToNext:" + to_string(it->distanceToNext)
        + "[m]<br>horizontalCurveRadius:" + to_string(it->horizontalCurveRadius)
        + "[m]<br>verticalCurveRadius:" + to_string(it->verticalCurveRadius)
        + "[m]<br>gradient:" + to_string(it->gradient)
        + "[%]<br>speed limit:" + to_string(it->speedLimit)
        + "[m/s]<br>current speed:" + to_string(it->speedIs)
        + "[m/s]<br>race time:" + to_string(it->raceTime)
        + "[s]\n]]>";
    return notes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// get min selector
double get_min_value(vector<node>& in_vector,int selector) {
    double min_value;
    switch (selector) {
    case 0: min_value = get_min_elevation(in_vector);
        break;
    case 1: min_value = get_min_horizontalCurveRadius(in_vector);
        break;
    case 2: min_value = get_min_speedLimit(in_vector);
        break;
    case 3: min_value = get_min_speedIs(in_vector);
        break;
    default: min_value = get_min_speedIs(in_vector);
        break;
    }
    return min_value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// get max selector
double get_max_value(vector<node>& in_vector, int selector) {
    double max_value;
    switch (selector) {
    case 0: max_value = get_max_elevation(in_vector);
        break;
    case 1: max_value = get_max_horizontalCurveRadius(in_vector);
        break;
    case 2: max_value = get_max_speedLimit(in_vector);
        break;
    case 3: max_value = get_max_speedIs(in_vector);
        break;
    default: max_value = get_max_speedIs(in_vector);
        break;
    }
    return max_value;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// get act selector
double get_act_value(node& in_node, int selector) {
    double act_value;
    switch (selector) {
    case 0: act_value = in_node.elevation;
        break;
    case 1: act_value = in_node.horizontalCurveRadius;
        break;
    case 2: act_value = in_node.speedLimit;
        break;
    case 3: act_value = in_node.speedIs;
        break;
    default: act_value = in_node.speedIs;
        break;
    }
    return act_value;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// get max/min functions 
//get max SpeedIs
double get_max_speedIs(vector<node>& in_vector) {
    double max = in_vector.begin()->speedIs;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->speedIs >= max) max = it->speedIs;
    }
    return max;
}
//get min SpeedIs
double get_min_speedIs(vector<node>& in_vector) {
    double min = in_vector.begin()->speedIs;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->speedIs <= min) min = it->speedIs;
    }
    return min;
}
//get max speedLimit
double get_max_speedLimit(vector<node>& in_vector) {
    double max = in_vector.begin()->speedLimit;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->speedLimit >= max) max = it->speedLimit;
    }
    return max;
}
//get min speedLimit
double get_min_speedLimit(vector<node>& in_vector) {
    double min = in_vector.begin()->speedLimit;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->speedLimit <= min) min = it->speedLimit;
    }
    return min;
}
//get max elevation
double get_max_elevation(vector<node>& in_vector) {
    double max = in_vector.begin()->elevation;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->elevation >= max) max = it->elevation;
    }
    return max;
}
//get min elevation
double get_min_elevation(vector<node>& in_vector) {
    double min = in_vector.begin()->elevation;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->elevation <= min) min = it->elevation;
    }

    return min;
}
//get max raceTime
double get_max_raceTime(vector<node>& in_vector) {
    double max = in_vector.begin()->raceTime;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->raceTime >= max) max = it->raceTime;
    }
    return max;
}
//get min raceTime
double get_min_raceTime(vector<node>& in_vector) {
    double min = in_vector.begin()->raceTime;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->raceTime <= min) min = it->raceTime;
    }

    return min;
}
//get max distanceToNext
double get_max_distanceToNext(vector<node>& in_vector) {
    double max = in_vector.begin()->distanceToNext;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->distanceToNext >= max) max = it->distanceToNext;
    }
    return max;
}
//get min distanceToNext
double get_min_distanceToNext(vector<node>& in_vector) {
    double min = in_vector.begin()->distanceToNext;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->distanceToNext <= min) min = it->distanceToNext;
    }

    return min;
}
//get max horizontalCurveRadius
double get_max_horizontalCurveRadius(vector<node>& in_vector) {
    double max = in_vector.begin()->horizontalCurveRadius;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->horizontalCurveRadius >= max) max = it->horizontalCurveRadius;
    }
    return max;
}
//get min horizontalCurveRadius
double get_min_horizontalCurveRadius(vector<node>& in_vector) {
    double min = in_vector.begin()->horizontalCurveRadius;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->horizontalCurveRadius <= min) min = it->horizontalCurveRadius;
    }

    return min;
}
//get max verticalCurveRadius
double get_max_verticalCurveRadius(vector<node>& in_vector) {
    double max = in_vector.begin()->verticalCurveRadius;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->verticalCurveRadius >= max) max = it->verticalCurveRadius;
    }
    return max;
}
//get min verticalCurveRadius
double get_min_verticalCurveRadius(vector<node>& in_vector) {
    double min = in_vector.begin()->verticalCurveRadius;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->verticalCurveRadius <= min) min = it->verticalCurveRadius;
    }

    return min;
}
//get max gradient
double get_max_gradient(vector<node>& in_vector) {
    double max = in_vector.begin()->gradient;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->gradient >= max) max = it->gradient;
    }
    return max;
}
//get min gradient
double get_min_gradient(vector<node>& in_vector) {
    double min = in_vector.begin()->gradient;
    for (std::vector<node>::iterator it = in_vector.begin(); it != in_vector.end(); ++it) {
        if (it->gradient <= min) min = it->gradient;
    }

    return min;
}

//function returns whether a given node is main node or not
bool is_main_node(string node_id) {
    if (node_id.find("_") == string::npos) return 1;
    else return 0;
}