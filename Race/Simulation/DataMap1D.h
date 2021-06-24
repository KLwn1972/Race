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
		/// <summary>
		/// Create a datamap from two data vectors
		/// </summary>
		/// <param name="xData"></param>
		/// <param name="yData"></param>
		DataMap1D(std::vector<double> xData, std::vector<double> yData);
		/// <summary>
		/// Create a datamap from two vectors but also save namestrings for the axis
		/// </summary>
		/// <param name="xData"></param>
		/// <param name="yData"></param>
		/// <param name="xAxisName"></param>
		/// <param name="yAxisName"></param>
		DataMap1D(std::vector<double> xData, std::vector<double> yData, std::string xAxisName, std::string yAxisName);
		/// <summary>
		/// Get interpolated data at point X
		/// </summary>
		/// <param name="X"></param>
		/// <returns></returns>
		double getY(double X);
		//Get axis names
		std::string getXAxisName();
		std::string getYAxisName();

		/// <summary>
		/// Get the data vector X dimension
		/// </summary>
		/// <returns></returns>
		std::vector<double> getXData();
		/// <summary>
		/// Get the data vector in Y dimension
		/// </summary>
		/// <returns></returns>
		std::vector<double> getYData();
	private:
		std::vector<double> xData;
		std::vector<double> yData;
		std::string xAxisName = "";
		std::string yAxisName = "";
	};

	/// <summary>
	/// general linear interpolation function
	/// </summary>
	/// <param name="startX"></param>
	/// <param name="startValue"></param>
	/// <param name="endX"></param>
	/// <param name="endValue"></param>
	/// <param name="whereToInterPolate"></param>
	/// <returns></returns>
	double interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate);
}