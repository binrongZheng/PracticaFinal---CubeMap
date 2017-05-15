#include "material.h"

Material::Material(char *DiffPath,char*SpectPath, float Shini){
	Shininess=Shini;
	glGenTextures(1, &TextSpec);
	glBindTexture(GL_TEXTURE_2D, TextSpec);
	glGenTextures(1, &TextDiff);
	glBindTexture(GL_TEXTURE_2D, TextDiff);
	int width, height;
	//diffuse
	unsigned char* imageDiff;
	imageDiff = SOIL_load_image(DiffPath, &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, TextDiff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageDiff);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageDiff);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//spect
	unsigned char* imageSpec;
	imageSpec = SOIL_load_image(SpectPath, &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, TextSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageSpec);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageSpec);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}


Material::~Material(){
	glDeleteTextures(1, &TextDiff);
	glDeleteTextures(1, &TextSpec);
}

void Material::SetMaterial(Shader *shad) {
	glUniform1i(glGetUniformLocation(shad->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shad->Program, "material.specular"), 1);
}

void Material::SetShininess(Shader *shad) {
	GLfloat variableShininess = glGetUniformLocation(shad->Program, "material.shininess");
	glUniform1f(variableShininess, Shininess);
}

void Material::ActivateTextures() {	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextDiff);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextSpec);
}
