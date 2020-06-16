#include "LaneDetector.h"
#include "LanePredictor.h"

int main()
{
	cv::VideoCapture stream("project_video.mp4");
	cv::Mat frame, deNoisedFrame, edges, mask;
	LaneDetector laneDetector;

	if (!stream.isOpened())
		return EXIT_FAILURE;

	while (true)
	{
		stream >> frame;
		if (!frame.empty())
		{
			// to implement set input_image
			laneDetector.calculator_->SetInputImage(frame); //tested

			// Image denoiseing using gaussian filter blurring.   
			deNoisedFrame = laneDetector.calculator_->deNoise(); //tested. 

			// Edge detection using Canny Edge detector.
			edges = laneDetector.calculator_->edgeDetector(deNoisedFrame);

			// masking each frame to focus on ROI (region of interest). [rect polygone]
			mask = laneDetector.calculator_->mask(edges);

			// Detecting Line in each frame using HoughLines (transform). 
			auto lines = laneDetector.calculator_->houghLines(mask);
			//auto lines = laneDetector->CalculateLane(frame);

			if (!lines.empty())
			{
				//Classify line into right/left lines 
				auto right_left_lines = laneDetector.predictor_->classifyLines(lines, frame);

				//Fitting lines into boundary of lane. 
				auto lane = laneDetector.predictor_->regression(right_left_lines, frame);

				//Predicting turn of the car based on slope of lines. 
				auto turn = laneDetector.predictor_->predictTurn();

				//Plotting final frame to be displayed. 
				auto final_frame = laneDetector.plotter_->plotLane(frame, lane, turn);

				//Show final frame. 
				cv::imshow("Lane Detection", final_frame);
			}
		}
		else
			break;
		const char key = static_cast<char> (cv::waitKey(30));
		if (key == 27)
			break;
		else if (key == 'p')
			cv::waitKey(-1);
	}
	stream.release();
	cv::destroyAllWindows();
	return EXIT_SUCCESS;
}