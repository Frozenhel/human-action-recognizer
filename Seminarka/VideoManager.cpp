#include "stdafx.h"
#include "VideoManager.h"

using namespace std;

VideoManager::VideoManager()
{
	windowsInitialized = false;
}

vector<cv::Mat>* VideoManager::loadVideo(string fileName)
{
	cout << "Loading RGB video" << endl;
	vector<cv::Mat>* video = new vector<cv::Mat>();
	cv::VideoCapture videoCapture(fileName);

	if (!videoCapture.isOpened())
	{
		cout << "Cannot open file" << endl;
		return video;
	}

	while (true)
	{
		cv::Mat frame;

		bool bSuccess = videoCapture.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			return video;
		}
		video->push_back(frame);
	}
}

vector<cv::Mat>* VideoManager::loadDepthMap(string fileName)
{
	cout << "Loading depth video" << endl;
	vector<cv::Mat>* depths = new vector<cv::Mat>();
	
	int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
	int ncols = 0;
	int nrows = 0;

	FILE* fp = fopen(fileName.c_str(), "rb");
	ReadDepthMapSktBinFileHeader(fp, nofs, ncols, nrows);

	CDepthMapSkt *depthMap = new CDepthMapSkt();
	depthMap->SetSize(ncols, nrows);

	for (int i = 0; i < nofs; i++)
	{
		ReadDepthMapSktBinFileNextFrame(fp, ncols, nrows, depthMap);
		cv::Mat depth = cv::Mat(nrows, ncols, CV_8UC1, cv::Scalar(255));
		for (int x = 0; x < nrows; x++)
		{
			for (int y = 0; y < ncols; y++)
			{
				depth.at<uchar>(x, y) = depthMap->GetItem(x, y);
			}
		}
		depths->push_back(depth);
	}
	return depths;
}

void VideoManager::actionPlay(Action* action)
{
	auto videoRGB = action->getVideo();
	auto videoDepth = action->getDepthMap();
	auto skeletons = action->getSkeletons();

	setUpWindows();

	for (auto frame = 0; frame < videoRGB->size(); frame++)
	{
		cv::imshow("Video RGB", videoRGB->at(frame));
		cv::imshow("Video Depth", videoDepth->at(frame));

		cvWaitKey(0);
	}
}

void VideoManager::setUpWindows()
{
	cv::namedWindow("Video RGB", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Video Depth", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Video RGB+skeleton", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Video Depth+skeleton", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Neural result", CV_WINDOW_AUTOSIZE);

	cv::resizeWindow("Video RGB", 300, 300);
	cv::resizeWindow("Video Depth", 300, 300);
	cv::resizeWindow("Video RGB+skeleton", 300, 300);
	cv::resizeWindow("Video Depth+skeleton", 300, 300);
	cv::resizeWindow("Neural result", 700, 300);

	cv::moveWindow("Video RGB", 0, 50);
	cv::moveWindow("Video Depth", 350, 50);
	cv::moveWindow("Video RGB+skeleton", 700, 50);
	cv::moveWindow("Video Depth+skeleton", 1050, 50);
	cv::moveWindow("Neural result", 350, 500);
}

void showRGB(Action * action, int frame)
{
	cv::Mat resizedMat;
	cv::resize(action->getVideo()->at(frame), resizedMat, cv::Size(300, 300));
	cv::imshow("Video RGB", resizedMat);
}

void showDepth(Action * action, int frame)
{
	cv::Mat resizedMat;
	cv::resize(action->getDepthMap()->at(frame), resizedMat, cv::Size(300, 300));
	cv::imshow("Video Depth", resizedMat);
}

void showRGBSkeleton(Action * action, int frame)
{
	cv::Mat resizedMat;
	cv::resize(action->getVideo()->at(frame), resizedMat, cv::Size(300, 300));

	auto skeleton = action->getSkeletons()[frame];
	for (auto i = 0; i < 20; i++)
	{
		cv::circle(resizedMat, skeleton.getPoint(i, false).getPos2D(300, 300), 5, cv::Scalar(255), -1);
	}

	auto color = cv::Scalar(255, 255, 255);
	for (auto i = 0; i < 17; i++)
	{
		cv::line(resizedMat, skeleton.getPart(i, false).getJ1().getPos2D(300, 300), skeleton.getPart(i, false).getJ2().getPos2D(300, 300), color, 2);
	}

	cv::imshow("Video RGB+skeleton", resizedMat);
}

void showDepthSkeleton(Action * action, int frame)
{
	cv::Mat resizedMat;
	cv::resize(action->getDepthMap()->at(frame), resizedMat, cv::Size(300, 300));

	auto skeleton = action->getSkeletons()[frame];
	for (auto i = 0; i < 20; i++)
	{
		cv::circle(resizedMat, skeleton.getPoint(i, false).getPos2D(300, 300), 5, cv::Scalar(255), -1);
	}

	auto color = cv::Scalar(255, 255, 255);
	for (int i = 0; i < 17; i++)
	{
		cv::line(resizedMat, skeleton.getPart(i, false).getJ1().getPos2D(300, 300), skeleton.getPart(i, false).getJ2().getPos2D(300, 300), color, 2);
	}

	cv::imshow("Video Depth+skeleton", resizedMat);
}

void VideoManager::showNeuralOutput(NeuralResult * neuralResult, int frame)
{
	cv::Mat displayMat = cv::Mat::ones(300, 700, CV_8UC3);
	cv::Mat resultMat = neuralResult->getAvgResultMat();
	
	if (frame == 0)
		generateColors(colors, neuralResult->getAvgResultMat().cols);
	colors.at(0) = cv::Scalar(255, 255, 255);

	if (resultMat.rows <= 35)
	{
		for (auto i = 1; i < resultMat.rows; i++)
		{
			for (auto action = 0; action < resultMat.cols; action++)
				line(displayMat, cv::Point((i - 1) * 20, 299 - resultMat.at<float>(i - 1, action) * 299), cv::Point((i)* 20, 299 - resultMat.at<float>(i, action) * 299), colors.at(action), 2);
		}
	}
	else
	{
		auto begining = resultMat.rows - 35;
		for (auto i = begining; i < resultMat.rows; i++)
		{
			for (auto action = 0; action < resultMat.cols; action++)
				line(displayMat, cv::Point((i - 1 - begining) * 20, 299 - resultMat.at<float>(i - 1, action) * 299), cv::Point((i - begining) * 20, 299 - resultMat.at<float>(i, action) * 299), colors.at(action), 2);
		}
	}

	for (int i = 0; i < colors.size(); i++)
	{
		putText(displayMat,to_string(i), cv::Point(5,i*20+10),CV_FONT_HERSHEY_SIMPLEX,0.5,colors.at(i),2);
	}
	
	imshow("Neural result", displayMat);
}

void VideoManager::actionShowFrame(Action* action, NeuralResult* neuralResult, int frame)
{
	if (!windowsInitialized)
		setUpWindows();

	showRGB(action, frame);
	showDepth(action, frame);
	showRGBSkeleton(action, frame);
	showDepthSkeleton(action, frame);
	showNeuralOutput(neuralResult, frame);

	cv::waitKey(200);
}

