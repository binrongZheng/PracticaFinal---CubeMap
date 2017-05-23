#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "myShader.h"
//para textura
#include <SOIL.h>
//para vector
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class TextureIcon
{
public:
	mat4 Model;
	TextureIcon(char *Path, vec3 scales, vec3 rotations, vec3 positions);
	~TextureIcon();
	void Draw();
	void ActiveTexture(Shader *shad);
	void Move(vec3 translation);
	void Rotate(GLfloat rotaX, GLfloat rotaY);
	void Scale(vec3 scal);
	mat4 GetModelMatrix();
	
private:
	GLuint VAO, VBO, EBO;
	vec3 position, rotation, scale;
	GLfloat angleX, angleY;
	GLuint Text;
};
