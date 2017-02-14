#ifndef CLASS_CFREPRESENTATION
#define CLASS_CFREPRESENTATION

#include "stdafx.h"
using namespace std;

class CFRepresentation
{
private:
	vector<int> cfVec[20];
	int position;

public:
	CFRepresentation();
	void createNewCfMat(vector<cv::Mat> * action_mat, float criteria, int toPass, int posun);
	vector<int> * getCFMat();
};

inline float part(int part1, int part2) 
{
	int adress = 0;
	int noF = 17;
	for (int i = 0; i < part1; i++) adress += noF--;
	return adress += part2 - part1;
}

// ReSharper disable once CppPossiblyUninitializedMember
inline CFRepresentation::CFRepresentation()
{
	position = 0;
	
	//leftThrow = new int[]{part(2,3),part(2,5),part(3,5),part(2,6),};
}

inline void CFRepresentation::createNewCfMat(vector<cv::Mat> * action_mats,float criteria, int toPass, int posun)
{
	cv::Mat row = cv::Mat::zeros(1, 136, CV_8UC1); // prehozene pro jednoduzsi pridavani radku
	cv::Mat CFdeltas = cv::Mat(136, action_mats->size(), CV_8UC1, cv::Scalar(0));

	for (int i = posun; i < action_mats->size(); i++)
	{
		cv::Mat action_mat = action_mats->at(i);
		for (int j = 0; j < 136; j++)
		{
			float suma_theta = 0;
			for (int fr = 0; fr < action_mat.cols; fr++)
			{
				suma_theta += action_mat.at<float>(j, fr);
			}
			suma_theta /= action_mat.cols;
			float sum_delta = 0;
			for (int fr = 0; fr < action_mat.cols; fr++)
			{
				sum_delta += action_mat.at<float>(j, fr) - suma_theta;
			}
			float delta = sqrt((sum_delta / action_mat.cols));
			if (delta > criteria) {
				//cout << "Position:" << j << "sum theta: " << suma_theta << "  Delta: " << delta << endl;
				CFdeltas.at<uchar>(j, i) = 255;
			}

		}
	}

	for (int i = 0; i < CFdeltas.rows; i++)
	{
		int pocet = 0;
		for (int j = 0; j < CFdeltas.cols; j++)
		{
			if (CFdeltas.at<uchar>(i, j) == 255) pocet++;
		}
		if (pocet>=toPass)
		{
			cout << "id: " << i << " pocet " << pocet << endl;
			cfVec[position].push_back(i);
		}
		
	}

	position++; 
}

inline vector<int> * CFRepresentation::getCFMat()
{
	
	return cfVec;
}

#endif




