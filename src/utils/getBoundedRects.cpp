
#include "utils/getBoundedRects.hpp"

std::vector<cv::RotatedRect> getBoundedRects(std::vector< std::vector<cv::Point> >& contours)
{
	std::vector<cv::RotatedRect> boundedRects (contours.size());
	for(int i = 0; i < contours.size(); i++)
	{
		// Get the minimal area bounded rects
		boundedRects[i] = cv::minAreaRect(contours[i]);
	}
	return boundedRects;
}