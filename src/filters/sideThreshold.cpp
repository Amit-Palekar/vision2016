#include "filters/sideThreshold.hpp"

// Probably split this into three thresholds
int sideThreshold (std::vector<std::vector<cv::Point> >& contours, std::vector<cv::RotatedRect>& rect, double sideRatio, double areaRatio, double minArea, double maxArea, double sideThreshold)
{
    for (int i = 0; i < rect.size(); i++)
    {
        double height = rect[i].size.height;
        double width = rect[i].size.width;
        double rectArea = height * width;
        double contourArea = cv::contourArea(contours[i]);

        // If any of the following are true, the shape is not the goal
        if (std::abs((height / width) - sideRatio) > sideThreshold || 
            rectArea < minArea || 
            rectArea > maxArea || 
            )
        {
            contours.erase(contours.begin() + i);
            rect.erase(rect.begin() + i);
            // Size of vector got smaller
            i--;
        }
        else
        {
            goalInd = i;
        }
    }
    return goalInd;
}