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
	/// <summary>
	/// Node which holds extra information which is only needed for simulation purposes
	/// </summary>
	class SimulationNode :public node {
	public:
		/// <summary>
		/// Acceleration at a point
		/// </summary>
		double MaxAcceleration = nan("");
		/// <summary>
		/// Air Resistance Force in [N]
		/// </summary>
		double AirResistance = nan("");
		/// <summary>
		/// Gradient Resistance Force in [N]
		/// </summary>
		double GradientResistance = nan("");
		/// <summary>
		/// Rolling Resistance Force in [N]
		/// </summary>
		double RollingResistance = nan("");

		/// <summary>
		/// Position in the startvector of this node for easier mapping. The init value means no position in startvector;
		/// </summary>
		size_t PositionInStartVector = INT32_MAX;
		SimulationNode();
		SimulationNode(node input);
	};
}

#endif