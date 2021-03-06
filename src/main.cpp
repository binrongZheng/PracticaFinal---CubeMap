﻿//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "myShader.h"
#include "camara.h"
#include "object.h"
#include "ReflectObject.h"
#include "material.h"
#include "Light.h"
#include "CubeMap.h"
#include "Model.h"
#include "GeraldWaves.h"
#include "2dTexture.h"
//para textura
#include <SOIL.h>
//para vector
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;
const GLint WIDTH = 1000, HEIGHT = 1000;
bool WIREFRAME = false;
int Mode = 0;
int ModeRefract= 0;
float ratioRefract = 1.33f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
vec3 mov, rot, scal;
vec3 movement;
vec3 BoatPos = { 0.0f, -7.50f, -50.f };
GLfloat radiansX,radiansY;
GLfloat mixValor=0;
GLfloat Time = 0;
GLfloat radX = 0;
GLfloat radY = 0;

bool hide = false;
bool play = true;

Camera myCamera({ 0,0,3 }, { 0,0,-1 }, 0.05, 45);

//Per MAR
float vertexData[1176 * 3];

GLfloat Deltatime;
GLfloat Lastframe;

void MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
	myCamera.MouseScroll(window, xScroll, yScroll);
};
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void deleteFunction(Object objA, Object objB, Object objC, Object objD, Object objE, ReflectObject refcub, TextureIcon gla, TextureIcon wat, TextureIcon ice, TextureIcon diam, Material mat, CubeMap sky);
int main() {
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al inicializar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	//set function when callback
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, MouseScroll);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//GLFW_CURSOR_DISABLED

	//set windows and viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	//cargamos los shader
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Shader objShader("./src/AssimpVertex.vertexshader", "./src/AssimpFragment.fragmentshader");
	Shader ReceiveShader("./src/ReceiveVertex.vertexshader", "./src/ReceiveFragment.fragmentshader");
	Shader generalLight("./src/VertexShaderPhongTexture.vs", "./src/FragmentShaderPhongTexture.fs");
	Shader CubemapShader("./src/CubemapVertex.vertexshader", "./src/CubemapFragment.fragmentshader");
	Shader ReflectShader("./src/ReflectVertex.vertexshader", "./src/ReflectFragment.fragmentshader");
	Shader RefractShader("./src/RefractVertex.vertexshader", "./src/RefractFragment.fragmentshader");
	Shader TextShader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");

	Model BoatModel("./src/boat/boat.obj");
	Model WaterModel("./src/Water/water.obj");
	
	CubeMap skybox( "./src/skyboxes/day/right.jpg", "./src/skyboxes/day/left.jpg",
				    "./src/skyboxes/day/top.jpg", "./src/skyboxes/day/bottom.jpg",
				    "./src/skyboxes/day/back.jpg", "./src/skyboxes/day/front.jpg",
					"./src/skyboxes/night/right.jpg", "./src/skyboxes/night/left.jpg",
					"./src/skyboxes/night/top.jpg", "./src/skyboxes/night/bottom.jpg",
					"./src/skyboxes/night/back.jpg", "./src/skyboxes/night/front.jpg");

	Material material("./src/difuso.png", "./src/especular.png", 32.0);
	
	Object cubA({ 0.3f,0.3f,0.3f }, { 0.f,0.f,0.0f }, { 0.f,-2.f,0.0f }, Object::cube);
	Object cubB({ 0.1,0.1,0.1 }, { 0.f,0.f,0.0f }, { 0.f,0.3f,0.0f }, Object::cube);
	Object cubC({ 0.1,0.1,0.1 }, { 0.f,0.f,0.0f }, { 1.5f,0.3f,0.0f }, Object::cube);
	Object cubD({ 0.1,0.1,0.1 }, { 0.f,0.f,0.0f }, { 3.f,0.3f,0.0f }, Object::cube);
	Object cubE({ 0.1,0.1,0.1 }, { 0.f,0.f,0.0f }, { 4.5f,0.3f,0.0f }, Object::cube);
	
	TextureIcon water("./src/icon/water.png", { 0.1, 0.1, 0.1 }, { 0.f,0.f,0.0f }, { 0.f,-10.f,0.0f });
	TextureIcon glass("./src/icon/glass.png", { 0.1, 0.1, 0.1 }, { 0.f,0.f,0.0f }, { 0.f,-10.f,0.0f });
	TextureIcon ice("./src/icon/ice.png", { 0.1, 0.1, 0.1 }, { 0.f,0.f,0.0f }, { 0.f,-10.f,0.0f });
	TextureIcon diamond("./src/icon/diamond.png", { 0.1, 0.1, 0.1 }, { 0.f,0.f,0.0f }, { 0.f,-10.f,0.0f });

	GLuint Day = skybox.loadCubemap(skybox.face1);
	GLuint Night = skybox.loadCubemap(skybox.face2);

	ReflectObject reflectCub({ 0.3f,0.3f,0.3f }, { 0.f,0.f,0.0f }, {3.f,-2.f,0.0f }, ReflectObject::cube,Day, Night);

	Light Ldir({ 0.0,0.0,0.0 }, { -0.f, -1.0f, -0.f }, { 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, Light::DIRECTIONAL, 0);
	Light Lpoint1({ cubB.GetPosition().x, cubB.GetPosition().y, cubB.GetPosition().z }, { 0.f, 0.f, 0.f, }, { 0.f, 0.2f, 0.f }, { 0.f, 0.5f, 0.f }, { 0.0f, 1.0f, 0.0f }, Light::POINT, 0);
	Light Lpoint2({ cubD.GetPosition().x, cubD.GetPosition().y, cubD.GetPosition().z }, { 0.f, 0.f, 0.f, }, { 0.0f, 0.0f, 0.2f }, { 0.f, 0.f, 0.5f }, { 0.0f, 0.0f, 1.0f }, Light::POINT, 1);
	Light LFocal1({ cubC.GetPosition().x, cubC.GetPosition().y, cubC.GetPosition().z }, { 0.f, -1.f, 0.f, }, { 0.2f, 0.0f, 0.0f }, { 0.5f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, Light::SPOT, 0);
	Light LFocal2({ cubE.GetPosition().x, cubE.GetPosition().y, cubE.GetPosition().z }, { 1.f, 0.0f, 0.f, }, { 0.2f, 0.07f, 0.0f }, { 0.5f, 0.170f, 0.0f }, { 0.9f, 0.30f, 0.0f }, Light::SPOT, 1);

	Sea mar(WaterModel.GetVertexArray());

	material.SetMaterial(&generalLight);

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Establecer el color de fondo
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//time
		GLfloat currentFrame = glfwGetTime();
		Deltatime = currentFrame - Lastframe;
		Lastframe = currentFrame;
		//establecer el shader

		GLint lightPosLoc, viewPosLoc, lightDirPos, lightColorLoc;
		mat4 proj;		mat4 view;		mat4 model;
		GLint modelLoc, viewLoc, projectionLoc;

		proj = perspective(radians(myCamera.GetFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
		myCamera.DoMovement(window);

		//SKYBOX//
		glDepthMask(GL_FALSE);
		CubemapShader.USE();
		
		if (play == true) {
			Time += Deltatime;
			mixValor = (sin(Time / 2) / 2) + 0.5;		
		}

		view = mat4(mat3(myCamera.LookAt()));
		viewLoc = glGetUniformLocation(CubemapShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(CubemapShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));
		GLfloat variableShader = glGetUniformLocation(CubemapShader.Program, "Valor");
		glUniform1f(variableShader, mixValor);
///////////////////////////////////////////////PINTAR SKYBOX////////////////////////////////////////////////////
		skybox.draw(&CubemapShader);
		glDepthMask(GL_TRUE);

///////////////////////////////////////////////PINTAR MAR///////////////////////////////////////////////////////
		if (hide == false) {
			//pintar MAR
			generalLight.USE();
			view = myCamera.LookAt();

			Ldir.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
			Ldir.setIntensity(&generalLight, mixValor * 2);

			viewLoc = glGetUniformLocation(generalLight.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			projectionLoc = glGetUniformLocation(generalLight.Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

			for (int i = 0; i < 10; i++) {
				vector<vec3> temp = mar.Update(Deltatime / 10);
				for (int i = 0; i < temp.size(); i++) {
					vertexData[i * 3] = temp[i].x;
					vertexData[(i * 3) + 1] = temp[i].y;
					vertexData[(i * 3) + 2] = temp[i].z;
				}
				WaterModel.Update(vertexData);
			}

			model = mat3(1.0);
			model = translate(model, vec3(0, -1.5, 1));
			model = scale(model, glm::vec3(1.f, .5f, 1.f));
			glUniformMatrix4fv(glGetUniformLocation(objShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			WaterModel.Draw(objShader, GL_FILL);
			
		}
		if (hide == true) {
			//pintar MAR
			objShader.USE();
			view = myCamera.LookAt();

			//direccional
			Ldir.SetLight(&objShader, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
			Ldir.setIntensity(&objShader, mixValor * 2);
			//PUNTUAL//
			Lpoint1.SetAtt(1.0f, 0.09, 0.032);
			Lpoint1.SetLight(&objShader, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
			Lpoint1.SetPosition({ cubB.GetPosition().x, cubB.GetPosition().y, cubB.GetPosition().z });

			Lpoint2.SetAtt(1.0f, 0.09, 0.032);
			Lpoint2.SetLight(&objShader, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
			Lpoint2.SetPosition({ cubD.GetPosition().x, cubD.GetPosition().y, cubD.GetPosition().z });
			//FOCAL//
			LFocal1.SetAtt(1.0f, 0.09, 0.032);
			LFocal1.SetAperture(glm::cos(glm::radians(13.5f)), glm::cos(glm::radians(15.5f)));
			LFocal1.SetLight(&objShader, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
			LFocal1.SetPosition({ cubC.GetPosition().x, cubC.GetPosition().y, cubC.GetPosition().z });

			LFocal2.SetAtt(1.0f, 0.09, 0.032);
			LFocal2.SetAperture(glm::cos(glm::radians(13.5f)), glm::cos(glm::radians(15.5f)));
			LFocal2.SetLight(&objShader, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
			LFocal2.SetPosition({ cubE.GetPosition().x, cubE.GetPosition().y, cubE.GetPosition().z });

			viewLoc = glGetUniformLocation(objShader.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			projectionLoc = glGetUniformLocation(objShader.Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

			for (int i = 0; i < 10; i++) {
				vector<vec3> temp = mar.Update(Deltatime / 10);
				for (int i = 0; i < temp.size(); i++) {
					vertexData[i * 3] = temp[i].x;
					vertexData[(i * 3) + 1] = temp[i].y;
					vertexData[(i * 3) + 2] = temp[i].z;
				}
				WaterModel.Update(vertexData);
			}

			model = mat3(1.0);
			model = translate(model, vec3(0, -1.5, 1));
			model = scale(model, glm::vec3(1.f, .5f, 1.f));
			glUniformMatrix4fv(glGetUniformLocation(objShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			WaterModel.Draw(objShader, GL_FILL);
			
		}

///////////////////////////////////REFLECT, REFRACT Y TEXTURE (CUB & BARCO)/////////////////////////////////////
	if (Mode == 0 & hide == false) {		
			generalLight.USE();
			material.SetShininess(&generalLight);
			material.ActivateTextures();
			view = myCamera.LookAt();

				//direccional
				Ldir.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
				Ldir.setIntensity(&generalLight, mixValor * 2);		
				//PUNTUAL//
				Lpoint1.SetAtt(1.0f, 0.09, 0.032);
				Lpoint1.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
				Lpoint1.SetPosition({ cubB.GetPosition().x, cubB.GetPosition().y, cubB.GetPosition().z });

				Lpoint2.SetAtt(1.0f, 0.09, 0.032);
				Lpoint2.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
				Lpoint2.SetPosition({ cubD.GetPosition().x, cubD.GetPosition().y, cubD.GetPosition().z });
				//FOCAL//
				LFocal1.SetAtt(1.0f, 0.09, 0.032);
				LFocal1.SetAperture(glm::cos(glm::radians(13.5f)), glm::cos(glm::radians(15.5f)));
				LFocal1.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
				LFocal1.SetPosition({ cubC.GetPosition().x, cubC.GetPosition().y, cubC.GetPosition().z });

				LFocal2.SetAtt(1.0f, 0.09, 0.032);
				LFocal2.SetAperture(glm::cos(glm::radians(13.5f)), glm::cos(glm::radians(15.5f)));
				LFocal2.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
				LFocal2.SetPosition({ cubE.GetPosition().x, cubE.GetPosition().y, cubE.GetPosition().z });
			
			model = glm::translate(model, cubA.GetPosition());
			model = cubA.GetModelMatrix();

			cubA.Rotate(radiansX, radiansY);
			cubA.Move(movement);

			modelLoc = glGetUniformLocation(generalLight.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
			viewLoc = glGetUniformLocation(generalLight.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			projectionLoc = glGetUniformLocation(generalLight.Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));
			
			//pintar BARCO
			cubA.Draw();

			model = mat3(1.0);

			model = scale(model, glm::vec3(0.1f, 0.1f, -0.1f));
			model = translate(model, BoatPos);

			glUniformMatrix4fv(glGetUniformLocation(generalLight.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			BoatModel.Draw(generalLight, GL_FILL);

		}
	//Per pintar barco i cub
	if (Mode == 0 & hide == true) {
		objShader.USE();
		view = myCamera.LookAt();
		material.SetShininess(&objShader);
		material.ActivateTextures();

		Ldir.SetLight(&objShader, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
		Ldir.setIntensity(&objShader, mixValor * 2);

		model = translate(model, cubA.GetPosition());
		model = cubA.GetModelMatrix();

		cubA.Rotate(radiansX, radiansY);
		cubA.Move(movement);

		modelLoc = glGetUniformLocation(objShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		viewLoc = glGetUniformLocation(objShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(objShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

		//pintar BARCO
		cubA.Draw();

		model = mat3(1.0);

		model = scale(model, glm::vec3(0.1f, 0.1f, -0.1f));
		model = translate(model, BoatPos);

		glUniformMatrix4fv(glGetUniformLocation(objShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		BoatModel.Draw(objShader, GL_FILL);		
	}
	//REFLECT
	if (Mode==1){
	ReflectShader.USE();
	
	view = myCamera.LookAt();
	
	model = glm::translate(model, reflectCub.GetPosition());
	model = reflectCub.GetModelMatrix();
	reflectCub.Rotate(radiansX, radiansY);
	reflectCub.Move(movement);

	modelLoc = glGetUniformLocation(ReflectShader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
	viewLoc = glGetUniformLocation(ReflectShader.Program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	projectionLoc = glGetUniformLocation(ReflectShader.Program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));
	glUniform3f(glGetUniformLocation(ReflectShader.Program, "viewPos"), myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z);
	variableShader = glGetUniformLocation(ReflectShader.Program, "Valor");
	glUniform1f(variableShader, mixValor);

	//pintar el BARCO
	reflectCub.Draw(&ReflectShader);

	model = mat3();
	model = scale(model, glm::vec3(0.1f, 0.1f, -0.1f));
	model = translate(model, BoatPos);
	glUniformMatrix4fv(glGetUniformLocation(ReflectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniform3f(glGetUniformLocation(ReflectShader.Program, "viewPos"), myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z);
	variableShader = glGetUniformLocation(ReflectShader.Program, "Valor");
	glUniform1f(variableShader, mixValor);

	BoatModel.Draw(ReflectShader, GL_FILL);
	}
	//REFRACT
	if (Mode == 2) {
		//TEXTURE icon
		
		TextShader.USE();
		view = myCamera.LookAt();
		
		water.ActiveTexture(&TextShader);
		if (ModeRefract == 0) {
			viewLoc = glGetUniformLocation(TextShader.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			projectionLoc = glGetUniformLocation(TextShader.Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

			model = water.GetModelMatrix();

			water.Move(cubA.GetPosition() + vec3(0.0, -0.25, 0.155));
			water.Rotate(0, 0);
			modelLoc = glGetUniformLocation(TextShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			water.Draw();
		}
		ice.ActiveTexture(&TextShader);
		if (ModeRefract == 1) {
			viewLoc = glGetUniformLocation(TextShader.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			projectionLoc = glGetUniformLocation(TextShader.Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

			model = ice.GetModelMatrix();

			ice.Move(cubA.GetPosition() + vec3(0.0, -0.25, 0.155));
			ice.Rotate(0, 0);
			modelLoc = glGetUniformLocation(TextShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			ice.Draw();
		}
		glass.ActiveTexture(&TextShader);
		if (ModeRefract == 2) {
			viewLoc = glGetUniformLocation(TextShader.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			projectionLoc = glGetUniformLocation(TextShader.Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

			model = glass.GetModelMatrix();

			glass.Move(cubA.GetPosition() + vec3(0.0, -0.25, 0.155));
			glass.Rotate(0, 0);
			modelLoc = glGetUniformLocation(TextShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			glass.Draw();
		}
		diamond.ActiveTexture(&TextShader);
		if (ModeRefract == 3) {
			viewLoc = glGetUniformLocation(TextShader.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			projectionLoc = glGetUniformLocation(TextShader.Program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

			model = diamond.GetModelMatrix();

			diamond.Move(cubA.GetPosition() + vec3(0.0, -0.25, 0.155));
			diamond.Rotate(0, 0);
			modelLoc = glGetUniformLocation(TextShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			diamond.Draw();
		}

		RefractShader.USE();

		view = myCamera.LookAt();

		model = glm::translate(model, reflectCub.GetPosition());
		model = reflectCub.GetModelMatrix();
		reflectCub.Rotate(radiansX, radiansY);
		reflectCub.Move(movement);

		modelLoc = glGetUniformLocation(RefractShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		viewLoc = glGetUniformLocation(RefractShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(RefractShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));
		glUniform3f(glGetUniformLocation(RefractShader.Program, "viewPos"), myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z);
		variableShader = glGetUniformLocation(RefractShader.Program, "Valor");
		glUniform1f(variableShader, mixValor);
		GLuint Ratio = glGetUniformLocation(RefractShader.Program, "Ratio");
		glUniform1f(Ratio, ratioRefract);

		//pintar el BARCO
		reflectCub.Draw(&RefractShader);
		model = mat3();
		model = scale(model, glm::vec3(0.1f, 0.1f, -0.1f));
		model = translate(model, BoatPos);
		glUniformMatrix4fv(glGetUniformLocation(RefractShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		BoatModel.Draw(RefractShader, GL_FILL);
	}
////////////////////////////////////////////////LightsCub///////////////////////////////////////////////////////
		ReceiveShader.USE();

		viewLoc = glGetUniformLocation(ReceiveShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(ReceiveShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

		//pintar el cub
		if (hide == false) {
					
			lightColorLoc = glGetUniformLocation(ReceiveShader.Program, "lightColor");
			glUniform3f(lightColorLoc, 0.0f, 1.0f, 0.0f);
			model = glm::translate(model, cubB.GetPosition());
			model = cubB.GetModelMatrix();
			modelLoc = glGetUniformLocation(ReceiveShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			cubB.Draw();
		
			lightColorLoc = glGetUniformLocation(ReceiveShader.Program, "lightColor");
			glUniform3f(lightColorLoc, 1.0f, 0.0f, 0.0f);
			model = glm::translate(model, cubC.GetPosition());
			model = cubC.GetModelMatrix();
			modelLoc = glGetUniformLocation(ReceiveShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			cubC.Draw();
		
			lightColorLoc = glGetUniformLocation(ReceiveShader.Program, "lightColor");
			glUniform3f(lightColorLoc, 0.0f, 0.0f, 1.0f);
			model = glm::translate(model, cubD.GetPosition());
			model = cubD.GetModelMatrix();
			modelLoc = glGetUniformLocation(ReceiveShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			cubD.Draw();
		
			lightColorLoc = glGetUniformLocation(ReceiveShader.Program, "lightColor");
			glUniform3f(lightColorLoc, 0.90f, 0.30f, 0.0f);
			model = glm::translate(model, cubE.GetPosition());
			model = cubE.GetModelMatrix();
			modelLoc = glGetUniformLocation(ReceiveShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
			cubE.Draw();
		}
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
///////////////////////////////// liberar la memoria de los VAO, EBO y VBO///////////////////////////////////////
	deleteFunction(cubA, cubB, cubC, cubD, cubE, reflectCub, glass, water,ice,diamond, material, skybox);
////////////////////////// Terminate GLFW, clearing any resources allocated by GLFW./////////////////////////////
	exit(EXIT_SUCCESS);
}
/////////////////////////////////////////////TECLAT///////////////////////////////////////////////////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//canviar reflect y textures
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		Mode += 1;
		Mode = Mode%3;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		ModeRefract += 1;
		ModeRefract = ModeRefract % 4;
		switch (ModeRefract)
		{
		//Water
		case 0:			ratioRefract = 1.33f;				break;
		//Ice
		case 1:			ratioRefract = 1.309f;				break;
		//Glass
		case 2:			ratioRefract = 1.52f;				break;
		//Diamond
		case 3:			ratioRefract = 2.42f;				break;
		default:											break;
		}
	}
	//rotar cubo
	if (key == GLFW_KEY_LEFT)		radiansX -= 0.5;
	if (key == GLFW_KEY_RIGHT)		radiansX += 0.5;
	if (key == GLFW_KEY_UP)			radiansY -= 0.5;
	if (key == GLFW_KEY_DOWN)		radiansY += 0.5;
	//mover cubo
	if (key == GLFW_KEY_KP_1)		movement.x -= 0.05;
	if (key == GLFW_KEY_KP_3)		movement.x += 0.05;
	if (key == GLFW_KEY_KP_5)		movement.y += 0.05;
	if (key == GLFW_KEY_KP_2)		movement.y -= 0.05;
	if (key == GLFW_KEY_KP_6)		movement.z += 0.05;
	if (key == GLFW_KEY_KP_4)		movement.z -= 0.05;
	//mover barco
	if (key == GLFW_KEY_I) 		BoatPos.z += 0.5;	
	if (key == GLFW_KEY_K) 		BoatPos.z -= 0.5;	
	if (key == GLFW_KEY_J) 		BoatPos.x -= 0.5;	
	if (key == GLFW_KEY_L) 		BoatPos.x += 0.5;
	//ocultar luz
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		if(hide==true)		hide = false;
		else				hide = true;
	}
	//play&stop
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (play == true)		play = false;
		else					play = true;
	}
}
/////////////////////////////////////////////RATÓN////////////////////////////////////////////////////
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	myCamera.MouseMove(window, xpos, ypos);
};
//////////////////////////////////////////liberar memoria/////////////////////////////////////////////
void deleteFunction(Object objA, Object objB, Object objC, Object objD, Object objE, ReflectObject refcub, TextureIcon gla, TextureIcon wat, TextureIcon ice, TextureIcon diam, Material mat,CubeMap sky) {
	// liberar la memoria de los VAO, EBO y VBO
	objA.Delete();
	objB.Delete();
	objC.Delete();
	objD.Delete();
	objE.Delete();
	refcub.Delete();

	gla.~gla();
	wat.~wat();
	ice.~ice();
	diam.~diam();
	
	mat.~Material();
	sky.~CubeMap();
};