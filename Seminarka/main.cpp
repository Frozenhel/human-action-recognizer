#include "stdafx.h"
#include "RecognizeMethod.h"

using namespace std;

const RecognizeMethod method = neuralMultiSkeleton;

CFRepresentation * cf;
CVNeural * neural;
DataLoader * dataLoader;
 
int main(int argc, char* argv[])
{
	dataLoader = new DataLoader();
	vector<int> actions = { 1, 3, 5, 7, 8};;
	const int samples = 9;
	vector<Action*> trainActions = dataLoader->loadDailyActivityDataset(actions, samples);

	neural = new CVNeural(NOFP, 50, actions.size(), method);
	//	neural->train(trainActions);
	neural->loadTrainedFromFile();

	//DEMO :)
	/*auto testAction = dataLoader->loadDailyActivitySampleWithVideo(1, 10);
	neural->renderedTest(testAction);

	testAction = dataLoader->loadDailyActivitySampleWithVideo(3, 5);
	neural->renderedTest(testAction);

	testAction = dataLoader->loadDailyActivitySampleWithVideo(5, 5);
	neural->renderedTest(testAction);

	testAction = dataLoader->loadDailyActivitySampleWithVideo(7, 10);
	neural->renderedTest(testAction);

	testAction = dataLoader->loadDailyActivitySampleWithVideo(8, 10);
	neural->renderedTest(testAction);*/
	
	neural->test(trainActions);

	cv::waitKey(0);

	return 0;

	//learning(moves, actionsCount, testRabits, &action_mats);  // cfRepresentation, move sets, num of move sets, num of humans

	//cf = new CFRepresentation();
	//cf->createNewCfMat(&action_mats, 0.0004, 4, actionsCount*testRabits);

	//testDTW();
	//testNeural();
}

