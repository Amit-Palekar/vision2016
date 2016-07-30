#include "filters/sideAngleAreaThresholdWindows.hpp"

void sideAngleAreaThresholdWindows(cv::Mat& output, std::vector<std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& rect, int& sideRatio, int& areaRatio, int& minArea, int& maxArea, int& sideThreshold, int& areaThreshold, int& angleThreshold, int& apply, int& visible)
{
	if (visible)
	{
		cv::namedWindow("Shape Threshold Editor", CV_WINDOW_AUTOSIZE);
		cv::createTrackbar("Apply Filter", "Shape Threshold Editor", &apply, 1);
		cv::createTrackbar("Percent Length to Width Ratio", "Shape Threshold Editor", &sideRatio, 500);
		cv::createTrackbar("Percent Contour to Rect Area Ratio", "Shape Threshold Editor", &areaRatio, 100);
		cv::createTrackbar("Minimum Rect Area", "Shape Threshold Editor", &minArea, 30000);
		cv::createTrackbar("Maximum Rect Area", "Shape Threshold Editor", &maxArea, 30000);
		cv::createTrackbar("+- Side Threshold (Out of 100)", "Shape Threshold Editor", &sideThreshold, 100);
		cv::createTrackbar("+- Area Threshold (Out of 100)", "Shape Threshold Editor", &areaThreshold, 100);
		cv::createTrackbar("+- Angle Threshold", "Shape Threshold Editor", &angleThreshold, 100);
    }
	else
	{
		cv::destroyWindow("Shape Threshold Editor");
		cv::destroyWindow("Shape Threshold Output");
	}
	if (apply)
	{
		shapeThreshold(output, contours, rect, sideRatio, areaRatio, minArea, maxArea, sideThreshold, areaThreshold, angleThreshold);
        if (visible)
        {
            cv::namedWindow("Shape Threshold Output", CV_WINDOW_AUTOSIZE);
            cv::imshow("Shape Threshold Output", output);
        }
	}	
	else
	{
		cv::destroyWindow("Shape Threshold Output");
	}
}