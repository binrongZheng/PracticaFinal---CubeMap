#include "CubeMap.h"

CubeMap::CubeMap(GLchar *RightPath1, GLchar*LeftPath1, GLchar *TopPath1, GLchar*BottomPath1, GLchar *BackPath1, GLchar*FrontPath1,
				 GLchar *RightPath2, GLchar*LeftPath2, GLchar *TopPath2, GLchar*BottomPath2, GLchar *BackPath2, GLchar*FrontPath2)
{
	//añadir las texturas
	
	face1.push_back(RightPath1);
	face1.push_back(LeftPath1);
	face1.push_back(TopPath1);
	face1.push_back(BottomPath1);
	face1.push_back(BackPath1);
	face1.push_back(FrontPath1);

	face2.push_back(RightPath2);
	face2.push_back(LeftPath2);
	face2.push_back(TopPath2);
	face2.push_back(BottomPath2);
	face2.push_back(BackPath2);
	face2.push_back(FrontPath2);

	//VAO,EBO,VBO
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//Reservar memoria para el VAO, VBO y EBO
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//Establecer el objeto
	glBindVertexArray(VAO); {
		//Enlazar el buffer con openGL
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		//Establecer las propiedades de los vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);	
	}

	//liberar el buffer de vertices
	glBindVertexArray(0);
	textureDay = loadCubemap(face1);
	textureNight = loadCubemap(face2);
}

CubeMap::~CubeMap(){
	glDeleteTextures(1, &textureDay);
	glDeleteTextures(1, &textureNight);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

//para leer las imatges
GLuint CubeMap::loadCubemap(vector <const GLchar*> face) {
//skyboxDay
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < face.size(); i++)
	{
		image = SOIL_load_image(face[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

//para pintar skybox
void CubeMap::draw(Shader *shad) {
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shad->Program, "day"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureDay);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(shad->Program, "night"), 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureNight);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}