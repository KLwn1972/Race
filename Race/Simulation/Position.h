#pragma once
namespace Simulation
{
	class Position
	{
	public:
		double PositionX;
		double PositionY;
		double PositionZ;
		double Distance(Position OtherPosition);
	};
}
