#include  "stdafx.h"

Skeleton::Skeleton()
{
	realJoints = new Joint[20];		// 20 body joints real
	realParts = new BodyPart[17];		// 17 body parts
	imgJoints = new Joint[20];		// 20 body joints img
	imgParts = new BodyPart[17];		// 17 body parts
	angles = new float[136];		// 17*16/2 combination of parts
}

void Skeleton::setPoint(int joint_type, float row, float col, float zDepth, bool real) const
{
	if (real) realJoints[joint_type].setJoint(joint_type, row, col, zDepth);
	else  imgJoints[joint_type].setJoint(joint_type, row, col, zDepth);
}

Joint Skeleton::getPoint(int joint_type, bool real) const
{
	if (real) return realJoints[joint_type];
	return imgJoints[joint_type];
}

BodyPart Skeleton::getPart(int part, bool real) const
{
	if (real) return realParts[part];
	return imgParts[part];
}

void setBodyParts(BodyPart * part, Joint * joints)
{
	part[0].setBodyPart(joints[6], joints[5], "Left Hand"); // todo avg 6/7
	part[1].setBodyPart(joints[5], joints[4], "Left Elbow");
	part[2].setBodyPart(joints[10], joints[9], "Right Hand");// todo avg 10/11
	part[3].setBodyPart(joints[9], joints[8], "Right Elbow");
	part[4].setBodyPart(joints[2], joints[4], "Left Shoudler");
	part[5].setBodyPart(joints[2], joints[8], "Right Shoulder");
	part[6].setBodyPart(joints[3], joints[2], "Head");
	part[7].setBodyPart(joints[2], joints[1], "Torso");
	part[8].setBodyPart(joints[4], joints[1], "Left Torso");
	part[9].setBodyPart(joints[8], joints[1], "Right Torso");
	part[10].setBodyPart(joints[1], joints[12], "Left Hip");
	part[11].setBodyPart(joints[1], joints[16], "Right Hip");
	part[12].setBodyPart(joints[12], joints[16], "Hip");
	part[13].setBodyPart(joints[12], joints[13], "Left Knee");
	part[14].setBodyPart(joints[13], joints[14], "Left Foot"); // todo avg 14/15
	part[15].setBodyPart(joints[16], joints[17], "Right Knee");
	part[16].setBodyPart(joints[17], joints[18], "Right Foot"); // todo avg 17/18

}

void Skeleton::bodyPartsInit() const
{
	setBodyParts(realParts, realJoints);
	setBodyParts(imgParts, imgJoints);
}

float* Skeleton::calculateAnglesBtwParts() const
{
	int position = 0;
	for (int i = 0; i < 17; i++)
	{
		for (int j = i + 1; j < 17; j++)
		{
			cv::Vec3f J1 = realParts[i].getJ1().getPos3D();
			cv::Vec3f J2 = realParts[i].getJ2().getPos3D();
			cv::Vec3f J3 = realParts[j].getJ1().getPos3D();
			cv::Vec3f J4 = realParts[j].getJ2().getPos3D();
			cv::Vec3f J12 = J1 - J2;
			cv::Vec3f J34 = J3 - J4;
			J12 = cv::normalize(J12);
			J34 = cv::normalize(J34);
			float angle = acos(J12.dot(J34));
			angles[position++] = angle;
		}
	}
	return angles;
}

float Skeleton::getAngleBtwParts(int part1, int part2) const
{
	int adress = 0;
	int noF = 17;
	for (int i = 0; i < part1; i++) adress += noF--;
	adress += part2 - part1;
	return angles[adress];
}