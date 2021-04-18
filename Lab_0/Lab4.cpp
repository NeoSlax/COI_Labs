#include "Lab4.h"

void Lab4::run() {
	Mat original = imread("D:\\COI\\Lab_0\\house.jpg");
	imshow("Original", original);
	Mat original_grayscale = imread("D:\\COI\\Lab_0\\house.jpg", IMREAD_GRAYSCALE);
	imshow("Original grayscale", original_grayscale);	
	Mat original_wb = Mat::zeros(original.size(), CV_8UC1);
	threshold(original_grayscale, original_wb, 128, 255, THRESH_BINARY);
	imshow("Original banary", original_wb);
	
	imshow("Бинарная эрозия", bin_er(original_wb));
	imshow("Бинарная дилатация", bin_dil(original_wb));
	imshow("Бинарное открытие", bin_dil(bin_er(original_wb)));
	imshow("Бинарное закрытие", bin_er(bin_dil(original_wb)));
	waitKey();
	destroyAllWindows();
	imshow("Полутоновая эрозия", ht_er(original_grayscale));
	imshow("Полутоновая дилатация", ht_dil(original_grayscale));
	imshow("Полутоновое открытие", ht_dil(ht_er(original_grayscale)));
	imshow("Полутоновое закрытие", ht_er(ht_dil(original_grayscale)));
	waitKey();

	
	imshow("Поиск контуров", fcontoursym( original_grayscale, 3));

	imshow("Масштабный морфологический градиент", morph( fcontoursym(original_grayscale, 3)));
	waitKey();
}

Mat Lab4::erosion(const Mat& input_img)
{
	Mat output_img = Mat::zeros(input_img.size(), CV_8U);
	
	for (int i = 1; i < input_img.cols - 1; i++)
		for (int j = 1; j < input_img.rows - 1; j++) {
			uchar pix_value = input_img.at<uchar>(j, i);
			float min = 255;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar Y = input_img.at<uchar>(j + jj, i + ii);
					if (Y < min)
						min = Y;
				}
			output_img.at<uchar>(j, i) = min;
		}
	return output_img;
}

Mat Lab4::bin_er(const cv::Mat& input_img) {
	Mat output_img = input_img.clone();
	for (int i = 1; i < input_img.rows - 1; i++)
		for (int j = 1; j < input_img.cols - 1; j++) {
			int cnt = 0;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					if (input_img.at<uchar>(i - ii, j - jj) == 0)
						cnt++;
				}
			if (cnt > 0)
				output_img.at<uchar>(i, j) = 0;
		}
	return output_img;
}

Mat Lab4::bin_dil(const cv::Mat& input_img) {
	Mat output_img = input_img.clone();
	for (int i = 1; i < input_img.rows - 1; i++)
		for (int j = 1; j < input_img.cols - 1; j++) {
			int cnt = 0;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					if (input_img.at<uchar>(i - ii, j - jj) == 255)
						cnt++;
				}
			if (cnt > 0)
				output_img.at<uchar>(i, j) = 255;
		}
	return output_img;
}

Mat Lab4::ht_er(const cv::Mat& input_img) {
	Mat output_img = cv::Mat::zeros(input_img.size(), CV_8U);
	for (int i = 1; i < input_img.rows - 1; i++)
		for (int j = 1; j < input_img.cols - 1; j++) {
			float min = 255;
			uchar pix_value = input_img.at<uchar>(i, j);
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar Y = input_img.at<uchar>(i - ii, j - jj);
					if (Y < min) min = Y;
				}
			output_img.at<uchar>(i, j) = min;
		}
	return output_img;
}

Mat Lab4::ht_dil(const cv::Mat& input_img) {
	Mat output_img = cv::Mat::zeros(input_img.size(), CV_8U);
	for (int i = 1; i < input_img.rows - 1; i++)
		for (int j = 1; j < input_img.cols - 1; j++) {
			float max = 0;
			uchar pix_value = input_img.at<uchar>(i, j);
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar Y = input_img.at<uchar>(i - ii, j - jj);
					if (Y > max) max = Y;
				}
			output_img.at<uchar>(i, j) = max;
		}
	return output_img;
}


