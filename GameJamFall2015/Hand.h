#pragma once
#include "Object.h"

class Hand: public Object
{
public:
	Hand(btDiscreteDynamicsWorld*);
	~Hand();

	void Update(double);
private:

};

