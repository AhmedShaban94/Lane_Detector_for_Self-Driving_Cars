#pragma once 
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <memory>

class LaneCalculator
{
public:
	LaneCalculator() = default;
	explicit LaneCalculator(const cv::Mat& input_image)
		: inputImage_(input_image)
	{
	}


	LaneCalculator(const LaneCalculator& other)
		: inputImage_(other.inputImage_)
	{
	}

	LaneCalculator(LaneCalculator&& other) noexcept
		: inputImage_(std::move(other.inputImage_))
	{
	}

	LaneCalculator& operator=(const LaneCalculator& other)
	{
		if (this == &other)
			return *this;
		inputImage_ = other.inputImage_;
		return *this;
	}

	LaneCalculator& operator=(LaneCalculator&& other) noexcept
	{
		if (this == &other)
			return *this;
		inputImage_ = std::move(other.inputImage_);
		return *this;
	}

	virtual ~LaneCalculator() = default;

	cv::Mat deNoise();  // Apply Gaussian blurring to the input Image
	cv::Mat edgeDetector(cv::Mat);  // Filter the image to obtain only edges
	cv::Mat mask(const cv::Mat);  // Mask the edges image to only care about ROI
	std::vector<cv::Vec4i> houghLines(const cv::Mat);  // Detect Hough lines in masked edges image

	// setters and getters
	cv::Mat GetInputImage() const { return inputImage_; }
	void SetInputImage(const cv::Mat source) { inputImage_ = source; }
private:
	cv::Mat inputImage_;
};