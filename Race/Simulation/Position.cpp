#include "Position.h"
#include <math.h>
#include "SimulationEnvironment.h"

double Simulation::Position::Distance(Position OtherPosition)
{
	double deltaX = OtherPosition.PositionX - this->PositionX;
	double deltaY = OtherPosition.PositionY - this->PositionY;
	double deltaZ = OtherPosition.PositionZ - this->PositionZ;
	return sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

Simulation::Position::Position()
{
	this->PositionX = nan("");
	this->PositionY = nan("");
	this->PositionZ = nan("");
}

Simulation::Position::Position(double X, double Y, double Z)
{
	this->PositionX = X;
	this->PositionY = Y;
	this->PositionZ = Z;
}

Simulation::Position Simulation::Position::PositionFromGeoCoordinates(double GeoLong, double GeoLat, double GeoHeight)
{
	double CartX, CartY;
	//Web Mercator Projektion
	const double EarthRadius_m = 6378.137 * 1000;
	CartX = EarthRadius_m * (GeoLong * PI) / (180.0);
	CartY = EarthRadius_m * atanh(sin((GeoLat * PI) / 180.0));

	return Position(CartX, CartY, GeoHeight);
}