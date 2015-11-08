#include "Planet.h"
#include <math.h>

Planet::Planet(btDiscreteDynamicsWorld* worldN, glm::vec3 offset)
{
	translate = offset;
	isGhost = true;
	rotateAngle = 0;
	rotateAxisAngle = rand() % 360;

	std::normal_distribution<float> rotateDistro(.05, .2);
	rotateRate = GetDistribution(rotateDistro);

	orbitAngle = 0;
	std::normal_distribution<float> orbitDistro(.02, .05);
	orbitRate = GetDistribution(orbitDistro);

	world = worldN;
	
	std::normal_distribution<float> freqDistro(.01, .15);
	float freq = GetDistribution(freqDistro);

	std::normal_distribution<float> ampDistro(.8, 1.2);
	float amp = GetDistribution(ampDistro);

	Perlin* perlin = new Perlin(5, freq, amp*METER, time(NULL));

	std::uniform_int_distribution<int> minDistro(-50,100);
	int minTemp = GetDistribution(minDistro);
	std::uniform_int_distribution<int> maxDistro(minTemp * (minTemp >= 0), 100);
	int maxTemp = GetDistribution(maxDistro);
	int width = 20;
	float fwidth = (float)width;

	isStatic = true;
	world = worldN; 

	continuousHeightData = new float[width*width];

	bool isStar = false;
	if (rand() % 100 < 0) {
		isStar = true;
	}


	std::uniform_int_distribution<int> sizeDistro(30, 150);
	int size = GetDistribution(sizeDistro);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {

			/*float x = ((i / (width - 1.0f)) - (0.5f))*KILOMETER;
			float z = ((j / (width - 1.0f)) - (0.5f))*KILOMETER;
			float y = perlin->Get(i, j); */
			float R = 1./(float)(width - 1);
			float S = 1./(float)(width - 1);

			float height = METER * perlin->Get(abs(i - ((fwidth - 1) / 2)), abs(j - ((fwidth - 1) / 2)));
				/* (1/((fwidth-1)/2)) * ((i * (i <= ((fwidth-1)/2))) + ((width-i) * (i > (fwidth-1)/2)) ); */


			if(isStar) {
				std::normal_distribution<float> sizeDistro(30, 70);
				size = GetDistribution(sizeDistro);
			}

			float y = size * METER * sin(-(M_PI_2)+M_PI * i * R);
			float x = size * METER * cos(2 * M_PI * j * S) * sin(M_PI * i * R);
			float z = size * METER * sin(2 * M_PI * j * S) * sin(M_PI * i * R);

			glm::vec3 center = glm::vec3(0, 0, 0);
			glm::vec3 vec1 = glm::vec3(x, y, z) - center;
			glm::vec3 unitVec = glm::vec3(vec1.x / glm::length(vec1), vec1.y / glm::length(vec1), vec1.z / glm::length(vec1));
			glm::vec3 heightVec = unitVec * height;

			/*float angle = glm::dot(center, vec1);
			angle = angle / (glm::length(center) * glm::length(vec1));
			angle = acos(angle);*/

			y += heightVec.y;
			x += heightVec.x;
			z += heightVec.z;
			
			GetVertices().push_back({ { x, y, z }, { i/((float)width/2.0), j/((float)width/2.0) }, { vec1.x, vec1.y, vec1.z } });

			continuousHeightData[i + j*width] = y;
		}
	}

	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			GetIndices().push_back({ glm::uvec3((i + (j*width)), (i + (j*width)) + 1, i + (1 + j)*(width)) });
			GetIndices().push_back({ glm::uvec3((i + (j*width)) + 1, i + (1 + j)*(width)+1, i + (1 + j)*(width)) });
		}
	}


	//ExtractFromFile("Box.obj");

	std::uniform_int_distribution<int> colorDistro(0, 3);
	int color = GetDistribution(colorDistro);
	if (color == 0) {
		textureName = "meteor.png";
	}
	else if (color == 1) {
		textureName = "meteor2.png";
	}
	else if (color == 2) {
		textureName = "meteor3.png";
	}
	else {
		textureName = "meteor4.png";
	}
	if (isStar) {
		textureName = "fire.jpg";
	}
	float height = 5;
	shape = new btBoxShape(btVector3(height / 2.0f, height / 2.0f, height / 2.0f));

	Load();
}


Planet::~Planet()
{
}

void Planet::UpdatePosition() {
	rotateAngle += rotateRate;
	if (rotateAngle > 360) {
		rotateAngle = 0;
	}
	orbitAngle += orbitRate;
	if (orbitAngle > 360) {
		orbitAngle = 0;
	}

	position = glm::rotate(glm::mat4(), glm::radians(orbitAngle), glm::vec3(cos(rotateAxisAngle), 0.0f, sin(rotateAxisAngle)));
	position = glm::translate(position, translate);
	position = glm::rotate(position, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
}