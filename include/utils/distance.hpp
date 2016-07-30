#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>

double distance(cv::Point one, cv::Point two);
double distance2D(double comp1, double comp2);
double getShortestDistance (cv::Point2f rectPoints[4], double focalLen, int dist, int height)

#endif // DISTANCE_HPP
