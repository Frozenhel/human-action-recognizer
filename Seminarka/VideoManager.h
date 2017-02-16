#pragma once
#include "TrackbarData.h"

class VideoManager
{
private:
	bool videoStoped;
	bool windowsInitialized;

	vector<cv::Scalar> colors;
	vector<DailyAction> actionList;

	TrackbarData* trackbarData;

	void setUpWindows(int frames);
public:
	VideoManager();
	VideoManager(vector<DailyAction> actionList);
	void actionShowFrame(Action * action, NeuralResult* neuralResult, int frame);
	void actionPlay(Action * action);
	void closeAllWindows() const;

	static vector<cv::Mat>* loadVideo(string fileName);
	static vector<cv::Mat>* loadDepthMap(string fileName);
};
