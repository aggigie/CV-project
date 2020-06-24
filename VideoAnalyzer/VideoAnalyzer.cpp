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
    VideoWriter videoWriter;
    videoWriter.open("TestVideo.mp4", cv::VideoWriter::fourcc('D','I','V','3'), 30.0,
        Size(640, 480));

    if (!videoWriter.isOpened())
    {
        throw runtime_error("Unable to open videoWriter");
    }
    Mat src2 = imread(samples::findFile("lena2.bmp"), IMREAD_COLOR);
    Mat src3 = imread(samples::findFile("color.jpg"), IMREAD_COLOR);
    Mat src4 = imread(samples::findFile("color2.jpg"), IMREAD_COLOR);

    for (size_t i = 0; i < 300; i++)
    {
        videoWriter.write(src2);
    }

    for (size_t i = 0; i < 300; i++)
    {
        auto cloned = src2.clone();
        putText(cloned, "TEST", Point(rand() % cloned.cols, rand() % cloned.rows), 1, 1, Scalar(0, 0, 0));
        videoWriter.write(cloned);
    }
    int poinSize = 2;
    for (size_t i = 0; i < 600; i++)
    {
        auto cloned = src2.clone();
        putText(cloned, "Points TEST", Point(100,100), 1, 1, Scalar(0, 0, 0));
        circle(cloned, Point(rand() % cloned.cols, rand() % cloned.rows), poinSize, Scalar(255, 255, 0), FILLED);
        if (i % 30 == 0) {
            poinSize++;
        }
        videoWriter.write(cloned);
    }
    for (size_t i = 0; i < 150; i++)
    {
        videoWriter.write(src2);
    }
    for (size_t i = 0; i < 300; i++)
    {
        videoWriter.write(src3);
    }
    for (size_t i = 0; i < 150; i++)
    {
        videoWriter.write(src2);
    }
    for (size_t i = 0; i < 300; i++)
    {
        videoWriter.write(src4);
    }

    videoWriter.release();
    /*CommandLineParser parser(argc, argv, "{@input | lena.bmp | input video}");
    VideoAnalyzer analyzer(samples::findFile(parser.get<String>("@input")));
    analyzer.Start();*/
    return EXIT_SUCCESS;
}