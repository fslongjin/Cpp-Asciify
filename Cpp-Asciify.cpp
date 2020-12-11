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
#include<time.h>
//#include<omp.h>
using namespace std;

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

		

		std::system("cls");
		cv::Mat img; //声明一个保存图像的对象
		string path;
		std::cout << "\n\n\n\n\n";
		std::cout<< "**********************************************\n"
				 << "*                                            *\n"
				 << "*               Cpp-Asciify                  *\n"
				 << "*                  V1.1.1                    *\n"
				 << "*                                            *\n"
				 << "*          这是一个生成字符画的程序！        *\n"
				 << "*               公众号：灯珑                 *\n"
				 << "*                                            *\n"
				 << "**********************************************\n";

		std::cout << endl;
		std::cout << "请把图片拉到这里并回车：";
		cin >> path;
		img = cv::imread(path); //读取图像，根据图片所在位置填写路径即可

		if (img.empty()) //判断图像文件是否存在
		{
			std::cout << "请确认图像文件名称是否正确" << endl;
			std::system("pause");
			continue;
		}

		std::cout << "请设置输出图像的宽度（200-4000）：";
		string ipt_width;
		int width = 600;//默认输出字符画的宽度
		cin >> ipt_width;
		if (stoi(ipt_width)<=4000&& stoi(ipt_width)>=200)
		{
			width = stoi(ipt_width);
		}
		else
		{
			std::cout << "输入值非法！，字符画宽度已设为默认值！(600)" << endl;
		}

		std::cout << "请问是否启用直方图均衡化？(y/n) ";
		bool is_equalize = false;
		char eq;
		cin >> eq;
		if (eq == 'y' || eq == 'Y')
			is_equalize = true;

		std::system("cls");

		std::cout << "\n\n\n\n\n";


		time_t start_time, end_time;

		std::cout << "\t正在计算，请稍候！" << endl << endl;
		start_time = clock();

		const char cc[6] = { ' ','.', '-', '+', '#','@' };


		
		/*
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
		std::system(cmd);
		*/
		

		int col = img.cols;
		int row = img.rows;

		//设置图像压缩的倍率
		double tmpc = double(col) / double(width);
		double tmpr = double(row) / (double(width) * double(row) / double(col));

		resize(img, img, cv::Size(double(col) / tmpc, double(row) / tmpr));//对图像进行压缩


		cv::Mat gray;
		cvtColor(img, gray, cv::COLOR_BGR2GRAY);//图像灰度化
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
		vector<string> text;
		for (int i = 0; i < row; i += x_jump)
		{
			text.push_back(" ");
			for (int j = 0; j < col; j += y_jump)
			{
				try
				{
					if (int(gray.ptr<uchar>(i)[j]) < 43)
					{
						text[i] += cc[0];

						//printf("%c ", cc[0]);
					}
					else if (int(gray.ptr<uchar>(i)[j]) < 86)
					{
						text[i] += cc[1];
						//printf("%c ", cc[1]);
					}
					else if ((int(gray.ptr<uchar>(i)[j]) < 128))
					{
						text[i] +=cc[2];
						//printf("%c ", cc[2]);
					}
					else if ((int(gray.ptr<uchar>(i)[j]) < 171))
					{
						text[i] += cc[3];
						//printf("%c ", cc[3]);
					}
					else if ((int(gray.ptr<uchar>(i)[j]) < 213))
					{
						text[i] += cc[4];
						//printf("%c ", cc[4]);
					}
					else
					{
						text[i] += cc[5];
						//printf("%c ", cc[5]);
					}
					text[i] += " ";
				}
				catch (cv::Exception e)
				{
					continue;
				}
			}
			//cout << endl;
		}


		//cout <<text[0]<< endl; 
		
		//输出图像
		int fontface = cv::FONT_HERSHEY_PLAIN;
		double fontscale = 0.1f;
		int thickness = 1;
		cv::Mat image(img.rows*2, img.cols*2, CV_8UC3, cv::Scalar::all(0));
		int baseline = 0;
		


		cv::Size ts = cv::getTextSize(text[0], fontface, fontscale, thickness, &baseline);

		//static omp_lock_t lock;
		//omp_init_lock(&lock); // 初始化互斥锁
//#pragma omp parallel for num_threads(2)
			for (int i = 0; i < row; i++)
			{
				
				double last_x=0,last_y = 0;
				last_y = (double(i) - 1.0) * ts.height;
				double delta_x = ts.height / 2.0;
					for (int j = 0; j < 2 * col; j++)//逐个字符打印的目的是为了保证每个字符占的空间一致
					{
						string tmp;
						tmp.push_back(text[i][j]);
						//cout << tmp << endl;
						cv::Size textsize = cv::getTextSize(tmp, fontface, fontscale, thickness, &baseline);


						cv::Point textorg(last_x, last_y + ts.height);

						//baseline += thickness;
						//omp_set_lock(&lock); //获得互斥器
						putText(image, tmp, textorg, fontface, fontscale, cv::Scalar::all(255), thickness, 8);
						//omp_unset_lock(&lock); //释放互斥器
						last_x += delta_x;
					}
				
					


			}
		
			//omp_destroy_lock(&lock); //销毁互斥器
		

		
		system("mkdir opt");

		
		string folder = "opt/";
		string file_name = to_string(time(0)) + ".jpg";

		

		cv::imwrite(folder+file_name, image);//保存图片


		std::system("cls");
		std::cout << "\n\n\n\n\n";
		std::cout << "\t字符画已成功保存到opt目录下！" << endl<< endl;
		std::cout << "\t文件名：" << file_name << endl << endl;

		end_time = clock();
		/*
		cv::imshow("字符画：", image);
		cv::waitKey(0);
		*/

		cout << "耗时： " << double(end_time - start_time) / 1000.0 << " s" << endl;
		Sleep(2000);
	}
	
	return 0; //程序结束
}
