#include "filters/uShapeThreshold.hpp"

// Threshold for a square 'U' shape
void uShapeThreshold (cv::Mat& img, std::vector< std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& boundedRects)
{
	for (int i = 0; i < contours.size(); i++)
    {
        cv::Moments mu = cv::moments(contours[i], false);
        double dist = cv::pointPolygonTest(contours[i], cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00), true);
        // Check if the center of mass is outside the 'U' and whether it is far enough outside
        if (dist > -5 || dist < -15)
        {
            contours.erase(contours.begin() + i);
            boundedRects.erase(boundedRects.begin() + i);
            // Vector got smaller, decrement loop counter
			i--;
        }
    }
}

