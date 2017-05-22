#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "myShader.h"
#include <GL\glew.h>
#include <gtc\matrix_transform.hpp>
#include<glm.hpp>
#include <assimp\types.h>

using namespace std;
using namespace glm;

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh
{
public:
	vector <Vertex> vertices;
	vector <GLuint> indices;
	vector <Texture> textures;

	Mesh(vector<Vertex>, vector<GLuint>, vector<Texture>);
	void Draw(Shader, GLint);
	void Update(float* vertexArray);
private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};