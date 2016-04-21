#ifndef BALL_H
#define BALL_H

#pragma pack(push)
#pragma pack(4)

#include "Vector3D.h"

class Ball
{
public:
	Vector3D position;
	Vector3D velocity;

	float mass;

	bool fixed;

	Vector3D normal;
};

#pragma pack(pop)

#endif