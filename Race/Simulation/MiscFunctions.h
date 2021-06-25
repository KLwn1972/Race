/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

Miscellanious static functions to provide extra utility.
*/

#pragma once
#ifndef MISCFUNCTIONDEF
#define MISCFUNCTIONDEF
#include "../Race.h"
#include <vector>
#include "SimulationNode.h"
namespace Simulation
{
	/// <summary>
	/// Plot a nodevector to csv format
	/// </summary>
	/// <param name="input"></param>
	/// <param name="filename"></param>
	void plotNodeVector(vector<SimulationNode> input, string filename);
	/// <summary>
	/// Calculate the angle in [rad] from a given gradient
	/// </summary>
	/// <param name="gradient"></param>
	/// <returns></returns>
	double angleRadFromGradient(double gradient);
};

#endif