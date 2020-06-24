#pragma once
#include "FrameDetector.h"

#include <opencv2/imgproc.hpp>

class CorruptedFrameDetector :
	public FrameDetector
{
public:
	~CorruptedFrameDetector();
	void Detect(const cv::Mat& inp, cv::Mat& out) override;
	std::string ToString() override;
};

