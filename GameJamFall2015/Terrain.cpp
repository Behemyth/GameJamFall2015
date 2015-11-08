#include "Terrain.h"


Terrain::Terrain(btDiscreteDynamicsWorld* worldN, uint widthN, int seed)
{


	float amplitude = 40 * METER;
	Perlin* perlin = new Perlin(5, 0.01, amplitude, seed);
	width = widthN;
	isStatic=true;
	world = worldN;

	continuousHeightData = new float[width*width];

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			float x = ((i / (width - 1.0f)) - (0.5f))*KILOMETER;
			float z = ((j / (width - 1.0f)) - (0.5f))*KILOMETER;
			float y =  perlin->Get(i, j);
			GetVertices().push_back({ { x, y, z }, { (i / (float)width) * 20, (j / (float)width) * 20 }, { 0.0f, 0.0f, 0.0f } });

			continuousHeightData[i + j*width] = y;
		}
	}

	for (int i = 0; i < width-1; i++) {
		for (int j = 0; j < width-1; j++) {
			GetIndices().push_back({ glm::uvec3((i + (j*width)), (i + (j*width)) + 1, i + (1 + j)*(width)) });
			GetIndices().push_back({ glm::uvec3((i + (j*width)) + 1, i + (1 + j)*(width)+1, i + (1 + j)*(width)) });
		}
	}


	for (GLuint i = 0; i < GetVertices().size(); i++){
		glm::vec3 pos= GetVertices().at(i).position;
		float falloff = 50 * METER;
		if (glm::distance(pos, glm::vec3(0))>0.4f*KILOMETER + falloff){
			GetVertices().at(i).position += glm::vec3(0, 50.0f*METER, 0);
		}
		else if (glm::distance(pos,glm::vec3(0))>0.4f*KILOMETER){
			float diff = glm::distance(pos, glm::vec3(0)) - 0.4f*KILOMETER;
			float falloff = 50 * METER;
			GetVertices().at(i).position += glm::vec3(0, 50.0f*METER*(diff / falloff), 0);

		}
		
	}


	GetNormals();

	shape = new btHeightfieldTerrainShape(width, width, continuousHeightData, 1.0f, -amplitude, amplitude, 1, PHY_FLOAT, false);

	btVector3 localScaling = btVector3(KILOMETER / (float)width, 1.0f, KILOMETER / (float)width);
	shape->setLocalScaling(localScaling);

	Load(); //loads drawing related stuff. Call after vertices/indices have been defined
}

//uses bilinear interpolation
float Terrain::GetHeight(float x, float y){

	float newX = (x + (KILOMETER / 2.0f)) / (KILOMETER/(float)width);
	float newY = (y + (KILOMETER / 2.0f)) / (KILOMETER / (float)width);

	uint px = glm::floor(newX);
	uint py = glm::floor(newY);

	float xFrac = glm::fract(newX);
	float yFrac = glm::fract(newY);

	float p1 = continuousHeightData[(py)*width + (px)];
	float p2 = continuousHeightData[(py)*width + (px+1)];
	float p3 = continuousHeightData[(py + 1) * width + (px)];
	float p4 = continuousHeightData[(py + 1) * width + (px + 1)];

	// Calculate the weights for each pixel
	float r1 = glm::mix(p1, p2, xFrac);
	float r2 = glm::mix(p3, p4, xFrac);

	return glm::mix(r1, r2, yFrac);

}
void Terrain::GetNormals(){


	for (GLuint i = 0; i < GetIndices().size(); i++){
		glm::uvec3 ind = GetIndices().at(i).indices;
		glm::vec3 V1 = (GetVertices().at(ind.y).position - GetVertices().at(ind.x).position);
		glm::vec3 V2 =(GetVertices().at(ind.z).position - GetVertices().at(ind.x).position);


		glm::vec3 surfaceNormal;

		surfaceNormal = glm::cross(glm::vec3(V1.x, V1.y, V1.z), glm::vec3(V2.x, V2.y, V2.z));

		GetVertices().at(ind.x).normal += surfaceNormal;
		GetVertices().at(ind.y).normal += surfaceNormal;
		GetVertices().at(ind.z).normal += surfaceNormal;
	}

	for (GLuint i = 0; i < GetVertices().size(); i++){
		glm::normalize(GetVertices().at(i).normal);
	}
}
Terrain::~Terrain()
{
}
