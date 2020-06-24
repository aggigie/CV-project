#define CVUI_IMPLEMENTATION
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "cvui.h"
#include <iostream>
#include <memory>

#include "StillFrameDetector.h"
#include "BlackBarsFrameDetector.h"
#include "CorruptedFrameDetector.h"
using namespace std;
using namespace cv;
class VideoAnalyzer
{
public:
    VideoAnalyzer(string videoPath);
    ~VideoAnalyzer();
    void Start();
    void Update();
private:
    bool HandleButtonPress();
    unique_ptr<FrameDetector> currentFrameDetector;
    VideoCapture video;
    Mat controls;
};

VideoAnalyzer::VideoAnalyzer(string videoPath) : currentFrameDetector(make_unique<StillFrameDetector>()), video(videoPath), controls(200, 500, CV_8UC3)
{
}

VideoAnalyzer::~VideoAnalyzer()
{
}

void VideoAnalyzer::Start()
{
    cvui::init("VideoAnalyzer");
    Update();
}

void VideoAnalyzer::Update()
{
    while (true) {
        // Fill the frame with a nice color
        controls = cv::Scalar(49, 52, 49);
        
        if (cvui::button(controls, 360, 80, "Button")) {
            std::cout << "Regular button clicked!" << std::endl;
        }
        if (HandleButtonPress()) {
            break;
        }
        Mat src2 = imread(samples::findFile("lena2.bmp"), IMREAD_COLOR);
        Mat out;
        currentFrameDetector->Detect(src2, out);

        cvui::text(controls, 110, 80, currentFrameDetector->ToString());

        cvui::imshow("VideoAnalyzer", controls);
        imshow("Preview", out);

        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

bool VideoAnalyzer::HandleButtonPress()
{
    if (cvui::button(controls, 360, 60, "Black Bars detection")) {
        currentFrameDetector = make_unique<BlackBarsFrameDetector>();
    }
    if (cvui::button(controls, 360, 80, "Still frames detection")) {
        currentFrameDetector = make_unique<StillFrameDetector>();
    }
    if (cvui::button(controls, 360, 100, "Corrupted frame detection")) {
        currentFrameDetector = make_unique<CorruptedFrameDetector>();
    }
    return cvui::button(controls, 360, 80, "Exit");
}


int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | lena.bmp | input video}");
    VideoAnalyzer analyzer(samples::findFile(parser.get<String>("@input")));
    analyzer.Start();
    Mat src = imread(samples::findFile(parser.get<String>("@input")), IMREAD_COLOR);

    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).

    
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