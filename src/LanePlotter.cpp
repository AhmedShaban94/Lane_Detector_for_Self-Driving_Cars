#include "LanePlotter.h"

cv::Mat LanePlotter::plotLane(cv::Mat inputImage, std::vector<cv::Point> lane, const std::string& turn)
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
