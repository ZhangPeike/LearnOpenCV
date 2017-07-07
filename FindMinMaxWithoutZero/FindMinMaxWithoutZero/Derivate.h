#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;
struct sCenter
{
	Point2d location;
	double radius;
	int gray;
	bool isRepeat;
};

void Derivate_Blob_Detector(Mat & grayImg, int scale, int delta, int ddepth, vector<sCenter> & vcenters);