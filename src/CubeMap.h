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
	CubeMap(GLchar *RightPath1, GLchar*LeftPath1, GLchar *TopPath1, GLchar*BottomPath1, GLchar *BackPath1, GLchar*FrontPath1,
		    GLchar *RightPath2, GLchar*LeftPath2, GLchar *TopPath2, GLchar*BottomPath2, GLchar *BackPath2, GLchar*FrontPath2);
	~CubeMap();
	GLuint loadCubemap(vector <const GLchar*> face);
	void draw(Shader *shad);

private:
	vector <const GLchar*> face1,face2;
	GLuint textureDay,textureNight;
	GLuint VBO, VAO, EBO;
	GLuint cubemapTexture1, cubemapTexture2;
};

