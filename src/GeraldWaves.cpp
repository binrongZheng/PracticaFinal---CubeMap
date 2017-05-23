#pragma once

#include "GeraldWaves.h"

Sea::Sea(vector <Vertex> data) {
	for (int i = 0; i < data.size();i++) {
		positions.push_back (data[i].Position);		
	}
	inicalPositions = positions;

	time = 0;

	ks.push_back({ 1,0,0 });
	ks.push_back({ 0,0,1 });
	As.push_back(0.4);
	As.push_back(0.7);
	frequencies.push_back(1 / 3.141516);
	frequencies.push_back(1 / (2 * 3.141516));
	phis.push_back(0.25);
	phis.push_back(0.5);	
	
	cout << positions.size() << endl;
}

vector<vec3> Sea::Update() {
	time = glfwGetTime();
	vec3 sumatoriXZ;
	GLfloat sumatoriY = 0;

	//para cada vertice calculamos posiciones
	for (int i = 0; i < positions.size();i++) {	
		//Fem el sumatori
		for (int j = 0; j < 2;j++) {
			GLfloat w = 2 * 3.141516*frequencies[j];
			sumatoriXZ += normalize(ks[j])*As[j] * sin(dot(ks[j], inicalPositions[i]) - w*time + phis[j]);
			sumatoriY += As[j] * cos(dot(ks[j], inicalPositions[i]) - w*time + phis[j]);
		}

		positions[i].x = (inicalPositions[i] - sumatoriXZ).x;
		positions[i].y = inicalPositions[i].y + sumatoriY;
		positions[i].z = (inicalPositions[i] - sumatoriXZ).z;
		
	}
	return positions;
	
}