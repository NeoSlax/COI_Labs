
#define M_PI           3.14159265358979323846

#include "Lab1.h"
#include "Lab2.h"

using namespace std;


Lab1 lab1;
Lab2 lab2;

void Lab2::test() {
	
	imshow("original", original);
	Rectangilize(original, 8);
	
	waitKey();
}


void Lab2::Rectangilize(Mat image, int blockSize) {

	
	lab2.blockSize = blockSize;
	for (int row = 0; row < image.rows; row = row + blockSize) {
		for (int col = 0; col < image.cols; col = col + blockSize) {
			Rect block = Rect(col, row, blockSize, blockSize);
			rectangle(lab2.imageWithBlocks, block, cv::Scalar(0, 255, 0));
		}
	}
	imshow("blocked", lab2.imageWithBlocks);
	setMouseCallback("blocked", CallBackFunc);
	
	
}

void CallBackFunc(int event, int x, int y, int flags, void*)
{
	if (event == EVENT_LBUTTONDOWN) {

		int blockX = x - x % lab2.blockSize;
		int blockY = y - y % lab2.blockSize;

		Rect block = Rect(blockX, blockY, lab2.blockSize, lab2.blockSize);
		destroyWindow("blocked");
		Mat imageWithBlocksSelected;
		lab2.imageWithBlocks.copyTo(imageWithBlocksSelected);
		rectangle(imageWithBlocksSelected, block, cv::Scalar(0, 0, 255));
		imshow("blocked", imageWithBlocksSelected);
		setMouseCallback("blocked", CallBackFunc);
		Mat imageBlock;
		lab2.original(block).copyTo(imageBlock);
		imshow("fjfj", imageBlock);
		lab2.dctConverter(imageBlock);
		lab2.dctConverter2(imageBlock);
		
	}
	
	
}

void Lab2::dctConverter2(Mat imageBlock) {
	int blocksize = lab2.blockSize;


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

void Lab2::dctConverter(Mat image)
{
	Mat dct = Mat::zeros(lab2.blockSize, lab2.blockSize, CV_64F);

	float ci, cj, dct1, sum;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {

			if (i == 0)
				ci = 1 / sqrt(dct.cols);
			else
				ci = sqrt(2) / sqrt(dct.cols);
			if (j == 0)
				cj = 1 / sqrt(dct.cols);
			else
				cj = sqrt(2) / sqrt(dct.cols);

			sum = 0;
			for (int k = 0; k < dct.cols; k++) {
				for (int l = 0; l < dct.cols; l++) {
					dct1 = image.at<uchar>(k, l) *
						cos((2 * k + 1) * i * M_PI / (2 * dct.cols)) *
						cos((2 * l + 1) * j * M_PI / (2 * dct.cols));
					sum = sum + dct1;
				}
			}
			dct.at<double>(i, j) = ci * cj * sum;
		}
	}
	std::cout << dct.elemSize1();
	
	resize(dct, dct, cv::Size(), 50, 50, INTER_NEAREST);
	imshow("dct", dct);
	//dct.convertTo(dct, CV_8U);
	imshow("hist", lab1.getHist(dct));

}


