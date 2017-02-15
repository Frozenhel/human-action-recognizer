#pragma once
#include "TrackbarData.h"

class VideoManager
{
private:
	bool videoStoped;
	bool windowsInitialized;
	vector<cv::Scalar> colors;
	TrackbarData* trackbarData;

	void setUpWindows(int frames);
public:
	VideoManager();
	void actionShowFrame(Action * action, NeuralResult* neuralResult, int frame);
	void actionPlay(Action * action);

	static vector<cv::Mat>* loadVideo(string fileName);
	static vector<cv::Mat>* loadDepthMap(string fileName);
};
