#include "CorruptedFrameDetector.h"

#include <iostream>
using namespace cv;

using namespace std;

Mat CorruptedFrameDetector::RemoveLight(Mat img, Mat pattern)
{
	Mat aux;
	// Require change our image to 32 float for division
	Mat img32, pattern32;
	img.convertTo(img32, CV_32F);
	pattern.convertTo(pattern32, CV_32F);
	// Divide the image by the pattern
	aux = 1 - (img32 / pattern32);
	// Scale it to convert to 8bit format
	aux = aux * 255;
	// Convert 8 bits format
	aux.convertTo(aux, CV_8U);
	return aux;
}

Mat CorruptedFrameDetector::CalculateLightPattern(Mat img)
{
	Mat pattern;
	// Basic and effective way to calculate the light pattern from one image
	blur(img, pattern, Size(img.cols / 3, img.cols / 3));
	return pattern;
}

bool CorruptedFrameDetector::ConnectedComponentsStats(Mat img1)
{
	// Use connected components with stats
	Mat labels(img1.rows, img1.cols, img1.type());

	int noObjects = connectedComponents(img1, labels);

	return noObjects >= 2;
}

void CorruptedFrameDetector::Detect(const cv::Mat& img, cv::Mat& out)
{
	Mat imgNoise, imgBoxSmooth, gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	medianBlur(gray, imgNoise, 3);
	blur(gray, imgBoxSmooth, Size(3, 3));
	Mat lightPattern = CalculateLightPattern(imgNoise);
	medianBlur(lightPattern, lightPattern, 3);
	Mat imgNoLight;
	imgNoise.copyTo(imgNoLight);
	Mat imgThr;
	threshold(imgNoLight, imgThr, 140, 255, THRESH_BINARY_INV);
	detectedNoise = ConnectedComponentsStats(imgThr);
	out = imgThr;
}


string CorruptedFrameDetector::ToString()
{
	return detectedNoise ? "Detected noise" : "Clear";
}