#include "LaneDetector.h"

int main()
{
	double resize_factor = 0.75; 
	cv::VideoCapture stream("project_video.mp4");
	cv::Mat frame, deNoisedFrame, edges, mask;
	std::unique_ptr<LaneDetector> laneDetector = std::make_unique<LaneDetector>();

	if (!stream.isOpened())
		return EXIT_FAILURE;

	while (1)
	{
		stream >> frame;
		if (!frame.empty())
		{
			// Image denoiseing using gaussian filter blurring.  
			deNoisedFrame = laneDetector->deNoise(frame); //tested. 
			// Edge detection using Canny Edge detector.
			edges = laneDetector->edgeDetector(deNoisedFrame);
			// masking each frame to focus on ROI (region of interest). [rect polygone]
			mask = laneDetector->mask(edges);
			//// Detecting Line in each frame using HoughLines (transform). 
			auto lines = laneDetector->houghLines(mask); 
			
			if (!lines.empty())
			{
				//Classify line into right/left lines 
				auto right_left_lines = laneDetector->classifyLines(lines, edges); 
				//Fitting lines into boundary of lane. 
				auto lane = laneDetector->regression(right_left_lines, frame); 
				//Predicting turn of the car based on slope of lines. 
				auto turn = laneDetector->predictTurn(); 
				//Plotting final frame to be displayed. 
				auto final_frame = laneDetector->plotLane(frame, lane, turn);
				//Show final frame. 
				cv::imshow("Lane Detection", final_frame); 
			}
		}
		else
			break;
		char c = static_cast<char> (cv::waitKey(30));
		if (c == 27)
			break;
	}
	stream.release();
	cv::destroyAllWindows();
	return EXIT_SUCCESS;
}