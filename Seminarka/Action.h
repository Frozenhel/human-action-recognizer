#pragma once

using namespace std;

class Action
{
private:
	DailyAction actionType;
	int actionID;
	int sample;
	int version;
	cv::Mat anglesMat;
	vector<cv::Mat>* video;
	vector<cv::Mat>* depthMap;
	Skeleton* skeletons;

public:
	Action(DailyAction actionType, int actionID, int sample, int version);
	Action(DailyAction actionType, int actionID, int sample, int version, cv::Mat anglesMat);

	DailyAction getActionType() const { return actionType; }
	int getActionId() const { return actionID; }

	int getSample() const { return sample; }

	cv::Mat getAnglesMatrix() const { return anglesMat; }
	void setAnglesMatrix(cv::Mat anglesMat){ this->anglesMat = anglesMat; }

	vector<cv::Mat>* getVideo() const { return video; }
	void setVideo(vector<cv::Mat>* video) { this->video = video; }

	vector<cv::Mat>* getDepthMap() const { return depthMap; }
	void setDepthMap(vector<cv::Mat>* depthMap){ this->depthMap = depthMap; }

	Skeleton* getSkeletons() const { return skeletons; }
	void setSkeletons(Skeleton * skeletons) { this->skeletons = skeletons; }
};

inline Action::Action(DailyAction actionType, int actionID, int sample, int version) : video(nullptr), depthMap(nullptr), skeletons(nullptr)
{
	this->actionType = actionType;
	this->actionID = actionID;
	this->sample = sample;
	this->version = version;
}

inline Action::Action(DailyAction actionType, int actionID, int sample, int version, cv::Mat anglesMat) : video(nullptr), depthMap(nullptr), skeletons(nullptr)
{
	this->actionType = actionType;
	this->actionID = actionID;
	this->sample = sample;
	this->version = version;
	this->anglesMat = anglesMat;;
}
