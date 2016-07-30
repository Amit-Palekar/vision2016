#ifndef U_SHAPE_THRESHOLD_HPP
#define U_SHAPE_THRESHOLD_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

void uShapeThreshold (cv::Mat& img, std::vector< std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& boundedRects);

#endif // U_SHAPE_THRESHOLD_HPP