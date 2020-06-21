#pragma once
namespace cv {
	class Mat;
}

class FrameDetector
{
protected:
	virtual ~FrameDetector() = default;
public:
	void virtual Detect(const cv::Mat& inp, cv::Mat& out) = 0;
};

