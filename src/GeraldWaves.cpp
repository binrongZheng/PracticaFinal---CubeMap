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
	As.push_back(0.2);
	As.push_back(0.5);
	frequencies.push_back(1 / 3.141516);
	frequencies.push_back(1 / (2 * 3.141516));
	phis.push_back(0.25);
	phis.push_back(0.5);	
	
	cout << positions.size() << endl;
}

vector<vec3> Sea::Update(GLfloat dt) {
	
	GLint i = 6;
	GLint fila = 1;

	time += dt;	
	while (i < 14 * 13 * 6) {

		vec3 sumatoriXZ;
		GLfloat sumatoriY = 0;

		//Fem el sumatori
		for (int j = 0; j < 2;j++) {
			GLfloat w = 2 * 3.141516*frequencies[j];
			sumatoriXZ += normalize(ks[j])*As[j] * sin(dot(ks[j], inicalPositions[i]) - w*time + phis[j]);
			sumatoriY += As[j] * cos(dot(ks[j], inicalPositions[i]) - w*time + phis[j]);
		}
		positions[i].x = (inicalPositions[i] - sumatoriXZ).x;
		positions[i].y = inicalPositions[i].y + sumatoriY;
		positions[i].z = (inicalPositions[i] - sumatoriXZ).z;
		positions[i - 4].x = (inicalPositions[i - 4] - sumatoriXZ).x;
		positions[i - 4].y = inicalPositions[i - 4].y + sumatoriY;
		positions[i - 4].z = (inicalPositions[i - 4] - sumatoriXZ).z;
		positions[i - 2].x = (inicalPositions[i - 2] - sumatoriXZ).x;
		positions[i - 2].y = inicalPositions[i - 2].y + sumatoriY;
		positions[i - 2].z = (inicalPositions[i - 2] - sumatoriXZ).z;
		positions[(i - 3) + 14 * 6].x = (inicalPositions[(i - 3) + 14 * 6] - sumatoriXZ).x;
		positions[(i - 3) + 14 * 6].y = inicalPositions[(i - 3) + 14 * 6].y + sumatoriY;
		positions[(i - 3) + 14 * 6].z = (inicalPositions[(i - 3) + 14 * 6] - sumatoriXZ).z;
		positions[(i - 5) + 15 * 6].x = (inicalPositions[(i - 5) + 15 * 6] - sumatoriXZ).x;
		positions[(i - 5) + 15 * 6].y = inicalPositions[(i - 5) + 15 * 6].y + sumatoriY;
		positions[(i - 5) + 15 * 6].z = (inicalPositions[(i - 5) + 15 * 6] - sumatoriXZ).z;
		positions[(i - 1) + 15 * 6].x = (inicalPositions[(i - 1) + 15 * 6] - sumatoriXZ).x;
		positions[(i - 1) + 15 * 6].y = inicalPositions[(i - 1) + 15 * 6].y + sumatoriY;
		positions[(i - 1) + 15 * 6].z = (inicalPositions[(i - 1) + 15 * 6] - sumatoriXZ).z;
		
		i += 6;
		if (i == 14 * 6 * fila) {
			i += 6;
			fila += 1;
		}
	}	
	
	return positions;
	
}