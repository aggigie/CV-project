#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

#include "StillFrameDetector.h"
using namespace std;
using namespace cv;
int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | lena.jpg | input video}");
    Mat src = imread(samples::findFile(parser.get<String>("@input")), IMREAD_COLOR);
    Mat out;
    StillFrameDetector sfd;
	if (src.empty())
    {
        return EXIT_FAILURE;
    }
    sfd.Detect(src, out);
    imshow("Source image", src);
    imshow("calcHist Demo", out);
    waitKey();
    return EXIT_SUCCESS;
}