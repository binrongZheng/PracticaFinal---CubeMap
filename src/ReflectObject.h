#include <gtc\matrix_transform.hpp>
using namespace glm;
#include "myShader.h"

class ReflectObject {

public:
	mat4 Model;

	enum FigureType {
		cube = 0,
	};

	ReflectObject(vec3 scale, vec3 rotation, vec3 position, FigureType typef, GLuint textureDay, GLuint textureNight);
	~ReflectObject();

	void Draw(Shader *shad);
	void Move(vec3 translation);
	void Rotate(GLfloat rotaX, GLfloat rotaY);
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix();
	vec3 GetPosition();

private:
	GLuint VBO, VAO;
	vec3 position;
	vec3 scale;
	vec3 rotation;
	GLfloat angleX;
	GLfloat angleY;
	GLuint day, night;
};

