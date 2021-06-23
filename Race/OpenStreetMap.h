#pragma once
#include "Race.h"
#include "cJSON.h"

using namespace std;

class OpenStreetMap {
public:
	string route = "";
	int waysOffset = 0;
	vector<node> nodes;
	~OpenStreetMap();
	OpenStreetMap(string route);
	int GetNodesFromOSM();
private:
	//enum data_type { relation, way, node };
	const string LinkPart = "https://www.openstreetmap.org/api/0.6";
	const string FileExtension = ".json";
	string LocalPath = "C:\\";
	string BuildLink(string type, string name);
	string BuildFileName(string name);
	wchar_t* StrToTchar(string in);
	HRESULT DownloadFile(string URL, string OutFile);
	cJSON* JSON_ReadFileToStructur(string FileName);
	vector<string> GetWays(string FileName);
	vector<string> GetNodes(string FileName);
	vector<double> GetCoordinates(string FileName);
};

