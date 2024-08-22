// Andre Hei Wang Law
// 4017 5600
// L_HEIWAN
//
// COMP 371 - Quiz 1
//
// Code skeleton and file structure taken from lab4
// 
// Sections of code based on lab 4, lab3, lab2 and lab1 AND my own Assignment 1
// 
// I commented out many irelevent sections that were done from my assignment 1
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// 
// Online Resources:
// https://stackoverflow.com/questions/57025341/how-to-understand-c11-random-number-generator (random num gen w/ min and max) - used "float randomFloat(float min, float max);"
// 
// 
//

#include <iostream>

#define GLEW_STATIC 1	// This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>	// Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>	// Cross-platform interface (graphics, init OpenGl and binding inputs)
#include <glm/glm.hpp>	// GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <random>

using namespace glm;
using namespace std;

// initialize  
int compileAndLinkShaders();			// compile and link shader program
const char* getVertexShaderSource();	// vertex shader
const char* getFragmentShaderSource();	// frag shader
bool initContext();						// setup GLFW and OpenGl version (based on lab2)
int createVertexArrayObject(vec3 color);// 3d cube model
float randomFloat(float a, float b);
void interaction(GLFWwindow* window, int key, int scancode, int action, int mods);
void resetWorldOrientation();
void rotateWorld(float angle, const vec3& axis);

// initialize - set matrices
void setWorldMatrix(int shaderProgram, mat4 worldMatrix);
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, mat4 viewMatrix);

// window
GLFWwindow * window = NULL;		// pointer to a GLFW window object
const float WINDOW_WIDTH = 1024;	// width
const float WINDOW_HEIGHT = 768;	// height

// toggle
float toggleFloorGrid = true;		// draw or not draw floor grid (used for testing)

// world grid parameters
float gridLength = 78.0f;
float gridWidth = 36.0f;

// render arm and tennis racket
void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);

// render alphabet W
void alphabetWBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetWLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetWMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetWRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);

// render alphabet A
void alphabetALeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetARight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetATop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetAMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);

// render alphabet L
void alphabetLBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetLleft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);

// render alphabet G
void alphabetGBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetGLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetGRightBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetGRightMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetGTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void alphabetGRightTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);

// alphabet properties
float alphLong = 5.0f;
float alphShort = 2.0f;
float alphThinness = 0.25f;
float alphWidth = 0.8f;

// tennis parameters
float armTennisScaleAmount = 0.05f;
float armTennisSpeed = 20.0f;
float rotateSpeed = 2.0f;
int currentNumber = 1;	// default 1, used to swtich racket

// tennis net parameters
float tennisNetWidth = gridWidth;
float tennisNetHeight = 8.0f;

