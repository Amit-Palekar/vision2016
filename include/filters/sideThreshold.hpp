#ifndef SIDE_THRESHOLD_HPP
#define SIDE_THRESHOLD_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

int sideThreshold (std::vector<std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& rect, double sideRatio, double areaRatio, double minArea, double maxArea, double sideThreshold);

#endif // SIDE_THRESHOLD_HPP