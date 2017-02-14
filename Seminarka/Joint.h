#pragma once
	
class Joint
{
private:
	int joint_type;
	float row;
	float col;
	float zDepth;
public:
	Joint();
	void setJoint(int type, float row, float col, float zDepth);
	cv::Point getPos2D(int imgRow, int imgCol) const;
	cv::Vec3f getPos3D() const;
};
