#include "StillFrameDetector.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void StillFrameDetector::SplitMat(const Mat& img, const int divisor, vector<Mat>& blocks)
{
    const auto colWidth = img.cols / divisor;
    const auto rowHeight = img.rows / divisor;

    for (int y = 0; y < img.rows; y += rowHeight) {
        for (int x = 0; x < img.cols; x += colWidth) {
            blocks.push_back(img(Rect(
                x, 
                y, 
                x + colWidth > img.cols ? img.cols - x : colWidth, 
                y + rowHeight > img.rows ? img.rows - y : rowHeight)));
        }
    }
}

void StillFrameDetector::GetHistagramsOfMat(const cv::Mat& chunk, std::vector<cv::Mat>& histagrams)
{
    vector<Mat> bgrPlanes;
    split(chunk, bgrPlanes);
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange = { range };
    bool uniform = true, accumulate = false;
    for (size_t i = 0; i < bgrPlanes.size(); i++) {
        Mat hist;
        calcHist(&bgrPlanes[i], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
        histagrams.push_back(hist);
    }
}

StillFrameDetector::~StillFrameDetector()
{
}


void StillFrameDetector::Detect(const Mat& inp, Mat& out)
{
    // Checking if the image was passed correctly
    if (!inp.data || inp.empty())
        throw exception("StillFrameDetector received wrong input image");
    vector<vector<Mat>> currentHistograms;
    vector<Mat> matChunks;
    SplitMat(inp, imgDivisor, matChunks);
    for (const Mat& chunk : matChunks) {
        vector<Mat> histagrams;
        GetHistagramsOfMat(chunk, histagrams);
        currentHistograms.emplace_back(histagrams);
    }

    
    if (!lastHistograms.empty()) {
        similarityRate = 100;
        //Compare current with last
        if (lastHistograms.size() == currentHistograms.size() && 
            lastHistograms.size() > 0 && 
            lastDepth == inp.channels()) {
            size_t differentHistagrams = 0;
            for (size_t i = 0; i < currentHistograms.size(); i++) {
                for (size_t j = 0; j < currentHistograms[i].size(); j++) {
                    if (countNonZero(currentHistograms[i][j] != lastHistograms[i][j]) != 0) {
                        differentHistagrams++;
                    }

                }
            }

            similarityRate -= ((double)differentHistagrams / (currentHistograms.size() * currentHistograms[0].size())) * 100;
        }
        else {
            // Different size of frames or depth
            similarityRate = 0;
        }

    }
    lastHistograms = currentHistograms;
    lastDepth = inp.channels();

    out = inp;
}

string StillFrameDetector::ToString()
{
    return similarityRate > threshold ? "Still frame" : "Different frame";
}
