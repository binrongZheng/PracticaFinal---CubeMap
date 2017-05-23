#include "2dTexture.h"

TextureIcon::TextureIcon(char *Path, vec3 scales, vec3 rotations, vec3 positions) {

	position = positions;
	scale = scales;
	rotation = rotations;

	GLfloat vertices[] = {
		// Positions          // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = { 
		0, 1, 3, 
		1, 2, 3  
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// ====================
	// Texture
	// ====================
	glGenTextures(1, &Text);
	glBindTexture(GL_TEXTURE_2D, Text); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(Path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 
}

void TextureIcon::Move(vec3 translation) {
	position = translation;
}

void TextureIcon::Rotate(GLfloat rotaX, GLfloat rotaY) {
	angleX = rotaX;
	angleY = rotaY;
}

void TextureIcon::Scale(vec3 scal) {
	scale = scal;
}

mat4 TextureIcon::GetModelMatrix() {
	Model = mat4();
	Model = translate(Model, position);
	Model = glm::scale(Model, scale);
	Model = rotate(Model, radians(angleX), { 0,1,0 });
	Model = rotate(Model, radians(angleY), { 1,0,0 });

	return Model;
}

TextureIcon::~TextureIcon(){
	glDeleteTextures(1, &Text);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
void TextureIcon::ActiveTexture(Shader *shad){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text);
	glUniform1i(glGetUniformLocation(shad->Program, "ourTexture1"), 0);
}
void TextureIcon::Draw() {
	//pintar el VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}