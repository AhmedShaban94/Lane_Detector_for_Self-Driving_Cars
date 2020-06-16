#pragma once 
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <memory>

//using Line = cv::Vec4i;

class LanePredictor
{
private:
	double img_center_;

	bool left_flag_;  // Tells us if there's left boundary of lane detected
	bool right_flag_;  // Tells us if there's right boundary of lane detected

	cv::Point right_b_;  // Members of both line equations of the lane boundaries:
	double right_m_;  // y = m*x + b

	cv::Point left_b_;
	double left_m;

public:

	// Rule of Five.
	LanePredictor() :
		img_center_{ 0.0 }, left_flag_{ false }, right_flag_{ false },
		right_b_{ cv::Point() }, right_m_{ 0.0 },
		left_b_{ cv::Point() }, left_m{ 0.0 }
	{}

	virtual ~LanePredictor() = default;

	LanePredictor(const LanePredictor& other)
		: img_center_(other.img_center_),
		left_flag_(other.left_flag_),
		right_flag_(other.right_flag_),
		right_b_(other.right_b_),
		right_m_(other.right_m_),
		left_b_(other.left_b_),
		left_m(other.left_m)
	{
	}

	LanePredictor(LanePredictor&& other) noexcept
		: img_center_(other.img_center_),
		left_flag_(other.left_flag_),
		right_flag_(other.right_flag_),
		right_b_(std::move(other.right_b_)),
		right_m_(other.right_m_),
		left_b_(std::move(other.left_b_)),
		left_m(other.left_m)
	{
	}

	LanePredictor& operator=(const LanePredictor& other)
	{
		if (this == &other)
			return *this;
		img_center_ = other.img_center_;
		left_flag_ = other.left_flag_;
		right_flag_ = other.right_flag_;
		right_b_ = other.right_b_;
		right_m_ = other.right_m_;
		left_b_ = other.left_b_;
		left_m = other.left_m;
		return *this;
	}

	LanePredictor& operator=(LanePredictor&& other) noexcept
	{
		if (this == &other)
			return *this;
		img_center_ = other.img_center_;
		left_flag_ = other.left_flag_;
		right_flag_ = other.right_flag_;
		right_b_ = std::move(other.right_b_);
		right_m_ = other.right_m_;
		left_b_ = std::move(other.left_b_);
		left_m = other.left_m;
		return *this;
	}

	std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>> classifyLines(const std::vector<cv::Vec4i>& lines, const cv::Mat img_edges);  // Sprt detected lines by their slope into right and left lines
	std::vector<cv::Point> regression(const std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>>& left_right_lines, const cv::Mat inputImage);  // Get only one line for each side of the lane
	std::string predictTurn();  // Determine if the lane is turning or not by calculating the position of the vanishing point
};