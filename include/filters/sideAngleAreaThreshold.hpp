#ifndef SIDE_ANGLE_AREA_THRESHOLD_HPP
#define SIDE_ANGLE_AREA_THRESHOLD_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

int sideAngleAreaThreshold (std::vector<std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& rect, double sideRatio, double areaRatio, double minArea, double maxArea, double sideThreshold, double areaThreshold, double angleThreshold);

#endif // SIDE_ANGLE_AREA_THRESHOLD_HPP