#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

#include "StillFrameDetector.h"
using namespace std;
using namespace cv;
int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | lena.bmp | input video}");
    Mat src = imread(samples::findFile(parser.get<String>("@input")), IMREAD_COLOR);
    Mat src2 = imread(samples::findFile("lena2.bmp"), IMREAD_COLOR);
    Mat src3 = imread(samples::findFile("emptyLana.jpg"), IMREAD_COLOR);
    Mat out;
    StillFrameDetector sfd;
	if (src.empty())
    {
        return EXIT_FAILURE;
    }
    sfd.Detect(src, out);
    sfd.Detect(src2, out);
    sfd.Detect(src3, out);
    imshow("Source image", src);
    imshow("calcHist Demo", out);
    waitKey();
    return EXIT_SUCCESS;
}