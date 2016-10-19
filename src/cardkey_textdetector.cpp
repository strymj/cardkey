#include <ros/ros.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <string.h>
#include <leptonica/allheaders.h>
using namespace std;

int main(int argc, char* argv[])
{
    ros::init(argc,argv,"ocr");
	ros::NodeHandle node_("~");

	cv::namedWindow("text");
	cv::startWindowThread();
	cv::Mat show = cv::imread("moji.png");
	cv::Mat gray;
	cv::cvtColor(show, gray, CV_RGB2GRAY);
	cv::imshow("text", gray);
	cv::waitKey(0);

	char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init("/home/strymj/tesseract/tessdata/tessdata/", "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix *image = pixRead("../moji.png");
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
