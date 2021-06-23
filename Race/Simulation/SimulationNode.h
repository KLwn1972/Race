/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

Simulationnode extends node to provide more data for the outputs. This data is not needed in the normal node.
*/

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