/*
void learning(int * moves, int movesCount, int humanCount, vector<cv::Mat> * action_mats)
{
Video* videoPlayer = new Video();
cv::Mat frame, depth, skeleton, histogram[NOFP], V, CF;
string path = "E:/testVideos/";
string fileName = "";
string action_str = "00";
string human_str = "00";

for (int action = 0; action < movesCount; action++)
{
cout << "Ucim se " << moves[action] << ". akci" << endl;
for (int i = 1; i <= humanCount; i++)
{
cout << "Pozoruji pokusneho kralika " << i << endl;
action_str = moves[action] < 10 ? "0"+to_string(moves[action]): to_string(moves[action]);
human_str = i<10 ? "0" + to_string(i) : to_string(i);
fileName = "a" + action_str + "_s"+ human_str + "_e02_";
FILE* fp = fopen((path + fileName + "depth.bin").c_str(), "rb");
if (fp == NULL) return;
int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
int ncols = 0;
int nrows = 0;
ReadDepthMapSktBinFileHeader(fp, nofs, ncols, nrows);


if (display)
{
videoPlayer->loadVideo(path + fileName + "rgb.avi");
for (int i = 0; i < NOFP; i++){histogram[i] = cv::Mat(100, nofs, CV_8UC1, cv::Scalar(0));}
}

cout << "Mapuji kostru " << endl;
Skeleton* skeletons = new Skeleton[nofs];
ReadSkeletonMap(path + fileName + "skeleton.txt", skeletons, nofs);

cout << "Pocitam uhly " << endl;
cv::Mat V = cv::Mat(NOFP, nofs, CV_32FC1, 0.0f);
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

if (display)
{
CDepthMapSkt *depthMap = new CDepthMapSkt();
depthMap->SetSize(ncols, nrows);
ReadDepthMapSktBinFileNextFrame(fp, ncols, nrows,depthMap);
frame = videoPlayer->getNextFrame();
depth = cv::Mat(nrows, ncols, CV_8UC1, cv::Scalar(255));
for (int x = 0; x < nrows; x++)
{
for (int y = 0; y < ncols; y++)
{
depth.at<uchar>(x, y) = depthMap->GetItem(x, y);
}
}
skeleton = cv::Mat(nrows, ncols, CV_8UC1, cv::Scalar(0));

for (int i = 0; i < 20; i++)
{
cv::circle(skeleton, sk.getPoint(i, false).getPos2D(nrows, ncols), 5, cv::Scalar(255), -1);
cv::circle(depth, sk.getPoint(i, false).getPos2D(nrows, ncols), 2, cv::Scalar(255), -1);
cv::circle(frame, sk.getPoint(i, false).getPos2D(frame.rows, frame.cols), 5, cv::Scalar(255), -1);
//		cv::putText(frame, to_string(i), sk.getPoint(i, false).getPos2D(frame.rows, frame.cols), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1, cv::Scalar(0,0,255), 3, 8);
}

for (int i = 0; i < 17; i++)
{
cv::Scalar color = cv::Scalar(255, 255, 255);
cv::line(frame, sk.getPart(i, false).getJ1().getPos2D(frame.rows, frame.cols), sk.getPart(i, false).getJ2().getPos2D(frame.rows, frame.cols), color, 2);
cv::putText(frame, to_string(i), sk.getPart(i, false).getMiddle2D(frame.rows, frame.cols), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1, cv::Scalar(0, 0, 255), 3, 8);
}
histogram[0].at<uchar>(sk.getAngleBtwParts(2, 11) * 30, f) = 255;
cv::imshow("histogram", histogram[0]);
cv::imshow("RGB video", frame); //sh
cv::imshow("Depth video", depth); //sh
cv::imshow("Skeleton video", skeleton); //sh
cv::imshow("V", V); //sh

cv::waitKey(1);
}

}
cout << "Naucen ;)" << endl << endl;
action_mats->push_back(V);
fclose(fp);
}

cv::waitKey(10000);
}
}

void calSimOrder(vector<float> similar, vector<float> * similarOrder)
{
auto min = 10000000.f;
auto minPos = 0;
for (int i = 0; i < similar.size(); i++)
{
min = 10000000.f;
minPos = 0;
for (int j = 0; j < similar.size(); j++)
{
if (min > similar[j])
{
min = similar[j];
minPos = j;
}
}
similar.at(minPos) = 10000000;
similarOrder->push_back(minPos);
}
}

void testDTW()
{

int correctNonCf[]{0, 0, 0, 0, 0, 0};
int correctCf[]{0, 0, 0, 0, 0, 0};

// testing
for (int chosen = 0; chosen < actionsCount * testRabits; chosen++)
//for (int chosen = 0; chosen < 1; chosen++)
{
vector<float> similar;
vector<float> similarCF;

cout << "Porovnani " << chosen << ". prvku" << endl;
clock_t begin = clock();
for (int i = 0; i < action_mats.size(); i++)
{
if (i == chosen) continue;
similar.push_back(DTWAnalyzer::calcSimilarity(&action_mats.at(i), &action_mats.at(chosen), i));
}

clock_t end = clock();
cout << "NonCFTime " << double(end - begin) / CLOCKS_PER_SEC << " s" << endl;;


begin = clock();
for (int i = 0; i < action_mats.size(); i++)
{
if (i == chosen) continue;
int cfRow = i / testRabits;
similarCF.push_back(DTWAnalyzer::calcSimilarityCF(&action_mats.at(i), &action_mats.at(chosen), &cf->getCFMat()[cfRow]));
}
end = clock();
cout << "CFTime " << double(end - begin) / CLOCKS_PER_SEC << " s" << endl;

vector<float> similarOrder;
vector<float> similarOrderCF;
calSimOrder(similar, &similarOrder);
calSimOrder(similarCF, &similarOrderCF);

cout << "nonCF  ";
for (int i = 0; i < 5; i++)
{

cout << similarOrder[i] << " ";
}

cout << endl;
cout << "CF  ";
for (int i = 0; i < 5; i++)
{
cout << similarOrderCF[i] << " ";
}
cout << endl;

if (similarOrder[0] >= chosen - (chosen%testRabits) && similarOrder[0] < chosen + (testRabits - chosen%testRabits)) correctNonCf[chosen / 10]++;
if (similarOrderCF[0] >= chosen - (chosen%testRabits) && similarOrderCF[0] < chosen + (testRabits - chosen%testRabits)) correctCf[chosen / 10]++;
}
cout << "Mira uspechu jednotlivych poznavani akci" << endl;
cout << "akce nonCF     CF" << endl;

for (int i = 0; i < actionsCount; i++)
{
cout << i << "    " << correctNonCf[i] << "/" << testRabits << "     " << correctCf[i] << "/" << testRabits << endl;
}

}

void testNeural()
{

}
*/