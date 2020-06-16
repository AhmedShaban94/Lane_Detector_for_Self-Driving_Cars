#pragma once
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <memory> 

class LanePlotter
{
public:
	// plotter functions 
	cv::Mat plotLane(cv::Mat inputImage, std::vector<cv::Point> lane, const std::string& turn);
};