#pragma once
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>

using namespace cv;

class Lab3
{
public:
	void run();
	void gauss3(const Mat& input_img, Mat& output_img);
	void gauss5(const Mat& input_img, Mat& output_img);
	Mat DOG(Mat inputImage);
	Mat gauss5(const cv::Mat& imgin);
	Mat ac3(int S, const cv::Mat& imgin);
	Mat mos3(const cv::Mat& imgin);
	Mat med3(const cv::Mat& imgin);

};