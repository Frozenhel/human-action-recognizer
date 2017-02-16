#pragma once
#include "RecognizeMethod.h"

class CVNeural
{
private:
	RecognizeMethod recognizeMethod;
	vector<DailyAction> actionList;

	int inputLayerCount;
	int hiddenLayerCount;
	int outputLayerCount;

	CvANN_MLP_TrainParams trainParams;
	CvANN_MLP * neuralNetwork;

	NeuralResult * completeResult;

	void prepareDataset(int numOfFrames, int numOfFramesSkipWindow, vector<Action*> actions, cv::Mat * actionData, cv::Mat * classificationData) const;
	void prepareSingleActionData(int numOfFrames, int numOfFramesSkipWindow, Action * action, cv::Mat * actionData, cv::Mat * classificationData) const;
	void trainNeural(cv::Mat trainData, cv::Mat trainClassification) const;
	void testNeural(Action * action, cv::Mat testData, cv::Mat testClassification, bool showVideo) const;
	void storeResults(NeuralResult * neuralResult, CvMat * classificationResult, cv::Mat classificator, int row) const;
	void storeNeuralToFile() const;
public:
	CVNeural(int inputNeuronsCount, int hiddenNeuronsCount, int outputNeuronsCount, vector<DailyAction> actionList,  RecognizeMethod recognize_method);
	void train(vector<Action*> actions);
	void loadTrainedFromFile() const;
	void test(vector<Action*> actions);
	void renderedTest(Action * action) const;
};
