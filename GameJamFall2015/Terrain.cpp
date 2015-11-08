#include "Terrain.h"


Terrain::Terrain(btDiscreteDynamicsWorld* worldN, float radiusN,uint widthN, int seed)
{

	radius = radiusN;
	float amplitude = 20 * METER;
	Perlin* perlin = new Perlin(7, 0.01, amplitude, seed);
	width = widthN;
	isStatic=true;
	world = worldN;

	displacementName = "asteroid_DISP.png";
	normalName = "asteroid_NRM.png";

	continuousHeightData = new float[width*width];

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			float x = ((i / (width - 1.0f)) - (0.5f))*radius;
			float z = ((j / (width - 1.0f)) - (0.5f))*radius;
			float y =  perlin->Get(i, j);
			glm::vec3 pos = glm::vec3(x, y, z);
			


			float falloff = radius/20.0f;
			if (glm::distance(pos, glm::vec3(0))>CUTOFF + falloff){
				y += radius / 20.0f;
			}
			else if (glm::distance(pos, glm::vec3(0))>CUTOFF){
				float diff = glm::distance(pos, glm::vec3(0)) - CUTOFF;
				float falloff = radius / 20.0f;
				y += radius / 20.0f*(diff / falloff);

			}

			GetVertices().push_back({ { x, y, z }, { (i / (float)width) * 8, (j / (float)width) * 8 }, { 0.0f, 0.0f, 0.0f } });

			continuousHeightData[i + j*width] = y;
		}
	}

	for (int i = 0; i < width-1; i++) {
		for (int j = 0; j < width-1; j++) {
			GetIndices().push_back({ glm::uvec3((i + (j*width)), (i + (j*width)) + 1, i + (1 + j)*(width)) });
			GetIndices().push_back({ glm::uvec3((i + (j*width)) + 1, i + (1 + j)*(width)+1, i + (1 + j)*(width)) });
		}
	}


	textureName = "asteroid.png";

	GetNormals();

	shape = new btHeightfieldTerrainShape(width, width, continuousHeightData, 1.0f, -amplitude, amplitude, 1, PHY_FLOAT, false);

	btVector3 localScaling = btVector3(radius / (float)width, 1.0f, radius / (float)width);
	shape->setLocalScaling(localScaling);

	Load(); //loads drawing related stuff. Call after vertices/indices have been defined
}

//uses bilinear interpolation
float Terrain::GetHeight(float x, float y){

	float newX = (x + (radius / 2.0f)) / (radius / (float)width);
	float newY = (y + (radius / 2.0f)) / (radius / (float)width);

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
