#pragma once

#include <QtWidgets/QWidget>
#include "ui_Calculate_Zc.h"
#include<opencv2/opencv.hpp>
#include<iostream>
#include<QString>
#include<string>
#include <QFileDialog>
#include<QDebug>
#include <QProcess>
#include<QSettings>
using namespace std;
using namespace cv;


class Calculate_Zc : public QWidget
{
	Q_OBJECT

public:
	Calculate_Zc(QWidget *parent = Q_NULLPTR);
	~Calculate_Zc();
public:
	//参数
	double fx=0, fy=0, u0=0, v0=0;
	float deta_d=0;
	double Zc;

	//图像变量
	cv::Mat frame;
	cv::VideoCapture cap;
	int camid;
	int fps;
	string cam_id_s;
	//Zc距离标定函数
	void ZcInit(Mat &img, double &zc);
	//config参数函数
	void ConfigGet();
public slots:
	void opencam_clicked();
	void closecam_clicked();
	void getdir_clicked();
	void closeui_clicked();
	void Config_init();

private:
	Ui::Calculate_ZcClass *ui;
	
};
