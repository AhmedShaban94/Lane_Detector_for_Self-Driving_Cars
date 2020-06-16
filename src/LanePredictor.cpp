#include "LanePredictor.h"

// SORT RIGHT AND LEFT LINES
std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>> LanePredictor::classifyLines(const std::vector<cv::Vec4i>& lines, cv::Mat img_edges)
{
	cv::Point ini;
	cv::Point fini;
	double slope_thresh = 0.3;
	std::vector<double> slopes;
	std::vector<cv::Vec4i> selected_lines;
	std::vector<cv::Vec4i> right_lines, left_lines;

	// Calculate the slope of all the detected lines
	for (const auto& line : lines) {
		ini = cv::Point(line[0], line[1]);
		fini = cv::Point(line[2], line[3]);

		// Basic algebra: slope = (y1 - y0)/(x1 - x0)
		double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y)) / (static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);

		// If the slope is too horizontal, discard the line
		// If not, save them  and their respective slope
		if (std::abs(slope) > slope_thresh) {
			slopes.push_back(slope);
			selected_lines.push_back(line);
		}
	}

	// Split the lines into right and left lines
	img_center_ = static_cast<double>((img_edges.cols / 2));
	std::vector<cv::Vec4i>::const_iterator selected_lines_iter;
	std::vector<double>::const_iterator slopes_iter;

	for (selected_lines_iter = selected_lines.begin(), slopes_iter = slopes.begin();
		selected_lines_iter != selected_lines.end(), slopes_iter != slopes.end();
		selected_lines_iter++, slopes_iter++)
	{
		auto selected_line = *selected_lines_iter;
		ini = cv::Point(selected_line[0], selected_line[1]);
		fini = cv::Point(selected_line[2], selected_line[3]);

		// Condition to classify line as left side or right side
		auto slope = *slopes_iter;
		if (slope > 0 && fini.x > img_center_ && ini.x > img_center_) {
			right_lines.push_back(selected_line);
			right_flag_ = true;
		}
		else if (slope < 0 && fini.x < img_center_ && ini.x < img_center_) {
			left_lines.push_back(selected_line);
			left_flag_ = true;
		}
	}

	return std::make_pair(right_lines, left_lines);
}

// REGRESSION FOR LEFT AND RIGHT LINES
std::vector<cv::Point> LanePredictor::regression(const std::pair<std::vector<cv::Vec4i>, std::vector<cv::Vec4i>>& left_right_lines, cv::Mat inputImage) {
	std::vector<cv::Point> output(4);
	cv::Point ini;
	cv::Point fini;
	cv::Point ini2;
	cv::Point fini2;
	cv::Vec4d right_line;
	cv::Vec4d left_line;
	std::vector<cv::Point> right_pts;
	std::vector<cv::Point> left_pts;

	// If right lines are being detected, fit a line using all the init and final points of the lines
	if (right_flag_ == true) {
		for (const auto& line : left_right_lines.first) {
			ini = cv::Point(line[0], line[1]);
			fini = cv::Point(line[2], line[3]);

			right_pts.push_back(ini);
			right_pts.push_back(fini);
		}

		if (right_pts.size() > 0) {
			// The right line is formed here
			cv::fitLine(right_pts, right_line, cv::DIST_L2, 0, 0.01, 0.01);
			right_m_ = right_line[1] / right_line[0];
			right_b_ = cv::Point(right_line[2], right_line[3]);
		}
	}

	// If left lines are being detected, fit a line using all the init and final points of the lines
	if (left_flag_ == true) {
		for (const auto& line : left_right_lines.second)
		{
			ini2 = cv::Point(line[0], line[1]);
			fini2 = cv::Point(line[2], line[3]);

			left_pts.push_back(ini2);
			left_pts.push_back(fini2);
		}

		if (left_pts.size() > 0) {
			// The left line is formed here
			cv::fitLine(left_pts, left_line, cv::DIST_L2, 0, 0.01, 0.01);
			left_m = left_line[1] / left_line[0];
			left_b_ = cv::Point(left_line[2], left_line[3]);
		}
	}

	// One the slope and offset points have been obtained, apply the line equation to obtain the line points
	int ini_y = inputImage.rows;
	int fin_y = 470;

	double right_ini_x = ((ini_y - right_b_.y) / right_m_) + right_b_.x;
	double right_fin_x = ((fin_y - right_b_.y) / right_m_) + right_b_.x;

	double left_ini_x = ((ini_y - left_b_.y) / left_m) + left_b_.x;
	double left_fin_x = ((fin_y - left_b_.y) / left_m) + left_b_.x;

	output[0] = cv::Point(right_ini_x, ini_y);
	output[1] = cv::Point(right_fin_x, fin_y);
	output[2] = cv::Point(left_ini_x, ini_y);
	output[3] = cv::Point(left_fin_x, fin_y);

	return output;
}

// TURN PREDICTION
std::string LanePredictor::predictTurn()
{
	std::string output;
	double vanish_x;
	double thr_vp = 10;

	// The vanishing point is the point where both lane boundary lines intersect
	vanish_x = static_cast<double>(((right_m_ * right_b_.x) - (left_m * left_b_.x) - right_b_.y + left_b_.y) / (right_m_ - left_m));

	// The vanishing points location determines where is the road turning
	if (vanish_x < (img_center_ - thr_vp))
		output = "Left Turn";
	else if (vanish_x > (img_center_ + thr_vp))
		output = "Right Turn";
	else if (vanish_x >= (img_center_ - thr_vp) && vanish_x <= (img_center_ + thr_vp))
		output = "Straight";

	return output;
}