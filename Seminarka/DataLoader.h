#pragma once

#define NOFP 136 

class DataLoader
{
private:
	static string getFileName(int action, int human);
public:
	static void loadDailyActivityDataset(vector<Action*>* actions);
	vector<Action*> loadAction3DDataset(vector<int> listOfActions, int numOfSamples);

	static void loadDailyActivitySampleWithVideo(Action * action);
};
