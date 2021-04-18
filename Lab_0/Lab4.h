#pragma once
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>

using namespace cv;

class Lab4
{
public:
	void run();
	Mat morph(const cv::Mat& input_img);
	Mat erosion(const Mat& input_img);
	
	Mat bin_er(const cv::Mat& input_img);
	Mat bin_dil(const cv::Mat& input_img);

	Mat ht_er(const cv::Mat& input_img);
	Mat ht_dil(const cv::Mat& input_img);


	Mat fcontoursym(const cv::Mat& input_img, int n);

};