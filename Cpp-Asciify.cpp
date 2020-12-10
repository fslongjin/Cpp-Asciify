#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include<vector>
#include<iomanip>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<windows.h>
#include<stdio.h>
#include<wingdi.h>
#include<cstdio>
using namespace std;
using namespace cv; //opencv的命名空间

int main()
{

	

	while (true)
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof cfi;
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0; //字宽
		cfi.dwFontSize.Y = 16;//字高
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;//粗细
		//wcscpy_s(cfi.FaceName, L"Raster"); //设置字体，此处设为点阵字体
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

		

		system("cls");
		Mat img; //声明一个保存图像的对象
		string path;
		cout << "\n\n\n\n\n";
		cout << "\t这是一个生成字符画的程序！" << endl;
		cout << endl;
		cout << "\t公众号：灯珑\n\n";
		cout << "\t请将程序最大化，也就是全屏，这样它才能正常运行哦" << endl;
		cout << endl;
		cout << "请把图片拉到这里： ";
		cin >> path;
		img = imread(path); //读取图像，根据图片所在位置填写路径即可

		if (img.empty()) //判断图像文件是否存在
		{
			cout << "请确认图像文件名称是否正确" << endl;
			system("pause");
			continue;
		}

		cout << "请问是否启用直方图均衡化？(y/n) ";
		bool is_equalize = false;
		char eq;
		cin >> eq;
		if (eq == 'y' || eq == 'Y')
			is_equalize = true;

		system("cls");
		const char cc[6] = { ' ','.', '-', '+', '#','@' };


		
		//缩小字符的宽高以呈现更高精度的图像
		cfi.cbSize = sizeof cfi;
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0; //字宽
		cfi.dwFontSize.Y = 1;//字高
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;//粗细
		//wcscpy_s(cfi.FaceName, L"Raster"); //设置字体，此处设为点阵字体
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

		string s = "mode con cols=" + to_string(1920) + " lines=" + to_string(1080);
		const char* cmd = s.c_str();
		system(cmd);
		const int width = 600;//输出字符画的宽度

		int col = img.cols;
		int row = img.rows;

		//设置图像压缩的倍率
		double tmpc = double(col) / double(width);
		double tmpr = double(row) / (double(width) * double(row) / double(col));

		resize(img, img, Size(double(col) / tmpc, double(row) / tmpr));//对图像进行压缩


		Mat gray;
		cvtColor(img, gray, COLOR_BGR2GRAY);//图像灰度化
		if (is_equalize)
			equalizeHist(gray, gray);//直方图均衡化

		const int height = width * double(row) / double(col);

		//获取图像的长宽
		col = gray.cols;
		row = gray.rows;
		

		

		//设置要忽略的像素的步长
		int x_jump = col / width;
		int y_jump = row / height;
		if (x_jump == 0) x_jump = 1;
		if (y_jump == 0) y_jump = 1;

		//根据图像的灰度值输出相应的字符
		for (int i = 0; i < row; i += x_jump)
		{
			for (int j = 0; j < col; j += y_jump)
			{
				try
				{
					if (int(gray.ptr<uchar>(i)[j]) < 43)
					{
						printf("%c ", cc[0]);
					}
					else if (int(gray.ptr<uchar>(i)[j]) < 86)
						printf("%c ", cc[1]);
					else if ((int(gray.ptr<uchar>(i)[j]) < 128))
						printf("%c ", cc[2]);
					else if ((int(gray.ptr<uchar>(i)[j]) < 171))
						printf("%c ", cc[3]);
					else if ((int(gray.ptr<uchar>(i)[j]) < 213))
						printf("%c ", cc[4]);
					else printf("%c ", cc[5]);
				}
				catch (Exception e)
				{
					continue;
				}
			}
			cout << endl;
		}

		
		system("pause");
	}
	
	return 0; //程序结束
}
