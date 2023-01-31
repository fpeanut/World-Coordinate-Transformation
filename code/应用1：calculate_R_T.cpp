#pragma once
#pragma once
#include<fstream>
#include<string>
#include <afx.h>
#include<iostream>
//#include<sstream>
#include<opencv2/opencv.hpp>
#define NULL    0
#define MAX_PATH          260
using namespace cv;
using  namespace std;
int index_1=0, index_2=0,index_3;
int num_r=12;

CString GetSaveFilePath() {
	//获取x64所在文件夹位置
	/*TCHAR  exepath[MAX_PATH];
	CString  strdir, tmpdir;
	memset(exepath, 0, MAX_PATH);
	GetModuleFileName(NULL, exepath, MAX_PATH);
	tmpdir = exepath;
	tmpdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
	tmpdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
	return tmpdir;*/

	//获取x64/debug(realese)文件夹所在位置
	TCHAR  exepath[MAX_PATH];
	CString  strdir;
	memset(exepath, 0, MAX_PATH);
	GetModuleFileName(NULL, exepath, MAX_PATH);
	strdir = exepath;
	strdir = strdir.Left(strdir.ReverseFind('\\'));
	return strdir;
}
//计算rt
void calculate_RT(Mat &R, Mat &T,Mat &camera,Mat &screen) {
	
	int r = camera.rows, c = camera.cols;
	//计算平均中心点向量centroid_A 向量centroid_B
	float sumsx = 0, sumsy = 0, sumsz = 0, sumcx = 0, sumcy = 0, sumcz = 0;
	Mat centroid_A(1, 3, CV_32FC1), centroid_B(1, 3, CV_32FC1), reA(r, c, CV_32FC1), reB(r, c, CV_32FC1), X(r, c, CV_32FC1), Y(r, c, CV_32FC1);
	//计算camera screen每一列的和
	for (int i = 0; i < camera.rows; i++) {
		sumsx += camera.ptr<float>(i)[0];
		sumsy += camera.ptr<float>(i)[1];
		sumsz += camera.ptr<float>(i)[2];

		sumcx += screen.ptr<float>(i)[0];
		sumcy += screen.ptr<float>(i)[1];
		sumcz += screen.ptr<float>(i)[2];
	}
	centroid_A.ptr<float>(0)[0] = sumsx / camera.rows, centroid_A.ptr<float>(0)[1] = sumsy / camera.rows, centroid_A.ptr<float>(0)[2] = sumsz / camera.rows;
	centroid_B.ptr<float>(0)[0] = sumcx / camera.rows, centroid_B.ptr<float>(0)[1] = sumcy / camera.rows, centroid_B.ptr<float>(0)[2] = sumcz / camera.rows;
	
	//repmat(centroid_A, N, 1)
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			reA.ptr<float>(i)[j] = centroid_A.ptr<float>(0)[j];
			reB.ptr<float>(i)[j] = centroid_B.ptr<float>(0)[j];
		}
	}

	//A - repmat(centroid_A, N, 1)
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			X.ptr<float>(i)[j] = camera.ptr<float>(i)[j] - reA.ptr<float>(i)[j];
			Y.ptr<float>(i)[j] = screen.ptr<float>(i)[j] - reB.ptr<float>(i)[j];
		}
	}
	
	//svd奇异值分解
	Mat w, u, v;
	Mat H =(Y.t()*X).t();
	
	SVD::compute(H, w, u, v);
	//求取旋转矩阵R
	R = (u * v).t();
	
	float d1, d2, d3, det;
	d1 = R.at<float>(Point(0, 0))*(R.at<float>(Point(1, 1))*R.at<float>(Point(2, 2)) - R.at<float>(Point(1, 2))*R.at<float>(Point(2, 1)));
	d2 = R.at<float>(Point(0, 1))*(R.at<float>(Point(1, 0))*R.at<float>(Point(2, 2)) - R.at<float>(Point(1, 2))*R.at<float>(Point(2, 0)));
	d3 = R.at<float>(Point(0, 2))*(R.at<float>(Point(1, 0))*R.at<float>(Point(2, 1)) - R.at<float>(Point(1, 1))*R.at<float>(Point(2, 0)));
	det = d1 + d2 + d3;//求行列式
	if (det < 0) {
		for (int k = 0; k < 3; k++) {
			v.at<float>(Point(k, 2)) = -v.at<float>(Point(k, 2));
		}
		R = (u * v).t();
	}
	//求解T
	T = -R * centroid_A.t() + centroid_B.t();
	
	
	
}
//求最优RT
void get_RT(Mat &R, Mat &T, Mat &camera, Mat &screen,Mat &screen_index) {
	
	while (1)
	{
		calculate_RT(R,T,camera,screen);
		Mat _T = Mat(3, camera.rows, CV_32FC1);
		//扩大T到3行5列
		for (int i = 0; i < _T.cols; i++) {
			_T.ptr<float>(0)[i] = T.ptr<float>(0)[0];
			_T.ptr<float>(1)[i] = T.ptr<float>(1)[0];
			_T.ptr<float>(2)[i] = T.ptr<float>(2)[0];
		}
		//计算坐标点的误差
		Mat err = (screen - (R*camera.t() + _T).t());
		
		if (err.rows <=6) {
			std::cout << "error" << endl;
			float sumx=0, sumy=0, sumz=0;
			for (int i = 0; i < err.rows; i++) {
				sumx += fabs(err.ptr<float>(i)[0]);
				sumy += fabs(err.ptr<float>(i)[1]);
				sumz += fabs(err.ptr<float>(i)[2]);
				for (int j = 0; j < err.cols; j++) {
					std::cout << err.ptr<float>(i)[j]<<" ";
				}
				cout << endl;
			}
			cout << "ave_err" << endl;
			cout << sumx / err.rows << "," << sumy / err.rows << "," << sumz / err.rows << endl;
			std::cout << "index" << endl;
			for (int i = 0; i < err.rows; i++) {
				std:; cout << screen_index.ptr<float>(i)[0] <<" "<< screen_index.ptr<float>(i)[1]<<" "<< screen_index.ptr<float>(i)[2]<<" "<<screen_index.ptr<float>(i)[3];
				cout << endl;
			}
			TCHAR nDefault[25];
			string a = "\\config.ini";
			CString dir = GetSaveFilePath();
			LPCSTR path = a.c_str();
			CString allpath = dir + path;

			for (int i = 0; i < err.rows; i++) {
				for (int j = 0; j < err.cols; j++) {
					CString str, str1;
					str.Format(_T("%d"), i + 1);
					str1.Format(_T("%d"), j + 1);

					/*ostringstream oss;
					oss << R.ptr<float>(i)[j];
					string strx1(oss.str());*/

					CString strx1;
					strx1.Format(_T("%f"), err.ptr<float>(i)[j]);
					WritePrivateProfileString(_T("err"), _T("e" + str + str1), strx1, allpath);
				}
			}
			break;
		}
		else
		{	//sum_err = sum(abs(err),2);取绝对值 行求和
			Mat abs_err = abs(err);
			Mat sum_err = Mat::zeros(camera.rows, 1, CV_32FC1);
			for (int i = 0; i < abs_err.rows; i++) {
				for (int j = 0; j < abs_err.cols; j++) {
					sum_err.ptr<float>(i)[0] += abs_err.ptr<float>(i)[j];
				}
			}
			//找出sum_err中的最大值下标索引（噪声最大的点）
			int index = 0;
			float max = 0.0;
			for (int i = 0; i < sum_err.rows; i++) {
				if (sum_err.ptr<float>(i)[0] > max) {
					max = sum_err.ptr<float>(i)[0];
					index = i;
				}
			}
			//删除最大值的行
			Mat dst1,dst2,dst3;
			for (int i = 0; i < camera.rows; i++) {
				if (i != index) {
					dst1.push_back(camera.row(i));
					dst2.push_back(screen.row(i));
					dst3.push_back(screen_index.row(i));
				}
			}
			camera = dst1.clone();
			screen = dst2.clone();
			screen_index = dst3.clone();
			//累计每个平面的点的个数。
		/*	for (int i = 0; i < screen_index.rows; i++) {
				if (screen_index.ptr<float>(i)[3] == 1)
					index_1++;
				else if (screen_index.ptr<float>(i)[3] == 2)
					index_2++;
				else
				{
					index_3++;
				}
			}*/
		}
	}
	//存取rt至config文件
	TCHAR nDefault[25];
	string a = "\\config.ini";
	CString dir = GetSaveFilePath();
	LPCSTR path = a.c_str();
	CString allpath = dir + path;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			CString str, str1;
			str.Format(_T("%d"), i + 1);
			str1.Format(_T("%d"), j + 1);

			CString strx1;
			strx1.Format(_T("%f"), R.ptr<float>(i)[j]);
			WritePrivateProfileString(_T("rational"), _T("R" + str + str1), strx1, allpath);
		}
	}
	for (int i = 0; i < T.rows; i++) {
		CString str2;
		str2.Format(_T("%d"), i + 1);

		/*ostringstream oss;
		oss << T.ptr<float>(i)[0];
		string strx2(oss.str());*/

		CString strx2;
		strx2.Format(_T("%f"), T.ptr<float>(i)[0]);
		WritePrivateProfileString(_T("translation"), _T("T" + str2), strx2, allpath);
	}


}
int main() {
	//读取txt矩阵数据
	float A[12][3] = { 0 };
	float B[12][3] = { 0 };
	ifstream infile1,infile2;
	infile1.open("D:\\Backup\\桌面\\求RT\\camera01.txt");
	infile2.open("D:\\Backup\\桌面\\求RT\\screen01.txt");
	for (int i = 0; i < num_r; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			infile1 >> A[i][j];
			std::cout << A[i][j];
			infile2 >> B[i][j];

		}
	}
	infile1.close();
	infile2.close();
	
	//矩阵转换成Mat矩阵
	Mat camera = Mat(num_r, 3, CV_32FC1);
	Mat screen = Mat(num_r, 3, CV_32FC1);
	Mat screen_index = Mat(num_r, 4, CV_32FC1);
	Mat R = cv::Mat::zeros(3, 3, CV_32FC1);
	Mat T = cv::Mat::zeros(3, 1, CV_32FC1);
	for (int i = 0; i < camera.rows; i++) {
		for (int j = 0; j < camera.cols; j++) {
			camera.ptr<float>(i)[j] = A[i][j];
			screen.ptr<float>(i)[j] = B[i][j];
			screen_index.ptr<float>(i)[j] = B[i][j];
		}
	}
	for (int i = 0; i < camera.rows; i++) {
		/*if (i < 4) {
			screen_index.ptr<float>(i)[3] = 1;
		}
		else if (i >= 4&&i<8) {
			screen_index.ptr<float>(i)[3] = 2;
		}
		else
		{
			screen_index.ptr<float>(i)[3] = 3;
		}*/
		screen_index.ptr<float>(i)[3] = i + 1;
	}
	/*cout << "index" << endl;
	for (int i = 0; i < screen_index.rows; i++) {
		for (int j = 0; j < screen_index.cols; j++) {
			std::cout << screen_index.ptr<float>(i)[j] << " ";
		}
		std::cout << endl;
	}*/
	get_RT(R,T,camera,screen, screen_index);

	//输出
	std::cout << "R=" << endl;
	for (int i = 0; i < R.rows; i++) {
		for (int j = 0; j < R.cols; j++) {
			std::cout << R.ptr<float>(i)[j] << " ";
		}
		std::cout << endl;
	}
	std::cout << "T=" << endl;
	for (int i = 0; i < T.rows; i++) {
		for (int j = 0; j < T.cols; j++) {
			std::cout << T.ptr<float>(i)[j] << " ";
		}
		std::cout << endl;
	}

}