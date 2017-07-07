#include<opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core.hpp>
#include<iostream>

using namespace std;
using namespace cv;

const int MaxVal = 180;
double Scale = 1.0 / 255;
int threshold_value; int ksize = 1;
Mat src, gray, dst, product;

void trackBarThreshold(int,void*);

void trackBarThreshold(int, void*)
{
	
	//int threshold_value;
	//threshold(gray, dst, threshold_value, 255, THRESH_BINARY);
	//imshow("Window_from_TrackBar", dst);
	
	Size Ksize(ksize, ksize);
	if (threshold_value == 1 || threshold_value == 2)
	{
		ksize = 3;
		cout << 1 << endl;
	}
	else if (threshold_value == 3 || threshold_value == 4)
	{
		ksize = 5;
		cout << 3 << endl;
	}
	if (threshold_value == 5 || threshold_value == 6)
	{
		ksize = 7;
		cout << 5 << endl;
	}
	else if (threshold_value == 7 || threshold_value == 8)
	{
		ksize = 9;
		cout << 7 << endl;
	}
	if (threshold_value == 9 || threshold_value == 10)
	{
		ksize = 9;
		cout << 9 << endl;
	}
	//else 
	//{
	//	ksize = 1;
	//}
	Ksize = Size(ksize,ksize);
	GaussianBlur(src, dst, Ksize, 0, 0, 4);
	imshow("A Lady",dst);
}

int main(int argc, char* argv[])
{
	namedWindow("A Lady", CV_WINDOW_AUTOSIZE);
	int Min=255;
	
	src = imread(argv[1],1);
	GaussianBlur(src, dst, Size(1, 1), 0, 0, 4);
	//threshold(src, dst, 100, 255, CV_THRESH_BINARY);
	imshow("A Lady",dst);
	createTrackbar("Threshold value:", "A Lady", &threshold_value, 10, trackBarThreshold);
#if 0
	cvtColor(src, gray, CV_RGB2GRAY);
	//dst = gray.clone();
	//imshow("A Lady", dst);
	//TrackbarCallback((int)1000);
	threshold(gray, dst, 150, 255, THRESH_BINARY);

	//namedWindow("Binary image", CV_WND_PROP_AUTOSIZE);
	imshow("Binary image", dst);

	int RowNum = dst.rows;
	int ColNum = dst.cols;

#endif // 0

	/*cout << "Row: "<<RowNum<< endl;
	cout << "Pixel Value: "<<(double)dst.at<uchar>(50, 50) << endl;
	multiply(gray, dst, product, Scale, -1);
	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
		{

			//cout << "Pixle:" << (double)dst.at<uchar>(i, j) << endl;
			if ((double)product.at<uchar>(i, j) == 0)
			{
				Min = Min;
				//cout << "zero" << endl;
			}

			else if (Min > (double) product.at<uchar>(i, j))
			{
				Min =(double) product.at<uchar>(i, j);
				//cout << "Updated: " << Min <<endl;
			}

		}

	}

	//cout << "The non-zero min pixel" << Min << endl;
	cout << "The non-zero min pixel: " << Min << endl;*/
	waitKey(0);

}