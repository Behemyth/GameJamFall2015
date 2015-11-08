#pragma once

#include "BasicIncludes.h"
#include "Object.h"
#include "Terrain.h"
#include "Camera.h"

class Rock : public Object
{
private:
	Terrain* terrain;
public:
	Rock(btDiscreteDynamicsWorld*, Terrain*);
	~Rock();
};

