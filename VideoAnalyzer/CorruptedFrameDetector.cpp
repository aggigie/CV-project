#include "CorruptedFrameDetector.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iostream>
using namespace cv;

CorruptedFrameDetector::~CorruptedFrameDetector()
{
}

Mat removeLight(Mat img, Mat pattern)
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

Mat calculateLightPattern(Mat img)
{
	Mat pattern;
	// Basic and effective way to calculate the light pattern from one image
	blur(img, pattern, Size(img.cols / 3, img.cols / 3));
	return pattern;
}

void ConnectedComponentsStats(Mat img)
{
	// Use connected components with stats
	Mat labels(img.rows, img.cols, img.type());
	int num_objects = connectedComponents(img, labels);
	// Check the number of objects detected
	if (num_objects < 2) {
		std::cout << "No objects detected" << std::endl;
		return;
	}
	else {
		std::cout << "Number of objects detected: " << num_objects - 1 << std::endl;
	}
}

void CorruptedFrameDetector::Detect(const cv::Mat & inp, cv::Mat & out)
{
	Mat wOutLight = imread(samples::findFile("old.jpg"), IMREAD_COLOR);
	Mat noiseRemoved, lightRemoved, binary, finalMat;
	// Noise removal
	medianBlur(inp, noiseRemoved, 3);
	//out = noiseRemoved;
	// Lighting removal
	lightRemoved = removeLight(noiseRemoved, calculateLightPattern(noiseRemoved));
//	out = lightRemoved;
	// Binarization
	threshold(lightRemoved, binary, 140, 255, THRESH_BINARY_INV);
	//out = noiseRemoved;
	// The connected component extraction (labeling)
	//int noFinalObjects = connectedComponents(binary, binary);
	//int noObjects = connectedComponents(inp, labels);
//	if (noObjects > noFinalObjects)
//	{
//		out = binary;
//	}
//	else out = inp;
	binary.convertTo(finalMat, CV_8U);
	ConnectedComponentsStats(finalMat);
}
