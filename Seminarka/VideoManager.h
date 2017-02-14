#pragma once

class VideoManager
{
private:
	bool windowsInitialized;
	vector<cv::Scalar> colors;
	
	void showNeuralOutput(NeuralResult* neuralResult, int frame);

	static void setUpWindows();
public:
	VideoManager();
	void actionShowFrame(Action * action, NeuralResult* neuralResult, int frame);

	static vector<cv::Mat>* loadVideo(string fileName);
	static vector<cv::Mat>* loadDepthMap(string fileName);
	static void actionPlay(Action * action);
};
