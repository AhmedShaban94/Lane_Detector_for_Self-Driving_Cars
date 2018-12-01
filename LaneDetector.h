#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp> 

class LaneDetector
{
private: 
	int img_size; 
	int img_center; 
	bool left_flag = false; 
	bool right_flag = false; 
	cv::Point left_bias;  // y = slope * x + bias 
	int left_slope; 
	cv::Point right_bias; 
	int right_slope; 

public:
	LaneDetector();
	~LaneDetector();
};
