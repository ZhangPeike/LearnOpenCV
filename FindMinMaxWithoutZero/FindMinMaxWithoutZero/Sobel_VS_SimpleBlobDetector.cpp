#include "Derivate.h"
#include <sstream>
#include <math.h>
//#include "Auto_Threshold_Otsu.h"

using namespace std;

/*int main(int argc, char *argv[])
{
	vector<KeyPoint> vKpt;
	vector<sCenter> vsCenter;
	Mat grayimg,img_cln,img = imread(argv[1], 1);
	img.copyTo(img_cln);
	imshow("RAW",img);
	cvtColor(img,grayimg,CV_RGB2GRAY);

    //double ATO=getThreshVal_Otsu_8u(grayimg);
	//cout << "ATO: " << ATO << endl;

	int ddepth = CV_16S;
	Derivate_Blob_Detector(grayimg, 1, 0, ddepth, vsCenter);
	for (size_t i = 0; i < vsCenter.size(); i++)
	{
		//circle(img, centers[i].location, 1, centers[i].gray, 1, 8, 0);
		circle(img, vsCenter[i].location, 1, Scalar(0, 0, 255), -1, 8, 0);
	}
	

	imshow("Mine Circle center", img);

	SimpleBlobDetector::Params SimpleBlobDetectorPara;
	SimpleBlobDetector Detector(SimpleBlobDetectorPara);
	Detector.detect(grayimg, vKpt);
	cout << "Original method: " << vKpt.size() << endl;

	for (size_t j = 0; j < vKpt.size(); j++)
	{
		//circle(img, centers[i].location, 1, centers[i].gray, 1, 8, 0);
		circle(img_cln, vKpt[j].pt, 1, Scalar(0, 0, 255), -1, 8, 0);
	}


	imshow("Original Circle center", img_cln);

	waitKey(0);
	return 0;
}*/