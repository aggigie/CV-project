#pragma once
#include "FrameDetector.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace cv;
class CorruptedFrameDetector :
	public FrameDetector
{
	bool detectedNoise;
public:
	Mat RemoveLight(Mat img, Mat pattern);
	Mat CalculateLightPattern(Mat img);
	bool ConnectedComponentsStats(Mat thresImg, Mat img2);
	void Detect(const cv::Mat& inp, cv::Mat& out) override;
	std::string ToString() override;
};

