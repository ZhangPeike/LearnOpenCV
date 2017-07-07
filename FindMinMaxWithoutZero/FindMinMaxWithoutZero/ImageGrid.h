#include <opencv2/core/core.hpp>
#include <opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//Divide an image into grids, for every grid get the binary image and merge them
void Divide_Binary_Merge(Mat &OriginalImg, int Row, int Column, int Scale);
 

