#include "include/utils/gui.cpp"

void putData(cv::Mat& image, double distance, double yaw, double pitch)
{
    char str[30];
    
	sprintf(str, "Distance = %4.2f", distance);
	cv::putText(image, str, cv::Point(10, 430), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.75, cv::Scalar(255, 0, 200), 1, 8, false);
	sprintf(str, "Yaw      = %4.2f", yaw);
	cv::putText(image, str, cv::Point(10, 450), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.75, cv::Scalar(255, 0, 200), 1, 8, false);
	sprintf(str, "Pitch    = %4.2f", pitch);
	cv::putText(image, str, cv::Point(10, 470), CV_FONT_HERSHEY_COMPLEX_SMALL, 0.75, cv::Scalar(255, 0, 200), 1, 8, false);
}

void putContours(cv::Mat& image, std::vector< std::vector<cv::Point> >& contours, cv::Scalar& color)
{
	std:vector<cv::Vec4i> hierarchy;
	
	// Draw the contours
    for(int i = 0; i < contours.size(); i++)
        cv::drawContours(image, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
}