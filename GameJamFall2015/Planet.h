#pragma once
#include <stdlib.h>
#include <iostream>
#include "BasicIncludes.h"
#include "Object.h"
#include "Perlin.h"
#include <cmath>
#define _USE_MATH_DEFINES

class Planet: public Object
{
public:
	Planet(btDiscreteDynamicsWorld*, glm::vec3);
	~Planet();

	float* continuousHeightData;
	void UpdatePosition();
	glm::vec3 translate;
	float rotateAngle;
	float rotateRate;

	float orbitAngle;
	float orbitRate;


	/*lat = py / height * 180- 90;
lon = px / width * 360-180;
r = Math.cos(DEG_TO_RAD *  lat);

//range between 0-1
_x = (r * Math.cos (DEG_TO_RAD * lon) +1)*.5
_y = (Math.sin (DEG_TO_RAD * lat)+1)*.5
_z = (r * Math.sin(DEG_TO_RAD * lon)+1)*.5*/
private:
	int minTemp;		//minimum temperature: -50 to 100 degrees C
	int maxTemp;		//maximum temperature: 0 to 150 degrees C
	int tempRange;		//max temp - mintemp

};

