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
			const auto lines = laneDetector.CalculateLane(frame);

			if (!lines.empty())
			{
				// Turn prediction 
				const auto [lane, turn] = laneDetector.PredictLaneTurn(lines, frame);

				//Plotting final frame to be displayed. 
				auto final_frame = laneDetector.plotLaneOnFrame(frame, lane, turn);

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