#pragma once

#define NOFP 136 

class DataLoader
{
private:
	vector<int> actions;
	string getFileName(int action, int human);
public:
	vector<Action*> loadDailyActivityDataset(vector<int> listOfActions, int numOfSamples);
	vector<Action*> loadAction3DDataset(vector<int> listOfActions, int numOfSamples);

	Action* loadDailyActivitySampleWithVideo(int action, int sample);

	int getActionId(int action) const;
};
