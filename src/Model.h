#pragma once
#include "Mesh.h"
#include <SOIL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Model
{
public:
	Model();
	Model(GLchar*);
	void Draw(Shader, GLint);
	void Update(float* aray_data);
	vector <Vertex> GetVertexArray();
private:
	vector<Vertex> vertices;
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;
	void loadModel(string);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, string);
	GLint TextureFromFile(const char*, string);


};