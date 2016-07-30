#ifndef SIDE_ANGLE_AREA_THRESHOLD_WINDOWS_HPP
#define SIDE_ANGLE_AREA_THRESHOLD_WINDOWS_HPP

#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void sideAngleAreaThresholdWindows(cv::Mat& output, std::vector<std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& rect, int& sideRatio, int& areaRatio, int& minArea, int& maxArea, int& sideThreshold, int& areaThreshold, int& angleThreshold, int& apply, int& visible);

#endif // SIDE_ANGLE_AREA_THRESHOLD_WINDOWS_HPP
