#pragma once

//a depthmap is a matrix of depth values. A depth value is invalid if it is equal to 0 (i.e. the sensor has no data on this pixel).
class BodyPart
{
private:
	Joint J1,J2;
	std::string name;
	
public:
	BodyPart();
	void setBodyPart(Joint J1, Joint J2, std::string name);
	Joint getJ1() const;
	Joint getJ2() const;
	std::string getName() const;
	cv::Point getMiddle2D(int imgRow, int imgCol) const;
};
