#include <sal.h>

#include "Race.h"
#include "OpenStreetMap.h"

using namespace std;

OpenStreetMap::OpenStreetMap(string route){
	this->route = route;
	// Get TEMP path
	_Post_ _Notnull_ char* var;
	size_t len;
	errno_t err = _dupenv_s(&var, &len, "TEMP");
	if (err) {
		this->LocalPath = "C:\\Temp";
	} else { 
		this->LocalPath = var;
	}
}

OpenStreetMap::~OpenStreetMap() {
	this->nodes.clear();
}

int OpenStreetMap::GetNodesFromOSM(){
	string URL = "";
	string FileName = "";
	// Dounload Route
	URL = this->BuidLink("relation", this->route);
	FileName = this->BuidFileName(this->route);
	if (this->DownloadFile(URL, FileName) != S_OK) {
		return -1;
	}
	// Get Ways
	vector<string> ways = this->GetWays(this->route, FileName);
	for (std::vector<string>::iterator way = ways.begin(); way != ways.end(); ++way) {
		// Dounload Way
		string way_id = *way;
		URL = this->BuidLink("way", way_id);
		FileName = this->BuidFileName(way_id);
		if (this->DownloadFile(URL, FileName) != S_OK) {
			return -1;
		}
		// Get Nodes
		vector<string> nodes = this->GetNodes(way_id, FileName);
		for (std::vector<string>::iterator node_item = nodes.begin(); node_item != nodes.end(); ++node_item) {
			// Dounload Nodes
			string node_id = *node_item;
			URL = this->BuidLink("node", node_id);
			FileName = this->BuidFileName(node_id);
			if (this->DownloadFile(URL, FileName) != S_OK) {
				return -1;
			}
			// Get Coordinates
			vector<double> coordinates = this->GetCoordinates(node_id, FileName);
			// Add Node
			node node_tmp;
			node_tmp.latitude = coordinates[0];
			node_tmp.longitude = coordinates[1];
			ElevationCalculator EvalCalc;
			node_tmp.elevation = EvalCalc.getElevationFromSRTM_SIRCdata(coordinates[0], coordinates[1]);
			this->nodes.push_back(node_tmp);
		}
	}
	return 0;
}

vector<string> OpenStreetMap::GetWays(string relation, string FileName) {
	// ToDo: Nur zum Testen
	vector<string> ways;
	ways.push_back("159310586");
	ways.push_back("198509974");
	ways.push_back("198509975");
	return ways;
}

vector<string> OpenStreetMap::GetNodes(string way, string FileName) {
	// ToDo: Nur zum Testen
	vector<string> nodes;
	nodes.push_back("2084302010");
	nodes.push_back("1714130342");
	nodes.push_back("6397909170");
	return nodes;
}

vector<double> OpenStreetMap::GetCoordinates(string node, string FileName) {
	// ToDo: Nur zum Testen
	vector<double> coordinates;
	coordinates.push_back(50.3746191);
	coordinates.push_back(6.9889162);
	return coordinates;
}

HRESULT OpenStreetMap::DownloadFile(string URL, string OutFile) {
	HRESULT result = URLDownloadToFile(NULL, this->StrToTchar(URL), this->StrToTchar(OutFile), NULL, NULL);
	switch (result) {
		case S_OK: cout << "Alles OK" << endl; break;
		case E_OUTOFMEMORY: cout << "Nicht genug freier Speicher oder ungueltiger Puffer" << endl; break;
		case INET_E_DOWNLOAD_FAILURE: cout << "Fehler beim Download" << endl; break;
	}
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



