#pragma once

#include "Action.h"
#include "NeuralResult.h"

class TrackbarData
{
private:
	Action * action;
	NeuralResult* neuralResult;
	vector<cv::Scalar> colors;
	int maxFrames;
	bool videoStopped;

public:
	TrackbarData(Action * action, NeuralResult * neuralResult, vector<cv::Scalar> colors, int maxFrames, bool videoStopped);

	void update(Action* action, NeuralResult* neuralResult, int maxFrames);

	Action* getAction() const
	{
		return action;
	}

	NeuralResult* getNeuralResult() const
	{
		return neuralResult;
	}

	vector<cv::Scalar> getColors() const
	{
		return colors;
	}

	int getMaxFrames() const
	{
		return maxFrames;
	}

	bool isVideoStopped() const
	{
		return videoStopped;
	}

	void stopVideo()
	{
		videoStopped = true;
	}

	void resumeVideo()
	{
		videoStopped = false;
	}
};

inline TrackbarData::TrackbarData(Action* action, NeuralResult* neuralResult, vector<cv::Scalar> colors, int maxFrames, bool videoStopped)
{
	this->action = action;
	this->neuralResult = neuralResult;
	this->maxFrames = maxFrames;
	this->videoStopped = videoStopped;
	this->colors = colors;
}

inline void TrackbarData::update(Action* action, NeuralResult* neuralResult, int maxFrames)
{
	this->action = action;
	this->neuralResult = neuralResult;
	this->maxFrames = maxFrames;
}


