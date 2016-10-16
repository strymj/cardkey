#include <ros/ros.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/text.hpp>
using namespace std;
using namespace cv::text;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "picture_joystick_node");
	ros::NodeHandle node_("~");

	cv::namedWindow("text");
	cv::startWindowThread();

	cv::Mat image = cv::imread("moji.png");
	cv::Mat gray;
	cv::cvtColor(image, gray, CV_RGB2GRAY);

	cv::imshow("text", gray);
	cv::waitKey(0);   // require for imshow

	static cv::Ptr<OCRTesseract> ocr = OCRTesseract::create("../tessdata", "eng");

	std::string text;
	std::vector<cv::Rect> boxes;
	std::vector<std::string> words;
	vector<float> confidences;

	// run ocr
	ocr->run(gray, text, &boxes, &words, &confidences);

	// print result
	printf("%s\n", text.c_str());
	cout<<" char      | point      | size       | reliability"<<endl;;
	cout<<"-----------+------------+------------+----------"<<endl;
	for (int i = 0; i < boxes.size(); i++)
	{
		printf("%-10s | (%3d, %3d) | (%3d, %3d) | %f\n",
				words[i].c_str(),
				boxes[i].x, boxes[i].y,
				boxes[i].width, boxes[i].height,
				confidences[i]);
	}
	return 0;
}
