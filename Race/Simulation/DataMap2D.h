#pragma once
#include <vector>
#include <stdexcept>
#include <string>
namespace Simulation
{
	/// <summary>
	/// DataMap2D takes x and y data of equal size and provides an interface for returning interpolated values. x data must be linear.
	/// </summary>
	class DataMap2D
	{
	public:
		DataMap2D(std::vector<double> xData, std::vector<double> yData);
		DataMap2D(std::vector<double> xData, std::vector<double> yData, std::string xAxisName, std::string yAxisName);
		double getY(double X);
		std::string getXAxisName();
		std::string getYAxisName();
	private:
		std::vector<double> xData;
		std::vector<double> yData;
		std::string xAxisName = "";
		std::string yAxisName = "";
	};

	double interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate);
}