#include "BlackBarsFrameDetector.h"
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;
BlackBarsFrameDetector::~BlackBarsFrameDetector()
{
}

void BlackBarsFrameDetector::Detect(const cv::Mat & inp, cv::Mat & out)
{
	Mat gray, thresh;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int largestContourIndex;
	double largestArea = 0;
	cvtColor(inp, gray, COLOR_BGR2GRAY);
	threshold(gray, thresh, 20, 2555, THRESH_BINARY);
	findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) 	{
		double a = contourArea(contours[i], false);  
		if (a > largestArea) {
			largestArea = a;
			largestContourIndex = i;                
			boundRect = boundingRect(contours[i]);
		}
	}
	cv::Rect currentMatRect = cv::Rect(cv::Point(0, 0), inp.size());
	if (currentMatRect.area() > boundRect.area()) {
		out = inp(boundRect);
	}
	else out = gray;
}

string BlackBarsFrameDetector::ToString()
{
	return currentMatRect.area() > boundRect.area() ? "Found black bars" : "Black bars not found";
}
