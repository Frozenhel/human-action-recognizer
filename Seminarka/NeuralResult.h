#pragma once

using namespace std;

class NeuralResult
{
private:
	int correct;
	int wrong;
	int correctAvg;
	int wrongAvg;

	cv::Mat resultMat;
	cv::Mat avgResultMat;

	void initMats(int numOfColumns);
	void checkResult(cv::Mat classificator, int row);
public:
	NeuralResult();

	int getNumOfCorrect() const;
	int getNumOfWrong() const;
	int getNumOfAvgCorrect() const;
	int getNumOfAvgWrong() const;
	int getNumOfResults() const;
	float getCorrectPercentages() const;
	float getWrongPercentages() const;
	void printResults() const;
	void printNeuralValuesAtFrame(int frame) const;
	void storeResultRow(CvMat * resultRow, cv::Mat classificator, int row);

	cv::Mat getResultMat() const;
	cv::Mat getAvgResultMat() const;

	int getLastResult() const;
	int getLastAvgResult() const;
};

inline NeuralResult::NeuralResult()
{
	correct = 0;
	wrong = 0;
	correctAvg = 0;
	wrongAvg = 0;
}

inline int NeuralResult::getNumOfCorrect() const
{
	return correct;
}

inline int NeuralResult::getNumOfWrong() const
{
	return wrong;
}

inline int NeuralResult::getNumOfAvgCorrect() const
{
	return correctAvg;
}

inline int NeuralResult::getNumOfAvgWrong() const
{
	return wrongAvg;
}

inline int NeuralResult::getNumOfResults() const
{
	return correct + wrong;
}

inline float NeuralResult::getCorrectPercentages() const
{
	return static_cast<float>(correct) / getNumOfResults();
}

inline float NeuralResult::getWrongPercentages() const
{
	return static_cast<float>(wrong) / getNumOfResults();
}

inline void NeuralResult::printResults() const
{
	cout << "Correct answers: " << correct << "/" << getNumOfResults() << endl;
	cout << "Wrong answers: " << wrong << "/" << getNumOfResults() << endl;

	cout << "Correct avg answers: " << correctAvg << "/" << getNumOfResults() << endl;
	cout << "Wrong avg answers: " << wrongAvg << "/" << getNumOfResults() << endl;
}

inline void NeuralResult::printNeuralValuesAtFrame(int frame) const
{
	cout << endl;
	cout << "Neural results at frame " << frame << endl;

	cout << "Neural output" << endl;
	for (auto i = 0; i < resultMat.cols; i++)
	{
		cout << resultMat.at<float>(frame, i) << "\t";
	}
	cout << endl;

	cout << "Avg results" << endl;
	for (auto i = 0; i < avgResultMat.cols; i++)
	{
		cout << avgResultMat.at<float>(frame, i) << "\t";
	}
	cout << endl;
}

inline cv::Mat createAvgResultRow(cv::Mat resultMat)
{
	cv::Mat avgResultRow = cv::Mat::zeros(1, resultMat.cols, CV_32FC1);
	for (auto begin = resultMat.rows - 15; begin < resultMat.rows; begin++)
	{
		if (begin < 0) continue;
		float value;
		for (auto i = 0; i < resultMat.cols; i++)
		{
			value = resultMat.at<float>(begin, i);
			if (value < 0) value = 0;
			avgResultRow.at<float>(0, i) += value;
		}
	}

	float sum = 0;
	for (auto i = 0; i < avgResultRow.cols; i++)
	{
		sum += avgResultRow.at<float>(0, i);
	}

	for (auto i = 0; i < avgResultRow.cols; i++)
	{
		avgResultRow.at<float>(0, i) /= sum;
	}

	return avgResultRow;
}

inline void NeuralResult::initMats(int numOfColumns)
{
	resultMat = cv::Mat::zeros(0, numOfColumns, CV_32FC1);
	avgResultMat = cv::Mat::zeros(0, numOfColumns, CV_32FC1);
}

inline void NeuralResult::checkResult(class cv::Mat classificator, int row)
{
	if (classificator.at<float>(row, getLastResult()) == 1)
	{
		correct++;
	}
	else
	{
		wrong++;
	}

	if (classificator.at<float>(row, getLastAvgResult()) == 1)
	{
		correctAvg++;
	}
	else
	{
		wrongAvg++;
	}
}

inline void NeuralResult::storeResultRow(CvMat * resultRow, cv::Mat classificator, int row)
{
	if (resultMat.empty())
		initMats(resultMat.cols);

	auto resultRowMat = cv::cvarrToMat(resultRow);

	resultMat.push_back(resultRowMat);
	avgResultMat.push_back(createAvgResultRow(resultMat));

	checkResult(classificator, row);

	printf("Testing Sample %i -> result AVG: %d, result nonAVG: %d \n", row, getLastAvgResult(), getLastResult());
}

inline cv::Mat NeuralResult::getResultMat() const
{
	return resultMat;
}

inline cv::Mat NeuralResult::getAvgResultMat() const
{
	return avgResultMat;
}

inline int NeuralResult::getLastResult() const
{
	float max = 0;
	auto pos = 0;

	for (auto i = 0; i < resultMat.cols; i++)
	{
		if (max < resultMat.at<float>(resultMat.rows - 1, i))
		{
			max = resultMat.at<float>(resultMat.rows - 1, i);
			pos = i;
		}
	}
	return pos;
}

inline int NeuralResult::getLastAvgResult() const
{
	float max = 0;
	auto pos = 0;

	for (auto i = 0; i < avgResultMat.cols; i++)
	{
		if (max < avgResultMat.at<float>(avgResultMat.rows - 1, i))
		{
			max = avgResultMat.at<float>(avgResultMat.rows - 1, i);
			pos = i;
		}
	}
	return pos;
}
