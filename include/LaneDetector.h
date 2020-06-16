#pragma once
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <memory> 
#include "LaneCalculator.h"
#include "LanePredictor.h"
#include "LanePlotter.h"

class LaneDetector
{
private:
	std::unique_ptr<LaneCalculator> calculator_;
	std::unique_ptr<LanePredictor> predictor_;
	std::unique_ptr<LanePlotter> plotter_;

public:

	LaneDetector() : calculator_{ std::make_unique<LaneCalculator>() },
		predictor_{ std::make_unique<LanePredictor>() },
		plotter_{ std::make_unique<LanePlotter>() }
	{}

	// process functions 
	std::vector<cv::Vec4i> CalculateLane(cv::Mat);

	// predict lane turn
	std::pair<std::vector<cv::Point>, std::string> PredictLaneTurn(const std::vector<cv::Vec4i>& lines, cv::Mat img_edges);

	// plot lane of frame
	cv::Mat plotLaneOnFrame(cv::Mat inputImage, std::vector<cv::Point> lane, const std::string& turn);
};