#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "stdio.h"
#include <iostream>

using namespace std;
using namespace cv;

struct Center
{
	Point2d location;
	double radius;
	//int gray;
};


static void help()
{
	cout
		<< "\n--------------------------------------------------------------------------" << endl
		<< "This program shows Canny detector for circles"<<endl
		<< "Usage:" << endl
		<< "ESC: stop" << endl
		<< "c C: capture" << endl
		<< "--------------------------------------------------------------------------" << endl
		<< endl;
}

int main(int argc, char * argv[])
{
	help();
	SimpleBlobDetector::Params SimpleBlobDetectorParams;
// 	SimpleBlobDetectorParams.minThreshold = 40;
// 	SimpleBlobDetectorParams.maxThreshold = 160;
// 	SimpleBlobDetectorParams.thresholdStep = 5;
// 	SimpleBlobDetectorParams.minArea = 30;
// 	SimpleBlobDetectorParams.maxArea = 8000;
// 	SimpleBlobDetectorParams.minConvexity = 0.95;
// 	SimpleBlobDetectorParams.minInertiaRatio = 0.1;
	SimpleBlobDetector detector(SimpleBlobDetectorParams);
	vector<KeyPoint> vKPt;

	int	minDistBetweenBlobs = 10;

	double minArea = 60;
	double maxArea = 5000;

	double minCircularity = 0.8f;
	float maxCircularity = std::numeric_limits<float>::max();

	//minInertiaRatio = 0.6;
	float minInertiaRatio = 0.1f;
	float maxInertiaRatio = std::numeric_limits<float>::max();

	//minConvexity = 0.8;
	float minConvexity = 0.95f;
	float maxConvexity = std::numeric_limits<float>::max();

	Mat img, edges, edges_Gaussian, edges_bi;
	int key = -1;
	//imshow("Test",img);
	cout << "Hello!" << endl;
	//fprintf(stdout,"FPRINTF. \n");

	VideoCapture USB_Cap(0);
	if (!USB_Cap.isOpened())
	{
		fprintf(stderr, "Cannot Open Camera... \n");
	}

	char* Gaussian_Window = "Canny edges Gaussian";
	char* Bilinear_Window = "Canny edges Bilinear";

	char* Track_bar_Threshold = "Threshold";

	//namedWindow(Gaussian_Window, 1);
	//namedWindow(Bilinear_Window, 1);

	//createTrackbar(Track_bar_Threshold, Gaussian_Window, );
	vector<vector<Point>> vvPoint2d;
	vector<Vec4i> hi;
	vector<Center> centers;
	Center center;
	Moments moms;
	double area, hullArea;
	double t;
	double perimeter;
	double ratio;
	double denominator;
	const double eps = 1e-2;
	double temp;
	size_t pointIdx;
	vector<double> dists;
	vector < Point > hull;
	
	USB_Cap >> img;

 	for (;;)
	{
		t = (double)getTickCount();

		detector.detect(img, vKPt);
		t = ( (double)getTickCount()-t) /getTickFrequency()*1000;
		cout << "Original method time: " << t << endl;
		key = waitKey(1);
		USB_Cap >> img;
		imshow("RAW", img);
		centers.clear();

		// 		GaussianBlur(img, edges_Gaussian, Size(7, 7), 3, 3);
		// 		GaussianBlur(img, edges_bi, Size(7, 7), 3, 3);
		// 		//bilateralFilter(img, edges_bi, 5, 3, 3);
		// 
		// 		Canny(edges_Gaussian, edges_Gaussian, 50, 150, 3);
		// 		Canny(edges_bi, edges_bi, 50, 150, 3);
		// 
		// 		imshow(Gaussian_Window, edges_Gaussian);

		
		t = (double)getTickCount();

		GaussianBlur(img, edges_Gaussian, Size(5, 5), 5, 5);
		Canny(edges_Gaussian, edges_Gaussian, 30, 70, 3);
		//imshow(Gaussian_Window, edges_Gaussian);

		findContours(edges_Gaussian, vvPoint2d, hi, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		Mat drawing = Mat::zeros(img.size(), CV_8UC1);
		drawContours(drawing, vvPoint2d, -1, Scalar(255), 1, 8);
		imshow("FindContour", drawing);

		for (size_t contourIdx = 0; contourIdx < vvPoint2d.size(); contourIdx++)
		{
			
			moms = moments(Mat(vvPoint2d[contourIdx]));

			area = moms.m00;
			if (area < minArea || area >= maxArea)
				continue;

			perimeter = arcLength(Mat(vvPoint2d[contourIdx]), true);
			ratio = 4 * CV_PI * area / (perimeter * perimeter);
			if (ratio < minCircularity || ratio >= maxCircularity)
				continue;

			denominator = sqrt(pow(2 * moms.mu11, 2) + pow(moms.mu20 - moms.mu02, 2));
			
			if (denominator > eps)
			{
// 				double cosmin = (moms.mu20 - moms.mu02) / denominator;
// 				double sinmin = 2 * moms.mu11 / denominator;
// 				double cosmax = -cosmin;
// 				double sinmax = -sinmin;
// 
// 				double imin = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;
// 				double imax = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;
// 				ratio = imin / imax;
				temp = moms.mu20 + moms.mu02;
				ratio = (temp - denominator) / (temp + denominator);
			}
			else
			{
				ratio = 1;
			}

			if (ratio < minInertiaRatio || ratio >= maxInertiaRatio)
				continue;

			convexHull(Mat(vvPoint2d[contourIdx]), hull);
			area = contourArea(Mat(vvPoint2d[contourIdx]));
			hullArea = contourArea(Mat(hull));
			ratio = area / hullArea;
			if (ratio < minConvexity || ratio >=maxConvexity)
				continue;

			center.location = Point2d(moms.m10 / moms.m00, moms.m01 / moms.m00);

			//compute blob radius
			{

				dists.clear();
				for (pointIdx = 0; pointIdx < vvPoint2d[contourIdx].size(); pointIdx++)
				{
					dists.push_back(norm(center.location - (Point2d)vvPoint2d[contourIdx][pointIdx]));
				}
				std::sort(dists.begin(), dists.end());
				center.radius = (dists[(dists.size() - 1) / 2] + dists[dists.size() / 2]) / 2.;
				//center.gray = 255.0 - img.at<uchar>(center.location.y, center.location.x);
			}
			centers.push_back(center);

		}
		t = ((double)getTickCount() - t) / getTickFrequency()*1000;
		//fprintf(stdout, "Cost:%03f\n ", time);
		cout << "My new method time: " << t << endl;
		for (size_t i = 0; i < centers.size(); i++)
		{
			//circle(img, centers[i].location, 1, centers[i].gray, 1, 8, 0);
			circle(img, centers[i].location, 1, Scalar(0,0,255), -1, 8, 0);
		}
		imshow("Circle center", img);
		
		if (key == 27)
		{
			fprintf(stdout, "ESC!\n");
			break;
		}
		if (key == 'C' || key == 'c')
		{
			//imwrite("EDGES_B.jpg", edges_bi);
			imwrite("EDGES_G.jpg", edges_Gaussian);
			imwrite("RAW.jpg", img);
			fprintf(stdout, "Capture!\n");
		}
	}
	return 0;
}