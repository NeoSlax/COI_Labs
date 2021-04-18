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
	return result*15;
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
	imshow("gaus5", gauss5(original));
	imshow("gauss3", blur);
	imshow("Фильтр «Мозаика»", mos3(original));
	imshow("Фильтр апертурный коррекции", ac3(3, original));
	imshow("Медианный фильтр", med3(original));
	Canny(original, edgeCanny, 50, 200);
	imshow("Canny", edgeCanny);
	imshow("DOG", DOG(original));
	imshow("sobelX", SobelX(original));
	waitKey();
}



Mat Lab3::gauss5(const cv::Mat& imgin) {
	Mat imgout = cv::Mat::zeros(imgin.size(), CV_8U);
	float k = 256;
	float Fk[5][5] = { { 1,4,6,4,1 },{ 4,16,24,16,4 },{ 6,16,36,24,6 },{ 4,16,24,16,4 },{ 1,4,6,4,1 } };
	for (int i = 2; i < imgin.cols - 2; i++)
		for (int j = 2; j < imgin.rows - 2; j++) {
			float conv = 0;
			for (int ii = -2; ii <= 2; ii++)
				for (int jj = -2; jj <= 2; jj++) {
					uchar blurred = imgin.at<uchar>(j + jj, i + ii);
					conv += Fk[ii + 2][jj + 2] * blurred;
				}
			uchar blurred = conv / k;
			imgout.at<uchar>(j, i) = blurred;
		}
	return imgout;
}

Mat Lab3::ac3(int S, const cv::Mat& imgin) {
	int X = (100 / S - 1) - 8; // вычисление коэффициента центрального эл-та маски
	float k = 8 + X; // коэффициент нормировки
	float Fk[3][3] = { { -1,-1,-1 },{ -1, X,-1 },{ -1,-1,-1 } };
	Mat imgout = cv::Mat::zeros(imgin.size(), CV_8U);
	for (int i = 1; i < imgin.cols - 1; i++)
		for (int j = 1; j < imgin.rows - 1; j++) {
			float conv = 0;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar aced = imgin.at<uchar>(j + jj, i + ii);
					conv += Fk[ii + 1][jj + 1] * aced;
				}
			uchar aced;
			if (conv / k >= 255) { 
				aced = 255; 
			}
			else {
				aced = conv / k;
			}
			;
			imgout.at<uchar>(j, i) = aced;
		}
	return imgout;
}

Mat Lab3::mos3(const cv::Mat& imgin) {
	Mat imgout = cv::Mat::zeros(imgin.size(), CV_8U);
	for (int i = 1; i < imgin.cols - 2; i += 3)
		for (int j = 1; j < imgin.rows - 2; j += 3) {
			// поиск среднего арифметического значения яркости элементов под маской
			double avg = 0;
			for (int ii = 0; ii <= 2; ii++) {
				for (int jj = 0; jj <= 2; jj++) {
					avg = imgin.at<uchar>(j + jj, i + ii) + avg;
				}
			}
			avg = avg / 9;
			for (int ii = 0; ii <= 2; ii++) {
				for (int jj = 0; jj <= 2; jj++) {
					imgout.at<uchar>(j + jj, i + ii) = avg;
				}
			}

		}
	return imgout;
}

Mat Lab3::med3(const cv::Mat& imgin) {
	Mat imgout = cv::Mat::zeros(imgin.size(), CV_8U);
	for (int i = 1; i < imgin.cols - 1; i++)
		for (int j = 1; j < imgin.rows - 1; j++) {
			// поиск среднего арифметического элементов под маской
			double avg = 0;
			for (int ii = -1; ii <= 1; ii++) {
				for (int jj = -1; jj <= 1; jj++) {
					avg = imgin.at<uchar>(j + jj, i + ii) + avg;
				}
			}
			avg = avg / 9;
			double med = 0;
			double mindiff = 0;
			double mindiffcur = 255;

			// поиск элемента с минимальной разницей между значением яркости и среднеарифметическим значением яркости матрицы 3на3
			for (int ii = -1; ii <= 1; ii++) {
				for (int jj = -1; jj <= 1; jj++) {
					mindiff = abs(imgin.at<uchar>(j + jj, i + ii) - avg);
					if (mindiff < mindiffcur) {
						med = imgin.at<uchar>(j + jj, i + ii);
						mindiffcur = mindiff;
					}
				}
			}

			imgout.at<uchar>(j, i) = med;
		}
	return imgout;
}