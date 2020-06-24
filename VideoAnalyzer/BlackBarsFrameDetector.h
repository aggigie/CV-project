#pragma once
#include "FrameDetector.h"
#include <opencv2/core.hpp>

class BlackBarsFrameDetector :
	public FrameDetector
{
	cv::Rect boundRect;
	cv::Rect currentMatRect;
public:
	~BlackBarsFrameDetector();
	void Detect(const cv::Mat& inp, cv::Mat& out) override;
	std::string ToString() override;
};

