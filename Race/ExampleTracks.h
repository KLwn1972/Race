#pragma once
#include <vector>
#include "Race.h"

std::vector<node> ExampleTrack();
std::vector<node> ExampleStraightTrack(double length);
std::vector<node> ExampleHillTrack();
std::vector<node> ExamplePointToPoint(node startPoint, node endPoint, size_t NumberOfSteps);