#define CVUI_IMPLEMENTATION
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <memory>
#include <exception>

#include "StillFrameDetector.h"
#include "BlackBarsFrameDetector.h"
#include "CorruptedFrameDetector.h"
#include "cvui.h"
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
    if (!video.isOpened()) {
        throw exception((string("Unable to open given video") + videoPath).c_str());
    }
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
    Mat src;
    while (true) {
        controls = cv::Scalar(49, 52, 49);
        
        video >> src;
        if (src.empty())
        {
            break;
        }
        // Fill the frame with a nice color
        if (HandleButtonPress()) {
            break;
        }

        //Mat src2 = imread(samples::findFile("lena2.bmp"), IMREAD_COLOR);

        Mat out;
        currentFrameDetector->Detect(src, out);

        cvui::text(controls, 300, 90, currentFrameDetector->ToString());

        cvui::imshow("VideoAnalyzer", controls);
        imshow("Preview", out);

        if (cv::waitKey(20) == 27) {
            break;
        }
    }
}

bool VideoAnalyzer::HandleButtonPress()
{
    if (cvui::button(controls, 40, 20, "Black Bars detection")) {
        currentFrameDetector = make_unique<BlackBarsFrameDetector>();
    }
    if (cvui::button(controls, 40, 80, "Still frames detection")) {
        currentFrameDetector = make_unique<StillFrameDetector>();
    }
    if (cvui::button(controls, 40, 140, "Corrupted frame detection")) {
        currentFrameDetector = make_unique<CorruptedFrameDetector>();
    }
    return cvui::button(controls, 40, 200, "Exit");
}


int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | TestVideo.mp4 | input video}");
    VideoAnalyzer analyzer(samples::findFile(parser.get<String>("@input")));
    analyzer.Start();
    return EXIT_SUCCESS;
}