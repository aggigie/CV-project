#include "CorruptedFrameDetector.h"

using namespace std;
CorruptedFrameDetector::~CorruptedFrameDetector()
{
}
void CorruptedFrameDetector::Detect(const cv::Mat& inp, cv::Mat& out)
{
	out = inp;
}

string CorruptedFrameDetector::ToString()
{
	return string();
}
