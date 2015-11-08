#pragma once
#include "Object.h"

class Skybox: public Object
{
public:
	Skybox(btDiscreteDynamicsWorld*);
	~Skybox();

	void Update(double);
private:

};

