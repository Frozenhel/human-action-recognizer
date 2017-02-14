#pragma once

class Skeleton
{
private:
	Joint *realJoints;
	BodyPart * realParts;
	Joint *imgJoints;
	BodyPart * imgParts;
	float *angles;

public:
	Skeleton();
	void setPoint(int joint_type, float row, float col, float zDepth, bool real) const;
	Joint getPoint(int joint_type, bool real) const;
	BodyPart getPart(int part, bool real) const;
	void bodyPartsInit() const;
	float * calculateAnglesBtwParts() const;
	float getAngleBtwParts(int part1, int part2) const;
};

