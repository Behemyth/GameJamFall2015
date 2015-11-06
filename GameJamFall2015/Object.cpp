#include "Object.h"
#include "ObjLoader.h"

Object::Object()
{
	isStatic = false;
	isGhost = false;
	position = glm::mat4();
}


Object::~Object()
{
}

void Object::ExtractFromFile(const char* name){
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, name, NULL);

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	uint overallSize = 0;
	uint faceOverallSize = 0;
	for (uint i = 0; i < shapes.size(); i++){
		overallSize += shapes[i].mesh.positions.size();
		faceOverallSize += shapes[i].mesh.indices.size();
	}


	verticeAmount = overallSize;
	faceAmount = faceOverallSize;


	uint overallOffset = 0;
	uint faceOffset = 0;

	for (uint i = 0; i < shapes.size(); i++){
		for (uint v = 0; v < verticeAmount / 3; v++){
			vertices[overallOffset + v].SetData(
				glm::vec3(shapes[i].mesh.positions[3 * v + 0], shapes[i].mesh.positions[3 * v + 1], shapes[i].mesh.positions[3 * v + 2]),
				glm::vec2(shapes[i].mesh.texcoords[2 * v + 0], shapes[i].mesh.texcoords[2 * v + 1]),
				glm::vec3(shapes[i].mesh.normals[3 * v + 0], shapes[i].mesh.normals[3 * v + 1], shapes[i].mesh.normals[3 * v + 2]));
		}
		overallOffset += shapes[i].mesh.positions.size();

		for (uint f = 0; f < faceAmount / 3; f++){
			faces[faceOffset + f].SetData(
				glm::uvec3(shapes[i].mesh.indices[3 * f + 0], shapes[i].mesh.indices[3 * f + 1], shapes[i].mesh.indices[3 * f + 2]),
				shapes[i].mesh.material_ids[f]);
		}
		faceOffset += shapes[i].mesh.indices.size();
	}
}


void Object::Draw(Camera& camera)
{
	if (shader == NULL){
		std::cout << "Forgot to call Load()" << std::endl;
	}
		shader->use();
		glBindVertexArray(vao);
		shader->setUniform(cameraUniform, camera.matrix());
		//shader->setUniform(cameraUniform, glm::ortho(0.0f, 1.0f*METER, 0.0f, 1.0f*METER, 2.0f*METER, -2.0f*METER));//camera.matrix() otherwise
		shader->setUniform(posUniform, position);
		glDrawElements(GL_TRIANGLES, (indices.size() * 3), GL_UNSIGNED_INT, (GLvoid*)0);
		glBindVertexArray(0);
		shader->stopUsing();
}
void Object::Load(){

	if (!isStatic){
		btTransform trans;
		trans.setFromOpenGLMatrix(glm::value_ptr(position));
		btDefaultMotionState* motState =
			new btDefaultMotionState(trans);
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		shape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motState, shape, fallInertia);
		rigidBody = new btRigidBody(fallRigidBodyCI);
		rigidBody->setFriction(1.35f);
		if (isGhost){
			rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | rigidBody->CF_NO_CONTACT_RESPONSE);
		}
		world->addRigidBody(rigidBody);
	}
	else{
		btTransform trans;
		trans.setFromOpenGLMatrix(glm::value_ptr(position));
		btDefaultMotionState* motState =
			new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0, motState, shape, btVector3(0, 0, 0));
		rigidBody = new btRigidBody(groundRigidBodyCI);
		rigidBody->setFriction(1.35f);
		if (isGhost){
			rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | rigidBody->CF_NO_CONTACT_RESPONSE);
		}
		world->addRigidBody(rigidBody);
	}

	shader = LoadShaders("vertex-shader[basic].txt", "geometry-shader[basic].txt","fragment-shader[basic].txt");
	cameraUniform = shader->uniform("camera");
	posUniform = shader->uniform("position");


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(shader->attrib("vert_VS_in"));
	glVertexAttribPointer(shader->attrib("vert_VS_in"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(shader->attrib("color_VS_in"));
	glVertexAttribPointer(shader->attrib("color_VS_in"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat)));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index)*indices.size(), &indices.front(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Object::Update(double dt){
	
}
void Object::UpdatePosition(){
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(glm::value_ptr(position));
}
std::vector<Index>& Object::GetIndices(){
	return indices;
}
std::vector<Vertex>& Object::GetVertices(){
	return vertices;
}
btRigidBody* Object::GetRigidBody(){
	return rigidBody;
}
glm::vec3 Object::GetPosition(){
	return glm::vec3(position[3]);
}

glm::mat4 Object::GetMatrix(){
	return position;
}
