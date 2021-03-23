#include "Lab1.h"


//using namespace cv;

void Lab1::runLab1() {
    std::cout << "Hello World!\n";
    const Mat pic = imread("D:\\COI\\Lab_0\\house_sad.png");
    imshow("original", pic);
    imshow("hist original ", getHist(pic));
    waitKey();
    int q_levels[] = { 2,4,8,16,32,64 };
    for (int level : q_levels)
    {
        imshow("k = " + std::to_string(level), quantizeIt(pic, level));
        imshow("hist for k = " + std::to_string(level), getHist(quantizeIt(pic, level)));
        std::cout << "sigma " << level << " = " << getSigma(pic, level) << "\n";
        waitKey();
    }
    //Mat result = quantizeIt(pic, 64);
    //std::cout << "sigma 2 = " << getSigma(pic, 2) <<"\n";
    //std::cout << "sigma 4 = " << getSigma(pic, 4) <<"\n";
    //std::cout << "sigma 8 = " << getSigma(pic, 8) <<"\n";
    //std::cout << "sigma 16 = " << getSigma(pic, 16) <<"\n";
    //std::cout << "sigma 32 = " << getSigma(pic, 32) <<"\n";

    //imshow("show", result);
    waitKey();
}

Mat Lab1::quantizeIt(const Mat& image, unsigned int quantScale)
{
    int inter = 255 / (quantScale - 1);

    Mat result = Mat::zeros(image.rows, image.cols, CV_8UC1);

    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++) {
            Vec3b BGR = image.at<Vec3b>(y, x);

            int Y = 0.36 * BGR[2] + 0.53 * BGR[1] + 0.11 * BGR[0];
            for (int k = 0; k < quantScale; k++)
            {
                if (Y > inter * k and Y <= inter * k + inter / 2) {

                    Y = inter * k;
                }
                else if (Y > inter * k + inter / 2 and Y <= inter * (k + 1)) {
                    Y = inter * (k + 1);
                }

            }
            result.at<uchar>(y, x) = Y;
        }
    }
    return result;
}

float Lab1::getSigma(const Mat& image, unsigned int quantScale) {
    int inter = 255 / (quantScale - 1);
    float result = 0;
    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++) {
            Vec3b BGR = image.at<Vec3b>(y, x);

            int I = 0.36 * BGR[2] + 0.53 * BGR[1] + 0.11 * BGR[0];
            int K = 0;
            for (int k = 0; k < quantScale; k++)
            {
                if (I > inter * k and I <= inter * k + inter / 2) {

                    K = inter * k;
                }
                else if (I > inter * k + inter / 2 and I <= inter * (k + 1)) {
                    K = inter * (k + 1);
                }

            }
            result += (I - K) * (I - K);
        }
    }
    std::cout << "sigma_med = " << (inter / sqrt(12)) << " ";
    result = sqrt(result / (image.cols * image.rows));
    return result;
}
Mat Lab1::getHist(const Mat& imageIn)
{
    Mat image;
    imageIn.convertTo(image, CV_8U);
    // Создаем заполненный нулями Mat-контейнер размером 1 x 256
    Mat hist = Mat::zeros(1, 256, CV_64FC1);
    // последовательно считываем яркость каждого элемента изображения
    // и увеличиваем на единицу значение соответствующего элемента матрицы hist
    for (int i = 0; i < image.cols; i++)
        for (int j = 0; j < image.rows; j++) {

            int r = image.at<unsigned char>(j, i);
            hist.at<double>(0, r) = hist.at<double>(0, r) + 1.0;
        }
    double m = 0, M = 0;
    minMaxLoc(hist, &m, &M); // ищем глобальный минимум и максимум
    hist = hist / M; // используем максимум для нормировки по высоте
    Mat hist_img = Mat::zeros(100, 256, CV_8U);
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 100; j++) {
            if (hist.at<double>(0, i) * 100 > j) {
                hist_img.at<unsigned char>(99 - j, i) = 255;
            }
        }
    bitwise_not(hist_img, hist_img); // инвертируем изображение
    return hist_img;
}
