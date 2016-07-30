#ifndef AREA_THRESHOLD_HPP
#define AREA_THRESHOLD_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

int areaThreshold (std::vector<std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& rect, double sideRatio, double areaRatio, double minArea, double maxArea, double areaThreshold);

#endif // AREA_THRESHOLD_HPP