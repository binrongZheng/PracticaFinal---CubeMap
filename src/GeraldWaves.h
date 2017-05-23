#pragma once
#include <vector>
#include "Mesh.h"

class Sea {
public:
	Sea(vector <Vertex> data);
	vector<vec3> Update(GLfloat dt);
private:
	vector <vec3> positions;
	vector <vec3> inicalPositions;
	vector <vec3> ks; //wave vector
	vector <GLfloat> frequencies;
	vector <GLfloat> As; //Amplitud
	vector <GLfloat> phis;
	GLfloat time;
};