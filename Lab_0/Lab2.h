#pragma once

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>

using namespace cv;

void CallBackFunc(int event, int x, int y, int flags, void*);

class Lab2 {

	
	
	
public:
	Mat original = imread("D:\\COI\\Lab_0\\house_sad4.png");
	Mat imageWithBlocks = imread("D:\\COI\\Lab_0\\house_sad4.png");

	int blockSize = 0;
	void dctConverter(Mat image);
	void dctConverter2(Mat image);
 	void test();
	void Rectangilize(Mat image, int blockSize);
	


};

