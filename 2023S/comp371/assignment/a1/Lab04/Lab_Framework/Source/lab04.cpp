// Andre Hei Wang Law
// 4017 5600
// L_HEIWAN
//
// COMP 371 - Assignment 1
//
// Code skeleton and file structure taken from lab4
// 
// Sections of code based on lab 4, lab3, lab2 and lab1 
//
// Created by Nicolas Bergeron on 20/06/2019.
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
int createLineVertexArrayObject();		// 2d line
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
float toggleFloorGrid = true;		// draw or not draw floor grid

// render arm and tennis racket
void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);

// tennis parameters
vec3 armTennisPosition(0.0f, 0.0f, 0.0f);
vec3 armTennisLookArt(1.0f, 0.0f, 0.0f);
vec3 armTennisUp(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector = cross(armTennisLookArt, armTennisUp);
float rotateSpeed = 2.0f;
float armTennisScale = 1.0f;
float armTennisScaleAmount = 0.05f;
float armTennisSpeed = 20.0f;
float armTennisAngleZ = 0.0f;
float armTennisAngleY = 0.0f;


// Camera parameters  
vec3 cameraPosition(0.6f, 6.0f, 25.0f);
vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
float dt = 0.0f;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = 0.0f;
float cameraPanningSpeed = 2.0f;
float cameraTiltingSpeed = 2.0f;
float cameraFovChangeSpeed = 0.01f;

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
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);

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
	int texturedLineVao = createLineVertexArrayObject();	// line

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	
	// render mode
	char renderMode = GL_TRIANGLES;	

	// interaction - key press event
	glfwSetKeyCallback(window, interaction);



	// ================================================================ //
	//							loop									//
	// ================================================================ //
	while (!glfwWindowShouldClose(window)) {
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(texturedLineVao);

		// ================================================================ //
		//							draw									//
		// ================================================================ //
		
		// Floor Grid
		mat4 worldGridMatrix;
		if (toggleFloorGrid)	// true = draw floor grid
		{
			for (int i = 0; i <= 100; ++i)	// 100x100
			{
				worldGridMatrix = translate(mat4(1.0f), vec3(-50.0f, 0.0f, -50.0f + i * 1.0f)) * scale(mat4(1.0f), vec3(100.0f, 1.0f, 1.0f));
				setWorldMatrix(shaderProgram, worldGridMatrix);
				glDrawArrays(GL_LINES, 6, 2);

				worldGridMatrix = translate(mat4(1.0f), vec3(-50.0f + i * 1.0f, 0.0f, -50.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 100.0f));
				setWorldMatrix(shaderProgram, worldGridMatrix);
				glDrawArrays(GL_LINES, 8, 2);
			}
		}
			   
		// Draw axis 
		mat4 axisLines;
		createVertexArrayObject(vec3(1.0f, 0.0f, 0.0f));	// red 
		axisLines = translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0), vec3(5.0f, 0.2f, 0.2f));
		setWorldMatrix(shaderProgram, axisLines);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		createVertexArrayObject(vec3(0.0f, 1.0f, 0.0f));	// green 
		axisLines = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0), vec3(0.2f, 5.0f, 0.2f));
		setWorldMatrix(shaderProgram, axisLines);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		createVertexArrayObject(vec3(0.0f, 0.0f, 1.0f));	// blue 
		axisLines = translate(mat4(1.0f), vec3(0.0f, -0.0f, 2.5f)) * scale(mat4(1.0), vec3(0.2f, 0.2f, 5.0f));
		setWorldMatrix(shaderProgram, axisLines);
		glDrawArrays(GL_TRIANGLES, 0, 36);


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
		createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));	// default black color
		mat4 armTennis = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));		// origin xyz position
		armTennis = translate(armTennis, armTennisPosition);				// wasd movement
		armTennis = scale(armTennis, vec3(armTennisScale, armTennisScale, armTennisScale));	// u,j scale
		armTennis = rotate(armTennis, glm::radians(armTennisAngleZ), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		armTennis = rotate(armTennis, glm::radians(armTennisAngleY), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis		
		glUniformMatrix4fv(shaderProgram, 1, GL_FALSE, &armTennis[0][0]);

		// render
		lowerArm(armTennis, shaderProgram, renderMode);			// render loawer arm
		upperArm(armTennis, shaderProgram, renderMode);			// render upper arm
		tennisHandle(armTennis, shaderProgram, renderMode);		// render tennis handle
		tennisHead(armTennis, shaderProgram, renderMode);		// render tennis head
		tennisRimBottom(armTennis, shaderProgram, renderMode);	// render tennis bottom rim
		tennisRimLeft(armTennis, shaderProgram, renderMode);	// render tennis left rim
		tennisRimRight(armTennis, shaderProgram, renderMode);	// render tennis right rim
		tennisRimTop(armTennis, shaderProgram, renderMode);		// render tennis top rim

		// Update Model position (wasd) 
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {		// Move right
			armTennisPosition += armTennisLookArt * armTennisSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {		// Move left
			armTennisPosition -= armTennisLookArt * armTennisSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		// Move up
			armTennisPosition -= armTennisSideVector * armTennisSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {		// Move down
			armTennisPosition += armTennisSideVector * armTennisSpeed * dt;
		}

		// model rotation around z-axis (q and e) 
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {		// rotate left
			armTennisAngleZ = armTennisAngleZ + rotateSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {		// rotate right
			armTennisAngleZ = armTennisAngleZ - rotateSpeed;
		}

		// model rotation around y-axis (z and c) 
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		// rotate left
			armTennisAngleY = armTennisAngleY + rotateSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {		// rotate right
			armTennisAngleY = armTennisAngleY - rotateSpeed;
		}

		// ================================================================ //
		//								Camera								//
		// ================================================================ //

		// Calculate mouse motion dx and dy
		// pdate camera horizontal and vertical angle
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
			camera_fov += cameraFovChangeSpeed * (float)dy;

			// Limit the camera_fov
			if (camera_fov == 90.0f) {
				camera_fov = 70.0f;
			}
		}
		// pan, right mouse botton pressed
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			cameraHorizontalAngle -= dx * cameraPanningSpeed * dt;
			// based on lab
			float theta = radians(cameraHorizontalAngle);
			float phi = radians(cameraVerticalAngle);
			cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
		}

		// tilt, middle mouse botton pressed
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			cameraVerticalAngle -= dy * cameraTiltingSpeed * dt;
			// based on lab
			float theta = radians(cameraHorizontalAngle);
			float phi = radians(cameraVerticalAngle);
			cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
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

		// update view
		mat4 updatedViewMatrix = viewMatrix * worldMatrix;	// update 
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		setViewMatrix(shaderProgram, updatedViewMatrix);	// set 

		mat4 projectionMatrix = perspective(camera_fov, 1024.0f/768.0f, 0.1f, 100.0f);	// update fov
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

int createLineVertexArrayObject() {				// based on the lab
	// A vertex is a point on a polygon, it contains positions and other data (eg: colors)
	vec3 vertexArray[] = {
		// Position              Color
		vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // line in x
		vec3(5.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),	// 5 x units, red

		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), // line in y
		vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),	// 5 y units, green

		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), // line in z
		vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 1.0f),	// 5 z units, blue

		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), // gridline in x
		vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f),	// black

		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), // gridline in z
		vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f)	// black
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

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
	glBindVertexArray(0); // Unbind to not modify the VAO

	return vertexArrayObject;
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
		tennis_head = translate(tennis_head, vec3(-0.38f + i / 2, 13.6f, 0.0f)) *
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
		glDrawArrays(renderMode, 0, 36);						// draw
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

float randomFloat(float min, float max) {	
	// based on https://stackoverflow.com/questions/57025341/how-to-understand-c11-random-number-generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

void resetWorldOrientation() {
	worldMatrix = mat4(1.0f);
}

void rotateWorld(float angle, const vec3& axis) {
	worldMatrix = rotate(worldMatrix, glm::radians(angle), axis);
}

void interaction(GLFWwindow* window, int key, int scancode, int action, int mods){
	
	// repositioning the Model (spacebar) 
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		armTennisPosition = vec3(	randomFloat(-50.0f, 50.0f),		// x, min and max
									0.0f,							// y (on the grid)
									randomFloat(-50.0f, 50.0f));	// z, min and max
	}

	// Scale up (u)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){ 
		armTennisScale += armTennisScaleAmount;
	}

	// Scale down (j)
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		armTennisScale -= armTennisScaleAmount;
		
		if (armTennisScale <= 0.05f) {	// Limit the scale 
			armTennisScale = 0.05f;
		}
	}
	
	// hanging world orientation (arrows and HOME) 5 pts 
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
			resetWorldOrientation();	// reset world position and orientation
			camera_fov = 70.0f;		// reset fov
			break;
		default:
			break;
		}
	}

}
