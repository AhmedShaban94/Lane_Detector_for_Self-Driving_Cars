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
	//double m_resize_factor;
	double img_size;
	double img_center;
	bool left_flag = false;  // Tells us if there's left boundary of lane detected
	bool right_flag = false;  // Tells us if there's right boundary of lane detected
	cv::Point right_b;  // Members of both line equations of the lane boundaries:
	double right_m;  // y = m*x + b
	cv::Point left_b;  //
	double left_m;  //

	cv::Mat input_image_;

public:
	std::unique_ptr<LaneCalculator> calculator_;
	std::unique_ptr<LanePredictor> predictor_;
	std::unique_ptr<LanePlotter> plotter_;
	LaneDetector() : calculator_{ std::make_unique<LaneCalculator>() },
		predictor_{ std::make_unique<LanePredictor>() },
		plotter_{ std::make_unique<LanePlotter>() }
	{}
	std::vector<cv::Vec4i> CalculateLane(const cv::Mat);

	// Calculator functions 
	cv::Mat deNoise(const cv::Mat inputImage);  // Apply Gaussian blurring to the input Image
	cv::Mat edgeDetector(cv::Mat img_noise);  // Filter the image to obtain only edges
	cv::Mat mask(const cv::Mat img_edges);  // Mask the edges image to only care about ROI
	std::vector<cv::Vec4i> houghLines(const cv::Mat img_mask);  // Detect Hough lines in masked edges image

	// Predictor functions 
	std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>> classifyLines(const std::vector<cv::Vec4i>& lines, const cv::Mat img_edges);  // Sprt detected lines by their slope into right and left lines
	std::vector<cv::Point> regression(const std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>>& left_right_lines, const cv::Mat inputImage);  // Get only one line for each side of the lane
	std::string predictTurn();  // Determine if the lane is turning or not by calculating the position of the vanishing point

	// plotter functions 
	cv::Mat plotLane(cv::Mat inputImage, std::vector<cv::Point> lane, std::string turn);  // Plot the resultant lane and turn prediction in the frame.

	// setters and getters
	[[nodiscard]] cv::Mat GetInputImage() const { return input_image_; }
	void SetInputImage(const cv::Mat source);
};