#include "utils/getCorners.hpp"

std::vector<cv::Point> getCorners (std::vector<cv::Point>& pts, int screenWidth, int screenHeight)
{
	cv::Point tLeft = cv::Point (0, 0);
	cv::Point tRight = cv::Point (screenWidth, 0);
	cv::Point bLeft = cv::Point (0, screenHeight);
	cv::Point bRight = cv::Point (screenWidth, screenHeight);

	// Initialize to the maximum possible values
	double tLeftD = screenWidth;
	double tRightD = screenWidth;
	double bLeftD = screenWidth;
	double bRightD = screenWidth;

	std::vector<cv::Point> corners;
	
	corners.push_back(bLeft);
	corners.push_back(tLeft);
	corners.push_back(tRight);
	corners.push_back(bRight);
	
	for (int i = 0; i < pts.size(); i++)
	{
		if (distance(pts[i], bLeft) < bLeftD)
		{
			bLeftD = distance(pts[i], bLeft);
			corners.at(0) = pts[i];
		}
		if (distance(pts[i], tLeft) < tLeftD)
		{
			tLeftD = distance(pts[i], tLeft);
			corners.at(1) = pts[i];
		}
		if (distance(pts[i], tRight) < tRightD)
		{
			tRightD = distance(pts[i], tRight);
			corners.at(2) = pts[i];
		}
		if (distance(pts[i], bRight) < bRightD)
		{
			bRightD = distance(pts[i], bRight);
			corners.at(3) = pts[i];
		}
	}
	// Draw the corners
	for (int i = 0; i < 4; i++)
	{
		cv::circle(src, corners[i], 5, cv::Scalar(255, 100, 100));
	}
	return corners;
}