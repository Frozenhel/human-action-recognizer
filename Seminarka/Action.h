#pragma once

using namespace std;

class Action
{
private:
	int id;
	int actionID;
	int sample;
	int version;
	cv::Mat anglesMat;
	vector<cv::Mat>* video;
	vector<cv::Mat>* depthMap;
	Skeleton* skeletons;

public:
	Action(int id, int actionID, int sample, int version, cv::Mat anglesMat);
	int getId() const { return id; }
	int getActionId() const { return actionID; }
	cv::Mat getAnglesMatrix() const { return anglesMat; }
	vector<cv::Mat>* getVideo() const { return video; }
	void setVideo(vector<cv::Mat>* video) { this->video = video; }
	vector<cv::Mat>* getDepthMap() const { return depthMap; }
	void setDepthMap(vector<cv::Mat>* depthMap){ this->depthMap = depthMap; }
	Skeleton* getSkeletons() const { return skeletons; }
	void setSkeletons(Skeleton * skeletons) { this->skeletons = skeletons; }
};

inline Action::Action(int id, int actionID, int sample, int version, cv::Mat anglesMat)
{
	this->id = id;
	this->actionID = actionID;
	this->sample = sample;
	this->version = version;
	this->anglesMat = anglesMat;;
}
