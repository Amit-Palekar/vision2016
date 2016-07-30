#ifndef GET_BOUNDED_RECTS_HPP
#define GET_BOUNDED_RECTS_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::RotatedRect> getBoundedRects(std::vector< std::vector<cv::Point> >& contours);

#endif // GET_BOUNDED_RECTS_HPP