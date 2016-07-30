#ifndef GET_CONTOURS_HPP
#define GET_CONTOURS_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

std::vector< std::vector<cv::Point> > getContours(cv::Mat& src, int& contoursThresh);

#endif // GET_CONTOURS_HPP
