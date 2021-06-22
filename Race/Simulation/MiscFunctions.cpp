#include "MiscFunctions.h"
#include <fstream>
#include <locale>

void Simulation::plotNodeVector(vector<SimulationNode> input, string filename)
{
	std::ofstream exportFile;
	exportFile.open(filename, ios::trunc);
	if (!exportFile) {
		cerr << "Output file could not be opened.\n\n";
		return;
	}
	else {
		exportFile.imbue(std::locale("de")); //Set the locale to get commas in the exportfile
		exportFile << "lat;" << "long;" << "elevation;" << "gradient;"<< "distance;" << "racetime;" << "speedIs;" << "speedLimit;" << "MaxAcceleration;";
		exportFile << "\n";
		double distance = 0 - input.at(0).distanceToNext;
		for (auto& currentnode : input) {
			distance += currentnode.distanceToNext;
			exportFile << currentnode.latitude << ";";
			exportFile << currentnode.longitude << ";";
			exportFile << currentnode.elevation << ";";
			exportFile << currentnode.gradient << ";";
			exportFile << distance << ";";
			exportFile << currentnode.raceTime << ";";
			exportFile << currentnode.speedIs << ";";
			exportFile << currentnode.speedLimit << ";";
			exportFile << currentnode.MaxAcceleration << ";";
			exportFile << "\n";
		}
		exportFile.close();
		cout << "\nExport finished. Please find results here: " << filename << endl;
		return;
	}
}