#pragma once
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h> 

using namespace cv;

class Lab1
{
public:

    Mat getHist(const Mat& image);
    void runLab1();
    Mat quantizeIt(const Mat& image, unsigned int quantScale);
    float getSigma(const Mat& image, unsigned int quantScale);

    
};