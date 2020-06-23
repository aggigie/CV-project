#include "BlackBarsFrameDetector.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

BlackBarsFrameDetector::~BlackBarsFrameDetector()
{
}

// wyznacz x,y,h,w z thresholda :)
void BlackBarsFrameDetector::Detect(const cv::Mat & inp, cv::Mat & out)
{
	Mat gray, thresh;
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	Rect bounding_rect;
	int largest_contour_index;
	double largest_area = 0;
	cvtColor(inp, gray, COLOR_BGR2GRAY);
	threshold(gray, thresh, 20, 2555, THRESH_BINARY);
	findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) 	{
		double a = contourArea(contours[i], false);  
		if (a > largest_area) {
			largest_area = a;
			largest_contour_index = i;                
			bounding_rect = boundingRect(contours[i]); 
		}
	}
	cv::Rect rect_orginal = cv::Rect(cv::Point(0, 0), inp.size());
	if (rect_orginal.area() > bounding_rect.area()) {
		out = inp(bounding_rect);
	}
	else out = gray;
}
