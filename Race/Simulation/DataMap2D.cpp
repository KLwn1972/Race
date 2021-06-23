#include "DataMap2D.h"
#include <string>
#include <iostream>

Simulation::DataMap2D::DataMap2D(std::vector<double> xData, std::vector<double> yData)
{
	//Check that data is valid
	if (xData.size() != yData.size())
		throw std::invalid_argument("X and Y dimnesions of DataMap2D need to be equal.");
	if (xData.size() > 1)
	{
		double oldXData = xData.at(0);
		for (auto newXdata : xData)
		{
			if (newXdata < oldXData)
			{
				std::string message = std::string("X Data needs to be increasing monotonously. Error at X= ") + std::to_string(newXdata);
				throw std::invalid_argument(message);
			}
		}
	}

	this->xData = xData;
	this->yData = yData;
}

Simulation::DataMap2D::DataMap2D(std::vector<double> xData, std::vector<double> yData, std::string xAxisName, std::string yAxisName) :DataMap2D(xData, yData)
{
	this->xAxisName = xAxisName;
	this->yAxisName = yAxisName;
}

double Simulation::DataMap2D::getY(double desiredX)
{
	if (xData.size() == 0)
		return nan("");

	if (xData.size() == 1)
		return yData.at(0);

	//Iterate through the array
	double result = yData.at(0);
	double previousX = xData.at(0);
	if (previousX < desiredX)
	{
		size_t i = 1;
		for (i; i < xData.size(); i++)
		{
			double currentX = xData.at(i);
			if (currentX == desiredX)
			{
				result = yData.at(i);
				break;
			}
			else if ((currentX > desiredX) && (previousX < desiredX))
			{
				result = interpolateValues(previousX, yData.at(i - 1), currentX, yData.at(i), desiredX);
				break;
			}
			previousX = currentX;
		}
		if (i == xData.size())
			result = yData.at(i - 1);
	}
	return result;
}

std::string Simulation::DataMap2D::getXAxisName()
{
	return this->yAxisName;
}

std::string Simulation::DataMap2D::getYAxisName()
{
	return this->xAxisName;
}

Simulation::DataMap2D::DataMap2D()
{
	this->xData = std::vector<double>();
	this->yData = std::vector<double>();
}

double Simulation::interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate)
{
	return startValue + ((endValue - startValue) / (endX - startX)) * (whereToInterPolate - startX);
}

std::vector<double> Simulation::DataMap2D::getXData()
{
	return this->xData;
}

std::vector<double> Simulation::DataMap2D::getYData()
{
	return this->yData;
}