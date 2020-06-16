#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "LaneDetector.h"

std::vector<cv::Vec4i> LaneDetector::CalculateLane(cv::Mat frame)
{
	// to implement set input_image
	calculator_->SetInputImage(frame); //tested

	// Image denoiseing using gaussian filter blurring.   
	auto deNoisedFrame = calculator_->deNoise(); //tested. 

	// Edge detection using Canny Edge detector.
	auto edges = calculator_->edgeDetector(deNoisedFrame);

	// masking each frame to focus on ROI (region of interest). [rect polygone]
	auto mask = calculator_->mask(edges);

	// Detecting Line in each frame using HoughLines (transform). 
	auto lines = calculator_->houghLines(mask);

	return lines;
}

