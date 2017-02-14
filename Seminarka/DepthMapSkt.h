#ifndef CLASS_DEPTH_MAP
#define CLASS_DEPTH_MAP

#include <stdint.h>
#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned char uchar;
//a depthmap is a matrix of depth values. A depth value is invalid if it is equal to 0 (i.e. the sensor has no data on this pixel).
class CDepthMapSkt
{
public:
	CDepthMapSkt(void);
	CDepthMapSkt(int ncols, int nrows, uchar* raw_data);
	~CDepthMapSkt(void);

	int GetNRows() const { return m_nrows; }
	int GetNCols() const { return m_ncols; }
	void GetCoords(int r, int c, float retCoord[3]) const;
	void SetSize(int ncols, int nrows); //allocate space

	void SetItem(int r, int c, float val) const;
	float GetItem(int r, int c) const;
	void SetSkeletonID(int r, int c, uint8_t val) const;
	uint8_t GetSkeletonID(int r, int c) const;
	void SetSkeletonImage(int r, int c, uint8_t val) const;
	void CopyDepthMapTo(CDepthMapSkt & dstMap) const;
	void convertToChar(uchar* dst) const;
	uint8_t GetSkeletonImage(int r, int c) const;
	//assume the memory has been located
	void Scale(float s) const; //scale the depth
	void ScaleSizeNN(float scaleFactor); //ncols = floor(scaleFactor * ncols), nrows = floor(scaleFactor * nrows)
	void convertToInt(uint8_t* dst) const;
	int NumberOfNonZeroPoints() const;
	float AvgNonZeroDepth() const;
	float* GetPtr() const;

	int NumPointsWithSkeletonID(uint8_t val) const;
protected:
	int m_ncols; //ncols
	int m_nrows; //nrows
	float * m_depthVals; //nrows * ncols
	uint8_t* m_skIDVals; //nrows * ncols	-- real
	uint8_t* m_skImgVals; //nrows * ncols	-- monitor
};
#endif

inline CDepthMapSkt::CDepthMapSkt(void)
{
	m_ncols = 0;
	m_nrows = 0;
	m_depthVals = NULL;
	m_skIDVals = NULL;
	m_skImgVals = NULL;
}

//from pixel+depth to XYZ coordinates. Make sure this is done consistently everywhere.
inline void CDepthMapSkt::GetCoords(int r, int c, float retCoord[3]) const
{
	//retCoord[0] = c;
	//retCoord[1] = r;//m_nrows-1-r; 
	retCoord[0] = r;
	retCoord[1] = c;
	retCoord[2] = GetItem(r, c);
}

inline CDepthMapSkt::CDepthMapSkt(int ncols, int nrows, uchar* raw_data): m_skIDVals(nullptr)
{
	m_nrows = nrows;
	m_ncols = ncols;

	m_depthVals = new float[nrows*ncols];
	uint16_t *data = (uint16_t*)raw_data;
	for (int i = 0; i<nrows; i++)
	for (int j = 0; j<ncols; ++j)
	{
		m_depthVals[i*ncols + j] = data[i*ncols + j];
	}
}

inline CDepthMapSkt::~CDepthMapSkt(void)
{
	if (m_depthVals)
	{
		delete[] m_depthVals;
		delete[] m_skIDVals;
		delete[] m_skImgVals;
		m_depthVals = NULL;
		m_skIDVals = NULL;
		m_skImgVals = NULL;
	}
}

inline void CDepthMapSkt::SetSize(int ncols, int nrows) //allocate space
{
	if (m_depthVals != NULL)
	{
		delete[] m_depthVals;
		m_depthVals = NULL;
	}
	if (m_skIDVals != NULL)
	{
		delete[] m_skIDVals;
		m_skIDVals = NULL;
	}
	if (m_skImgVals != NULL)
	{
		delete[] m_skImgVals;
		m_skImgVals = NULL;
	}
	m_nrows = nrows;
	m_ncols = ncols;

	m_depthVals = new float[nrows*ncols];
	m_skIDVals = new uint8_t[nrows*ncols];
	m_skImgVals = new uint8_t[nrows*ncols];
	return;
}

inline void CDepthMapSkt::SetItem(int r, int c, float val) const
{
	m_depthVals[r * m_ncols + c] = val;
}

