#include "MiscFunctions.h"
#include <fstream>
#include <locale>
#include "SimulationEnvironment.h"
#include "SimulationNode.h"
void Simulation::plotNodeVector(vector<SimulationNode> input, string filename)
{
	std::ofstream exportFile;
	exportFile.open(filename);
	if (!exportFile) {
		cerr << "Output file could not be opened.\n\n";
		return;
	}
	else {
		exportFile.imbue(std::locale("de")); //Set the locale to get commas in the exportfile
		exportFile << "lat;" << "long;" << "elevation;" << "gradient;" << "distance;" << "racetime;" << "speedIs;" << "speedLimit;" << "MaxAcceleration;";
		exportFile << "AirResistance;" << "RollingResistance;" << "GradientResistance;";
		exportFile << "\n";
		double distance = 0 - input.at(0).distanceToNext;
		for (SimulationNode& currentnode : input) {
			distance = distance + currentnode.distanceToNext;
			exportFile << currentnode.latitude << ";";
			exportFile << currentnode.longitude << ";";
			exportFile << currentnode.elevation << ";";
			exportFile << currentnode.gradient << ";";
			exportFile << distance << ";";
			exportFile << currentnode.raceTime << ";";
			exportFile << currentnode.speedIs * MS2KMH << ";";
			exportFile << currentnode.speedLimit * MS2KMH << ";";
			exportFile << currentnode.MaxAcceleration << ";";
			exportFile << currentnode.AirResistance << ";";
			exportFile << currentnode.RollingResistance << ";";
			exportFile << currentnode.GradientResistance << ";";

			exportFile << "\n";
		}
		exportFile.close();
		cout << "Export in excel format finished. Please find results here: " << filename << endl;
		return;
	}
}

double Simulation::angleRadFromGradient(double gradient)
{
	return atan(gradient / 100);
}