#include "stdafx.h"
#include "VideoManager.h"
#include "TrackbarData.h"

using namespace std;

VideoManager::VideoManager()
{
	windowsInitialized = false;
	videoStoped = false;
}

VideoManager::VideoManager(vector<DailyAction> actionList)
{
	windowsInitialized = false;
	videoStoped = false;

	this->actionList = actionList;
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

	setUpWindows(action->getAnglesMatrix().rows);

	for (auto frame = 0; frame < videoRGB->size(); frame++)
	{
		cv::imshow("Video RGB", videoRGB->at(frame));
		cv::imshow("Video Depth", videoDepth->at(frame));

		cvWaitKey(0);
	}
}

void VideoManager::closeAllWindows() const
{
	cv::destroyAllWindows();
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

int setBegining(int row)
{
	if (row <= 36) 
		return 0;
	return row - 36;
}

void drawNeuralResultLines(cv::Mat displayMat, cv::Mat resultMat, int begining, vector<cv::Scalar> colors)
{
	for (auto i = 1; i < resultMat.rows; i++)
	{
		for (auto action = 0; action < resultMat.cols; action++)
			line(displayMat, cv::Point((i - 1 - begining) * 20, 299 - resultMat.at<float>(i - 1, action) * 299), cv::Point((i - begining) * 20, 299 - resultMat.at<float>(i, action) * 299), colors.at(action), 2);
	}
}

void currentNeuralLine(cv::Mat displayMat, int frame)
{
	line(displayMat, cv::Point(frame * 20, 0), cv::Point(frame * 20, 299), cv::Scalar(0, 255, 255), 1);

}

void drawCurrentLine(cv::Mat displayMat, cv::Mat resultMat, int frame, int * begining)
{
	if (frame + 1 != resultMat.rows)
	{
		if (*begining == 0)
		{
			currentNeuralLine(displayMat, frame);
		}
		else
		{
			if (frame < 20)
			{
				currentNeuralLine(displayMat, frame);
				*begining = 0;
			}
			else if (frame > resultMat.rows - 16)
			{
				currentNeuralLine(displayMat, 36 - (resultMat.rows - frame));
			}
			else
			{
				currentNeuralLine(displayMat, 20);
				*begining = frame - 20;
			}
		}
	}
	else
	{
		currentNeuralLine(displayMat, resultMat.rows - 1 - *begining);
	}
}

void drawNeuralResultGraph(cv::Mat displayMat, cv::Mat resultMat, vector<cv::Scalar> colors, int frame)
{
	int begining = setBegining(resultMat.rows);
	drawCurrentLine(displayMat, resultMat, frame, &begining);
	drawNeuralResultLines(displayMat, resultMat, begining, colors);
}

void drawLegend(cv::Mat displayMat, vector<DailyAction> actions, vector<cv::Scalar> colors)
{
	for (auto i = 0; i < colors.size(); i++)
	{
		putText(displayMat, ToString(actions.at(i)), cv::Point(5, i * 20 + 10), CV_FONT_HERSHEY_SIMPLEX, 0.5, colors.at(i), 2);
	}
}

void drawLegendWithValues(cv::Mat displayMat, NeuralResult* neuralResult, int frame, vector<DailyAction> actions, vector<cv::Scalar> colors)
{
	
	for (auto i = 0; i < colors.size(); i++)
	{
		stringstream text;
		text << ToString(actions.at(i)) << " - AVG:" << neuralResult->getAvgResultMat().at<float>(frame, i) << "  NONAVG:" << neuralResult->getResultMat().at<float>(frame, i);
		putText(displayMat, text.str(), cv::Point(8, i * 20 + 15), CV_FONT_HERSHEY_SIMPLEX, 0.5, colors.at(i), 2);
	}
}

void showNeuralOutput(NeuralResult * neuralResult, int frame, vector<DailyAction> actions, vector<cv::Scalar> colors)
{
	cv::Mat displayMat = cv::Mat::ones(300, 701, CV_8UC3);
	cv::Mat resultMat = neuralResult->getAvgResultMat();
	
	drawNeuralResultGraph(displayMat, resultMat, colors, frame);
	drawLegendWithValues(displayMat, neuralResult, frame, actions, colors);
	imshow("Neural result", displayMat);
}

void showFrame(Action* action, NeuralResult* neuralResult, int frame, vector<DailyAction> actions, vector<cv::Scalar> colors)
{
	showRGB(action, frame);
	showDepth(action, frame);
	showRGBSkeleton(action, frame);
	showDepthSkeleton(action, frame);
	showNeuralOutput(neuralResult, frame, actions, colors);
}

void onTrackbar(int frame, void* data)
{
	auto trackbarData = static_cast<TrackbarData*>(data);
	trackbarData->stopVideo();

	if (frame == trackbarData->getMaxFrames())
	{
		cout << frame << " Video Resumed " << trackbarData->getMaxFrames() << endl;
		trackbarData->resumeVideo();
	}

	trackbarData->getNeuralResult()->printNeuralValuesAtFrame(frame);

	showRGB(trackbarData->getAction() , frame);
	showDepth(trackbarData->getAction() , frame);
	showRGBSkeleton(trackbarData->getAction() , frame);
	showDepthSkeleton(trackbarData->getAction() , frame);
	showNeuralOutput(trackbarData->getNeuralResult(), frame, trackbarData->getActions(), trackbarData->getColors());

	while (trackbarData->isVideoStopped())
	{
		cv::waitKey(15);
	}
	//	actionShowFrame(action, neuralResult, frame);
}


void updateTrackbar(TrackbarData * trackbarData, Action * action, NeuralResult* neuralResult, int maxFrames)
{
	trackbarData->update(action, neuralResult, maxFrames);
	cv::createTrackbar("Control", "Neural result", &maxFrames, maxFrames, onTrackbar, trackbarData);
}

void VideoManager::actionShowFrame(Action* action, NeuralResult* neuralResult, int frame)
{
	if (!windowsInitialized){
		setUpWindows(action->getAnglesMatrix().rows);

		generateColors(colors, neuralResult->getAvgResultMat().cols);
		colors.at(0) = cv::Scalar(255, 255, 255);

		trackbarData = new TrackbarData(action, neuralResult, actionList, colors, frame, false);
	}
	
	showFrame(action, neuralResult, frame, actionList, colors);
	updateTrackbar(trackbarData, action, neuralResult, frame);

	cv::waitKey(60);
}

void VideoManager::setUpWindows(int frames)
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

//	cv::createTrackbar("", "Neural result", 0, frames, onTrackbar);

	windowsInitialized = true;
}



