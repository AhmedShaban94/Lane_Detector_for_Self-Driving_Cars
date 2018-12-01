///////////////////// Sharpening the image using filter2D of OpenCV ////////////////////


//Note: cv::saturate_cast<type>(): is used to make shure that the pixel will not exceed or under 
// the specified data_type, to prevent overflow of underflow. ex. uchar 0->255, the values above 
//255 will return to 255 and values under 0 will set to 0. 

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp> 


int main()
{
	cv::Mat image, edges; 
	image = cv::imread("coins.jpg", cv::IMREAD_UNCHANGED);  
	cv::resize(image, image, cv::Size(image.size().width / 4, image.size().height / 4)); 
	cv::Canny(image, edges, 150, 500); 
	cv::imshow("coins", edges);  
	cv::waitKey(); 
	return EXIT_SUCCESS;
}