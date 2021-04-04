#include <iostream>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <cmath>
#include "windows.h"
#include "Lab2.h"
#include "Lab3.h"

cv::Mat img_start = cv::imread("D:\\COI\\Lab_0\\house_sad.png");
cv::Mat imageWithBlocks = img_start.clone();
cv::Mat imageWithBlocks2 = img_start.clone();
int blocksize = 8;


cv::Mat setka(int sizeblock, const cv::Mat& image)
{
    for (int row = 0; row < img_start.rows; row = row + blocksize)
        for (int col = 0; col < img_start.cols; col = col + blocksize)
        {
            cv::Rect blocc = cv::Rect(col, row, blocksize, blocksize);
            rectangle(imageWithBlocks, blocc, cv::Scalar(0, 255, 0));
        }
    return imageWithBlocks;

}

static void onMouse(int event, int x, int y, int, void*)
{
    if (event != cv::EVENT_LBUTTONDOWN)
        return;

    int blockX = 0, blockY = 0;
    blockX = x - x % blocksize;
    blockY = y - y % blocksize;

    cv::Rect block = cv::Rect(blockX, blockY, blocksize, blocksize);
    imageWithBlocks2 = imageWithBlocks.clone();
    rectangle(imageWithBlocks2, block, cv::Scalar(255, 0, 0));
    cv::imshow("Blocked image", imageWithBlocks2);
    cv::Mat imageBlock = img_start(block);

    cv::Mat iB = imageBlock.clone();
    cv::resize(iB, iB, cv::Size(iB.rows * 32, iB.cols * 32), 0, 0, cv::INTER_NEAREST);

    cv::imshow("Выделение блока", iB);

    cv::Mat cb = cv::Mat::zeros(blocksize, blocksize, CV_8U);
    for (int i = 0; i < blocksize; i++)
        for (int j = 0; j < blocksize; j++)
        {
            uchar b = imageBlock.at<cv::Vec3b>(i, j)[0];
            uchar g = imageBlock.at<cv::Vec3b>(i, j)[1];
            uchar r = imageBlock.at<cv::Vec3b>(i, j)[2];
            cb.at<uchar>(i, j) = 0.11 * b + 0.53 * g + 0.36 * r;
        }

    cv::Mat basisMat = cv::Mat::zeros(blocksize, blocksize, CV_64F);
    double N = 8;
    for (int i = 0; i < blocksize; i++)
        for (int j = 0; j < blocksize; j++)
        {
            if (i == 0)
            {
                basisMat.at<double>(i, j) = 1 / sqrt(N);
            }
            else
            {
                basisMat.at<double>(i, j) = sqrt(2 / N) * cos(((3.1415 * i) / N) * (j + 0.5));
            }
        }
    cv::Mat basisMatrixTr = basisMat.t();
    cv::Mat imageBlockCV64F;
    cb.convertTo(imageBlockCV64F, CV_64F);
    cv::Mat A = basisMatrixTr * imageBlockCV64F * basisMat;
    cv::Mat ADCT = A;
    ADCT.convertTo(ADCT, CV_8U);
    cv::Mat ADCT_r = ADCT.clone();
    cv::resize(ADCT_r, ADCT_r, cv::Size(ADCT_r.rows * 32, ADCT_r.cols * 32), 0, 0, cv::INTER_NEAREST);
    cv::imshow("ADCT", ADCT_r);

}


int main()
{
    Lab3 lab3;
    lab3.run();

    
}
