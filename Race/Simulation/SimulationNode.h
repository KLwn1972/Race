#pragma once
#ifndef SIMNODE
#define SIMNODE

#include "../Race.h"

namespace Simulation
{
	class SimulationNode :public node {
		double MaxAcceleration = nan("");
	};
}

#endif