Mat Lab4::fcontoursym(const cv::Mat& input_img, int n) {
	int cycle = 0;
	cv::Mat res_1 = cv::Mat::zeros(input_img.size(), CV_8U);
	cv::Mat prep1 = input_img.clone();
	cv::Mat output_img = input_img.clone();

	while (cycle < n) {
		for (int i = 1; i < prep1.rows - 1; i++)
			for (int j = 1; j < prep1.cols - 1; j++) {
				float min = 255;
				uchar pix_value = prep1.at<uchar>(i, j);
				for (int ii = -1; ii <= 1; ii++)
					for (int jj = -1; jj <= 1; jj++) {
						uchar Y = prep1.at<uchar>(i - ii, j - jj);
						if (Y < min) min = Y;
					}
				res_1.at<uchar>(i, j) = min;
			}
		prep1 = res_1.clone();
		cycle++;
	}

	cycle = 0;
	cv::Mat res_2 = cv::Mat::zeros(input_img.size(), CV_8U);
	cv::Mat prep2 = input_img.clone();

	while (cycle < n) {
		for (int i = 1; i < prep2.rows - 1; i++)
			for (int j = 1; j < prep2.cols - 1; j++) {
				float max = 0;
				uchar pix_value = prep2.at<uchar>(i, j);
				for (int ii = -1; ii <= 1; ii++)
					for (int jj = -1; jj <= 1; jj++) {
						uchar Y = prep2.at<uchar>(i - ii, j - jj);
						if (Y > max) max = Y;
					}
				res_2.at<uchar>(i, j) = max;
			}
		prep2 = res_2.clone();
		cycle++;
	}

	cv::subtract(prep2, prep1, output_img);
	return output_img;
}


Mat Lab4::morph(const cv::Mat& input_img) {
	cv::Mat output_img_1 = cv::Mat::zeros(input_img.size(), CV_8U);
	Mat output_img = cv::Mat::zeros(input_img.size(), CV_8U);
	for (int i = 1; i < input_img.cols - 1; i++)
		for (int j = 1; j < input_img.rows - 1; j++) {
			float max = 0;
			float min = 255;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar Y_min = input_img.at<uchar>(j + jj, i + ii);
					uchar Y_max = input_img.at<uchar>(j + jj, i + ii);

					if (Y_max > max)
						max = Y_max;

					if (Y_min < min)
						min = Y_min;
				}
			output_img_1.at<uchar>(j, i) = (max - min);
		}


	cv::Mat output_img_2_1 = cv::Mat::zeros(input_img.size(), CV_8U);
	for (int i = 2; i < input_img.cols - 2; i++)
		for (int j = 2; j < input_img.rows - 2; j++) {
			float max = 0;
			float min = 255;
			for (int ii = -2; ii <= 2; ii++)
				for (int jj = -2; jj <= 2; jj++) {
					uchar Y_min = input_img.at<uchar>(j + jj, i + ii);
					uchar Y_max = input_img.at<uchar>(j + jj, i + ii);

					if (Y_max > max)
						max = Y_max;

					if (Y_min < min)
						min = Y_min;
				}
			output_img_2_1.at<uchar>(j, i) = (max - min);
		}
	cv::Mat output_img_2 = cv::Mat::zeros(output_img_2_1.size(), CV_8U);
	for (int i = 1; i < output_img_2_1.cols - 1; i++)
		for (int j = 1; j < output_img_2_1.rows - 1; j++) {
			float min = 255;
			for (int ii = -1; ii <= 1; ii++)
				for (int jj = -1; jj <= 1; jj++) {
					uchar Y = output_img_2_1.at<uchar>(j + jj, i + ii);
					if (Y < min)
						min = Y;
				}
			output_img_2.at<uchar>(j, i) = min;
		}

	cv::Mat output_img_3_1 = cv::Mat::zeros(input_img.size(), CV_8U);
	for (int i = 3; i < input_img.cols - 3; i++)
		for (int j = 3; j < input_img.rows - 3; j++) {
			float max = 0;
			float min = 255;
			for (int ii = -3; ii <= 3; ii++)
				for (int jj = -3; jj <= 3; jj++) {
					uchar Y_min = input_img.at<uchar>(j + jj, i + ii);
					uchar Y_max = input_img.at<uchar>(j + jj, i + ii);

					if (Y_max > max)
						max = Y_max;

					if (Y_min < min)
						min = Y_min;
				}
			output_img_3_1.at<uchar>(j, i) = (max - min);
		}

	cv::Mat output_img_3 = cv::Mat::zeros(output_img_3_1.size(), CV_8U);
	for (int i = 2; i < output_img_3_1.cols - 2; i++)
		for (int j = 2; j < output_img_3_1.rows - 2; j++) {
			float min = 255;
			for (int ii = -2; ii <= 2; ii++)
				for (int jj = -2; jj <= 2; jj++) {
					uchar Y = output_img_3_1.at<uchar>(j + jj, i + ii);
					if (Y < min)
						min = Y;
				}
			output_img_3.at<uchar>(j, i) = min;
		}

	cv::Mat output_img_1CV64F;
	output_img_1.convertTo(output_img_1CV64F, CV_64F);
	cv::Mat output_img_2CV64F;
	output_img_2.convertTo(output_img_2CV64F, CV_64F);
	cv::Mat output_img_3CV64F;
	output_img_3.convertTo(output_img_3CV64F, CV_64F);
	cv::Mat SUM;
	SUM = output_img_1CV64F + output_img_2CV64F + output_img_3CV64F;
	SUM = SUM / 3;
	SUM.convertTo(output_img, CV_8U);
	return output_img;
}