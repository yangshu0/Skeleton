#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/objdetect/objdetect.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include<vector>  
#include<iostream>  
#include<algorithm>  

using std::vector;

vector<int> GetFlags(int a[], int length)
{
	vector<int> vec;
	int neighbour[] = { 1,2,4,8,16,32,64,128,1,2,4,8,16,32,64 };
	for (int i = 0; i<length; i++)
	{
		for (int j = 0; j<8; j++)
		{
			int sum = 0;
			for (int k = j; k <= j + a[i]; k++)
				sum += neighbour[k];
			vec.push_back(sum);
			std::cout << sum << " ";
		}
	}
	std::cout << std::endl;
	return vec;
}

void skeleton(cv::Mat &Input) //Input-binary image  
{
	int a0[] = { 1,2,3,4,5,6 };
	int a1[] = { 2 };
	int a2[] = { 2,3 };
	int a3[] = { 2,3,4 };
	int a4[] = { 2,3,4,5 };
	int a5[] = { 2,3,4,5,6 };

	//A0 2, 3, 4, 5, 6, 7 neighbor sum
	vector<int> A0 = GetFlags(a0, 6);
	//A1, 3 neighbor sum
	vector<int> A1 = GetFlags(a1, 1);
	//A2, 3,4 neighbor sum
	vector<int> A2 = GetFlags(a2, 2);
	//A3, 3,4,5 neighbor sum
	vector<int> A3 = GetFlags(a3, 3);
	//A3, 3,4,5,6 neighbor sum
	vector<int> A4 = GetFlags(a4, 4);
	//A3, 3,4,5,6,7 neighbor sum
	vector<int> A5 = GetFlags(a5, 5);
	vector<cv::Point2i> border;

	bool modify = true;
	int neighbour[3][3] = {
		{ 128,1,2 },
		{ 64,0,4 },
		{ 32,16,8 }
	};
	int row = Input.rows;
	int col = Input.cols;
	
	while (modify)
	{
		modify = false;
		// flag the border Pharse 0  
		for (int m = 1; m < row - 1; ++m)
		{
			for (int n = 1; n < col - 1; ++n)
			{
				int weight = 0;
				for (int j = -1; j <= 1; ++j)
				{
					for (int k = -1; k <= 1; k++)
					{
						weight += neighbour[j + 1][k + 1] * Input.at<uchar>(m + j, n + k);
					}
				}
				if (std::find(A0.begin(), A0.end(), weight) != A0.end())
					border.push_back(cv::Point2i(m, n));
			}
		}

		//Pharse 1  
		vector<cv::Point2i>::iterator first = border.begin();
		while (first != border.end())
		{
			int weight = 0;
			for (int j = -1; j <= 1; ++j)
			{
				for (int k = -1; k <= 1; k++)
				{
					weight += neighbour[j + 1][k + 1] * Input.at<uchar>((*first).x + j, (*first).y + k);
				}
			}
			if (std::find(A1.begin(), A1.end(), weight) != A1.end())
			{
				Input.at<uchar>((*first).x, (*first).y) = 0;
				first = border.erase(first);
			}
			else
				++first;
		}
		//Pharse2  
		first = border.begin();
		while (first != border.end())
		{
			int weight = 0;
			for (int j = -1; j <= 1; ++j)
			{
				for (int k = -1; k <= 1; k++)
				{
					weight += neighbour[j + 1][k + 1] * Input.at<uchar>((*first).x + j, (*first).y + k);
				}
			}
			if (std::find(A2.begin(), A2.end(), weight) != A2.end())
			{
				Input.at<uchar>((*first).x, (*first).y) = 0;
				first = border.erase(first);
			}
			else
				++first;
		}
		//Pharse3  
		first = border.begin();
		while (first != border.end())
		{
			int weight = 0;
			for (int j = -1; j <= 1; ++j)
			{
				for (int k = -1; k <= 1; k++)
				{
					weight += neighbour[j + 1][k + 1] * Input.at<uchar>((*first).x + j, (*first).y + k);
				}
			}
			if (std::find(A3.begin(), A3.end(), weight) != A3.end())
			{
				Input.at<uchar>((*first).x, (*first).y) = 0;
				first = border.erase(first);
			}
			else
				++first;
		}
		//Pharse4  
		first = border.begin();
		while (first != border.end())
		{
			int weight = 0;
			for (int j = -1; j <= 1; ++j)
			{
				for (int k = -1; k <= 1; k++)
				{
					weight += neighbour[j + 1][k + 1] * Input.at<uchar>((*first).x + j, (*first).y + k);
				}
			}
			if (std::find(A4.begin(), A4.end(), weight) != A4.end())
			{
				Input.at<uchar>((*first).x, (*first).y) = 0;
				first = border.erase(first);
			}
			else
				++first;
		}
		//Pharse5  
		first = border.begin();
		while (first != border.end())
		{
			int weight = 0;
			for (int j = -1; j <= 1; ++j)
			{
				for (int k = -1; k <= 1; k++)
				{
					weight += neighbour[j + 1][k + 1] * Input.at<uchar>((*first).x + j, (*first).y + k);
				}
			}
			if (std::find(A5.begin(), A5.end(), weight) != A5.end())
			{
				Input.at<uchar>((*first).x, (*first).y) = 0;
				first = border.erase(first);
				modify = true;
			}
			else
				++first;
		}
		//Pharse6  
		border.clear();
	}

	//re-calculate border
	for (int m = 1; m<row - 1; ++m)
	{
		for (int n = 1; n<col - 1; ++n)
		{
			int weight = 0;
			if (Input.at<uchar>(m, n) == 0)
				continue;
			for (int j = -1; j <= 1; ++j)
			{
				for (int k = -1; k <= 1; k++)
				{
					weight += neighbour[j + 1][k + 1] * Input.at<uchar>(m + j, n + k);
				}
			}
			if (std::find(A0.begin(), A0.end(), weight) != A0.end())
				Input.at<uchar>(m, n) = 0;;
		}
	}

}
int main()
{
	Mat raw = cv::imread("test.jpg");
	Mat image(raw.rows, raw.cols, CV_8UC1);
	
	cvtColor(raw, image, CV_RGB2GRAY);
	Mat binaryImage(image.rows, image.cols, CV_8UC1);
	
	threshold(image, binaryImage, 150, 1, CV_THRESH_BINARY_INV);
	cv::imshow("input", image);

	skeleton(binaryImage);
	unsigned char* data = binaryImage.data;

	for (int p = 0; p<binaryImage.rows; p++)
	{
		for (int q = 0; q<binaryImage.cols; q++)
		{
			if (data[p * binaryImage.cols + q] == 1)
				data[p * binaryImage.cols + q] = 0;
			else
				data[p * binaryImage.cols + q] = 255;
		}
	}
	cv::imshow("output", binaryImage);

	cv::waitKey(0);
	return 0;
}
