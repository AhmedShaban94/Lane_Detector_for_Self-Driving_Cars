#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "LaneDetector.h"

// IMAGE BLURRING
cv::Mat LaneDetector::deNoise(const cv::Mat inputImage) {
	cv::Mat output;
	cv::GaussianBlur(inputImage, output, cv::Size(5, 5), 0, 0);
	return output;
}

// EDGE DETECTION
cv::Mat LaneDetector::edgeDetector(cv::Mat img_noise) {
	cv::Mat output;
	// Convert image from RGB to gray
	cv::cvtColor(img_noise, img_noise, cv::COLOR_RGB2GRAY);
	cv::Canny(img_noise, output, 50, 150);
	return output;
}

// MASK THE EDGE IMAGE
cv::Mat LaneDetector::mask(const cv::Mat img_edges) {
	cv::Mat output;
	cv::Mat mask = cv::Mat::zeros(img_edges.size(), img_edges.type());
	std::vector<cv::Point> points;

	points.push_back(cv::Point(210, 720));
	points.push_back(cv::Point(550, 450));
	points.push_back(cv::Point(717, 450));
	points.push_back(cv::Point(1280, 720));

	// Create a binary polygon mask
	cv::fillConvexPoly(mask, points, cv::Scalar(255, 0, 0));
	// Multiply the edges image and the mask to get the output
	cv::bitwise_and(img_edges, mask, output);
	return output;
}

// HOUGH LINES
std::vector<cv::Vec4i> LaneDetector::houghLines(const cv::Mat img_mask) {
	std::vector<cv::Vec4i> line;
	// rho and theta are selected by trial and error
	HoughLinesP(img_mask, line, 1, CV_PI / 180, 20, 20, 30);
	return line;
}

std::vector<cv::Vec4i> LaneDetector::CalculateLane(const cv::Mat frame)
{
	cv::Mat deNoisedFrame, edges, mask;

	calculator_->SetInputImage(frame); //tested
			// Image denoiseing using gaussian filter blurring.  
			//deNoisedFrame = laneDetector->deNoise(frame); //tested. 
	deNoisedFrame = calculator_->deNoise(); //tested. 
	// Edge detection using Canny Edge detector.
	edges = calculator_->edgeDetector(deNoisedFrame);
	// masking each frame to focus on ROI (region of interest). [rect polygone]
	mask = calculator_->mask(edges);
	//// Detecting Line in each frame using HoughLines (transform). 
	auto lines = calculator_->houghLines(mask);
	return lines;
}

