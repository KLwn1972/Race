#include <sal.h>
#include <fstream>
#include <string>

#include "Race.h"
#include "OpenStreetMap.h"
#include "cJSON.h"

using namespace std;

OpenStreetMap::OpenStreetMap(string route) {
	this->route = route;
	// Get TEMP path
	_Post_ _Notnull_ char* var;
	size_t len;
	errno_t err = _dupenv_s(&var, &len, "TEMP");
	if (err) {
		this->LocalPath = "C:\\Temp";
	}
	else {
		this->LocalPath = var;
	}
}

OpenStreetMap::~OpenStreetMap() {
	this->nodes.clear();
}

int OpenStreetMap::GetNodesFromOSM() {
	string URL = "";
	string FileName = "";
	// Dounload Route
	URL = this->BuidLink("relation", this->route);
	FileName = this->BuidFileName(this->route);
	if (this->DownloadFile(URL, FileName) != S_OK) {
		return -1;
	}
	// Get Ways
	vector<string> ways = this->GetWays(FileName);
	for (std::vector<string>::iterator way_item = ways.begin(); way_item != ways.end(); ++way_item) {
		// Dounload Way
		string way_id = *way_item;
		URL = this->BuidLink("way", way_id);
		FileName = this->BuidFileName(way_id);
		if (this->DownloadFile(URL, FileName) != S_OK) {
			return -1;
		}
		// Get Nodes
		vector<string> nodes = this->GetNodes(FileName);
		for (std::vector<string>::iterator node_item = nodes.begin(); node_item != nodes.end(); ++node_item) {
			// Dounload Nodes
			string node_id = *node_item;
			URL = this->BuidLink("node", node_id);
			FileName = this->BuidFileName(node_id);
			if (this->DownloadFile(URL, FileName) != S_OK) {
				return -1;
			}
			// Get Coordinates
			vector<double> coordinates = this->GetCoordinates(FileName);
			// Add Node
			node node2add;
			node2add.latitude = coordinates[0];
			node2add.longitude = coordinates[1];
			ElevationCalculator EvalCalc;
			node2add.elevation = EvalCalc.getElevationFromSRTM_SIRCdata(coordinates[0], coordinates[1]);
			this->nodes.push_back(node2add);
		}
	}
	return 0;
}

vector<string> OpenStreetMap::GetWays(string FileName) {
	vector<string> ways;
	cJSON* json_struct_complete = this->JSON_ReadFileToStructur(FileName);
	if (json_struct_complete != NULL) {
		cJSON* elements = cJSON_GetObjectItemCaseSensitive(json_struct_complete, "elements");
		cJSON* members = cJSON_GetObjectItemCaseSensitive(elements->child, "members"); //elements: Array elements
		cJSON* jsonobject = members->child;
		//Schleife zum durchlaufen aller Wege in einer relation
		while (jsonobject != NULL) {
			cJSON* ref = cJSON_GetObjectItemCaseSensitive(jsonobject, "ref");
			long long int id = (long long int)ref->valuedouble;
			ways.push_back(to_string(id));
			jsonobject = jsonobject->next;
		}
	}
	return ways;
}

vector<string> OpenStreetMap::GetNodes(string FileName) {
	vector<string> nodes;
	cJSON* json_struct_complete = this->JSON_ReadFileToStructur(FileName);
	if (json_struct_complete != NULL) {
		cJSON* elements = cJSON_GetObjectItemCaseSensitive(json_struct_complete, "elements");
		cJSON* node_ids = cJSON_GetObjectItemCaseSensitive(elements->child, "nodes");
		cJSON* jsonobject = node_ids->child;
		//Schleife zum durchlaufen aller Knoten in einem Weg
		while (jsonobject != NULL) {
			long long int id = (long long int)jsonobject->valuedouble;
			nodes.push_back(to_string(id));
			jsonobject = jsonobject->next;
		}
	}
	return nodes;
}

vector<double> OpenStreetMap::GetCoordinates(string FileName) {
	vector<double> coordinates;
	cJSON* json_struct_complete = this->JSON_ReadFileToStructur(FileName);
	if (json_struct_complete != NULL) {
		cJSON* elements = cJSON_GetObjectItemCaseSensitive(json_struct_complete, "elements"); //elements: Array elements
		cJSON* jsonobject = elements->child;
		cJSON* latitude = cJSON_GetObjectItemCaseSensitive(jsonobject, "lat");
		cJSON* longitude = cJSON_GetObjectItemCaseSensitive(jsonobject, "lon");
		coordinates.push_back(latitude->valuedouble);
		coordinates.push_back(longitude->valuedouble);
	}
	return coordinates;
}

cJSON* OpenStreetMap::JSON_ReadFileToStructur(string FileName) {
	string FileContent;
	ifstream t(FileName);
	if (t.is_open()) {
		t.seekg(0, ios::end);
		t.seekg(0, ios::beg);
		FileContent.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
		t.close();
		return cJSON_Parse(FileContent.c_str());
	}
	else {
		return NULL;
	}
}

HRESULT OpenStreetMap::DownloadFile(string URL, string OutFile) {
	HRESULT result = URLDownloadToFile(NULL, this->StrToTchar(URL), this->StrToTchar(OutFile), NULL, NULL);
#ifdef DEBUG
	cout << endl << URL << endl << OutFile << endl;
	switch (result) {
	case S_OK: cout << "Download was successful." << endl; break;
	case E_OUTOFMEMORY: cout << "#####ERROR: The buffer length is invalid, or there is insufficient memory to complete the operation." << endl; break;
	case INET_E_DOWNLOAD_FAILURE: cout << "#####ERROR: The specified resource or callback interface was invalid." << endl; break;
	}
#endif
	return result;
}

string OpenStreetMap::BuidLink(string type, string name) {
	string url = this->LinkPart;
	url += "/";
	url += type;
	url += "/";
	url += name;
	url += this->FileExtension;
	return url;
}

string OpenStreetMap::BuidFileName(string name) {
	string file_name = this->LocalPath;
	file_name += "\\";
	file_name += name;
	file_name += this->FileExtension;
	return file_name;
}

wchar_t* OpenStreetMap::StrToTchar(string in) {
	TCHAR* retval = new TCHAR[in.size() + 1];
	retval[in.size()] = 0;
	std::copy(in.begin(), in.end(), retval);
	return retval;
}