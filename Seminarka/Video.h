using namespace std;

class Video{
private:
	cv::VideoCapture cap;

public:
	Video();
	void loadVideo(string file);
	cv::Mat getNextFrame();
	static void playVideo(std::string file);
	static void createVideo(std::string fileName, int width, int height, int nFrames, cv::Mat * images);
};

inline Video::Video()
{
	return;
}

inline void Video::loadVideo(string file)
{
	cap.open(file);
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return;
	}

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

	std::cout << "Frame per seconds : " << fps << std::endl;

}

inline cv::Mat Video::getNextFrame()
{
	cv::Mat frame;

	bool bSuccess = cap.read(frame); // read a new frame from video

	if (!bSuccess) //if not success, break loop
	{
		std::cout << "Cannot read the frame from video file" << std::endl;
		return cv::Mat(480, 640, CV_8UC3, cv::Scalar(0));
	}
	return frame;
}


inline void Video::playVideo(std::string file)
{
	cv::VideoCapture cap(file); // open the default camera
	if (!cap.isOpened())  // if not success, exit program
	{
		std::cout << "Cannot open the video file" << std::endl;
		return;
	}

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

	std::cout << "Frame per seconds : " << fps << std::endl;

	cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	while (1)
	{
		cv::Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			std::cout << "Cannot read the frame from video file" << std::endl;
			return;
		}

		cv::imshow("MyVideo", frame); //show the frame in "MyVideo" window

		if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
}

inline void Video::createVideo(std::string fileName, int width, int height, int nFrames, cv::Mat * images)
{
	cv::VideoCapture in_capture();
	cv::VideoWriter out_capture("F:/skeletal/videos/" + fileName + ".avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(width, height));
	
	for (int i = 0; i < nFrames; i++)
	{
		out_capture.write(images[i]);
	}
	
}
