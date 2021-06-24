/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

DataMap1d is a x-y map which gives back y data based on x data to the caller.
*/

#pragma once
#include <vector>
#include <stdexcept>
#include <string>
namespace Simulation
{
	/// <summary>
	/// DataMap1D takes x and y data of equal size and provides an interface for returning interpolated values. x data must be linear.
	/// </summary>
	class DataMap1D
	{
	public:
		DataMap1D();
		DataMap1D(std::vector<double> xData, std::vector<double> yData);
		DataMap1D(std::vector<double> xData, std::vector<double> yData, std::string xAxisName, std::string yAxisName);
		//Get Y data at point X
		double getY(double X);
		//Get axis names
		std::string getXAxisName();
		std::string getYAxisName();

		//Get the original data vectors
		std::vector<double> getXData();
		std::vector<double> getYData();
	private:
		std::vector<double> xData;
		std::vector<double> yData;
		std::string xAxisName = "";
		std::string yAxisName = "";
	};

	double interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate);
}