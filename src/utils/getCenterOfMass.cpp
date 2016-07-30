#include "utils/getCenterOfMass.hpp"

cv::Point2f getCenterOfMass(std::vector<cv::Point> >& contour)
// The rest of these, put into src/utils. Make sure to remove this comment when you do
{
	if (!(contour.size() > 0))
	{
		return 0; // TODO: ThrowException
	}
	cv::Moments mu = cv::moments(contour, false);
	// Find center of mass from the contour vector
	cv::Point2f mc = cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
	// Draw the center of mass
	cv::circle(src, mc, 5, cv::Scalar(255, 0, 255)); 
	
	return mc;
}
