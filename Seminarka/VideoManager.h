#pragma once

class VideoManager
{
private:
	bool windowsInitialized;
	vector<cv::Scalar> colors;

	void setUpWindows(int frames);
	static void onTrackbar(int value, void* data);
public:
	VideoManager();
	void actionShowFrame(Action * action, NeuralResult* neuralResult, int frame);
	void actionPlay(Action * action);

	static vector<cv::Mat>* loadVideo(string fileName);
	static vector<cv::Mat>* loadDepthMap(string fileName);
};
