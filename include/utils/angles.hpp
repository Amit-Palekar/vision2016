#ifndef ANGLES_HPP
#define ANGLES_HPP

#include <vector>
#include <opencv2/imgproc/imgproc>

double getYaw(double xDist, std::vector<cv::Point>& corners, cv::Point2f& mc);
double getPitch(double height, double hypotenuse);

#endif // ANGLES_HPP

