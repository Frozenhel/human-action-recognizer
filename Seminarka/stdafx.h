// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _USE_MATH_DEFINES
#include "targetver.h"

#include <stdio.h>
#include <math.h>
#include <thread>
#include <iomanip>
#include <opencv2/opencv.hpp>

#include "Joint.h"
#include "BodyPart.h"
#include "Skeleton.h"
#include "Action.h"
#include "NeuralResult.h"

#include "VideoManager.h"
#include "DepthMapSktBinFileIO.h"
#include "DataLoader.h"
#include "Video.h"
#include "CvHMM.h"
#include "CFRepresentation.h"
#include "DTWanalyzer.h"
#include "CVNeural.h"








// TODO: reference additional headers your program requires here