//// SORT RIGHT AND LEFT LINES
//std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>> LaneDetector::classifyLines(const std::vector<cv::Vec4i>& lines, cv::Mat img_edges)
//{
//	cv::Point ini;
//	cv::Point fini;
//	double slope_thresh = 0.3;
//	std::vector<double> slopes;
//	std::vector<cv::Vec4i> selected_lines;
//	std::vector<cv::Vec4i> right_lines, left_lines;
//
//	// Calculate the slope of all the detected lines
//	for (const auto& line : lines) {
//		ini = cv::Point(line[0], line[1]);
//		fini = cv::Point(line[2], line[3]);
//
//		// Basic algebra: slope = (y1 - y0)/(x1 - x0)
//		double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y)) / (static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);
//
//		// If the slope is too horizontal, discard the line
//		// If not, save them  and their respective slope
//		if (std::abs(slope) > slope_thresh) {
//			slopes.push_back(slope);
//			selected_lines.push_back(line);
//		}
//	}
//
//	// Split the lines into right and left lines
//	img_center = static_cast<double>((img_edges.cols / 2));
//	std::vector<cv::Vec4i>::const_iterator selected_lines_iter;
//	std::vector<double>::const_iterator slopes_iter;
//
//	for (selected_lines_iter = selected_lines.begin(), slopes_iter = slopes.begin();
//		selected_lines_iter != selected_lines.end(), slopes_iter != slopes.end();
//		selected_lines_iter++, slopes_iter++)
//	{
//		auto selected_line = *selected_lines_iter;
//		ini = cv::Point(selected_line[0], selected_line[1]);
//		fini = cv::Point(selected_line[2], selected_line[3]);
//
//		// Condition to classify line as left side or right side
//		auto slope = *slopes_iter;
//		if (slope > 0 && fini.x > img_center && ini.x > img_center) {
//			right_lines.push_back(selected_line);
//			right_flag = true;
//		}
//		else if (slope < 0 && fini.x < img_center && ini.x < img_center) {
//			left_lines.push_back(selected_line);
//			left_flag = true;
//		}
//	}
//
//	return std::make_pair(right_lines, left_lines);
//}
//
//// REGRESSION FOR LEFT AND RIGHT LINES
//std::vector<cv::Point> LaneDetector::regression(const std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>>& left_right_lines, cv::Mat inputImage) {
//	std::vector<cv::Point> output(4);
//	cv::Point ini;
//	cv::Point fini;
//	cv::Point ini2;
//	cv::Point fini2;
//	cv::Vec4d right_line;
//	cv::Vec4d left_line;
//	std::vector<cv::Point> right_pts;
//	std::vector<cv::Point> left_pts;
//
//	// If right lines are being detected, fit a line using all the init and final points of the lines
//	if (right_flag == true) {
//		for (const auto& line : left_right_lines.first) {
//			ini = cv::Point(line[0], line[1]);
//			fini = cv::Point(line[2], line[3]);
//
//			right_pts.push_back(ini);
//			right_pts.push_back(fini);
//		}
//
//		if (right_pts.size() > 0) {
//			// The right line is formed here
//			cv::fitLine(right_pts, right_line, cv::DIST_L2, 0, 0.01, 0.01);
//			right_m = right_line[1] / right_line[0];
//			right_b = cv::Point(right_line[2], right_line[3]);
//		}
//	}
//
//	// If left lines are being detected, fit a line using all the init and final points of the lines
//	if (left_flag == true) {
//		for (const auto& line : left_right_lines.second)
//		{
//			ini2 = cv::Point(line[0], line[1]);
//			fini2 = cv::Point(line[2], line[3]);
//
//			left_pts.push_back(ini2);
//			left_pts.push_back(fini2);
//		}
//
//		if (left_pts.size() > 0) {
//			// The left line is formed here
//			cv::fitLine(left_pts, left_line, cv::DIST_L2, 0, 0.01, 0.01);
//			left_m = left_line[1] / left_line[0];
//			left_b = cv::Point(left_line[2], left_line[3]);
//		}
//	}
//
//	// One the slope and offset points have been obtained, apply the line equation to obtain the line points
//	int ini_y = inputImage.rows;
//	int fin_y = 470;
//
//	double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
//	double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;
//
//	double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
//	double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;
//
//	output[0] = cv::Point(right_ini_x, ini_y);
//	output[1] = cv::Point(right_fin_x, fin_y);
//	output[2] = cv::Point(left_ini_x, ini_y);
//	output[3] = cv::Point(left_fin_x, fin_y);
//
//	return output;
//}
//
//// TURN PREDICTION
//std::string LaneDetector::predictTurn()
//{
//	std::string output;
//	double vanish_x;
//	double thr_vp = 10;
//
//	// The vanishing point is the point where both lane boundary lines intersect
//	vanish_x = static_cast<double>(((right_m * right_b.x) - (left_m * left_b.x) - right_b.y + left_b.y) / (right_m - left_m));
//
//	// The vanishing points location determines where is the road turning
//	if (vanish_x < (img_center - thr_vp))
//		output = "Left Turn";
//	else if (vanish_x > (img_center + thr_vp))
//		output = "Right Turn";
//	else if (vanish_x >= (img_center - thr_vp) && vanish_x <= (img_center + thr_vp))
//		output = "Straight";
//
//	return output;
//}

// PLOT RESULTS
cv::Mat LaneDetector::plotLane(cv::Mat inputImage, std::vector<cv::Point> lane, std::string turn)
{
	cv::Mat output;

	// Create the transparent polygon for a better visualization of the lane
	inputImage.copyTo(output);
	const std::vector<cv::Point> poly_points{ lane[2], lane[0], lane[1], lane[3] };
	cv::fillConvexPoly(output, poly_points, cv::Scalar(0, 255, 0), cv::LINE_AA, 0);
	cv::addWeighted(output, 0.3, inputImage, 0.7, 0, inputImage);

	// Plot both lines of the lane boundary
	cv::Point center_point(inputImage.size().width / 2, inputImage.size().height / 2);
	cv::line(inputImage, lane[0], lane[1], cv::Scalar(0, 0, 255), 5, cv::LINE_AA);
	cv::line(inputImage, lane[2], lane[3], cv::Scalar(0, 0, 255), 5, cv::LINE_AA);
	//cv::arrowedLine(inputImage, )
	// Plot the turn message
	cv::putText(inputImage, turn, cv::Point(50, 90), cv::FONT_HERSHEY_SIMPLEX
		, 3, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);

	// Show the final output image
	return inputImage;
}

void LaneDetector::SetInputImage(const cv::Mat source)
{
	input_image_.release();
	input_image_ = source.clone();
}
