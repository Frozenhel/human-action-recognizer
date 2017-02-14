#ifndef CLASS_DTW_ANALYZER
#define CLASS_DTW_ANALYZER

#include "stdafx.h"

using namespace std;

class DTWAnalyzer
{
private:


public:
	DTWAnalyzer();
	static float calcSimilarity(cv::Mat *T, cv::Mat *S, int z);
	static float calcSimilarityCF(cv::Mat *T, cv::Mat *S, vector<int> * cf);
};

inline float sqr(float x)
{
	return x*x;
}
inline float minW(cv::Mat * W, int i, int j)
{
	if (i == 0 && j == 0) return 0;
	if (i == 0) return W->at<float>(0, j-1);
	if (j == 0) return W->at<float>(i-1, 0);

	float min = W->at<float>(i-1, j-1) < W->at<float>(i - 1, j) ? W->at<float>(i-1, j-1) : W->at<float>(i - 1, j);
	min = W->at<float>(i, j - 1) < min ? W->at<float>(i, j - 1) : min;
	return min;
}

inline float DTWAnalyzer::calcSimilarity(cv::Mat *T, cv::Mat *S,int z)
{
	cv::Mat W = cv::Mat(T->cols, S->cols, CV_32FC1, cv::Scalar(numeric_limits<float>::infinity()));
	float sum=0;
	float distance;
	for (int i = 0; i < T->cols; i++)
	{
		for (int j = 0; j < S->cols; j++)
		{
			sum = 0;
			for (int k = 0; k < 136; k++)
			{
				float Ti = T->at<float>(k, i);
				float Sj = S->at<float>(k, j);
				sum += sqr(Ti - Sj);
			}

			W.at<float>(i, j) = sqrt(sum) + minW(&W, i, j);
		}
	}
	float max = 0;
	float suma = 0;
	//cout << "T S" << endl;

	//cout << "q " << W.at<float>(W.rows - 1, W.cols - 1) << endl;

	cv::Mat matUk = cv::Mat(W.rows, W.cols, CV_8UC1, cv::Scalar(0));
	int x = 0,y = 0;
	while (x != W.rows - 1 || y != W.cols - 1)
	{
		float max = 0;
		matUk.at<uchar>(x, y) = 255;
		if (x == W.rows - 1) y++;
		else if (y == W.cols - 1) x++;
		else if (W.at<float>(x + 1, y) < W.at<float>(x + 1, y + 1) && W.at<float>(x + 1, y) < W.at<float>(x, y + 1)) x++;
		else if (W.at<float>(x, y + 1) < W.at<float>(x + 1, y + 1) && W.at<float>(x, y + 1) < W.at<float>(x + 1, y)) y++;
		else
		{
			x++;
			y++;
		}
	}

	cv::imshow("img" + z, matUk);
	cv::waitKey(1000);
	
	return W.at<float>(W.rows - 1, W.cols - 1);
	cv::Mat dst;
	cv::sortIdx(W, dst, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
	cv::sort(W, W, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);

	//cout << "p " << W.at<float>(W.rows - 1, 0) << endl;
	
	//return W.at<float>(W.rows-1, 0);

	for (int i = 0; i < W.rows; i++)
	{
		suma += W.at<float>(i, 0);
		for (int j = 0; j < W.cols; j++)
		{
			if (W.at<float>(i, j) > max) max = W.at<float>(i, j);
			//cout << W.at<float>(i, j) << " ";
		}
		//cout << endl;
	}
	//cout << "Podobnost2:   " << suma/(W.rows*W.cols) << "   suma:" << suma  << " MxN " << W.rows * W.cols << " rows " << W.rows << "  max:" << max << endl;
	
	//return suma / (W.rows*W.cols);
	return suma / (W.rows*W.cols);
	
}


inline float DTWAnalyzer::calcSimilarityCF(cv::Mat *T, cv::Mat *S,vector<int> * cf)
{
	cv::Mat W = cv::Mat(T->cols, S->cols, CV_32FC1, cv::Scalar(numeric_limits<float>::infinity()));
	float sum = 0;
	float distance;
	int k;
	for (int i = 0; i < T->cols; i++)
	{
		for (int j = 0; j < S->cols; j++)
		{
			sum = 0;
			for (int z = 0; z < cf->size(); z++)
			{
				k = cf->at(z);
				float Ti = T->at<float>(k, i);
				float Sj = S->at<float>(k, j);
				sum += sqr(Ti - Sj);
			}

			W.at<float>(i, j) = sqrt(sum) + minW(&W, i, j);
		}
	}
	/*float max = 0;
	float suma = 0;
	//cout << "T S" << endl;

	cv::Mat dst;
	cv::sortIdx(W, dst, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
	cv::sort(W, W, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
	for (int i = 0; i < W.rows; i++)
	{
		suma += W.at<float>(i, 0);
		for (int j = 0; j < W.cols; j++)
		{
			if (W.at<float>(i, j) > max) max = W.at<float>(i, j);
			//cout << W.at<float>(i, j) << " ";
		}
		//cout << endl;
	}*/
	//cout << "Podobnost2:   " << suma / (W.rows*W.cols) << "   suma:" << suma << " MxN " << W.rows * W.cols << " rows " << W.rows << "  max:" << max << endl;

	return W.at<float>(W.rows - 1, W.cols - 1);

}

#endif
