#include "ImageGrid.h"
//#include "Auto_Threshold_Otsu.h"

using namespace std;
using namespace cv;

void Divide_Binary_Merge(Mat &OriginalImg, int Row, int Column, int Scale)
{
	Size2i ImgSize = OriginalImg.size();
	int PixelRow = ImgSize.height / Row;
	int PixelCol = ImgSize.width  / Column;
	Mat UpdatedImg = Mat::zeros(OriginalImg.rows, OriginalImg.cols, CV_8UC1);
	// Mat::create(OriginalImg.rows, OriginalImg.cols, CV_8UC1);
	//imshow("Hello", UpdatedImg);
	//UpdatedImg.reshape();
	//Mat temp, UpdatedImg(OriginalImg.size(),0);
	Mat temp,temp1;
	vector<Mat> vMat;
	vector<vector<Mat>> vvMat;
	vvMat.clear();
	for (size_t i = 0; i < Row; i++)
	{
		vMat.clear();
		for (size_t j = 0; j < Column; j++)
		{
			//The subscript is wrong? check 20170620
			//temp = OriginalImg(Range(i*PixelRow, (i + 1)*PixelRow ), Range(j*PixelCol, (j + 1)*PixelCol )).clone();
			//temp = OriginalImg(Range(i*PixelRow, (i + 1)*PixelRow-1), Range(j*PixelCol, (j + 1)*PixelCol-1)).clone();
			temp = OriginalImg.rowRange(i*PixelRow, (i + 1)*PixelRow).colRange(j*PixelCol, (j + 1)*PixelCol).clone();
			vMat.push_back(temp);
		}
		vvMat.push_back(vMat);
	}

	for (size_t m = 0; m < Row; m++)
	{
		for (size_t n = 0; n < Column; n++)
		{
			Scalar meanVal = mean(vvMat[m][n]);
			//double Threshold = Scale*meanVal.val[0];
			//double Threshold = 100;
			double Threshold = Scale*meanVal.val[0];
			//Mat bridge = UpdatedImg(Range(m*PixelRow, (m + 1)*PixelRow), Range(n*PixelCol, (n + 1)*PixelCol));
			//bridge = temp;
			//threshold(vvMat[m][n], temp1, Threshold, 255, CV_THRESH_BINARY);
			threshold(vvMat[m][n], temp1,  0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			//UpdatedImg(Range(m*PixelRow, (m + 1)*PixelRow - 1), Range(n*PixelCol, (n + 1)*PixelCol - 1)) = temp;
			temp1.copyTo(UpdatedImg(Range(m*PixelRow, (m + 1)*PixelRow ), Range(n*PixelCol, (n + 1)*PixelCol)));
		}
	}
	//imshow("Test ImageGrid", UpdatedImg);
// 	for (int i = 0; i < Row; i++)
// 	{
// 		for (int j = 0; j < Column; j++)
// 		{
// 			Rect rect(i*PixelRow, i*PixelRow,50,50);
// 		}
// 	}
 	imshow("Hello 1", UpdatedImg);
	//return UpdatedImg;
	UpdatedImg.copyTo(OriginalImg);
}