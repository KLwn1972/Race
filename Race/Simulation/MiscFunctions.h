#pragma once
#include "../Race.h"
#include <vector>
#include "SimulationNode.h"
namespace Simulation
{
	void plotNodeVector(vector<SimulationNode> input, string filename);
	double angleRadFromGradientVector(double gradient);
};