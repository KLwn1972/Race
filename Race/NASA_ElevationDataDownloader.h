#pragma once
#include <cstring>

using namespace std;

class FileDownloader {
public:
	string createDownloadURL(int longitude, int latitude);
	void downloadFile(string downloadURL, string targetpath);
	void downloadMultipleFile(int longitude_start, int longitude_end, int latitude_start, int latitude_end);
	void downloadGermany();

private:
	static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream); 
};



