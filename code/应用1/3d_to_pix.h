#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
cv::Point3f screenxyz(cv::Point3f cameraxyz, float translation[1][3], float rotation[3][3]);
cv::Point2f picxy(cv::Point3f screenxyz, float px_width, float px_heigh, float width, float heigh);