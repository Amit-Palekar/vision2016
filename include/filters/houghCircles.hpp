#ifndef HOUGH_CIRCLES_WINDOWS_HPP
#define HOUGH_CIRCLES_WINDOWS_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void houghCirclesWindows(cv::Mat& img, int& hcMinRadius, int& hcMaxRadius, int& threshLow, int& threshHigh, int& visible, int& apply);

#endif // HOUGH_CIRCLES_WINDOWS_HPP