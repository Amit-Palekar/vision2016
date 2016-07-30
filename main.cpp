#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include "include/utils/angles.hpp"
#include "include/utils/distance.hpp"
#include "include/utils/gaussianBlur.hpp"
#include "include/utils/getBoundedRects.hpp"
#include "include/utils/getCenterOfMass.hpp"
#include "include/utils/getContours.hpp"
#include "include/utils/netThread.hpp"
#include "include/utils/udpClientServer.hpp"

#include "include/filters/cannyEdgeDetect.hpp"
#include "include/filters/dilateErode.hpp"
#include "include/filters/gaussianBlur.hpp"
#include "include/filters/houghLines.hpp"
#include "include/filters/hsvColorThreshold.hpp"
#include "include/filters/laplacianSharpen.hpp"
#include "include/filters/mergeFinal.hpp"
#include "include/filters/sideAngleAreaThreshold.hpp"
#include "include/filters/sideAngleAreaThresholdWindows.hpp"
#include "include/filters/uShapeThreshold.hpp"

const int DEBUG = 0;
const int FPS = 0;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const double PI = 3.14159265;

// Measurements in inches
const double TOWER_HEIGHT = 83;		
const double CAMERA_HEIGHT = 6;
const double FOCAL_LENGTH = 555;
const int CAMERA_NUM = 0;
const int CALIB_DIST = 186;

double pitch = 0;
double yaw = 0;
double fps = 0;

char buff[50];
const std::string TARGET_ADDR = "10.1.15.2";
const std::string HOST_ADDR = "10.1.15.8";
const int UDP_PORT = 5810;
const char START_SIGNAL = '@';
const char STOP_SIGNAL = '#';
const char GET_SIGNAL = 'G';
const char RESUME_SIGNAL = 'R';
const char PAUSE_SIGNAL = 'P';

int main( int argc, char *argv[])
{
	// gaussianBlur parameters
	int blur_ksize = 7;
	int sigmaX = 25;
	int sigmaY = 25;

	// hsvColorThreshold parameters
	int hMin = 135;
	int hMax = 180;
	int sMin = 0;
	int sMax = 100;
	int vMin = 80;
	int vMax = 100;
	int debugMode = 0;
	// 0 is none, 1 is bitAnd between h, s, and v
	int bitAnd = 1;

	// drawBoundedRects parameters
	double calcDist = 0;

	// Tower height is 7 feet (to the bottom of the tape) which is 84 inches
	int height = TOWER_HEIGHT - CAMERA_HEIGHT;
	int contoursThresh = 140;
	
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::RotatedRect> boundedRects;
	cv::Point2f rectPoints[4];
	int goalInd = 0;
	int screenWidth = 0;
    int screenHeight = 0;
    int isCalib = 0;

    // Shape threshold parameters
    int s = 210;
    int a = 30;
    int minA = 800;
    int maxA = 10000;
    int sideT = 30;
    int areaT = 30;
    int angleT = 20;
    double sideRatio = (double) s / 100;
    double areaRatio = (double) a / 100;
    double minArea = (double) minA;
    double maxArea = (double) maxA;
    double sideThreshold = (double) sideT / 100;
    double areaThreshold = (double) areaT / 100;
    double angleThreshold = (double) angleT;
   
	udp_client_server::udp_client client(TARGET_ADDR, UDP_PORT);
	udp_client_server::udp_server server(HOST_ADDR, UDP_PORT);

    std::cerr << "\nPlease send initial ping to " << HOST_ADDR << "\n\n";

    std::thread listenForPing (receivePing, std::ref(server));
    listenForPing.join();
    std::thread netSend (sendData, std::ref(client));
    netSend.detach();
    std::thread netReceive (receiveData, std::ref(server));
    netReceive.detach();

	cv::VideoCapture camera (CAMERA_NUM);
    if (!camera.isOpened())
    {
        std::cerr << "Error - Could not open camera at port " << CAMERA_NUM << "\n";
        return -1;
    }
	std::cerr << "Opened camera at port " << CAMERA_NUM << "\n";
	
    cv::Mat image;
    char kill = ' ';
    
	while ((kill != 'q'))
	{
		camera >> image;
		cv::imshow("RGB", image);
		gaussianBlur(image, blur_ksize, sigmaX, sigmaY);
		hsvColorThreshold(image, hMin, hMax, sMin, sMax, vMin, vMax, debugMode, bitAnd);
		
		contours = getContours(src, contoursThresh);
		boundedRects = getBoundedRects(contours);
		// Filter out contours that do not match the 'U' shape
		// TODO: Determine what to do in the case that there are multiple 'U' shapes
		uShapeThreshold(contours);
		goalInd = sideAngleAreaThresholdWindows(image, contours, boundedRects, sideRatio, areaRatio, minArea, maxArea, sideThreshold, areaThreshold, angleThreshold);
		// Extract the corners of the target's bounded box
		boundedRects[goalInd].points(rectPoints);
		
		// Find the corners of the target's contours
		std::vector<cv::Point> corners = getCorners(contours[goalInd], SCREEN_WIDTH, SCREEN_HEIGHT);
		
		cv::Point2f mc = getCenterOfMass(contours[goalInd]);
		
		hypotenuse = getHypotenuse(rectPoints, focalLen, CALIB_DISTANCE, height, isCalib);
		yaw = getYaw(SCREEN_WIDTH, hypotenuse, corners, mc);
		pitch = getPitch(height, hypotenuse);
		
		if (DEBUG) putContours(image, contours, cv::Scalar(255, 255, 0));
		
        putData(image, CALIB_DIST, yaw, pitch);
        cv::imshow("Final", image);
        
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
