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

std::pair<std::vector<cv::Point>, std::string> LaneDetector::PredictLaneTurn(const std::vector<cv::Vec4i>& lines, cv::Mat img_edges)
{
	//Classify line into right/left lines 
	auto right_left_lines = predictor_->classifyLines(lines, img_edges);

	//Fitting lines into boundary of lane. 
	auto lane = predictor_->regression(right_left_lines, img_edges);

	//Predicting turn of the car based on slope of lines. 
	auto turn = predictor_->predictTurn();

	return std::make_pair(lane, turn);
}

cv::Mat LaneDetector::plotLaneOnFrame(cv::Mat inputImage, std::vector<cv::Point> lane, const std::string& turn)
{
	return plotter_->plotLane(inputImage, lane, turn);
}
