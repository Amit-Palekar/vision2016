#include "filters/houghCirclesWindows.hpp"

void houghCirclesWindows(cv::Mat& img, int& hcMinRadius, int& hcMaxRadius, int& threshLow, int& threshHigh, int& visible, int& apply)
{
	if (visible)
	{
		cv::namedWindow("Hough Circles Editor", cv::WINDOW_AUTOSIZE);	
		cv::createTrackbar("Apply Filter", "Hough Circles Editor", &apply, 1);
		cv::createTrackbar("Min Radius", "Hough Circles Editor", &hcMaxRadius, 500);
		cv::createTrackbar("Max Radius", "Hough Circles Editor", &hcMinRadius, 500);
	}
	else
	{
		cv::destroyWindow("Hough Circles Editor");
		cv::destroyWindow("Hough Circles Output");
	}
	if (apply)
	{
		houghCircles(img, hcMinRadius, hcMaxRadius, threshLow, threshHigh);
        if (visible)
        {
            cv::namedWindow("Hough Circles Output", CV_WINDOW_AUTOSIZE);
            cv::imshow("Hough Circles Output", img);
        }
	}
	else
	{
		cv::destroyWindow("Hough Circles Output");
	}
}