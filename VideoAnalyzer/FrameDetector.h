#pragma once
#include <string>
#include <opencv2/core.hpp>
class FrameDetector
{
protected:
	FrameDetector() = default;
public:
	void virtual Detect(const cv::Mat& inp, cv::Mat& out) = 0;
	std::string virtual ToString() = 0;
};

