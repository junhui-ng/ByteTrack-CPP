#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <dirent.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "test_utils.h"


bool fileExists(const std::string& path)
{
#ifdef _WIN32
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES &&
            !(attr & FILE_ATTRIBUTE_DIRECTORY));
#else
    std::ifstream f(path);
    return f.good();
#endif
}

std::string pathJoin(const std::vector<std::string>& pathSegments)
{
    std::string path = "";
    std::string sep;
#if _win32
    sep = "\\";
#else
    sep = "/";
#endif

    int numSegments = pathSegments.size();
    for (int i = 0; i < numSegments; ++i)
    {
        path += pathSegments[i];
        if (i < numSegments - 1)
        {
            path += sep;
        }
    }

    return path;
}

void readYoloLabelFile(const std::string& path, int imgWidth, int imgHeight, std::vector<Object>& objects) 
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open label file: " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        int class_id;
        float x_center, y_center, width, height, conf;

        ss >> class_id >> x_center >> y_center >> width >> height >> conf;

        float x = (x_center - width / 2.0f) * imgWidth;
        float y = (y_center - height / 2.0f) * imgHeight;
        float w = width * imgWidth;
        float h = height * imgHeight;

        Object obj;
        obj.label = class_id;
        obj.prob = conf;
        obj.rect = cv::Rect_<float>(x, y, w, h);
        objects.push_back(obj);
    }
}

bool hasValidExtension(const std::string& fileName, const std::vector<std::string>& validExtensions) 
{
    for (const std::string& ext : validExtensions) {
        if (fileName.size() >= ext.size() && 
            fileName.compare(fileName.size() - ext.size(), ext.size(), ext) == 0) {
            return true;
        }
    }
    return false;
}

void drawTracklets(cv::Mat& image, const std::vector<STrack>& tracklets)
{
    for (const STrack& tracklet : tracklets) {
        cv::Rect box(
            static_cast<int>(tracklet.tlwh[0]),
            static_cast<int>(tracklet.tlwh[1]),
            static_cast<int>(tracklet.tlwh[2]),
            static_cast<int>(tracklet.tlwh[3])
        );

        cv::Scalar color = tracklet.get_color();

        cv::rectangle(image,
            box,
            color,
            2,
            cv::LINE_AA,
            false
        );

        cv::Point2i textLocation(
            static_cast<int>(tracklet.tlwh[0]),
            static_cast<int>(tracklet.tlwh[1]) - 5
        );
        cv::putText(image, 
            std::to_string(tracklet.track_id), 
            textLocation, 
            cv::FONT_HERSHEY_SIMPLEX, 
            0.5, 
            color, 
            1,
            cv::LINE_AA,
            false
        );
    }
}