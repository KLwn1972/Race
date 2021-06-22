#include "MiscFunctions.h"
#include <fstream>

void Simulation::plotNodeVector(vector<node> input, string filename)
{
	std::ofstream exportFile;
	exportFile.open(filename, ios::trunc);
	if (!exportFile) {
		cerr << "Output file could not be opened.\n\n";
		return;
	}
	else {
		exportFile << "lat;" << "long;" << "elevation;" << "gradient;" << "racetime;" << "speedIs;" << "speedLimit;" << "\n";
		for (auto& currentnode : input) {
			exportFile << currentnode.latitude << ";";
			exportFile << currentnode.longitude << ";";
			exportFile << currentnode.elevation << ";";
			exportFile << currentnode.gradient << ";";
			exportFile << currentnode.raceTime << ";";
			exportFile << currentnode.speedIs << ";";
			exportFile << currentnode.speedLimit << ";";
			exportFile << "\n";
		}
		exportFile.close();
		cout << "\nExport finished. Please find results here: resultShortNames.csv\n" << endl;
		return;
	}
}