#include "LaneDetector.h"

int main()
{
	double resize_factor = 0.75; 
	cv::VideoCapture stream("project_video.mp4");
	cv::Mat frame, deNoisedFrame, edges, mask;
	std::unique_ptr<LaneDetector> laneDetector = std::make_unique<LaneDetector>();

	while (1)
	{
		stream >> frame;
		if (!frame.empty())
		{
			cv::resize(frame, frame, cv::Size(), resize_factor, resize_factor, cv::INTER_LINEAR);
			deNoisedFrame = laneDetector->deNoise(frame);
			edges = laneDetector->edgeDetector(deNoisedFrame);
			mask = laneDetector->mask(edges, resize_factor);
			cv::imshow("video", mask);
		}
		else
			break;
		char c = static_cast<char> (cv::waitKey(25));
		if (c == 27)
			break;
	}
	stream.release();
	cv::destroyAllWindows();
	return EXIT_SUCCESS;
}