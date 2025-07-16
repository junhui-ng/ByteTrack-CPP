#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#include "BYTETracker.h"
#include "test_utils.h"


int main(int argc, char* argv[])
{
    if (argc < 3) 
    {
        std::cout << "Please provide the required arguments: video_path, labels_path, output_path." << std::endl;
        std::cout << "  video_path   : Input video path." << std::endl;
        std::cout << "  labels_path  : Path to the detection model predictions (YOLO format)" << std::endl;
        std::cout << "  output_path  : Path to save the output video" << std::endl;
        return 1;
    }

    const char* video_path = argv[1];
    const char* labels_path = argv[2];
    const std::string output_path = argv[3];

    // Initialize video capture
    cv::VideoCapture cap(video_path);
    if ( !cap.isOpened() ) return 1;
    // Input video information
    int img_w = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int img_h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(cv::CAP_PROP_FPS);
    long nFrame = static_cast<long>(cap.get(cv::CAP_PROP_FRAME_COUNT));

    // Output video writer
    std::string output_video_path = output_path;
    if (!hasValidExtension(output_path, {"mp4"}))
    {
        output_video_path = pathJoin({output_path, "output.mp4"});
    }
    cv::VideoWriter writer(
        output_video_path,
        cv::VideoWriter::fourcc('M','J','P','G'), 
        fps, 
        cv::Size(img_w, img_h)
    );

    // Initialize tracker
    BYTETracker tracker(60, 0.3, 0.1, 0.5, 0.8);
    std::vector<Object> objects;
    std::vector<STrack> tracklets;
    std::vector<STrack> lostTracklets;

    int frame_idx = 1;
    cv::Mat frame;
    cv::Mat outputFrame;
    while (true)
    {
        // Read frame
        if(!cap.read(frame)) break;
        outputFrame = frame;

        // Read and process model predictions
        std::string labelFile = pathJoin({labels_path, std::to_string(frame_idx) + ".txt"});
        if (fileExists(labelFile))
        {
            objects.clear();
            tracklets.clear();

            // Read labels
            readYoloLabelFile(labelFile, img_w, img_h, objects);

            // Tracking
            tracker.update(objects, lostTracklets, tracklets);

            // Draw boxes
            drawTracklets(outputFrame, tracklets);
        }

        // Write output frame
        writer.write(outputFrame);

        frame_idx++;
    }

    // Release video capture and writer
    cap.release();
    writer.release();

    std::cout << "Terminating program" << std::endl;

    return 0;
}
