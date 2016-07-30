#include "utils/distance.hpp"

// Returns the minimum distance (straight line) between two points
double distance(cv::Point one, cv::Point two)
{
    return std::sqrt(std::pow(one.x - two.x, 2) + std::pow(one.y - two.y, 2));
}

// Returns the two dimensional distance between the components of two points
double distance2D(double comp1, double comp2)
{
    return std::abs(comp2 - comp1);
}

double getShortestDistance (cv::Point2f rectPoints[4], double focalLen, int dist, int height)
{
	// 20 inches real width
	int realWidth = 20;
	int pixelWidth = 0;
	double theta = 0;
	double d = 0;

	int length = static_cast<int>(distance (rectPoints[0], rectPoints[3]));
	int width = static_cast<int>(distance (rectPoints[0], rectPoints[1]));
	if (length > width)
    {
		pixelWidth = length;
    }
	else
	{
		pixelWidth = width;
    }
	d = (realWidth * focalLen) / pixelWidth;
	theta = std::asin(static_cast<double>(height) / dist) * 180 / PI;
	return d;
}
