
#include "filters/houghCircles.hpp"

void houghCircles(cv::Mat& img, int hcMinRadius, int hcMaxRadius, int& threshLow, int& threshHigh)
{    
  	std::vector<cv::Vec3f> circles;

	// TODO: Error if image does not contain edges
 	// Apply the Hough Transform to find the circles
 	cv::HoughCircles( img, circles, CV_HOUGH_GRADIENT, 2, img.rows / 8, hcMaxRadius, hcMinRadius);
	cv::cvtColor(img, img, CV_GRAY2BGR);

  	// Draw the circles detected
  	for( size_t i = 0; i < circles.size(); i++ )
  	{
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // Draw center of enclosing circle
        cv::circle(img, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
        // Draw enclosing circle
        cv::circle(img, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
   	}
}