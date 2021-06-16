#pragma once
#include "Position.h"
namespace Simulation
{
	class MovingObject
	{
	public:
		Position CurrentPosition;
		Position PreviousPosition;

		int CurrentWayPointId;
		int PreviousWayPointID;

		double Time;
		double TimePrevious;
		double Velocity;
		double VelocityPrevious;
		double TotalForce;

		double AccX;
		double AccXPrevious;
	};
}
