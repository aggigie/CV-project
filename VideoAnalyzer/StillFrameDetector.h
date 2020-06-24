#pragma once
#include <vector>
#include "FrameDetector.h"

class StillFrameDetector :
	public FrameDetector
{
private:
	const size_t imgDivisor = 50;
	int lastDepth = 0;
	std::vector<std::vector<cv::Mat>> lastHistograms;
	double threshold = 95.0;
	double similarityRate = 100;
	static void SplitMat(const cv::Mat& img, const int divisor, std::vector<cv::Mat>& blocks);
	static void GetHistagramsOfMat(const cv::Mat& chunk, std::vector<cv::Mat>& histagrams);
public:
	~StillFrameDetector();
	void Detect(const cv::Mat& inp, cv::Mat& out) override;
	std::string ToString() override;
};


