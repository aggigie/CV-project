#pragma once
#include "FrameDetector.h"
class BlackBarsFrameDetector :
	public FrameDetector
{
public:
	~BlackBarsFrameDetector();
	void Detect(const cv::Mat& inp, cv::Mat& out) override;
	
};

