#include "ImageGrid.h"
#include "Derivate.h"
#include <iostream>
//#define  DEBUG

/*
int scale = 1;
int delta = 0;
int ddepth = CV_16S;
*/
#define  SCALE 3
void Derivate_Blob_Detector(Mat & grayImg, 	int scale,	int delta,	int ddepth, vector<sCenter> & vcenters)
{
	int	minDistBetweenBlobs = 9;

	double minArea = 20;
	double maxArea = 5000;

	float minCircularity = 0.8f;
	float maxCircularity = std::numeric_limits<float>::max();

	//minInertiaRatio = 0.6;
	float minInertiaRatio = 0.1f;
	float maxInertiaRatio = std::numeric_limits<float>::max();

	//minConvexity = 0.8;
	float minConvexity = 0.95f;
	float maxConvexity = std::numeric_limits<float>::max();

	Mat grad_x, grad_y, abs_grad_x, abs_grad_y, grad, binary, TT;
	vector<vector<Point>> vvPoint2d;
	vector<Vec4i> hi;
	vector<sCenter> centers;
	sCenter center;
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

	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(grayImg, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(grayImg, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	Divide_Binary_Merge(grad, 3, 4, 2.5);

	Scalar tempVal = cv::mean(grad);
	//std::cout <<"Get the mean value:"<< tempVal <<std::endl;
	//cvWaitKey(0);
	//double Threshold = tempVal.val[0];
 	double Threshold = 100;
	//double Threshold = SCALE*tempVal.val[0];
	//std::cout << "T:"<< Threshold << std::endl;
 	//threshold(grad, binary, Threshold, 255, CV_THRESH_BINARY);
	//imshow("Binary", binary);
	findContours(grad, vvPoint2d, hi, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));
	std::cout << "Original N:" <<vvPoint2d.size()<< std::endl;
 	//imshow("1st DERIVATE", grad);
 	
// 	Mat drawing = Mat::zeros(grayImg.size(),CV_8UC1);
// 	drawContours(drawing,contours,-1,Scalar(255),1,80);
// 	imshow("Draw contour", drawing);
	double DistanceIsRepeat = 1.0;
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
		if (ratio < minConvexity || ratio >= maxConvexity)
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
			center.isRepeat = false;
			center.gray = 255.0 - grayImg.at<uchar>(center.location.y, center.location.x);
		}
		centers.push_back(center);
	}

	sCenter tempCenter;
	int N = centers.size();
	for (int i = 0; i < N - 1; i++)
	{
		if (centers[i].isRepeat && i!=0)
		{
			continue;
		}
		else
		{	int CNT = 1;
			tempCenter.radius = 0.0;
			tempCenter.location = Point2d(0.0, 0.0);
			bool isRepeat = false;
			for (int j = i + 1; j < N;j++)
			{
				if (norm(centers[i].location - centers[j].location) < DistanceIsRepeat)
				{
					centers[j].isRepeat = true;
					CNT++; 
					if (isRepeat)
					{
						tempCenter.location = tempCenter.location + centers[j].location;
						tempCenter.radius = tempCenter.radius + centers[j].radius;
					}
					else
					{
						tempCenter.location = centers[j].location + centers[j].location;
						tempCenter.radius = centers[j].radius + centers[j].radius;
						isRepeat = true;
					}
				}
			}
			if (isRepeat)
			{
				tempCenter.location = tempCenter.location*(1.0 / CNT);
				tempCenter.radius = tempCenter.radius / CNT;
				tempCenter.gray = centers[i].gray;
				vcenters.push_back(tempCenter);
			}
			else
			{
				vcenters.push_back(centers[i]);
			}
		}
	}
	std::cout << "My Method Updated N:" << vcenters.size() << std::endl;
}

