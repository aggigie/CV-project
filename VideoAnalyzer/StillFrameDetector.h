#pragma once
#include <vector>

#include "FrameDetector.h"

class StillFrameDetector :
	public FrameDetector
{
private:
	static void SplitMat(const cv::Mat& img, const int divisor, std::vector<cv::Mat>& blocks);
public:
	             ~StillFrameDetector();
	void Detect(const cv::Mat& inp, cv::Mat& out) override;
};


