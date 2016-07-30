#ifndef ANGLE_THRESHOLD_HPP
#define ANGLE_THRESHOLD_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

int angleThreshold (std::vector<std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& rect, double sideRatio, double areaRatio, double minArea, double maxArea, double angleThreshold);

#endif // ANGLE_THRESHOLD_HPP