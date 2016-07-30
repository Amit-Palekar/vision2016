#ifndef GET_CENTER_OF_MASS_HPP
#define GET_CENTER_OF_MASS_HPP

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

cv::Point2f getCenterOfMass(std::vector<cv::Point> >& contour);

#endif // GET_CENTER_OF_MASS