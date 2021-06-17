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
		Position();
		Position(double X, double Y, double Z);
		static Position PositionFromGeoCoordinates(double GeoLong, double GeoLat, double GeoHeight);
	};
}
