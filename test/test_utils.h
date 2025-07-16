#include <string>
#include <vector>

#include "BYTETracker.h"


bool fileExists(const std::string& path);

std::string pathJoin(const std::vector<std::string>& pathSegments);

void readYoloLabelFile(const std::string& path, int imgWidth, int imgHeight, std::vector<Object>& objects);

bool hasValidExtension(const std::string& fileName, const std::vector<std::string>& validExtensions);

void drawTracklets(cv::Mat& image, const std::vector<STrack>& tracklets);