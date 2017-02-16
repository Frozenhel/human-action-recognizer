#include "stdafx.h"

string DataLoader::getFileName(int action, int human)
{
	stringstream action_str;
	action_str << setw(2) << setfill('0') << action;

	stringstream human_str;
	human_str << setw(2) << setfill('0') << human;

	return "a" + action_str.str() + "_s" + human_str.str() + "_e02_";
}

void DataLoader::loadDailyActivityDataset(vector<Action*>* actions)
{
	cout << "Loading data" << endl;

	cv::Mat skeleton;
	string path = "E:/testVideos/";
	string fileName = "";

	for (Action * action : *actions)
	{
		fileName = getFileName(action->getActionType(), action->getSample());
		FILE* fp = fopen((path + fileName + "depth.bin").c_str(), "rb");
		if (fp == NULL) return;

		int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
		int ncols = 0;
		int nrows = 0;
		ReadDepthMapSktBinFileHeader(fp, nofs, ncols, nrows);
		fclose(fp);

		Skeleton* skeletons = new Skeleton[nofs];
		ReadSkeletonMap(path + fileName + "skeleton.txt", skeletons, nofs);

		auto singleAction = cv::Mat(nofs, NOFP, CV_32FC1, 0.0f);
		for (auto f = 0; f < nofs; f++)
		{
			Skeleton sk = skeletons[f];
			sk.bodyPartsInit();
			float* angles = sk.calculateAnglesBtwParts();
			//** matice V
			for (auto j = 0; j < NOFP; j++)
			{
				singleAction.at<float>(f, j) = angles[j];
			}
		}
		action->setAnglesMatrix(singleAction);
	}

	cout << "Loaded " << actions->size() << " samples" << endl;
}

//TODO sprovoznit
vector<Action*> DataLoader::loadAction3DDataset(vector<int> listOfActions, int numOfSamples)
{
	cout << "Loading data" << endl;

	auto actions = new vector<Action*>();
	cv::Mat frame, depth, skeleton, histogram[NOFP], V, CF;
	string path = "E:/testVideos/";
	string fileName = "";
	string action_str = "00";
	string human_str = "00";

	for (int action = 0; action < listOfActions.size(); action++)
	{
		for (int i = 1; i <= numOfSamples; i++)
		{
			//nutne pridan 0 k nazvu, pokud je cislo akce/pokusu < 10

			stringstream action_str;
			action_str << setw(2) << setfill('0') << listOfActions[action];

			stringstream human_str;
			human_str << setw(2) << setfill('0') << i;

			fileName = "a" + action_str.str() + "_s" + human_str.str() + "_e02_depth.bin";
			FILE* fp = fopen((path + fileName).c_str(), "rb");
			if (fp == NULL) return *actions;;

			int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
			int ncols = 0;
			int nrows = 0;
			ReadDepthMapSktBinFileHeader(fp, nofs, ncols, nrows);
			fclose(fp);

			Skeleton* skeletons = new Skeleton[nofs];
			ReadSkeletonMap(path + fileName + "skeleton.txt", skeletons, nofs);

			auto V = cv::Mat(NOFP, nofs, CV_32FC1, 0.0f);
			for (int f = 0; f < nofs; f++)
			{
				Skeleton sk = skeletons[f];
				sk.bodyPartsInit();
				float* angles = sk.calculateAnglesBtwParts();
				//** matice V
				for (auto j = 0; j < NOFP; j++)
				{
					V.at<float>(j, f) = angles[j];
				}
			}
		//	actions->push_back(new Action(listOfActions[action], action, i, 0, V));
		}
	}
	cout << "Loaded " << actions->size() << " samples" << endl;
	return *actions;
}


void DataLoader::loadDailyActivitySampleWithVideo(Action * action)
{
	cout << "Loading data" << endl;

	//cout << "Actions vector size " << actions.size() << endl;

	cv::Mat skeleton;
	string path = "E:/testVideos/";
	string fileName = "";

	fileName = getFileName(action->getActionType(), action->getSample());
	FILE* fp = fopen((path + fileName + "depth.bin").c_str(), "rb");
	if (fp == NULL) return;

	int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
	int ncols = 0;
	int nrows = 0;
	ReadDepthMapSktBinFileHeader(fp, nofs, ncols, nrows);
	fclose(fp);

	Skeleton* skeletons = new Skeleton[nofs];
	ReadSkeletonMap(path + fileName + "skeleton.txt", skeletons, nofs);

	auto anglesMatrix = cv::Mat(nofs, NOFP, CV_32FC1, 0.0f);
	for (auto f = 0; f < nofs; f++)
	{
		Skeleton sk = skeletons[f];
		sk.bodyPartsInit();
		float* angles = sk.calculateAnglesBtwParts();

		for (auto j = 0; j < NOFP; j++)
		{
			anglesMatrix.at<float>(f, j) = angles[j];
		}
	}

	cout << "Loaded single action" << endl;

	action->setAnglesMatrix(anglesMatrix);
	action->setVideo(VideoManager().loadVideo(path + fileName + "rgb.avi"));
	action->setDepthMap(VideoManager().loadDepthMap(path + fileName + "depth.bin"));
	action->setSkeletons(skeletons);
}
