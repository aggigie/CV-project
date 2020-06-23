#pragma once
#include "FrameDetector.h"

class CorruptedFrameDetector :
	public FrameDetector
{
public:
	~CorruptedFrameDetector();
	void Detect(const cv::Mat& inp, cv::Mat& out) override;
};

