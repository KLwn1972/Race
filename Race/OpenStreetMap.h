#pragma once
#include "Race.h"

using namespace std;

class OpenStreetMap {
public:
	string route = "";
	vector<node> nodes;
	~OpenStreetMap();
	OpenStreetMap(string route);
	int GetNodesFromOSM();
private:
	//enum data_type { relation, way, node };
	const string LinkPart = "https://www.openstreetmap.org/api/0.6";
	const string FileExtension = ".json";
	string LocalPath = "";
	string BuidLink(string type, string name);
	string BuidFileName(string name);
	wchar_t* StrToTchar(string in);
	HRESULT DownloadFile(string URL, string OutFile);
	vector<string> GetWays(string relation, string FileName);
	vector<string> GetNodes(string way, string FileName);
	vector<double> GetCoordinates(string node, string FileName);
};

