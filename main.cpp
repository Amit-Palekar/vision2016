#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <vector>

#include "include/utils/getAngles.hpp"
#include "include/utils/distance.hpp"
#include "include/utils/getBoundedRects.hpp"
#include "include/utils/getCenterOfMass.hpp"
#include "include/utils/getContours.hpp"
#include "include/utils/getCorners.hpp"
#include "include/utils/netThread.hpp"
#include "include/utils/udpClientServer.hpp"
#include "include/utils/mjpgStream.hpp"
#include "include/utils/gui.hpp"

#include "include/filters/selectMode.hpp"
#include "include/filters/cannyEdgeDetect.hpp"
#include "include/filters/dilateErode.hpp"
#include "include/filters/gaussianBlur.hpp"
#include "include/filters/houghLines.hpp"
#include "include/filters/houghCircles.hpp"
#include "include/filters/hsvColorThreshold.hpp"
#include "include/filters/laplacianSharpen.hpp"
#include "include/filters/mergeFinal.hpp"
#include "include/filters/shapeThresholds.hpp"

#include "include/filters/cannyEdgeDetectWindows.hpp"
#include "include/filters/dilateErodeWindows.hpp"
#include "include/filters/gaussianBlurWindows.hpp"
#include "include/filters/houghLinesWindows.hpp"
#include "include/filters/houghCirclesWindows.hpp"
#include "include/filters/hsvColorThresholdWindows.hpp"
#include "include/filters/laplacianSharpenWindows.hpp"
#include "include/filters/mergeFinalWindows.hpp"
#include "include/filters/shapeThresholdsWindows.hpp"

// Calculating fps
const bool FPS = false;
// Calibrating with windows instead of deployment
const bool CALIB = true;
// Streaming to mjpg-streamer instead of cv::imshow
const bool STREAM = false;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Measurements are in inches
const double TOWER_HEIGHT = 83;		
const double CAMERA_HEIGHT = 6;
const double GAME_ELEMENT_WIDTH = 20;
const int CAMERA_NUM = 0;
const int CALIB_DISTANCE = 186;

double pitch = 0;
double yaw = 0;
double fps = 0;

//const std::string TARGET_ADDR = "10.1.15.2";
//const std::string HOST_ADDR = "10.1.15.8";
const std::string TARGET_ADDR = "localhost";
const std::string HOST_ADDR = "localhost";
const int UDP_PORT = 5810;

cv::Scalar GREEN (0, 255, 0);
cv::Scalar BLUE_GREEN (255, 255, 0);
cv::Scalar PURPLE (255, 0, 255);
cv::Scalar LIGHT_GREEN (255, 100, 100);