inline float CDepthMapSkt::GetItem(int r, int c) const
{

	return (uint16_t)(m_depthVals[r * m_ncols + c]);
}

inline void CDepthMapSkt::SetSkeletonID(int r, int c, uint8_t val) const
{
	m_skIDVals[r * m_ncols + c] = val;
}

inline uint8_t CDepthMapSkt::GetSkeletonID(int r, int c) const
{
	return ((uint8_t)(m_skIDVals[r * m_ncols + c]));
}

inline void CDepthMapSkt::SetSkeletonImage(int r, int c, uint8_t val) const
{
	m_skImgVals[r * m_ncols + c] = val;
}

inline uint8_t CDepthMapSkt::GetSkeletonImage(int r, int c) const
{
	return ((uint8_t)(m_skImgVals[r * m_ncols + c]));
}

//scale the depth values
inline void CDepthMapSkt::Scale(float s) const
{
	int r, c;
	for (r = 0; r<GetNRows(); r++)
	{
		for (c = 0; c<GetNCols(); c++)
		{
			float temp = GetItem(r, c);
			SetItem(r, c, temp*s);
		}
	}
}

//scale the dimensions:
//ncols = floor(scaleFactor * ncols), nrows = floor(scaleFactor * nrows)
inline void CDepthMapSkt::ScaleSizeNN(float scaleFactor)
{
	//buffer the original depthmap
	CDepthMapSkt bufDepthMap;
	CopyDepthMapTo(bufDepthMap);

	int new_ncols = floor(m_ncols * scaleFactor);
	int new_nrows = floor(m_nrows * scaleFactor);

	SetSize(new_ncols, new_nrows);

	int r, c;
	for (r = 0; r<GetNRows(); r++)
	{
		for (c = 0; c<GetNCols(); c++)
		{
			int origR = floor(r / scaleFactor);
			int origC = floor(c / scaleFactor);
			float origVal = bufDepthMap.GetItem(origR, origC);
			SetItem(r, c, origVal);
		}
	}
}

inline void CDepthMapSkt::CopyDepthMapTo(CDepthMapSkt & dstMap) const
{
	dstMap.SetSize(m_ncols, m_nrows);
	int r, c;
	for (r = 0; r<GetNRows(); r++)
	{
		for (c = 0; c<GetNCols(); c++)
		{
			float temp = GetItem(r, c);
			dstMap.SetItem(r, c, temp);
		}
	}
}

inline int CDepthMapSkt::NumberOfNonZeroPoints() const
{
	int count = 0;
	int r, c;
	for (r = 0; r<GetNRows(); r++)
	{
		for (c = 0; c<GetNCols(); c++)
		{
			float temp = GetItem(r, c);
			if (temp != 0)
			{
				count++;
			}
		}
	}
	return count;
}

inline float CDepthMapSkt::AvgNonZeroDepth() const
{
	int count = 0;
	float sum = 0;
	int r, c;
	for (r = 0; r<GetNRows(); r++)
	{
		for (c = 0; c<GetNCols(); c++)
		{
			float temp = GetItem(r, c);
			if (temp != 0)
			{
				count++;
				sum += temp;
			}
		}
	}
	if (count == 0)
		return 0;
	else
		return sum / count;
}

inline void CDepthMapSkt::convertToChar(uchar* dst) const
{
	for (int i = 0; i<m_nrows; i++)
	for (int j = 0; j<m_ncols; ++j)
	{
		dst[i*m_ncols + j] = (uint8_t)(m_depthVals[i*m_ncols + j] / 16);
	}
}

inline void CDepthMapSkt::convertToInt(uint8_t* dst) const
{
	uint16_t * dst_int = (uint16_t*)dst;
	for (int i = 0; i<m_nrows; i++)
	for (int j = 0; j<m_ncols; ++j)
	{
		dst_int[i*m_ncols + j] = ((uint16_t)(m_depthVals[i*m_ncols + j]) & 0xfff8) >> 3;
	}
}

inline float* CDepthMapSkt::GetPtr() const
{
	return m_depthVals;
}

inline int CDepthMapSkt::NumPointsWithSkeletonID(uint8_t val) const
{
	int count = 0;
	for (int i = 0; i<m_nrows; i++)
	for (int j = 0; j<m_ncols; ++j)
	{
		if (this->GetSkeletonID(i, j) == val)
		{
			count++;
		}
	}

	return count;
}