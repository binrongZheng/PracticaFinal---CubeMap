//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "myShader.h"
#include "camara.h"
#include "object.h" 
#include "material.h"
#include "Light.h"
#include "CubeMap.h"
#include "Model.h"
//para textura
#include <SOIL.h>
//para vector
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//midmaping
using namespace glm;
using namespace std;
const GLint WIDTH = 1000, HEIGHT = 1000;
bool WIREFRAME = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
vec3 mov, rot, scal;
vec3 movement;
GLfloat radiansX,radiansY;
GLfloat mixValor;
GLfloat radX = 0;
GLfloat radY = 0;
GLint LightOption=3;
Camera myCamera({ 0,0,3 }, { 0,0,-1 }, 0.05, 45);

void MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
	myCamera.MouseScroll(window, xScroll, yScroll);
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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

	Shader objShader("./src/coordVertex.vertexshader", "./src/coordFragment.fragmentshader");
	Shader ReceiveShader("./src/ReceiveVertex.vertexshader", "./src/ReceiveFragment.fragmentshader");
	Shader generalLight("./src/VertexShaderPhongTexture.vs", "./src/FragmentShaderPhongTexture.fs");
	Shader CubemapShader("./src/CubemapVertex.vertexshader", "./src/CubemapFragment.fragmentshader");

	Model SpiderModel("./src/spider/spider/spider.obj");

	/*
	CubeMap skybox("./src/skybox/lagoon_rt.jpg", "./src/skybox/lagoon_lf.jpg", 
				   "./src/skybox/lagoon_up.jpg", "./src/skybox/lagoon_dn.jpg", 
				   "./src/skybox/lagoon_bk.jpg", "./src/skybox/lagoon_ft.jpg");
	*/
	CubeMap skybox("./src/skybox/right.jpg", "./src/skybox/left.jpg",
		"./src/skybox/top.jpg", "./src/skybox/bottom.jpg",
		"./src/skybox/back.jpg", "./src/skybox/front.jpg");
		
	skybox.pushTexture();
	
	Material material("./src/difuso.png", "./src/especular.png", 32.0);
	
	Object cubA({ 0.3f,0.3f,0.3f }, { 0.f,0.f,0.0f }, { 0.f,-2.f,0.0f }, Object::cube);
	Object cubB({ 0.1,0.1,0.1 }, { 0.f,1.f,0.0f }, { 0.f,0.3f,0.0f }, Object::cube);
	Object cubC({ 0.1,0.1,0.1 }, { 1.f,0.f,0.0f }, { 1.5f,0.3f,0.0f }, Object::cube);
	Object cubD({ 0.1,0.1,0.1 }, { 0.f,1.f,0.0f }, { 3.f,0.3f,0.0f }, Object::cube);
	Object cubE({ 0.1,0.1,0.1 }, { 0.f,0.f,1.0f }, { 4.5f,0.3f,0.0f }, Object::cube);
	
	material.SetMaterial(&generalLight);

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
				
		//Establecer el color de fondo
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//establecer el shader
		
		GLint lightPosLoc, viewPosLoc,lightDirPos;
		mat4 proj;		mat4 view;		mat4 model;
		GLint modelLoc, viewLoc, projectionLoc;

		proj = perspective(radians(myCamera.GetFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
		myCamera.DoMovement(window);
	
//SKYBOX//
		glDepthMask(GL_FALSE);
		CubemapShader.USE();
		// ... Set view and projection matrix
		view = mat4(mat3(myCamera.LookAt()));
		viewLoc = glGetUniformLocation(CubemapShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(CubemapShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));
		//pintar skybox
		skybox.draw(&CubemapShader);
		glDepthMask(GL_TRUE);
		

		generalLight.USE();
		material.SetShininess(&generalLight);
		material.ActivateTextures();
		view = myCamera.LookAt();
//DURECCIONAL//
		Light Ldir({0.0,0.0,0.0}, { -0.f, -1.0f, -0.f }, { 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, Light::DIRECTIONAL, 0);
		Ldir.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
		
//PUNTUAL
		Light Lpoint1({ cubB.GetPosition().x, cubB.GetPosition().y, cubB.GetPosition().z }, { 0.f, 0.f, 0.f, }, { 0.f, 0.2f, 0.f }, { 0.f, 0.5f, 0.f }, { 0.0f, 1.0f, 0.0f }, Light::POINT, 0);
		Lpoint1.SetAtt(1.0f, 0.09, 0.032);
		Lpoint1.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
		Lpoint1.SetPosition({ cubB.GetPosition().x, cubB.GetPosition().y, cubB.GetPosition().z });

		Light Lpoint2({ cubD.GetPosition().x, cubD.GetPosition().y, cubD.GetPosition().z }, { 0.f, 0.f, 0.f, }, { 0.0f, 0.0f, 0.2f }, { 0.f, 0.f, 0.5f }, { 0.0f, 0.0f, 1.0f }, Light::POINT, 1);
		Lpoint2.SetAtt(1.0f, 0.09, 0.032);
		Lpoint2.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
		Lpoint2.SetPosition({ cubD.GetPosition().x, cubD.GetPosition().y, cubD.GetPosition().z });

//FOCAL
		Light LFocal1({ cubC.GetPosition().x, cubC.GetPosition().y, cubC.GetPosition().z }, { 0.f, -1.f, 0.f, }, { 0.2f, 0.0f, 0.0f }, { 0.5f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, Light::SPOT, 0);
		LFocal1.SetAtt(1.0f, 0.09, 0.032);
		LFocal1.SetAperture(glm::cos(glm::radians(8.5f)), glm::cos(glm::radians(10.5f)));
		LFocal1.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
		LFocal1.SetPosition({ cubC.GetPosition().x, cubC.GetPosition().y, cubC.GetPosition().z });

		Light LFocal2({ cubE.GetPosition().x, cubE.GetPosition().y, cubE.GetPosition().z }, { 1.f, 0.0f, 0.f, }, { 0.2f, 0.07f, 0.0f }, { 0.5f, 0.170f, 0.0f }, { 0.9f, 0.30f, 0.0f }, Light::SPOT, 1);
		LFocal2.SetAtt(1.0f, 0.09, 0.032);
		LFocal2.SetAperture(glm::cos(glm::radians(8.5f)), glm::cos(glm::radians(10.5f)));
		LFocal2.SetLight(&generalLight, { myCamera.cameraPos.x, myCamera.cameraPos.y, myCamera.cameraPos.z });
		LFocal2.SetPosition({ cubE.GetPosition().x, cubE.GetPosition().y, cubE.GetPosition().z });

		cubA.Rotate(radiansX, radiansY);
		cubA.Move(movement);
		model = glm::translate(model, cubA.GetPosition());
		model = cubA.GetModelMatrix();

		modelLoc = glGetUniformLocation(generalLight.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		viewLoc = glGetUniformLocation(generalLight.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(generalLight.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

		//pintar el VAO
		cubA.Draw();
////////////////////////////////////////////////LightsCub///////////////////////////////////////////////////////
		ReceiveShader.USE();

		model = glm::translate(model, cubB.GetPosition());
		model = cubB.GetModelMatrix();
		
		modelLoc = glGetUniformLocation(ReceiveShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		
		GLint lightColorLoc = glGetUniformLocation(ReceiveShader.Program, "lightColor");
		glUniform3f(lightColorLoc, 0.0f, 1.0f, 0.0f);

		viewLoc = glGetUniformLocation(ReceiveShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(ReceiveShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

		//pintar el VAO
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
//pintar Model
		objShader.USE();

		model = translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(glGetUniformLocation(objShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		SpiderModel.Draw(objShader, GL_FILL);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO
	cubA.Delete();
	cubB.Delete();
	cubC.Delete();
	cubD.Delete();

	material.~Material();
	skybox.~CubeMap();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		WIREFRAME = !WIREFRAME;
	if (key == GLFW_KEY_1 && mixValor + 0.02 <= 1)
		mixValor += 0.02;
	if (key == GLFW_KEY_2 && mixValor - 0.02 >= 0)
		mixValor -= 0.02;

	if (key == GLFW_KEY_LEFT)
		radiansX -= 0.5;
	if (key == GLFW_KEY_RIGHT)
		radiansX += 0.5;
	if (key == GLFW_KEY_UP)
		radiansY -= 0.5;
	if (key == GLFW_KEY_DOWN)
		radiansY += 0.5;

	if (key == GLFW_KEY_KP_1)
		movement.x -= 0.05;
	if (key == GLFW_KEY_KP_3)
		movement.x += 0.05;
	if (key == GLFW_KEY_KP_5)
		movement.y += 0.05;
	if (key == GLFW_KEY_KP_2)
		movement.y -= 0.05;
	if (key == GLFW_KEY_KP_6)
		movement.z += 0.05;
	if (key == GLFW_KEY_KP_4)
		movement.z -= 0.05;

	if (key == GLFW_KEY_5)
		LightOption = 1;
	if (key == GLFW_KEY_6)
		LightOption = 2;
	if (key == GLFW_KEY_7)
		LightOption = 3;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	myCamera.MouseMove(window, xpos, ypos);
};
