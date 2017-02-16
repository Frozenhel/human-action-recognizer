#include "stdafx.h"

using namespace std;

const RecognizeMethod method = neuralMultiSkeleton;

CFRepresentation* cf;
CVNeural* neural;
DataLoader* dataLoader;

const vector<DailyAction> actions = {eating, reading, phoning, vacuuming, celebrating};

const vector<int> trainSamples = {1, 2, 3, 4, 5, 7, 8, 9, 10};
const vector<int> testSamples = {6};

vector<Action*> initDataset(vector<int> samples)
{
	auto actionDataset = vector<Action*>();
	for (auto i = 0; i < actions.size(); i++)
	{
		for (auto trainSample : samples)
		{
			actionDataset.push_back(new Action(actions[i], i, trainSample, 0));
		}
	}
	return actionDataset;
}

void useNeuralNetwork(vector<Action*> trainActions, vector<Action*> testActions)
{
	neural = new CVNeural(NOFP, 50, actions.size(), actions, method);
	neural->train(trainActions);
	//neural->loadTrainedFromFile();

	//neural->test(testActions);
	for (auto action : testActions)
	{
		DataLoader().loadDailyActivitySampleWithVideo(action);
		neural->renderedTest(action);

		cv::waitKey(0);
	}
}

void useDTW(vector<Action*> trainActions, vector<Action*> testActions)
{
	//cf = new CFRepresentation();
	//cf->createNewCfMat(&action_mats, 0.0004, 4, trainActions);

	testDTW(trainActions, testActions);
}

int main(int argc, char* argv[])
{
	auto trainActions = initDataset(trainSamples);
	auto testActions = initDataset(testSamples);

	DataLoader().loadDailyActivityDataset(&trainActions);
	DataLoader().loadDailyActivityDataset(&testActions);

	switch (method)
	{
	case neuralDiff:
	case neuralMultiSkeleton:
		useNeuralNetwork(trainActions, testActions);
		break;
	case DTW:
		useDTW(trainActions, testActions);
		break;
	default:
		cout << "Unknown recognize method" << endl;
	}

	cv::waitKey(0);
	return 0;
}
