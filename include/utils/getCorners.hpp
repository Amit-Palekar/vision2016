#ifndef GET_CORNERS_HPP
#define GET_CORNERS_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

std::vector<cv::Point> getCorners (std::vector<cv::Point>& pts, int screenWidth, int screenHeight);

#endif // GET_CORNERS_HPP