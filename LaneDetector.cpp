#include "LaneDetector.h"



LaneDetector::LaneDetector()
{
}


LaneDetector::~LaneDetector()
{
}

cv::Mat LaneDetector::deNoise(const cv::Mat & frame)
{
	cv::Mat deNoised_frame; 
	cv::GaussianBlur(frame, deNoised_frame, cv::Size(3, 3), 0, 0);
	return deNoised_frame; 
}

cv::Mat LaneDetector::edgeDetector(const cv::Mat deNoisedFrame)
{
	cv::Mat edges; 
	cv::Canny(deNoisedFrame, edges, 100, 200); 
	return edges; 
}

cv::Mat LaneDetector::mask(const cv::Mat edges, const double resize_factor)
{
	cv::Mat output; 
	cv::Mat mask = cv::Mat::zeros(edges.size(), edges.type()); 
	std::vector<cv::Point> pts; 
	pts.push_back(cv::Point(210 * resize_factor, 720 * resize_factor));
	pts.push_back(cv::Point(550 * resize_factor, 450 * resize_factor));
	pts.push_back(cv::Point(717 * resize_factor, 450 * resize_factor));
	pts.push_back(cv::Point(1280 * resize_factor, 720 * resize_factor)); 

	// Create a binary polygon mask. 
	cv::fillConvexPoly(mask, pts, cv::Scalar(250, 0, 0));
	// perform bitwise_and on the mask and the edges to create the output. 
	cv::bitwise_and(edges, mask, output); 
	return output;
}
