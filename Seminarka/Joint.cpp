#include "stdafx.h"

Joint::Joint() : joint_type(0), row(0), col(0), zDepth(0){}

void Joint::setJoint(int type, float row, float col, float zDepth)
{
	this->joint_type = type;
	this->row = row;
	this->col = col;
	this->zDepth = zDepth;
}

cv::Point Joint::getPos2D(int imgRow, int imgCol) const
{
	return cv::Point(row*imgCol, col*imgRow);
}

cv::Vec3f Joint::getPos3D() const
{
	return cv::Vec3f(row, col, zDepth);
}