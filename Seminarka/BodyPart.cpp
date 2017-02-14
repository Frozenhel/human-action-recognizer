#include "stdafx.h"

BodyPart::BodyPart()
{
}

void BodyPart::setBodyPart(Joint J1, Joint J2, std::string name)
{
	this->J1 = J1;
	this->J2 = J2;
	this->name = name;
}

Joint BodyPart::getJ1() const
{
	return J1;
}

Joint BodyPart::getJ2() const
{
	return J2;
}

std::string BodyPart::getName() const
{
	return name;
}

cv::Point BodyPart::getMiddle2D(int imgRow, int imgCol) const
{
	return cv::Point(abs(J1.getPos2D(imgRow, imgCol).x + J2.getPos2D(imgRow, imgCol).x) / 2, abs(J1.getPos2D(imgRow, imgCol).y + J2.getPos2D(imgRow, imgCol).y) / 2);
}
