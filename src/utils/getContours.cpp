#include "utils/getContours.hpp"

std::vector< std::vector<cv::Point> > getContours(cv::Mat& src, int& contoursThresh)
{
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	// Convert src to gray format
	cv::cvtColor(src, src, CV_BGR2GRAY);
	// Detect edges using Threshold
	cv::threshold(src, src, contoursThresh, 255, cv::THRESH_BINARY);
	cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	cv::cvtColor(src, src, CV_GRAY2BGR);
	
	return contours;
}

