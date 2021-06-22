#pragma once
#ifndef SIMNODE
#define SIMNODE

#include "../Race.h"

namespace Simulation
{
	class SimulationNode :public node {
	public:
		double MaxAcceleration = nan("");
		SimulationNode();
		SimulationNode(node input);
	};
}

#endif