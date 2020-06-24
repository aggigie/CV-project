#include "CorruptedFrameDetector.h"

#include <iostream>
using namespace cv;

using namespace std;

Mat CorruptedFrameDetector::RemoveLight(Mat img, Mat pattern)
{
	Mat aux;
	// Remove light from background and normalize
	Mat img32, pattern32;
	img.convertTo(img32, CV_32F);
	pattern.convertTo(pattern32, CV_32F);
	aux = 1 - (img32 / pattern32);
	aux = aux * 255;
	aux.convertTo(aux, CV_8U);
	return aux;
}

Mat CorruptedFrameDetector::CalculateLightPattern(Mat img)
{
	Mat pattern;
	blur(img, pattern, Size(img.cols / 3, img.cols / 3));
	return pattern;
}

bool CorruptedFrameDetector::ConnectedComponentsStats(Mat img1, Mat img2)
{
	Mat labels(img1.rows, img1.cols, img1.type());
	Mat labels2(img2.rows, img2.cols, img2.type());

	int noObjects = connectedComponents(img1, labels);
	int noObjectsOrig = connectedComponents(img2, labels);

	return noObjects / (double)noObjectsOrig > 0.5;
}

void CorruptedFrameDetector::Detect(const cv::Mat& img, cv::Mat& out)
{
	Mat imgNoise, imgBoxSmooth, gray, grayBin;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	threshold(gray, grayBin, 140, 255, THRESH_BINARY_INV);
	medianBlur(gray, imgNoise, 3);
	blur(gray, imgBoxSmooth, Size(3, 3));
	Mat lightPattern = CalculateLightPattern(imgNoise);
	medianBlur(lightPattern, lightPattern, 3);
	Mat imgNoLight;
	imgNoise.copyTo(imgNoLight);
	Mat imgThr;
	threshold(imgNoLight, imgThr, 140, 255, THRESH_BINARY_INV);
	detectedNoise = ConnectedComponentsStats(imgThr, grayBin);
	out = imgThr;
}


string CorruptedFrameDetector::ToString()
{
	return detectedNoise ? "Detected noise" : "Clear";
}