int main (int argc, char *argv[])
{
	// Parameters for selecting which filters to apply
	int blur = 1;
	int color = 0;
	int dilateErode = 0;
	int edge = 0;
	int laplacian = 0;
	int houghLines = 0;
	int houghCircles = 0;
    int uShapeThresholdWindow = 0;
    int sideRatioThresholdWindow = 0;
    int areaRatioThresholdWindow = 0;
    int angleThresholdWindow = 0;
	int merge = 0;

	// Parameters for applying filters even if windows are closed
	int applyBlur = 1;
	int applyColor = 1;
	int applyDilateErode = 0;
	int applyEdge = 0;
	int applyLaplacian = 0;
	int applyHoughLines = 0;
	int applyHoughCircles = 0;
    int applyUShapeThreshold = 1;
    int applySideRatioThreshold = 1;
    int applyAreaRatioThreshold = 1;
    int applyAngleThreshold = 1;
	int applyMerge = 0;

	// gaussianBlur parameters
	int blur_ksize = 7;
	int sigmaX = 10;
	int sigmaY = 10;

	// hsvColorThreshold parameters
	int hMin = 110;
	int hMax = 180;
	int sMin = 0;
	int sMax = 100;
	int vMin = 30;
	int vMax = 50;
	int debugMode = 0;
	// 0 is none, 1 is bitAnd between h, s, and v
	int bitAnd = 1;

	// dilateErode parameters
	int holes = 0;
	int noise = 0;
	int size = 1;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
					  cv::Size(2 * size + 1, 2 * size + 1), 
					  cv::Point(size, size) );
	
	// cannyEdgeDetect parameters
	int threshLow = 100;
	int threshHigh = 245;
	
	// laplacian paraeters
	int laplacian_ksize = 3;
	int scale = 1;
	int delta = 0;
	
	// houghLines parameters
	int rho = 1;
	int theta = 180;
	int threshold = 50;
	int lineMin = 50;
	int maxGap = 10;		

	// houghCircles parameters
    int hcMinDist = SCREEN_HEIGHT / 8;
	int hcMinRadius = 116;
	int hcMaxRadius = 212;	

	// mergeFinal parameters
	int mergeWeight1 = 100;
	int mergeWeight2 = 100;

    // Shape threshold parameters
    // Params need to be ints for opencv windows
    int sideRatioParam = 210;
    int areaRatioParam = 30;
    int minAreaParam = 800;
    int maxAreaParam = 10000;
    int sideRatioMaxDeviationParam = 30;
    int areaRatioMaxDeviationParam = 30;
    int angleMaxDeviationParam = 20;
    double sideRatio = (double) sideRatioParam / 100;
    double areaRatio = (double) areaRatioParam / 100;
    double minArea = (double) minAreaParam;
    double maxArea = (double) maxAreaParam;
    double sideRatioMaxDeviation = (double) sideRatioMaxDeviationParam / 100;
    double areaRatioMaxDeviation = (double) areaRatioMaxDeviationParam / 100;
    double angleMaxDeviation = (double) angleMaxDeviationParam;

    // uShape
    int minDistFromContours = 5;
    int maxDistFromContours = 15;

	// Tower height is 7 feet (to the bottom of the tape) which is 84 inches

	int contoursThresh = 140;
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::RotatedRect> boundedRects;
	cv::Point2f rectPoints[4];
	int goalInd = 0;
    // Game specific elements
	int height = TOWER_HEIGHT - CAMERA_HEIGHT;

    // Distance calculation
    int isFocalLengthCalib = 0;
    double focalLength = 600;

    udp_client_server::udp_client client(TARGET_ADDR, UDP_PORT);
    udp_client_server::udp_server server(HOST_ADDR, UDP_PORT);

    std::thread netSend (sendData, std::ref(client));
    netSend.detach();
    std::thread netReceive (receiveData, std::ref(server));
    netReceive.detach();

	cv::VideoCapture camera (CAMERA_NUM);
    if (!camera.isOpened())
        throw std::runtime_error(std::string("Error - Could not open camera at port ") + std::to_string(CAMERA_NUM));
    else
        std::cerr << "Opened camera at port " << CAMERA_NUM << "\n";

    std::cerr << "\n";
    std::cerr << " ============== NOTICE ============= " << "\n";
    std::cerr << "|                                   |" << "\n";
    std::cerr << "| Press 'q' to quit without saving  |" << "\n";
    std::cerr << "| Press ' ' to pause                |" << "\n";
    std::cerr << "|                                   |" << "\n";
    std::cerr << " =================================== " << "\n";
    std::cerr << "\n";
	
    cv::Mat img;
    char kill = ' ';

    std::chrono::high_resolution_clock::time_point start, end;

    // gnuplot parameters
    FILE *pipe_gp = popen("gnuplot", "w");  
    fputs("set terminal png\n", pipe_gp);
    fputs("plot '-' u 1:2 \n", pipe_gp);
    double avg = 0;
    double fpsTick = 1;

	while (kill != 'q')
	{
		// Press space to pause program, then any key to resume
		if (kill == ' ')
			cv::waitKey(0);

        if (FPS) start = std::chrono::high_resolution_clock::now();
        if (argc > 2)
        {
            // TODO: acquire input through imgs / videos
        }
        else
        {
            camera >> img;
        }
        //if (!STREAM) cv::imshow("RGB", img);
        if (CALIB)
        {
            cv::Mat rgb = img.clone();

            //selectMode(blur, color, dilateErode, edge, laplacian, houghLines, houghCircles, uShapeThresholdWindow, sideRatioThresholdWindow, areaRatioThresholdWindow, angleThresholdWindow, merge);
            gaussianBlurWindows(img, blur_ksize, sigmaX, sigmaY, applyBlur, blur, STREAM);
            hsvColorThresholdWindows(img, hMin, hMax, sMin, sMax, vMin, vMax, debugMode, bitAnd, applyColor, color, STREAM);
            dilateErodeWindows(img, element, holes, noise, applyDilateErode, dilateErode, STREAM);
            cannyEdgeDetectWindows(img, threshLow, threshHigh, applyEdge, edge, STREAM);
            laplacianSharpenWindows(img, laplacian_ksize, scale, delta, applyLaplacian, laplacian, STREAM);
            houghLinesWindows(img, rho, theta, threshold, lineMin, maxGap, applyHoughLines, houghLines, STREAM);
            houghCirclesWindows(img, hcMinDist, hcMinRadius, hcMaxRadius, applyHoughCircles, houghCircles, STREAM);
            mergeFinalWindows(rgb, img, mergeWeight1, mergeWeight2, applyMerge, merge, STREAM);
        }
        else
        {
            gaussianBlur(img, blur_ksize, sigmaX, sigmaY);
            hsvColorThreshold(img, hMin, hMax, sMin, sMax, vMin, vMax, debugMode, bitAnd);
        }

        try
        {
            contours = getContours(img, contoursThresh);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << "\n";
        }

        boundedRects = getBoundedRects(contours);

        if (CALIB)
        {
            uShapeThresholdWindows(img, contours, boundedRects, minDistFromContours, maxDistFromContours, applyUShapeThreshold, uShapeThresholdWindow, STREAM);
            sideRatioThresholdWindows(img, contours, boundedRects, sideRatioParam, sideRatioMaxDeviationParam, applySideRatioThreshold, sideRatioThresholdWindow, STREAM);
            areaRatioThresholdWindows(img, contours, boundedRects, minAreaParam, maxAreaParam, areaRatioParam, areaRatioMaxDeviationParam, applyAreaRatioThreshold, areaRatioThresholdWindow, STREAM);
            angleThresholdWindows(img, contours, boundedRects, angleMaxDeviationParam, applyAngleThreshold, angleThresholdWindow, STREAM);
        }
        else
        {
            // Filter out contours that do not match the 'U' shape
            uShapeThreshold(contours, boundedRects, minDistFromContours, maxDistFromContours);
            sideRatioThreshold(contours, boundedRects, sideRatio, sideRatioMaxDeviation);
            areaRatioThreshold(contours, boundedRects, minArea, maxArea, areaRatio, areaRatioMaxDeviation);
            angleThreshold(contours, boundedRects, angleMaxDeviation);
        }

        if (contours.size() > 0)
        {
            // Extract the corners of the target's bounded box
            boundedRects[goalInd].points(rectPoints);
            
            // Find the corners of the target's contours
            std::vector<cv::Point> corners = getCorners(contours[goalInd], SCREEN_WIDTH, SCREEN_HEIGHT);
            cv::Point2f mc = getCenterOfMass(contours[goalInd]);
            
            double hypotenuse = getShortestDistance(rectPoints, GAME_ELEMENT_WIDTH, focalLength, CALIB_DISTANCE, isFocalLengthCalib);
            yaw = getYaw(SCREEN_WIDTH, hypotenuse, GAME_ELEMENT_WIDTH, corners, mc);
            pitch = getPitch(height, hypotenuse);
        
            putData(img, CALIB_DISTANCE, yaw, pitch);
            drawContours(img, contours, BLUE_GREEN);
            drawBoundedRects(img, boundedRects, GREEN);

            // Draw line from center of img to center of mass
            cv::line(img, cv::Point(SCREEN_WIDTH / 2, mc.y), mc, PURPLE);
            // Draw center of mass
            cv::circle(img, mc, 5, PURPLE); 

            // Draw the corners of the target's contour
            for (int i = 0; i < 4; ++i)
                cv::circle(img, corners[i], 5, LIGHT_GREEN);
        }

        //if (!STREAM) cv::imshow("Final", img);
        if (FPS)
        {
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> timeElapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            fps = 1.0 / timeElapsed.count();

            fprintf(pipe_gp, "%f %f\n", fpsTick, fps);
            avg += fps;
            fpsTick++;
        }
		kill = cv:: waitKey(5);
	}
	return 0;	
}
