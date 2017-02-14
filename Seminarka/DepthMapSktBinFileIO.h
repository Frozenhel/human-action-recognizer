#ifndef _DEPTH_MAP_SKT_BIN_FILE_IO_H
#define _DEPTH_MAP_SKT_BIN_FILE_IO_H

#include "DepthMapSkt.h"
#include <fstream>
#include "Skeleton.h";
#include <iterator>

using namespace std;

class Skeleton;
int ReadDepthMapSktBinFileHeader(FILE * fp, int &retNumFrames, int &retNCols, int &retNRows);

//the caller needs to allocate space for <retDepthMap>
int ReadDepthMapSktBinFileNextFrame(FILE * fp, int numCols, int numRows, CDepthMapSkt * retDepthMap);

//<fp> must be opened with "wb"
int WriteDepthMapSktBinFileHeader(FILE * fp, int nFrames, int nCols, int nRows);

//<fp> must be opened with "wb"
int WriteDepthMapSktBinFileNextFrame(FILE * fp, const CDepthMapSkt & depthMap);

int ReadSkeletonMap(string fileName, Skeleton * skeleton, int skelCount);

#endif

inline int ReadDepthMapSktBinFileHeader(FILE * fp, int &retNumFrames, int &retNCols, int &retNRows)
{
	if (fp == NULL)
		return 0;

	fread(&retNumFrames, 4, 1, fp); //read 4 bytes 
	fread(&retNCols, 4, 1, fp);
	fread(&retNRows, 4, 1, fp);
	//fscanf(fp, "%i", &retNumFrames);
	//fscanf(fp, "%i", &retWidth);
	//fscanf(fp, "%i", &retHeight);

	return 1;
}

//the caller needs to allocate space for <retDepthMap>
inline int ReadDepthMapSktBinFileNextFrame(FILE * fp, int numCols, int numRows, CDepthMapSkt * retDepthMap)
{
	int r, c;
	//for(h=0; h<height; h++) //for each row
	int * tempRow = new int[numCols];
	uint8_t* tempRowID = new uint8_t[numCols];
	for (r = 0; r<numRows; r++) //one row at a time
	{
		fread(tempRow, 4, numCols, fp);
		fread(tempRowID, 1, numCols, fp);
		for (c = 0; c<numCols; c++) //for each colume	
		{
			//int temp=0;
			//fread(&temp, 4, 1, fp);
			//retDepthMap.SetItem(r,c,temp);
			int temp = tempRow[c];
			retDepthMap->SetItem(r, c, (float)temp);
			retDepthMap->SetSkeletonID(r, c, tempRowID[c]);
		}
	}
	delete[] tempRow;
	tempRow = NULL;
	delete[] tempRowID;
	tempRowID = NULL;
	return 1;
}

//<fp> must be opened with "wb"
inline int WriteDepthMapSktBinFileHeader(FILE * fp, int nFrames, int nCols, int nRows)
{
	if (fp == NULL)
		return 0;


	fwrite(&nFrames, 4, 1, fp); //read 4 bytes 
	fwrite(&nCols, 4, 1, fp);
	fwrite(&nRows, 4, 1, fp);
	//fscanf(fp, "%i", &retNumFrames);
	//fscanf(fp, "%i", &retWidth);
	//fscanf(fp, "%i", &retHeight);

	return 1;
}

//<fp> must be opened with "wb"
inline int WriteDepthMapSktBinFileNextFrame(FILE * fp, const CDepthMapSkt & depthMap)
{
	int numCols = depthMap.GetNCols();
	int numRows = depthMap.GetNRows();

	int r, c;
	//for(h=0; h<height; h++) //for each row
	int * tempRow = new int[numCols];
	uint8_t* tempRowID = new uint8_t[numCols];
	for (r = 0; r<numRows; r++) //one row at a time
	{
		for (c = 0; c<numCols; c++) //for each colume
		{
			int temp = (int)(depthMap.GetItem(r, c));
			tempRow[c] = temp;
			tempRowID[c] = depthMap.GetSkeletonID(r, c);
		}
		fwrite(tempRow, 4, numCols, fp);
		fwrite(tempRowID, 1, numCols, fp);
	}
	delete[] tempRow;
	tempRow = NULL;
	delete[] tempRowID;
	tempRowID = NULL;
	return 1;
}

inline int ReadSkeletonMap(string fileName, Skeleton * skeleton, int skelCount)
{
	int position = 0;
	ifstream file(fileName);
	string line;
	if (file.is_open())
	{
		getline(file, line);
		int nofs = atoi(line.substr(0, line.find(" ")).c_str());
		while (getline(file, line))
		{
			if (position == skelCount) return 0;
			int noj = atoi(line.c_str());
			for (int i = 0; i < 20; i++)
			{
				getline(file, line);
				istringstream iss(line);
				vector<string> tokens{istream_iterator<string>{iss},
					istream_iterator<string>{} };
				skeleton[position].setPoint(i, stof(tokens.at(0).c_str()), stof(tokens.at(1).c_str()), stof(tokens.at(2).c_str()), true);

				getline(file, line);
				istringstream issImg(line);
				vector<string> tokensImg{ istream_iterator<string>{issImg},
					istream_iterator<string>{} };
				skeleton[position].setPoint(i, stof(tokensImg.at(0).c_str()), stof(tokensImg.at(1).c_str()), stof(tokensImg.at(2).c_str()), false);
				
			}
			for (int i = 40; i < noj; i++)
			{
				getline(file, line);
			}
			
			position++;	
			
		}

	}

	return 1;
}
