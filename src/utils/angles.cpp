
double getYaw(double xDist, std::vector<cv::Point>& corners, cv::Point2f& mc)
{
	double pixelWidth = distance2D(corners[0].x, corners[3].x);
	double center = src.cols / 2;
	double distFromCenter = mc.x - center;
	double inchesPerPixel = 20 / pixelWidth;

	// X and Y from standard XYZ axes
	double yDist = distFromCenter * inchesPerPixel;
	double yaw = std::asin(yDist / xDist) * 180 / PI;
	
	// Draw line from center of image to center of mass
	cv::line(src, cv::Point(center, mc.y), mc, cv::Scalar (255, 0, 255));
	
	return yaw;
}

double getPitch(double height, double hypotenuse)
{
	return (std::asin (height / hypotenuse) * 180 / PI);
}
