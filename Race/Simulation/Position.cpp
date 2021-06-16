#include "Position.h"
#include <math.h>

double Simulation::Position::Distance(Position OtherPosition)
{
	double deltaX = OtherPosition.PositionX - this->PositionX;
	double deltaY = OtherPosition.PositionY - this->PositionY;
	double deltaZ = OtherPosition.PositionZ - this->PositionZ;
	return sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}