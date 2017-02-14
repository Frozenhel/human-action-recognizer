#pragma once

class CVNeural
{
private:
	int inputLayerCount;
	int hiddenLayerCount;
	int outputLayerCount;

	CvANN_MLP_TrainParams trainParams;
	CvANN_MLP * neuralNetwork;

	void prepareDataset(int numOfFrames, int numOfFramesSkipWindow, vector<Action*> actions, cv::Mat * actionData, cv::Mat * classificationData) const;
	void prepareSingleActionData(int numOfFrames, int numOfFramesSkipWindow, Action * action, cv::Mat * actionData, cv::Mat * classificationData) const;
	void trainNeural(cv::Mat trainData, cv::Mat trainClassification) const;
	void testNeural(Action * action, cv::Mat testData, cv::Mat testClassification, bool showVideo) const;
	void showNeuralOutput(CvMat * neuralOutput) const;
	void storeResults(NeuralResult * neuralResult, CvMat * classificationResult, cv::Mat classificator, int row) const;
	void storeNeuralToFile() const;

public:
	CVNeural(int inputNeuronsCount, int hiddenNeuronsCount, int outputNeuronsCount);
	void train(vector<Action*> actions);
	void loadTrained() const;
	void test(vector<Action*> actions) const;
	void renderedTest(Action * action) const;
};