// tennis player 1 parameters - bottom right
float armTennisScale1 = 1.0f;
float armTennisAngleZ1 = 0.0f;
float armTennisAngleY1 = 0.0f;
vec3 armTennisPosition1(5.0f, 0.0f, 15.0f);
vec3 armTennisLookAt1(1.0f, 0.0f, 0.0f);
vec3 armTennisUp1(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector1 = cross(armTennisLookAt1, armTennisUp1);

// tennis player 2 parameters - bottom left
float armTennisScale2 = 1.0f;
float armTennisAngleZ2 = 0.0f;
float armTennisAngleY2 = 0.0f;
vec3 armTennisPosition2(-5.0f, 0.0f, 15.0f);
vec3 armTennisLookAt2(1.0f, 0.0f, 0.0f);
vec3 armTennisUp2(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector2 = cross(armTennisLookAt2, armTennisUp2);

// tennis player 3 parameters - top right
float armTennisScale3 = 1.0f;
float armTennisAngleZ3 = 0.0f;
float armTennisAngleY3 = 0.0f;
vec3 armTennisPosition3(5.0f, 0.0f, -15.0f);
vec3 armTennisLookAt3(1.0f, 0.0f, 0.0f);
vec3 armTennisUp3(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector3 = cross(armTennisLookAt3, armTennisUp3);

// tennis player 4 parameters - top left
float armTennisScale4 = 1.0f;
float armTennisAngleZ4 = 0.0f;
float armTennisAngleY4 = 0.0f;
vec3 armTennisPosition4(-5.0f, 0.0f, -15.0f);
vec3 armTennisLookAt4(1.0f, 0.0f, 0.0f);
vec3 armTennisUp4(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector4 = cross(armTennisLookAt4, armTennisUp4);

// Camera parameters  
vec3 cameraPosition(0.6f, 10.0f, 45.0f);		// default camera view
vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
float dt = 0.0f;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = 0.0f;
float cameraPanningSpeed = 2.0f;
float cameraTiltingSpeed = 2.0f;
float cameraFovChangeSpeed = 0.01f;
vec3 offset(0.0f, 10.0f, -1.5f);	// 1st person view
vec3 offset2(0.0f, 10.0f, 1.5f);	// 1st person view (mirror)
bool toggleFirstPersonView = true;		// true = 1st person, false = 3rd person

// camera parameters - tilt and pan
float cameraRotateX = 0.0f;
float cameraRotateY = 0.0f;
float cameraRotationSpeed = 5.0f;
float camera_fov = 70.0f;

// Set initial view matrix
mat4 viewMatrix = lookAt(cameraPosition,  // eye
						cameraPosition + cameraLookAt,  // center
						cameraUp); // up
mat4 worldMatrix = mat4(1.0f);			// Initial world orientation


// ================================================================ //
//							main 
// ================================================================ //
int main(int argc, char*argv[]) {

	// setup GLFW and OpenGl version (based on lab2)
	if (!initContext())
		return -1;		// exit

	// background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Compile, link and set shaders here ...
	int shaderProgram = compileAndLinkShaders();	// compile and link
	glUseProgram(shaderProgram);					// set
		
	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
											WINDOW_WIDTH/WINDOW_HEIGHT,  // aspect ratio
											0.01f, 100.0f);   // near and far (near > 0)

	// Set View and Projection matrices on both shaders
	setViewMatrix(shaderProgram, viewMatrix);
	setProjectionMatrix(shaderProgram, projectionMatrix);

	// Define and upload geometry to the GPU here ...
	int vao = createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));	

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	
	// render mode
	char renderMode = GL_TRIANGLES;	

	// interaction - key press event
	glfwSetKeyCallback(window, interaction);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);	// in order to avoid seeing through

	// ================================================================ //
	//							loop									//
	// ================================================================ //
	while (!glfwWindowShouldClose(window)) {
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);

		// ================================================================ //
		//							draw									//
		// ================================================================ //
		
		// draw large cube - blue sky
		createVertexArrayObject(vec3(0.5f, 0.8f, 0.9f));	// sky blue color, 135, 206, 235
		mat4 largeCube = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(100.0f, 100.0f, 100.0f));
		setWorldMatrix(shaderProgram, largeCube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Floor Grid
		mat4 worldGridMatrix;
		if (toggleFloorGrid)	// true = draw floor grid
		{
			createVertexArrayObject(vec3(0.5f, 1.0f, 0.0f));	// grass color rgb, 124, 252, 0
			for (int i = 0; i <= gridLength; ++i)	// length-wise grid, --
			{
				worldGridMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -gridWidth + i * 1.0f)) * scale(mat4(1.0f), vec3(gridWidth, 0.1f, 0.1f));
				setWorldMatrix(shaderProgram, worldGridMatrix);
				glDrawArrays(GL_TRIANGLES, 0, 36);

			}
			for (int i = 0; i <= gridWidth; ++i)	// width-wise grid, |
			{
				worldGridMatrix = translate(mat4(1.0f), vec3(-gridWidth/2 + i * 1.0f, 0.0f, 3.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, gridLength));
				setWorldMatrix(shaderProgram, worldGridMatrix);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
			   
		// Draw axis 
		mat4 axisLines;
		createVertexArrayObject(vec3(1.0f, 0.0f, 0.0f));	// red, x-axis
		axisLines = translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0), vec3(5.0f, 0.2f, 0.2f));
		setWorldMatrix(shaderProgram, axisLines);
		glDrawArrays(GL_TRIANGLES, 0, 36);	// draw

		createVertexArrayObject(vec3(0.0f, 1.0f, 0.0f));	// green, y-axis
		axisLines = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0), vec3(0.2f, 5.0f, 0.2f));
		setWorldMatrix(shaderProgram, axisLines);
		glDrawArrays(GL_TRIANGLES, 0, 36);	// draw

		createVertexArrayObject(vec3(0.0f, 0.0f, 1.0f));	// blue, z=axis
		axisLines = translate(mat4(1.0f), vec3(0.0f, -0.0f, 2.5f)) * scale(mat4(1.0), vec3(0.2f, 0.2f, 5.0f));
		setWorldMatrix(shaderProgram, axisLines);
		glDrawArrays(GL_TRIANGLES, 0, 36);	// draw

		// ================================================================ //
		//							Tennis Net								//
		// ================================================================ //

		// Floor Grid
		mat4 tennisNet;
		createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));	// grass color rgb, 124, 252, 0
		for (int i = 0; i <= tennisNetHeight; ++i)	// length-wise grid, --
		{
			tennisNet = translate(mat4(1.0f), vec3(0.0f, i * 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(tennisNetWidth, 0.1f, 0.1f));
			setWorldMatrix(shaderProgram, tennisNet);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		for (int i = 0; i <= tennisNetWidth; ++i)	// width-wise grid, |
		{
			tennisNet = translate(mat4(1.0f), vec3(-tennisNetWidth/2 + i * 1.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, tennisNetHeight, 0.1f));
			setWorldMatrix(shaderProgram, tennisNet);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// ================================================================ //
		//						Arm and Tennis Racket						//
		// ================================================================ //

		// render mode - triangle, line or point 
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			renderMode = GL_TRIANGLES;			// triangles
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			renderMode = GL_POINTS;				// points
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			renderMode = GL_LINES;				// lines

		// Draw arm and racket
		createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));	// default black color, testing
		mat4 armTennis1 = translate(mat4(1.0f), armTennisPosition1);		// origin xyz position
		mat4 armTennis2 = translate(mat4(1.0f), armTennisPosition2);		// origin xyz position
		mat4 armTennis3 = translate(mat4(1.0f), armTennisPosition3);		// origin xyz position
		mat4 armTennis4 = translate(mat4(1.0f), armTennisPosition4);		// origin xyz position

		// tennis 1 translate, scale, rotate
		armTennis1 = translate(armTennis1, armTennisPosition1);				// wasd movement
		armTennis1 = scale(armTennis1, vec3(armTennisScale1, armTennisScale1, armTennisScale1));	// u,j scale
		armTennis1 = rotate(armTennis1, glm::radians(armTennisAngleZ1), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		armTennis1 = rotate(armTennis1, glm::radians(armTennisAngleY1), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis
		glUniformMatrix4fv(shaderProgram, 1, GL_FALSE, &armTennis1[0][0]);
		
		// tennis 2 translate, scale, rotate
		armTennis2 = translate(armTennis2, armTennisPosition2);				// wasd movement
		armTennis2 = scale(armTennis2, vec3(armTennisScale2, armTennisScale2, armTennisScale2));	// u,j scale
		armTennis2 = rotate(armTennis2, glm::radians(armTennisAngleZ2), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		armTennis2 = rotate(armTennis2, glm::radians(armTennisAngleY2), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis
		glUniformMatrix4fv(shaderProgram, 1, GL_FALSE, &armTennis2[0][0]);

		// tennis 3 translate, scale, rotate
		armTennis3 = translate(armTennis3, armTennisPosition3);				// wasd movement
		armTennis3 = scale(armTennis3, vec3(armTennisScale3, armTennisScale3, armTennisScale3));	// u,j scale
		armTennis3 = rotate(armTennis3, glm::radians(armTennisAngleZ3), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		armTennis3 = rotate(armTennis3, glm::radians(armTennisAngleY3), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis
		glUniformMatrix4fv(shaderProgram, 1, GL_FALSE, &armTennis3[0][0]);

		// tennis 4 translate, scale, rotate
		armTennis4 = translate(armTennis4, armTennisPosition4);				// wasd movement
		armTennis4 = scale(armTennis4, vec3(armTennisScale4, armTennisScale4, armTennisScale4));	// u,j scale
		armTennis4 = rotate(armTennis4, glm::radians(armTennisAngleZ4), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		armTennis4 = rotate(armTennis4, glm::radians(armTennisAngleY4), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis
		glUniformMatrix4fv(shaderProgram, 1, GL_FALSE, &armTennis4[0][0]);
		
		// render racket 1
		lowerArm(armTennis1, shaderProgram, renderMode);			// render loawer arm
		upperArm(armTennis1, shaderProgram, renderMode);			// render upper arm
		tennisHandle(armTennis1, shaderProgram, renderMode);		// render tennis handle
		tennisHead(armTennis1, shaderProgram, renderMode);		// render tennis head
		tennisRimBottom(armTennis1, shaderProgram, renderMode);	// render tennis bottom rim
		tennisRimLeft(armTennis1, shaderProgram, renderMode);	// render tennis left rim
		tennisRimRight(armTennis1, shaderProgram, renderMode);	// render tennis right rim
		tennisRimTop(armTennis1, shaderProgram, renderMode);		// render tennis top rim
		
		// render racket 2
		lowerArm(armTennis2, shaderProgram, renderMode);			// render loawer arm
		upperArm(armTennis2, shaderProgram, renderMode);			// render upper arm
		tennisHandle(armTennis2, shaderProgram, renderMode);		// render tennis handle
		tennisHead(armTennis2, shaderProgram, renderMode);		// render tennis head
		tennisRimBottom(armTennis2, shaderProgram, renderMode);	// render tennis bottom rim
		tennisRimLeft(armTennis2, shaderProgram, renderMode);	// render tennis left rim
		tennisRimRight(armTennis2, shaderProgram, renderMode);	// render tennis right rim
		tennisRimTop(armTennis2, shaderProgram, renderMode);		// render tennis top rim

		// render racket 3
		lowerArm(armTennis3, shaderProgram, renderMode);			// render loawer arm
		upperArm(armTennis3, shaderProgram, renderMode);			// render upper arm
		tennisHandle(armTennis3, shaderProgram, renderMode);		// render tennis handle
		tennisHead(armTennis3, shaderProgram, renderMode);		// render tennis head
		tennisRimBottom(armTennis3, shaderProgram, renderMode);	// render tennis bottom rim
		tennisRimLeft(armTennis3, shaderProgram, renderMode);	// render tennis left rim
		tennisRimRight(armTennis3, shaderProgram, renderMode);	// render tennis right rim
		tennisRimTop(armTennis3, shaderProgram, renderMode);		// render tennis top rim

		// render racket 4
		lowerArm(armTennis4, shaderProgram, renderMode);			// render loawer arm
		upperArm(armTennis4, shaderProgram, renderMode);			// render upper arm
		tennisHandle(armTennis4, shaderProgram, renderMode);		// render tennis handle
		tennisHead(armTennis4, shaderProgram, renderMode);		// render tennis head
		tennisRimBottom(armTennis4, shaderProgram, renderMode);	// render tennis bottom rim
		tennisRimLeft(armTennis4, shaderProgram, renderMode);	// render tennis left rim
		tennisRimRight(armTennis4, shaderProgram, renderMode);	// render tennis right rim
		tennisRimTop(armTennis4, shaderProgram, renderMode);		// render tennis top rim
		

		// ================================================================ //
		//							Four Alphabets							//
		// ================================================================ //

		// Full Name: Andre Hei Wang Law
		// Last four alphabets:
		//		1: W
		//		2: A
		//		3: L
		//		4: G

		// render alphabet W
		alphabetWBottom(armTennis1, shaderProgram, renderMode);
		alphabetWLeft(armTennis1, shaderProgram, renderMode);
		alphabetWMiddle(armTennis1, shaderProgram, renderMode);
		alphabetWRight(armTennis1, shaderProgram, renderMode);

		// render alphabet A
		alphabetALeft(armTennis2, shaderProgram, renderMode);
		alphabetARight(armTennis2, shaderProgram, renderMode);
		alphabetATop(armTennis2, shaderProgram, renderMode);
		alphabetAMiddle(armTennis2, shaderProgram, renderMode);

		// render alphabet L
		alphabetLBottom(armTennis3, shaderProgram, renderMode);
		alphabetLleft(armTennis3, shaderProgram, renderMode);

		// render alphabet G
		alphabetGBottom(armTennis4, shaderProgram, renderMode);
		alphabetGLeft(armTennis4, shaderProgram, renderMode);
		alphabetGRightBottom(armTennis4, shaderProgram, renderMode);;
		alphabetGRightMiddle(armTennis4, shaderProgram, renderMode);
		alphabetGTop(armTennis4, shaderProgram, renderMode);
		alphabetGRightTop(armTennis4, shaderProgram, renderMode);


		// ================================================================ //
		//						WASD Movement Control						//
		// ================================================================ //

		switch (currentNumber) {
		case 1:
			// Update Model position (wasd) 
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {		// Move right
				armTennisPosition1 += armTennisLookAt1 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition1 * 2.0f + offset; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {		// Move left
				armTennisPosition1 -= armTennisLookAt1 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition1 * 2.0f + offset; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		// Move up
				armTennisPosition1 -= armTennisSideVector1 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition1 * 2.0f + offset; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {		// Move down
				armTennisPosition1 += armTennisSideVector1 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition1 * 2.0f + offset; // offset to be in front of racket
			}
			// model rotation around z-axis (q and e) 
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {		// rotate left
				armTennisAngleZ1 = armTennisAngleZ1 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {		// rotate right
				armTennisAngleZ1 = armTennisAngleZ1 - rotateSpeed;
			}

			// model rotation around y-axis (z and c) 
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		// rotate left
				armTennisAngleY1 = armTennisAngleY1 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {		// rotate right
				armTennisAngleY1 = armTennisAngleY1 - rotateSpeed;
			}
			break;
		case 2:
			// Update Model position (wasd) 
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {		// Move right
				armTennisPosition2 += armTennisLookAt2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {		// Move left
				armTennisPosition2 -= armTennisLookAt2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		// Move up
				armTennisPosition2 -= armTennisSideVector2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {		// Move down
				armTennisPosition2 += armTennisSideVector2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset; // offset to be in front of racket
			}
			// model rotation around z-axis (q and e) 
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {		// rotate left
				armTennisAngleZ2 = armTennisAngleZ2 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {		// rotate right
				armTennisAngleZ2 = armTennisAngleZ2 - rotateSpeed;
			}

			// model rotation around y-axis (z and c) 
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		// rotate left
				armTennisAngleY2 = armTennisAngleY2 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {		// rotate right
				armTennisAngleY2 = armTennisAngleY2 - rotateSpeed;
			}
			break;
		case 3:
			
			// Update Model position (wasd) 
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {		// Move left
				armTennisPosition3 += armTennisLookAt3 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition3 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {		// Move right
				armTennisPosition3 -= armTennisLookAt3 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition3 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {		// Move down
				armTennisPosition3 -= armTennisSideVector3 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition3 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		// Move up
				armTennisPosition3 += armTennisSideVector3 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition3 * 2.0f + offset2; // offset to be in front of racket
			}
			// model rotation around z-axis (q and e) 
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {		// rotate left
				armTennisAngleZ3 = armTennisAngleZ3 - rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {		// rotate right
				armTennisAngleZ3 = armTennisAngleZ3 + rotateSpeed;
			}

			// model rotation around y-axis (z and c) 
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		// rotate left
				armTennisAngleY3 = armTennisAngleY3 - rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {		// rotate right
				armTennisAngleY3 = armTennisAngleY3 + rotateSpeed;
			}
			break;
		case 4:
			
			// Update Model position (wasd) 
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {		// Move left
				armTennisPosition4 += armTennisLookAt4 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition4 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {		// Move right
				armTennisPosition4 -= armTennisLookAt4 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition4 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {		// Move down
				armTennisPosition4 -= armTennisSideVector4 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition4 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		// Move up
				armTennisPosition4 += armTennisSideVector4 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition4 * 2.0f + offset2; // offset to be in front of racket
			}
			// model rotation around z-axis (q and e) 
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {		// rotate left
				armTennisAngleZ4 = armTennisAngleZ4 - rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {		// rotate right
				armTennisAngleZ4 = armTennisAngleZ4 + rotateSpeed;
			}

			// model rotation around y-axis (z and c) 
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		// rotate left
				armTennisAngleY4 = armTennisAngleY4 - rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {		// rotate right
				armTennisAngleY4 = armTennisAngleY4 + rotateSpeed;
			}
			break;
		}

		// ================================================================ //
		//								Camera								//
		// ================================================================ //

		// Calculate mouse motion dx and dy
		// pdate camera horizontal and vertical angle		based on lab
		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		// Determine the change in cursor position
		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		// Update previous cursor position values
		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Allow zomming if left mouse btn is held
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!(camera_fov > 70.5f || camera_fov < 69.5f)) {	// range of fov
				camera_fov += cameraFovChangeSpeed * (float)dy;	// change fov
			}

			if (camera_fov > 70.5f)		// max 
				camera_fov = 70.49f;	// avoid stuck at FOV
			else if (camera_fov < 69.5f)	// min
				camera_fov = 69.51f;	// avoid stuck at FOV
		}

		// pan, right mouse botton pressed
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			cameraHorizontalAngle -= dx * cameraPanningSpeed * dt;
			// based on lab
			float theta = radians(cameraHorizontalAngle);
			float phi = radians(cameraVerticalAngle);
			cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));	// update camera look at
		}

		// tilt, middle mouse botton pressed
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			cameraVerticalAngle -= dy * cameraTiltingSpeed * dt;
			// based on lab
			float theta = radians(cameraHorizontalAngle);
			float phi = radians(cameraVerticalAngle);
			cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));	// update camera look at
		}

		// limit vertical angle 
		cameraVerticalAngle = std::max(-88.0f, std::min(88.0f, cameraVerticalAngle));

		// limit angle between -360 and 3600
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		mat4 updatedViewMatrix = viewMatrix * worldMatrix;	// update 

		// camera control
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)	// camera 1
		{
			if (toggleFirstPersonView) {
				cameraPosition = armTennisPosition1 * 2.0f + offset;
				cameraLookAt = vec3(-0.0f, 0.0f, -1.0f);
			} else
				toggleFirstPersonView = true;
		}
		
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)	// camera 2
		{
			if (toggleFirstPersonView) {
			cameraPosition = armTennisPosition2 * 2.0f + offset;
			cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
			}
			else
				toggleFirstPersonView = true;
		}
		
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)	// camera 3
		{
			if (toggleFirstPersonView) {
				cameraPosition = armTennisPosition3 * 2.0f + offset2;
				cameraLookAt = vec3(0.0f, 0.0f, 1.0f);
			}
			else
				toggleFirstPersonView = true;
		}
		
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)	// camera 4
		{
			if (toggleFirstPersonView) {
				cameraPosition = armTennisPosition4 * 2.0f + offset2;
				cameraLookAt = vec3(0.0f, 0.0f, 1.0f);
			}
			else
				toggleFirstPersonView = true;
		}
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)	// camera 4
		{
			toggleFirstPersonView = false;

			cameraPosition = vec3(0.6f, 10.0f, 45.0f);
			cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)	// camera 4
		{
			toggleFirstPersonView = false;

			cameraPosition = vec3(0.6f, 10.0f, -45.0f);
			cameraLookAt = vec3(0.0f, 0.0f, 1.0f);
		}

		// update view
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		setViewMatrix(shaderProgram, updatedViewMatrix);	// set 


		projectionMatrix = perspective(camera_fov, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);	// update fov
		setProjectionMatrix(shaderProgram, projectionMatrix);	// set new fov


		// End frame
		glfwSwapBuffers(window);

		// Detect inputs
		glfwPollEvents();

		// exit program
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

	}


	// Shutdown GLFW
	glfwTerminate();

	return 0;
}


int compileAndLinkShaders() {		// based on the lab

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSource();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSource();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// return shader program id
	return shaderProgram;
}

const char* getVertexShaderSource() {				// based on the lab
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		""
		"uniform mat4 worldMatrix = mat4(1.0);"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}

const char* getFragmentShaderSource() {				// based on the lab
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
		"}";
}

bool initContext() {				// based on the lab
	// Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 4.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 800x600
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the window's content current
	glfwMakeContextCurrent(window);

	// disable cursor
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
	return true;
}

int createVertexArrayObject(vec3 color) {				// based on the lab
	vec3 vertexArray[] = {
		// Position              Color
		vec3(-0.5f,-0.5f,-0.5f), color,
		vec3(-0.5f,-0.5f, 0.5f), color,
		vec3(-0.5f, 0.5f, 0.5f), color,

		vec3(-0.5f,-0.5f,-0.5f), color,
		vec3(-0.5f, 0.5f, 0.5f), color,
		vec3(-0.5f, 0.5f,-0.5f), color,

		vec3(0.5f, 0.5f,-0.5f), color,
		vec3(-0.5f,-0.5f,-0.5f), color,
		vec3(-0.5f, 0.5f,-0.5f), color,

		vec3(0.5f, 0.5f,-0.5f), color,
		vec3(0.5f,-0.5f,-0.5f), color,
		vec3(-0.5f,-0.5f,-0.5f),color,

		vec3(0.5f,-0.5f, 0.5f), color,
		vec3(-0.5f,-0.5f,-0.5f), color,
		vec3(0.5f,-0.5f,-0.5f), color,

		vec3(0.5f,-0.5f, 0.5f), color,
		vec3(-0.5f,-0.5f, 0.5f), color,
		vec3(-0.5f,-0.5f,-0.5f), color,

		vec3(-0.5f, 0.5f, 0.5f), color,
		vec3(-0.5f,-0.5f, 0.5f), color,
		vec3(0.5f,-0.5f, 0.5f), color,

		vec3(0.5f, 0.5f, 0.5f), color,
		vec3(-0.5f, 0.5f, 0.5f), color,
		vec3(0.5f,-0.5f, 0.5f), color,

		vec3(0.5f, 0.5f, 0.5f), color,
		vec3(0.5f,-0.5f,-0.5f), color,
		vec3(0.5f, 0.5f,-0.5f), color,

		vec3(0.5f,-0.5f,-0.5f), color,
		vec3(0.5f, 0.5f, 0.5f), color,
		vec3(0.5f,-0.5f, 0.5f), color,

		vec3(0.5f, 0.5f, 0.5f), color,
		vec3(0.5f, 0.5f,-0.5f), color,
		vec3(-0.5f, 0.5f,-0.5f), color,

		vec3(0.5f, 0.5f, 0.5f), color,
		vec3(-0.5f, 0.5f,-0.5f), color,
		vec3(-0.5f, 0.5f, 0.5f), color
	};


	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(	0,                   // attribute 0 matches aPos in Vertex Shader
							3,                   // size
							GL_FLOAT,            // type
							GL_FALSE,            // normalized?
							2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
							(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(	1,                            // attribute 1 matches aColor in Vertex Shader
							3,
							GL_FLOAT,
							GL_FALSE,
							2 * sizeof(vec3),
							(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBindVertexArray(0);

	glBindVertexArray(vertexBufferObject);

	return vertexArrayObject;
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix) {				// based on the lab
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix) {				// based on the lab
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix) {				// based on the lab
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

// render arm
void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {	
	mat4 lower_arm = armTennis;
	createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	lower_arm = translate(lower_arm, vec3(0.0f, 0.0f, 0.0f)) *
				rotate(mat4(1.0f), glm::radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *	// 45 angle
				scale(mat4(1.0), vec3(4.0f, 1.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, lower_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 upper_arm = armTennis;
	createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	upper_arm = translate(upper_arm, vec3(1.3f, 3.1f, 0.0f)) *
				scale(mat4(1.0), vec3(1.0f, 4.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, upper_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

// render tennis racket
void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_handle = armTennis;
	createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_handle = translate(tennis_handle, vec3(1.3f, 7.3f, 0.0f)) *
					scale(mat4(1.0), vec3(0.8f, 5.0f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_handle);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}	

void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 8.0f; i++) {	// |
		mat4 tennis_head = armTennis;
		createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(-0.38f + i/2, 13.6f, 0.0f)) *
			scale(mat4(1.0), vec3(0.2f, 7.0f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
	for (float i = 0.0f; i < 15.0f; i++) {	// --
		mat4 tennis_head = armTennis;
		createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(1.35f, 10.0f + i / 2, 0.0f)) *
			scale(mat4(1.0), vec3(4.0f, 0.2f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw (crash)
	}
}

void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimBottom = armTennis;
	createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimBottom =	translate(tennis_RimBottom, vec3(1.35f, 10.0f, 0.0f)) *
						scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimBottom);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimLeft = armTennis;
	createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimLeft =	translate(tennis_RimLeft, vec3(-0.8f, 13.6f, 0.0f)) *
						scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimLeft);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimRight = armTennis;
	createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimRight =	translate(tennis_RimRight, vec3(3.5f, 13.6f, 0.0f)) *
						scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimRight);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimTop = armTennis;
	createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimTop =		translate(tennis_RimTop, vec3(1.35f, 17.2f, 0.0f)) *
						scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimTop);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

// render alphabet W
void alphabetWBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WBottom = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WBottom = translate(alphabet_WBottom, vec3(1.35f, 19.0f, 0.5f-i/3)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WBottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetWLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WLeft = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WLeft = translate(alphabet_WLeft, vec3(3.5f, 21.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WLeft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetWMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WMiddle = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WMiddle = translate(alphabet_WMiddle, vec3(1.5f, 21.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WMiddle);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetWRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WRight = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WRight = translate(alphabet_WRight, vec3(-0.8f, 21.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WRight);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}


// render alphabet A
void alphabetALeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ALeft = armTennis;
		createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_ALeft = translate(alphabet_ALeft, vec3(3.5f, 21.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ALeft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetARight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ARight = armTennis;
		createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_ARight = translate(alphabet_ARight, vec3(-0.8f, 21.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ARight);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetATop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ATop = armTennis;
		createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_ATop = translate(alphabet_ATop, vec3(1.35f, 23.9f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ATop);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetAMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_AMiddle = armTennis;
		createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_AMiddle = translate(alphabet_AMiddle, vec3(1.35f, 21.2f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_AMiddle);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}


// render alphabet L
void alphabetLBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ABottom = armTennis;
		createVertexArrayObject(vec3(1.0f, 0.8f - i / 10, 0.2f + i / 10));    // color 
		alphabet_ABottom = translate(alphabet_ABottom, vec3(1.35f, 19.0f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ABottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetLleft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_Aleft = armTennis;
		createVertexArrayObject(vec3(1.0f, 0.8f - i / 10, 0.2f + i / 10));    // color 
		alphabet_Aleft = translate(alphabet_Aleft, vec3(3.5f, 21.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_Aleft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}


// render alphabet G
void alphabetGBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GBottom = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GBottom = translate(alphabet_GBottom, vec3(1.35f, 19.0f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GBottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetGLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GLeft = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GLeft = translate(alphabet_GLeft, vec3(3.5f, 21.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GLeft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetGRightBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GRightBottom = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GRightBottom = translate(alphabet_GRightBottom, vec3(-0.8f, 20.3f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong/2, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GRightBottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetGRightMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GRightMiddle = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GRightMiddle = translate(alphabet_GRightMiddle, vec3(0.0f, 21.2f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphLong/2, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GRightMiddle);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetGTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GTop = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GTop = translate(alphabet_GTop, vec3(1.35f, 23.9f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GTop);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void alphabetGRightTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GRightTop = armTennis;
		createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GRightTop = translate(alphabet_GRightTop, vec3(-0.8f, 23.5f, 0.5f - i / 3)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong/3, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GRightTop);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}


float randomFloat(float min, float max) {	
	// based on https://stackoverflow.com/questions/57025341/how-to-understand-c11-random-number-generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

void resetWorldOrientation() {
	worldMatrix = mat4(1.0f);			// Initial world orientation}
}

void rotateWorld(float angle, const vec3& axis) {
	worldMatrix = rotate(worldMatrix, glm::radians(angle), axis);	// rotate rad degree on axis
}

void interaction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	/*
	// repositioning the Model (spacebar) 
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		armTennisPosition = vec3(	randomFloat(-50.0f, 50.0f),		// x, min and max
									0.0f,							// y (on the grid)
									randomFloat(-50.0f, 50.0f));	// z, min and max
	}
	*/
	
	// Scale up (u)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){ 
		switch (currentNumber) {
		case 1:
			armTennisScale1 += armTennisScaleAmount;	// racket W
			break;
		case 2:
			armTennisScale2 += armTennisScaleAmount;	// racket A
			break;
		case 3:
			armTennisScale3 += armTennisScaleAmount;	// racket L
			break;
		case 4:
			armTennisScale4 += armTennisScaleAmount;	// racket G
			break;
		}
	}

	// Scale down (j)
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		switch (currentNumber) {
		case 1:
			if (armTennisScale1 <= 0.05f) {	// Limit the scale 
				armTennisScale1 = 0.05f;
			} else
				armTennisScale1 -= armTennisScaleAmount;	// racket W
			break;
		case 2:
			if (armTennisScale2 <= 0.05f) {	// Limit the scale 
				armTennisScale2 = 0.05f;
			}
			else
				armTennisScale2 -= armTennisScaleAmount;	// racket A
			break;
		case 3:
			if (armTennisScale3 <= 0.05f) {	// Limit the scale 
				armTennisScale3 = 0.05f;
			}
			else
				armTennisScale3 -= armTennisScaleAmount;	// racket L
			break;
		case 4:
			if (armTennisScale4 <= 0.05f) {	// Limit the scale 
				armTennisScale4 = 0.05f;
			}
			else
				armTennisScale4 -= armTennisScaleAmount;	// racket G
			break;
		}
	}
	
	// hanging world orientation (arrows and HOME) 
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		const float rotationAngle = 5.0f; // Adjust the rotation angle as needed

		switch (key) {
		case GLFW_KEY_UP:
			rotateWorld(rotationAngle, vec3(1.0f, 0.0f, 0.0f)); // Rx
			break;
		case GLFW_KEY_DOWN:
			rotateWorld(-rotationAngle, vec3(1.0f, 0.0f, 0.0f)); // R-x
			break;
		case GLFW_KEY_LEFT:
			rotateWorld(rotationAngle, vec3(0.0f, 1.0f, 0.0f)); // Ry
			break;
		case GLFW_KEY_RIGHT:
			rotateWorld(-rotationAngle, vec3(0.0f, 1.0f, 0.0f)); // R-y
			break;
		case GLFW_KEY_HOME:
			//resetWorldOrientation();	// reset world position and orientation
			break;
		default:
			break;
		}
	}
	if (action == GLFW_PRESS) {		// choose which 1st person view racket
		switch (key) {
		case GLFW_KEY_1:
			currentNumber = 1;	// racket W
			break;
		case GLFW_KEY_2:
			currentNumber = 2;	// racket A
			break;
		case GLFW_KEY_3:
			currentNumber = 3;	// racket L
			break;
		case GLFW_KEY_4:
			currentNumber = 4;	// racket G
			break;
		}
	}
}