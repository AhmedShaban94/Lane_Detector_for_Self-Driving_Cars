#include "LaneCalculator.h"

// IMAGE BLURRING
cv::Mat LaneCalculator::deNoise() {
	cv::Mat output;
	cv::GaussianBlur(inputImage_, output, cv::Size(5, 5), 0, 0);
	return output;
}

// EDGE DETECTION
cv::Mat LaneCalculator::edgeDetector(cv::Mat img_noise) {
	cv::Mat output;
	// Convert image from RGB to gray
	cv::cvtColor(img_noise, img_noise, cv::COLOR_RGB2GRAY);
	cv::Canny(img_noise, output, 50, 150);
	return output;
}

// MASK THE EDGE IMAGE
cv::Mat LaneCalculator::mask(const cv::Mat img_edges) {
	cv::Mat output;
	cv::Mat mask = cv::Mat::zeros(img_edges.size(), img_edges.type());
	std::vector<cv::Point> points;

	points.emplace_back(cv::Point(210, 720));
	points.emplace_back(cv::Point(550, 450));
	points.emplace_back(cv::Point(717, 450));
	points.emplace_back(cv::Point(1280, 720));

	// Create a binary polygon mask
	cv::fillConvexPoly(mask, points, cv::Scalar(255, 0, 0));
	// Multiply the edges image and the mask to get the output
	cv::bitwise_and(img_edges, mask, output);
	return output;
}

// HOUGH LINES
std::vector<cv::Vec4i> LaneCalculator::houghLines(const cv::Mat img_mask) {
	std::vector<cv::Vec4i> lines;
	// rho and theta are selected by trial and error
	HoughLinesP(img_mask, lines, 1, CV_PI / 180, 20, 20, 30);
	return std::move(lines);
}
