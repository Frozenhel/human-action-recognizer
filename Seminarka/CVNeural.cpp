#include "stdafx.h"

CVNeural::CVNeural(int inputNeuronsCount, int hiddenNeuronsCount, int outputNeuronsCount)
{
	trainParams = CvANN_MLP_TrainParams();
	neuralNetwork = new CvANN_MLP();

	inputLayerCount = inputNeuronsCount;
	hiddenLayerCount = hiddenNeuronsCount; 
	outputLayerCount = outputNeuronsCount;
}

void CVNeural::train(vector<Action*> actions)
{
	cout << "Neural train START" << endl;

	cv::Mat training_data = cv::Mat::zeros(0, inputLayerCount, CV_32FC1);						// 0 rows - 136(pocet uhlu) columns
	cv::Mat training_classifications = cv::Mat(0, outputLayerCount, CV_32FC1);		// 0 rows - pocet akci columns - format (0,0,0,1,0,0,0)

	int layers_d[] = { inputLayerCount, hiddenLayerCount, outputLayerCount };		// neural 136 input -> 10 hidden -> output according to actions
	CvMat * layers = cvCreateMatHeader(1, 3, CV_32SC1);
	cvInitMatHeader(layers, 1, 3, CV_32SC1, layers_d);

	neuralNetwork->create(layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1);

	trainParams = CvANN_MLP_TrainParams(
		// terminate train after 1000 samples or very small change
		cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0),
		// use backpropagation for training
		CvANN_MLP_TrainParams::BACKPROP,
		0.1,
		0.1);

	// bude se pocitat diff uhlu po urcitem poctu snimku, podle toho neuronka "snad" urci, jaky pohyb byl delan

	cout << "Neural preparing data" << endl;

	prepareDataset(15, 5, actions, &training_data, &training_classifications);

	cout << "Neural training" << endl;

	trainNeural(training_data, training_classifications);

	cout << "Storing neural network" << endl;

	storeNeuralToFile();

	cout << "Neural train END" << endl;
}

void CVNeural::prepareDataset(int numOfFrames,int numOfFramesSkipWindow, vector<Action*> actions, cv::Mat * actionData, cv::Mat * classificationData) const
{
	Action *action;

	for (int i = 0; i < actions.size(); i++)
	{
		action = actions.at(i);
		prepareSingleActionData(numOfFrames, numOfFramesSkipWindow, action, actionData, classificationData);
	}
}

void CVNeural::prepareSingleActionData(int numOfFrames, int numOfFramesSkipWindow, Action * action, cv::Mat * actionData, cv::Mat * classificationData) const
{
	for (int j = 0; j < action->getAnglesMatrix().rows - numOfFrames; j += numOfFramesSkipWindow)
	{
		cv::Mat dataRow = cv::Mat::zeros(1, inputLayerCount, CV_32FC1);
		cv::Mat classificationRow = cv::Mat::zeros(1, outputLayerCount, CV_32FC1);

		for (int angleId = 0; angleId < NOFP; angleId++)
		{
			dataRow.at<float>(0, angleId) = action->getAnglesMatrix().at<float>(j, angleId) - action->getAnglesMatrix().at<float>(j + numOfFrames, angleId);

		}
		actionData->push_back(dataRow);

		classificationRow.at<float>(0, action->getActionId()) = 1;
		classificationData->push_back(classificationRow);
	}
}

void CVNeural::trainNeural(cv::Mat trainData, cv::Mat trainClassification) const
{
	CvMat trainDataCvMat = trainData;
	CvMat trainClassificationCvMat = trainClassification;
	int iterations = neuralNetwork->train(&trainDataCvMat, &trainClassificationCvMat, NULL, NULL, trainParams);
	cout << "Training - num of iterations: " << iterations << endl;
}

void CVNeural::test(vector<Action*> actions) const
{
	cout << "Neural test START" << endl;

	cv::Mat test_data = cv::Mat::zeros(0, inputLayerCount, CV_32FC1);			// 0 rows - 136(pocet uhlu) columns
	cv::Mat test_classifications = cv::Mat(0, outputLayerCount, CV_32FC1);		// 0 rows - pocet akci columns - format (0,0,0,1,0,0,0)

	cout << "Neural preparing data" << endl;

	prepareDataset(15, 1, actions, &test_data, &test_classifications);

	testNeural(actions.at(0),test_data, test_classifications, false);

	cout << "Neural test END" << endl;
}

void CVNeural::renderedTest(Action * action) const
{
	cout << "Rendered neural test START" << endl;

	cv::Mat test_data = cv::Mat::zeros(0, inputLayerCount, CV_32FC1);			// 0 rows - 136(pocet uhlu) columns
	cv::Mat test_classifications = cv::Mat(0, outputLayerCount, CV_32FC1);		// 0 rows - pocet akci columns - format (0,0,0,1,0,0,0)
	cv::Mat test_avgMat = cv::Mat(0, outputLayerCount, CV_32FC1);

	cout << "Rendered neural preparing data" << endl;

	prepareSingleActionData(15, 1, action, &test_data, &test_classifications);

	testNeural(action, test_data, test_classifications, true);

	cout << "Rendered neural test END" << endl;
}

void CVNeural::testNeural(Action * action, cv::Mat testData, cv::Mat testClassification, bool showVideo) const
{
	CvMat testSample;
	CvMat testDataCvMat = testData;
	CvMat* classificationResult = cvCreateMat(1, testClassification.cols, CV_32FC1);
	NeuralResult* neuralResult = new NeuralResult();
	VideoManager* videoManager = new VideoManager();

	for (auto row = 0; row < testData.rows; row++)
	{
		cvGetRow(&testDataCvMat, &testSample, row);

		neuralNetwork->predict(&testSample, classificationResult);

		//showNeuralOutput(classificationResult);

		storeResults(neuralResult,classificationResult, testClassification, row);

		if (showVideo)
			videoManager->actionShowFrame(action, neuralResult, row);
	}

	neuralResult->printResults();
}

void CVNeural::showNeuralOutput(CvMat * neuralOutput) const
{
	cv::Mat resultMat = cv::cvarrToMat(neuralOutput);;
	for (int i = 0; i < neuralOutput->cols; i++)
	{
		cout << resultMat.at<float>(0, i) << "\t";
		if (i == neuralOutput->cols - 1)
		{
			cout << endl;
		}
	}
}

void CVNeural::storeResults(NeuralResult * neuralResult, CvMat * classificationResult, cv::Mat classificator, int row) const
{
	neuralResult->storeResultRow(classificationResult, classificator, row);	
	printf("Testing Sample %i -> result AVG: %d, result nonAVG: %d \n", row, neuralResult->getLastAvgResult(), neuralResult->getLastResult());
}

void CVNeural::storeNeuralToFile() const
{
	cv::FileStorage fs("mlp.yml", cv::FileStorage::WRITE);
	neuralNetwork->write(*fs, "mlp");
}

void CVNeural::loadTrainedFromFile() const
{
	neuralNetwork->load("mlp.yml", "mlp");
}








