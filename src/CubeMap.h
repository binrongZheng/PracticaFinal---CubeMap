#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <SOIL.h>
#include "myShader.h"
#include <vector>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>

using namespace std;
using namespace glm;

class CubeMap
{
public:
	CubeMap(GLchar *RightPath, GLchar*LeftPath, GLchar *TopPath, GLchar*BottomPath, GLchar *BackPath, GLchar*FrontPath);
	~CubeMap();
	GLuint loadCubemap();
	void draw(Shader *shad);
	void pushTexture();

private:
	vector <const GLchar*> faces;
	GLuint texture;
	GLuint VBO, VAO, EBO;
	GLuint cubemapTexture;
};

