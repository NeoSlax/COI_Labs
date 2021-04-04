#include "Lab3.h"

void Lab3::gauss3(const cv::Mat& imgin, cv::Mat& imgout) {
	imgout = cv::Mat::zeros(imgin.size(), CV_8U);
	float k = 36;
	float Fk[3][3] = { { 1,4,1 },{ 4,16,4 },{ 1,4,1 } };

	for (int i = 1; i < imgin.cols - 1; i++)
		for (int j = 1; j < imgin.rows - 1; j++) {
			float conv = 0;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar blurred = imgin.at<uchar>(j + jj, i + ii);
					conv += Fk[ii + 1][jj + 1] * blurred;
				}
			uchar blurred = conv / k;
			imgout.at<uchar>(j, i) = blurred;
		}
}

Mat Lab3::DOG(Mat inputImage) {
	Mat g1, g2, result;
	GaussianBlur(inputImage, g1, Size(1, 1), 0);
	GaussianBlur(inputImage, g2, Size(3, 3), 0, 0);
	//absdiff(g1, g2, result);
	result = g1 - g2;
	return result*3;
}

Mat SobelX(Mat imgin) {

	Mat imgout = cv::Mat::zeros(imgin.size(), CV_8U);
	float k = 36;
	float Fk[3][3] = {-1,0,1,-2,0,2,-1,0,1};

	for (int i = 1; i < imgin.cols - 1; i++)
		for (int j = 1; j < imgin.rows - 1; j++) {
			float conv = 0;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar blurred = imgin.at<uchar>(j + jj, i + ii);
					conv += Fk[ii + 1][jj + 1] * blurred;
				}
			uchar blurred = conv / k;
			imgout.at<uchar>(j, i) = blurred;
		}
	return imgout;

}

void Lab3::run() {
	Mat original = imread("D:\\COI\\Lab_0\\house_sad.png");
	Mat blur = original.clone();
	Mat edgeCanny = original.clone();
	imshow("original", original);
	gauss3(original, blur);
	imshow("gauss3", blur);
	Canny(original, edgeCanny, 50, 200);
	imshow("Canny", edgeCanny);
	imshow("DOG", DOG(original));
	imshow("sobelX", SobelX(original));
	waitKey();
}