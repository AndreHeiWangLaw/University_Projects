// Andre Hei Wang Law
// 4017 5600
// L_HEIWAN
//
// COMP 371 
//
// Code skeleton and file structure taken from lab4
// 
// Sections of code based on lab 4, lab3, lab2 and lab1 
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Icosphere logic based on these online ressources:
// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
// https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
// http://www.songho.ca/opengl/gl_sphere.html
// https://pastebin.com/erWMP4s4
//
// Lighting based on these online ressources:
// https://learnopengl.com/Lighting/Basic-Lighting
// 
// Incomplete, still work in progress in the point light and shadow!!!!

#include <iostream>
#define GLEW_STATIC 1	// This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>	// Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>	// Cross-platform interface (graphics, init OpenGl and binding inputs)
#include <glm/glm.hpp>	// GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <random>
#include <cmath>

#include "OBJloader.h"    //For loading .obj files
#include "OBJloaderV2.h"  //For loading .obj files using a polygon list format
#include "shaderloader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>



using namespace glm;
using namespace std;

// initialize  
int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource); // compile and link shader program
GLuint loadTexture(const char* filename);	// load shaders
const char* getVertexShaderSource();	// vertex shader
const char* getFragmentShaderSource();	// frag shader
const char* getTexturedVertexShaderSource();
const char* getTexturedFragmentShaderSource();
bool initContext();						// setup GLFW and OpenGl version (based on lab2)
//int createVertexArrayObject(vec3 color);// 3d cube model
int createTexturedCubeVertexArrayObject();	// 3d model, lab4
int createSphereObject(); // from https://pastebin.com/erWMP4s4
float randomFloat(float a, float b);
void interaction(GLFWwindow* window, int key, int scancode, int action, int mods);
void resetWorldOrientation();
void rotateWorld(float angle, const vec3& axis);

// initialize - set matrices
void setWorldMatrix(int shaderProgram, mat4 worldMatrix);
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, mat4 viewMatrix);
void SetUniformMat4(GLuint shader_id, const char* uni_name, mat4 uniform_value);

// window
GLFWwindow * window = NULL;		// pointer to a GLFW window object
float WINDOW_WIDTH = 1024;	// width
float WINDOW_HEIGHT = 768;	// height

// toggle
float toggleFloorGrid = true;		// draw or not draw floor grid

// render arm and tennis racket
void cube(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void wrist(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
mat4 original_cube;
mat4 lower_arm;
mat4 upper_arm;
mat4 wrist_arm;
mat4 tennis_handle;
mat4 tennis_head;
mat4 tennis_RimBottom;
mat4 tennis_RimLeft;
mat4 tennis_RimRight;
mat4 tennis_RimTop;

// tennis parameters
vec3 armTennisPosition(0.0f, 0.0f, 0.0f);
vec3 armTennisLookArt(1.0f, 0.0f, 0.0f);
vec3 armTennisUp(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector = cross(armTennisLookArt, armTennisUp);
float rotateSpeed = 2.0f;
float armTennisScale = 1.0f;
float armTennisScaleAmount = 0.05f;
float armTennisSpeed = 20.0f;

float armTennisAngleY = 0.0f;
float armTennisAngleYlower = 0.0f;
float armTennisAngleYHigher = 0.0f;

float armTennisAngleX = 0.0f;
float armTennisAngleXlower = 0.0f;
float armTennisAngleXHigher = 0.0f;

float armTennisAngleZ = 0.0f;
float armTennisAngleZlower = 0.0f;
float armTennisAngleZHigher = 0.0f;

float displacementLR = 0.0f;
float displacementUD = 0.0f;
float displacementX = 0.0f;
float displacementX2 = 0.0f;

float displacementY1 = 0.0f;
float displacementY2 = 0.0f;

constexpr float PI = 3.14159265358979323846f;



// Camera parameters  
vec3 cameraPosition(0.6f, 6.0f, 25.0f);
vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
//float dt = 0.0f;
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
mat4 worldMatrix = mat4(1.0f);			// Initial world orientation
mat4 updatedViewMatrix;

// toggle
bool toggleTexture = true;		// texture, true = on, false = off

// ================================================================ //
//			assignment 2 additional code - tennis ball				//
// ================================================================ //

unsigned int indexCount;

// texture cube, based on lab
struct TexturedColoredVertex{ // based on lab 
	TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
		: position(_position), colorr(_color), uv(_uv) {}

	vec3 position;
	vec3 colorr;
	vec2 uv;
};
vec3 color(0.0f, 0.0f, 1.0f);	// default color

TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)), //left - red
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), color, vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), color, vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), color, vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), color, vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), color, vec2(1.0f, 1.0f)), // far - blue
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), color, vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), color, vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), color, vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), color, vec2(1.0f, 1.0f)), // bottom - turquoise
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), color, vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), color, vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), color, vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), color, vec2(0.0f, 1.0f)), // near - green
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), color, vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), color, vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), color, vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), color, vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), color, vec2(1.0f, 1.0f)), // right - purple
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), color, vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), color, vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), color, vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), color, vec2(1.0f, 1.0f)), // top - yellow
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), color, vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), color, vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), color, vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), color, vec2(0.0f, 1.0f))
};

GLuint tennisBallVertexArrayObject;		// tennis ball

std::vector<glm::vec3> generateIcosphereVertices(int recursionLevel) {	// icosphere 
	const float X = 0.525731112119133606f;
	const float Z = 0.850650808352039932f;

	std::vector<glm::vec3> vertices = {
		glm::vec3(-X, 0.0f, Z), glm::vec3(X, 0.0f, Z), glm::vec3(-X, 0.0f, -Z), glm::vec3(X, 0.0f, -Z),
		glm::vec3(0.0f, Z, X), glm::vec3(0.0f, Z, -X), glm::vec3(0.0f, -Z, X), glm::vec3(0.0f, -Z, -X),
		glm::vec3(Z, X, 0.0f), glm::vec3(-Z, X, 0.0f), glm::vec3(Z, -X, 0.0f), glm::vec3(-Z, -X, 0.0f)
	};

	std::vector<GLuint> indices = {
		0, 4, 1, 0, 9, 4, 9, 5, 4, 4, 5, 8, 4, 8, 1,
		8, 10, 1, 8, 3, 10, 5, 3, 8, 5, 2, 3, 2, 7, 3,
		7, 10, 3, 7, 6, 10, 7, 11, 6, 11, 0, 6, 0, 1, 6,
		6, 1, 10, 9, 0, 11, 9, 11, 2, 9, 2, 5, 7, 2, 11
	};

	// normalize the vertices
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i] = glm::normalize(vertices[i]);
	}

	// subdivide the triangles to increase the recursion level
	for (int r = 0; r < recursionLevel; ++r)
	{
		std::vector<GLuint> newIndices;
		for (int i = 0; i < indices.size(); i += 3)
		{
			GLuint v1 = indices[i];
			GLuint v2 = indices[i + 1];
			GLuint v3 = indices[i + 2];

			glm::vec3 mid1 = glm::normalize(vertices[v1] + vertices[v2]);
			glm::vec3 mid2 = glm::normalize(vertices[v2] + vertices[v3]);
			glm::vec3 mid3 = glm::normalize(vertices[v3] + vertices[v1]);

			vertices.push_back(mid1);
			vertices.push_back(mid2);
			vertices.push_back(mid3);

			GLuint m1 = vertices.size() - 3;
			GLuint m2 = vertices.size() - 2;
			GLuint m3 = vertices.size() - 1;

			newIndices.insert(newIndices.end(), { v1, m1, m3 });
			newIndices.insert(newIndices.end(), { v2, m2, m1 });
			newIndices.insert(newIndices.end(), { v3, m3, m2 });
			newIndices.insert(newIndices.end(), { m1, m2, m3 });
		}
		indices = std::move(newIndices);
	}

	return vertices;
}

// tennis ball parameters
void drawIcosphere(GLuint vertexArrayObject, int vertexCount, int renderMode);	// draw ball
void ball(const mat4& tennisBall, const GLuint& worldMatrixLocation, int renderMode);	// render
float ballXScale = 2.0f;
float ballYScale = 2.0f;
float ballZScale = 2.0f;

// ================================================================ //
//			assignment 2 additional code - resize window			//
// ================================================================ //

// resize window
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	// Update the projection matrix with the new aspect ratio
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	mat4 projectionMatrix = perspective(camera_fov, aspectRatio, 0.1f, 100.0f);
	//SetUniformMat4(shaderProgram, "projectionMatrix", projectionMatrix);
}

// ================================================================ //
//			assignment 2 additional code - lighting	(lab 8)			//
// ================================================================ //

const char* defaultVert();	// vertex shader
const char* defaultFrag();	// vertex shader

template <class T>
void SetUniform1Value(GLuint shader_id, const char* uniform_name,
	T uniform_value) {
	glUseProgram(shader_id);
	glUniform1i(glGetUniformLocation(shader_id, uniform_name), uniform_value);
	glUseProgram(0);
}

void SetUniformVec3(GLuint shader_id, const char* uniform_name,
	vec3 uniform_value) {
	glUseProgram(shader_id);
	glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1,
		value_ptr(uniform_value));
}

const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

// For spinning model
float spinningAngle = 0.0f;

// ================================================================ //
//							main 
// ================================================================ //
int main(int argc, char*argv[]) {
		// setup GLFW and OpenGl version (based on lab2)
	if (!initContext())
		return -1;		// exit

	// Load Textures, based on lab
#if defined(PLATFORM_OSX)
	GLuint brickTextureID = loadTexture("Textures/brick.jpg");
	GLuint cementTextureID = loadTexture("Textures/cement.jpg");`
#else
	//GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
	//GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
	GLuint greenTextureID = loadTexture("../Assets/Textures/green.jpg");
	GLuint clayTextureID = loadTexture("../Assets/Textures/clay.jpg");
	GLuint glossTextureID = loadTexture("../Assets/Textures/gloss.jpg");
	GLuint skinTextureID = loadTexture("../Assets/Textures/skin.jpg");

	//GLuint shaderShadow = loadSHADER("../Assets/Shader/shadow_vertex.glsl", "../Assets/Shader/shadow_fragment.glsl");
	//GLuint shaderScene = loadSHADER("../Assets/Shader/scene_vertex.glsl","../Assets/Shader/scene_fragment.glsl");
#endif

	// background color
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);

	// Compile and link shaders here ...
	int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
	int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
	//glUseProgram(shaderProgram);					// set
		
	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
											WINDOW_WIDTH/WINDOW_HEIGHT,  // aspect ratio
											0.01f, 100.0f);   // near and far (near > 0)
	
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	// Set View and Projection matrices on both shaders
	setViewMatrix(colorShaderProgram, viewMatrix);
	setViewMatrix(texturedShaderProgram, viewMatrix);
	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);

	//SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);
	//SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // resize window

	// Define and upload geometry to the GPU here ...
	//int vao = createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));
	int texturedCubeVAO = createTexturedCubeVertexArrayObject(); // texture
	int sphereVAO = createSphereObject();

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	
	// render mode
	char renderMode = GL_TRIANGLES;	

	// interaction - key press event
	glfwSetKeyCallback(window, interaction);

	//glBindVertexArray(vao);
	//glBindVertexArray(texturedCubeVAO);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);


	GLuint colorLocation = glGetUniformLocation(colorShaderProgram, "objectColor");

	// ================================================================ //
	//							light	(lab 8)							//
	// ================================================================ //
	


	int defaultLightShader = compileAndLinkShaders(defaultVert(), defaultFrag());

#if defined(PLATFORM_OSX)
	std::string shaderPathPrefix = "Shaders/";
#else
	std::string shaderPathPrefix = "../Assets/Shaders/";
#endif

	GLuint shaderScene = loadSHADER(shaderPathPrefix + "scene_vertex.glsl",
		shaderPathPrefix + "scene_fragment.glsl");

	GLuint shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl",
		shaderPathPrefix + "shadow_fragment.glsl");


	// Set projection matrix on both shaders
	SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);

	// Set view matrix on both shaders
	SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

	float lightAngleOuter = radians(30.0f);
	float lightAngleInner = radians(20.0f);
	// Set light cutoff angles on scene shader
	SetUniform1Value(shaderScene, "light_cutoff_inner", cos(lightAngleInner));
	SetUniform1Value(shaderScene, "light_cutoff_outer", cos(lightAngleOuter));

	// Set light color on scene shader
	SetUniformVec3(shaderScene, "light_color", vec3(1));

	// Set object color on scene shader
	SetUniformVec3(shaderScene, "object_color", vec3(1));

	

	// ================================================================ //
	//							loop									//
	// ================================================================ //
	while (!glfwWindowShouldClose(window)) {
		
		glBindVertexArray(texturedCubeVAO);

		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw textured geometry
		if (toggleTexture) {
			//glUseProgram(texturedShaderProgram);
		}
		else {
			//glUseProgram(colorShaderProgram);
		}
		//glUseProgram(colorShaderProgram);

		// ================================================================ //
		//							draw									//
		// ================================================================ //
		
		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
		glBindTexture(GL_TEXTURE_2D, clayTextureID);
		//glBindTexture(GL_TEXTURE_2D, cementTextureID);
		glUniform1i(textureLocation, 0);   // Set our Texture sampler to user Texture Unit 0

		// Floor Grid
		mat4 worldGridMatrix;
		if (toggleFloorGrid)	// true = draw floor grid
		{
			//createVertexArrayObject(vec3(0.5f, 1.0f, 0.0f));	// grass color rgb, 124, 252, 0
			for (int i = 0; i <= 100; ++i)	// 100x100
			{
				worldGridMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -50.0f + i * 1.0f)) * scale(mat4(1.0f), vec3(100.0f, 0.15f, 0.15f));
				// toggle texture
				if (toggleTexture) {
					setWorldMatrix(texturedShaderProgram, worldGridMatrix);
				}
				else {
					setWorldMatrix(colorShaderProgram, worldGridMatrix);
				}
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5f, 1.0f, 0.0f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);

				worldGridMatrix = translate(mat4(1.0f), vec3(-50.0f + i * 1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.15f, 0.15f, 100.0f));
				// toggle texture
				if (toggleTexture) {
					setWorldMatrix(texturedShaderProgram, worldGridMatrix);
				}
				else {
					setWorldMatrix(colorShaderProgram, worldGridMatrix);
				}
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5f, 1.0f, 0.0f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// Draw colored geometry
		//glUseProgram(colorShaderProgram);
			   
		// Draw axis 
		mat4 axisLines;
		glBindTexture(GL_TEXTURE_2D, glossTextureID);

		//createVertexArrayObject(vec3(1.0f, 0.0f, 0.0f));	// red 
		axisLines = translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0), vec3(5.0f, 0.2f, 0.2f));
		setWorldMatrix(colorShaderProgram, axisLines);
		glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//createVertexArrayObject(vec3(0.0f, 1.0f, 0.0f));	// green 
		axisLines = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0), vec3(0.2f, 5.0f, 0.2f));
		setWorldMatrix(colorShaderProgram, axisLines);
		glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//createVertexArrayObject(vec3(0.0f, 0.0f, 1.0f));	// blue 
		axisLines = translate(mat4(1.0f), vec3(0.0f, -0.0f, 2.5f)) * scale(mat4(1.0), vec3(0.2f, 0.2f, 5.0f));
		setWorldMatrix(colorShaderProgram, axisLines);
		glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 1.0f)));
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

		/*
		// Draw arm and racket
		//createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));	// default black color
		mat4 armTennis = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f));		// origin xyz position
		armTennis = translate(armTennis, armTennisPosition);				// wasd movement
		armTennis = scale(armTennis, vec3(armTennisScale, armTennisScale, armTennisScale));	// u,j scale
		armTennis = rotate(armTennis, glm::radians(armTennisAngleZ), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		armTennis = rotate(armTennis, glm::radians(armTennisAngleY), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis		
		glUniformMatrix4fv(texturedShaderProgram, 1, GL_FALSE, &armTennis[0][0]);
		*/

		// Draw arm and racket
		//createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));	// default black color
		original_cube = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));		// origin xyz position
		original_cube = translate(original_cube, armTennisPosition);				// wasd movement
		original_cube = scale(original_cube, vec3(armTennisScale, armTennisScale, armTennisScale));	// u,j scale
		original_cube = rotate(original_cube, glm::radians(armTennisAngleZ), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		original_cube = rotate(original_cube, glm::radians(armTennisAngleY), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis		
		original_cube = rotate(original_cube, glm::radians(armTennisAngleX), vec3(1.0f, 0.0f, 0.0f));		// z,c y axis		
		glUniformMatrix4fv(texturedShaderProgram, 1, GL_FALSE, &original_cube[0][0]);
		

		//lower_arm = original_cube;

		// toggle tennis racket texture
		if (toggleTexture) {	// on
			//glUseProgram(texturedShaderProgram);		// set texture
			glBindTexture(GL_TEXTURE_2D, skinTextureID);	// green texture

			// render arm
			lowerArm(original_cube, texturedShaderProgram, renderMode);			// render loawer arm
			upperArm(original_cube, texturedShaderProgram, renderMode);			// render upper arm
			wrist(original_cube, texturedShaderProgram, renderMode);			// render wrist


			glBindTexture(GL_TEXTURE_2D, glossTextureID);	// green texture

			// render tennis head
			tennisHead(original_cube, texturedShaderProgram, renderMode);		// render tennis head

			// render tennis body
			tennisHandle(original_cube, texturedShaderProgram, renderMode);		// render tennis handle
			tennisRimBottom(original_cube, texturedShaderProgram, renderMode);	// render tennis bottom rim
			tennisRimLeft(original_cube, texturedShaderProgram, renderMode);	// render tennis left rim
			tennisRimRight(original_cube, texturedShaderProgram, renderMode);	// render tennis right rim
			tennisRimTop(original_cube, texturedShaderProgram, renderMode);		// render tennis top rim
		}
		else {	// off
			//glUseProgram(colorShaderProgram);	// set color

			// render arm
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.9f, 0.75f, 0.67f)));
			lowerArm(original_cube, colorShaderProgram, renderMode);			// render loawer arm
			upperArm(original_cube, colorShaderProgram, renderMode);			// render upper arm
			wrist(original_cube, colorShaderProgram, renderMode);			// render wrist

			// render tennis head
			glUniform3fv(colorLocation, 1, value_ptr(vec3(00.0f, 0.4f, 0.0f)));
			tennisHead(original_cube, colorShaderProgram, renderMode);		// render tennis head

			// render tennis body
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.4f, 0.0f, 0.0f)));
			tennisHandle(original_cube, colorShaderProgram, renderMode);		// render tennis handle
			tennisRimBottom(original_cube, colorShaderProgram, renderMode);	// render tennis bottom rim
			tennisRimLeft(original_cube, colorShaderProgram, renderMode);	// render tennis left rim
			tennisRimRight(original_cube, colorShaderProgram, renderMode);	// render tennis right rim
			tennisRimTop(original_cube, colorShaderProgram, renderMode);		// render tennis top rim
		}

		/*
		// render arm
		glUniform3fv(colorLocation, 1, value_ptr(vec3(0.9f, 0.75f, 0.67f)));
		lowerArm(armTennis, colorShaderProgram, renderMode);			// render loawer arm
		upperArm(armTennis, colorShaderProgram, renderMode);			// render upper arm

		// render tennis head
		glUniform3fv(colorLocation, 1, value_ptr(vec3(00.0f, 0.4f, 0.0f)));
		tennisHead(armTennis, colorShaderProgram, renderMode);		// render tennis head

		// render tennis body
		glUniform3fv(colorLocation, 1, value_ptr(vec3(0.4f, 0.0f, 0.0f)));
		tennisHandle(armTennis, colorShaderProgram, renderMode);		// render tennis handle
		tennisRimBottom(armTennis, colorShaderProgram, renderMode);	// render tennis bottom rim
		tennisRimLeft(armTennis, colorShaderProgram, renderMode);	// render tennis left rim
		tennisRimRight(armTennis, colorShaderProgram, renderMode);	// render tennis right rim
		tennisRimTop(armTennis, colorShaderProgram, renderMode);		// render tennis top rim
		*/

		// ================================================================ //
		//						Tennis Ball Sphere							//
		// ================================================================ //

		glBindVertexArray(sphereVAO);

		mat4 pillarWorldMatrix = original_cube;	// connect with other models
		pillarWorldMatrix = translate(pillarWorldMatrix, vec3(-3.0f, 14.0f, 0.0f));		// origin xyz position
		//pillarWorldMatrix = scale(pillarWorldMatrix, vec3(-3.0f, 7.0f, 0.0f));		// origin xyz position

		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, greenTextureID);	// green texture
			setWorldMatrix(texturedShaderProgram, pillarWorldMatrix);

		}
		else {	// off
			glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));	// color
			setWorldMatrix(colorShaderProgram, pillarWorldMatrix);
		}

		glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

		/*
		mat4 tennisBall = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));		// origin xyz position
		// toggle texture, tennis ball texture
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, greenTextureID);	// green texture
			//glUseProgram(texturedShaderProgram);		// set texture
			ball(original_cube, texturedShaderProgram, renderMode);	// render, original_cube means move together
		}
		else {	// off
			//glUseProgram(colorShaderProgram);	// set color
			glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));	// color
			ball(original_cube, colorShaderProgram, renderMode);	// render, original_cube means move together
		}
		*/

		glBindVertexArray(texturedCubeVAO);

		// ================================================================ //
		//							Controls								//
		// ================================================================ //

		bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS; // check if shift is pressed
		bool ctrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS; // check if ctrl is pressed

		// left/right movement on z-axis
		bool qPressed = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
		bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

		// turning movement on y-axis
		bool zPressed = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
		bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;

		// leaning movement on x-axis
		bool fPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
		bool gPressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;

		// ================================================================ //
		//							Movement								//
		// ================================================================ //

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


		// === z-axis (lean left right) ===

		// model rotation around z-axis 
		if (qPressed && !shiftPressed && !ctrlPressed) {		// rotate left
			armTennisAngleZ = armTennisAngleZ + rotateSpeed;
		}
		if (ePressed && !shiftPressed && !ctrlPressed) {		// rotate right
			armTennisAngleZ = armTennisAngleZ - rotateSpeed;
		}
		// lower model rotation around z-axis  
		if (qPressed && shiftPressed && !ctrlPressed) {		// rotate left
			armTennisAngleZlower = armTennisAngleZlower - rotateSpeed;
			armTennisAngleZ = armTennisAngleZ + rotateSpeed;
		}
		if (ePressed && shiftPressed && !ctrlPressed) {		// rotate right
			armTennisAngleZlower = armTennisAngleZlower + rotateSpeed;
			armTennisAngleZ = armTennisAngleZ - rotateSpeed;
		}
		// lower model rotation around z-axis 
		if (qPressed && !shiftPressed && ctrlPressed) {		// rotate left
			armTennisAngleZHigher = armTennisAngleZHigher - rotateSpeed;
			armTennisAngleZ = armTennisAngleZ + rotateSpeed;
			displacementLR = displacementLR + 0.1f;
			displacementUD = displacementUD - 0.05f;
		}
		if (ePressed && !shiftPressed && ctrlPressed) {		// rotate right
			armTennisAngleZHigher = armTennisAngleZHigher + rotateSpeed;
			armTennisAngleZ = armTennisAngleZ - rotateSpeed;
			displacementLR = displacementLR - 0.1f;
			displacementUD = displacementUD + 0.05f;
		}

		// === y-axis (turn left right) ===

		// model rotation around y-axis
		if (zPressed && !shiftPressed && !ctrlPressed) {		// rotate left
			armTennisAngleY = armTennisAngleY + rotateSpeed;
		}
		if (cPressed && !shiftPressed && !ctrlPressed) {		// rotate right
			armTennisAngleY = armTennisAngleY - rotateSpeed;
		}
		// lower model rotation around y-axis 
		if (zPressed && shiftPressed && !ctrlPressed) {		// rotate left
			armTennisAngleYlower = armTennisAngleYlower - rotateSpeed;
			armTennisAngleY = armTennisAngleY + rotateSpeed;
			//displacementY1 = displacementY1 + 0.1f;
			//displacementY2 = displacementY2 + 0.1f;

		}
		if (cPressed && shiftPressed && !ctrlPressed) {		// rotate right
			armTennisAngleYlower = armTennisAngleYlower + rotateSpeed;
			armTennisAngleY = armTennisAngleY - rotateSpeed;
			//displacementY1 = displacementY1 + 0.1f;
			//displacementY2 = displacementY2 + 0.1f;

		}
		// lower model rotation around y-axis 
		if (zPressed && !shiftPressed && ctrlPressed) {		// rotate left
			armTennisAngleYHigher = armTennisAngleYHigher - rotateSpeed;
			armTennisAngleY = armTennisAngleY + rotateSpeed;
			//displacementY1 = displacementY1 - 0.05f;
			displacementY2 = displacementY2 + 0.10f;
		}
		if (cPressed && !shiftPressed && ctrlPressed) {		// rotate right
			armTennisAngleYHigher = armTennisAngleYHigher + rotateSpeed;
			armTennisAngleY = armTennisAngleY - rotateSpeed;
			//displacementY1 = displacementY1 + 0.05f;
			displacementY2 = displacementY2 - 0.10f;
		}



		// === x-axis (lean front back) ===

		// model rotation around x-axis 
		if (fPressed && !shiftPressed && !ctrlPressed) {		// learn forward
			armTennisAngleX = armTennisAngleX + rotateSpeed;
		}
		if (gPressed && !shiftPressed && !ctrlPressed) {		// lean backwards
			armTennisAngleX = armTennisAngleX - rotateSpeed;
		}
		// lower model rotation around x-axis 
		if (fPressed && shiftPressed && !ctrlPressed) {		// learn forward
			armTennisAngleXlower = armTennisAngleXlower - rotateSpeed;
			armTennisAngleX = armTennisAngleX + rotateSpeed;
		}
		if (gPressed && shiftPressed && !ctrlPressed) {		// lean backwards
			armTennisAngleXlower = armTennisAngleXlower + rotateSpeed;
			armTennisAngleX = armTennisAngleX - rotateSpeed;
		}
		// lower model rotation around x-axis 
		if (fPressed && !shiftPressed && ctrlPressed) {		// learn forward
			//armTennisAngleXHigher = armTennisAngleXHigher - rotateSpeed;
			//armTennisAngleX = armTennisAngleX + rotateSpeed;

			//displacementX = displacementX - 0.08f;
			//displacementX2 = displacementX2 - 0.16f;
		}
		if (gPressed && !shiftPressed && ctrlPressed) {		// lean backwards
			//armTennisAngleXHigher = armTennisAngleXHigher + rotateSpeed;
			//armTennisAngleX = armTennisAngleX - rotateSpeed;
			//displacementX = displacementX + 0.08f;
			//displacementX2 = displacementX2 + 0.16f;

		}

		// limit angle values, realistic movements
		float limit = 10.0f;
		if (armTennisAngleZlower > limit)	// upper arm, racket movement, left right
		{
			armTennisAngleZlower = limit;
		}
		else if (armTennisAngleZlower < -limit)
		{
			armTennisAngleZlower = -limit;
		}
		
		if (armTennisAngleZ > limit)	// movement of all parts, left right
		{
			armTennisAngleZ = limit;
		}
		else if (armTennisAngleZ < -limit)
		{
			armTennisAngleZ = -limit;
		}

		if (armTennisAngleZHigher > limit)		// racket movement only, left right
		{
			armTennisAngleZHigher = limit;
		}
		else if (armTennisAngleZHigher < -limit)
		{
			armTennisAngleZHigher = -limit;
		}


		float limit2 = 25.0f;
		if (armTennisAngleX > limit2)	// movement of all parts, lean front back
		{
			armTennisAngleX = limit2;
		}
		else if (armTennisAngleX < -limit2)
		{
			armTennisAngleX = -limit2;
		}
		if (armTennisAngleXHigher > limit2)	// movement of all parts, lean front back
		{
			armTennisAngleXHigher = limit2;
		}
		else if (armTennisAngleXHigher < -limit2)
		{
			armTennisAngleXHigher = -limit2;
		}
		if (armTennisAngleXlower > limit2)	// movement of all parts, lean front back
		{
			armTennisAngleXlower = limit2;
		}
		else if (armTennisAngleXlower < -limit2)
		{
			armTennisAngleXlower = -limit2;
		}

		// limit readjust displacement values, realistic movements
		float limitDispl1 = 0.5f;	// z axis limit, left and right
		if (displacementLR > limitDispl1)
		{
			displacementLR = limitDispl1;
		}
		else if (displacementLR < -limitDispl1)
		{
			displacementLR = -limitDispl1;
		}

		float limitDispl2 = 0.2f;	// z axis limit, up an down
		if (displacementUD > limitDispl2)
		{
			displacementUD = limitDispl2;
		}
		else if (displacementUD < -limitDispl2)
		{
			displacementUD = -limitDispl2;
		}

		float limitDispl3 = 1.0f;	// x axis limist
		if (displacementX > limitDispl3)
		{
			displacementX = limitDispl3;
		}
		else if (displacementX < -limitDispl3)
		{
			displacementX = -limitDispl3;
		}

		float limitDispl4 = 2.0f;	// x axis limist
		if (displacementX2 > limitDispl4)
		{
			displacementX2 = limitDispl4;
		}
		else if (displacementX2 < -limitDispl4)
		{
			displacementX2 = -limitDispl4;
		}



		float limitDispl5 = 0.15f;	// y axis limist
		if (displacementY1 > limitDispl5)
		{
			displacementY1 = limitDispl5;
		}
		else if (displacementY1 < -limitDispl5)
		{
			displacementY1 = -limitDispl5;
		}

		float limitDispl6 = 0.6f;	// y axis limist
		if (displacementY2 > limitDispl6)
		{
			displacementY2 = limitDispl6;
		}
		else if (displacementY2 < -limitDispl6)
		{
			displacementY2 = -limitDispl6;
		}

		float limitDispl7 = 30.0f;	// y axis limist
		if (armTennisAngleY > limitDispl7)
		{
			armTennisAngleY = limitDispl7;
		}
		else if (armTennisAngleY < -limitDispl7)
		{
			armTennisAngleY = -limitDispl7;
		}
		if (armTennisAngleYlower > limitDispl7)
		{
			armTennisAngleYlower = limitDispl7;
		}
		else if (armTennisAngleYlower < -limitDispl7)
		{
			armTennisAngleYlower = -limitDispl7;
		}
		if (armTennisAngleYHigher > limitDispl7)
		{
			armTennisAngleYHigher = limitDispl7;
		}
		else if (armTennisAngleYHigher < -limitDispl7)
		{
			armTennisAngleYHigher = -limitDispl7;
		}


		// ================================================================ //
		//			Light	(based on lab 8 and online ressource)			//
		// ================================================================ //
		
		/*
		// light parameters
		float phi = glfwGetTime() * 0.5f * 3.14f;
		vec3 lightPosition = vec3(0.6f, 50.0f, 5.0f); // the location of the light in 3D space: fixed position
		vec3(cosf(phi)* cosf(phi), sinf(phi),
			-cosf(phi) * sinf(phi)) *
			5.0f;  // variable position

		vec3 lightFocus(0, 0, -1);  // the point in 3D space the light "looks" at
		vec3 lightDirection = normalize(lightFocus - lightPosition);

		float lightNearPlane = 0.01f;
		float lightFarPlane = 400.0f;

		mat4 lightProjMatrix = //frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
			perspective(50.0f, (float)DEPTH_MAP_TEXTURE_SIZE / (float)DEPTH_MAP_TEXTURE_SIZE, lightNearPlane, lightFarPlane);
		mat4 lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0, 1, 0));
		*/
		// light parameters
		float phi = glfwGetTime() * 0.5f * 3.14f;
		float radius = 5.0f;
		vec3 lightPosition = vec3(cosf(phi) * radius, 50.0f, sinf(phi) * radius); // Updated position
		vec3 lightFocus(0, 0, 0); // Point the light towards the origin or any specific point you want
		vec3 lightDirection = normalize(lightFocus - lightPosition);

		float lightNearPlane = 0.01f;
		float lightFarPlane = 400.0f;

		mat4 lightProjMatrix = perspective(50.0f, 1.0f, lightNearPlane, lightFarPlane); // Use aspect ratio 1.0f for point light
		mat4 lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0, 1, 0));

		SetUniformMat4(shaderScene, "light_proj_view_matrix", lightProjMatrix* lightViewMatrix);
		SetUniform1Value(shaderScene, "light_near_plane", lightNearPlane);
		SetUniform1Value(shaderScene, "light_far_plane", lightFarPlane);
		SetUniformVec3(shaderScene, "light_position", lightPosition);
		SetUniformVec3(shaderScene, "light_direction", lightDirection);

		// Spinning model rotation animation
		spinningAngle += 45.0f * dt;  // This is equivalent to 45 degrees per second

		// Set model matrix and send to both shaders
		mat4 modelMatrix =  // mat4(1.0f);
			glm::translate(mat4(1.0f), vec3(0.0f, 1.0f, -3.0f)) *
			glm::rotate(mat4(1.0f), radians(spinningAngle), vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(mat4(1.0f), vec3(0.1f));

		SetUniformMat4(shaderScene, "model_matrix", modelMatrix);
		SetUniformMat4(shaderShadow, "transform_in_light_space", lightProjMatrix* lightViewMatrix* modelMatrix);

		// Set the view matrix for first person camera and send to both shaders
		mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

		// Set view position on scene shader
		SetUniformVec3(shaderScene, "view_position", cameraPosition);


		mat4 new_test;
		new_test = translate(new_test, vec3(1.0f, 1.0f, 1.0f)) *
			scale(mat4(1.0), vec3(4.0f, 4.0f, 4.0f));


		glUseProgram(shaderShadow);
		SetUniformVec3(shaderScene, "light_position", lightPosition);
		//glDrawArrays(renderMode, 0, 36);






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
			cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
		}

		// tilt, middle mouse botton pressed
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
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
		updatedViewMatrix = viewMatrix * worldMatrix;	// update 
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

		setViewMatrix(colorShaderProgram, updatedViewMatrix);	// set 
		setViewMatrix(texturedShaderProgram, updatedViewMatrix);	// set 

		mat4 projectionMatrix = perspective(camera_fov, WINDOW_WIDTH/ WINDOW_HEIGHT, 0.1f, 100.0f);
		setProjectionMatrix(colorShaderProgram, projectionMatrix);	// set new fov
		setProjectionMatrix(texturedShaderProgram, projectionMatrix);	// set 
		//SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);


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

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

	return shaderProgram;
}


GLuint loadTexture(const char* filename)	// based on lab 4
{
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);


	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}


const char* defaultVert() {				// based on the lab
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	const char* default_vert = R"(
		// Outputs the texture coordinates to the Fragment Shader
		out vec2 texCoord;
		// Outputs the fragment position of the light
		out vec4 fragPosLight;

		// Imports the camera matrix
		uniform mat4 camMatrix;
		// Imports the transformation matrices
		uniform mat4 model;
		uniform mat4 translation;
		uniform mat4 rotation;
		uniform mat4 scale;
		// Imports the light matrix
		uniform mat4 lightProjection;

		void main()
		{
			// Calculates current position
			crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
			// Assigns the normal from the Vertex Data to "Normal"
			Normal = aNormal;
			// Assigns the colors from the Vertex Data to "color"
			color = aColor;
			// Assigns the texture coordinates from the Vertex Data to "texCoord"
			texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
			// Calculates the position of the light fragment for the fragment shader
			fragPosLight = lightProjection * vec4(crntPos, 1.0f);
	
			// Outputs the positions/coordinates of all vertices
			gl_Position = camMatrix * vec4(crntPos, 1.0);
		}
	)";
	return default_vert;
}

const char* defaultFrag() {				// based on the lab
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	const char* default_frag = R"(
		#version 330 core

		// Outputs colors in RGBA
		out vec4 FragColor;

		// Imports the current position from the Vertex Shader
		in vec3 crntPos;
		// Imports the normal from the Vertex Shader
		in vec3 Normal;
		// Imports the color from the Vertex Shader
		in vec3 color;
		// Imports the texture coordinates from the Vertex Shader
		in vec2 texCoord;
		// Imports the fragment position of the light
		in vec4 fragPosLight;

		// Gets the Texture Units from the main function
		uniform sampler2D diffuse0;
		uniform sampler2D specular0;
		uniform sampler2D shadowMap;
		uniform samplerCube shadowCubeMap;
		// Gets the color of the light from the main function
		uniform vec4 lightColor;
		// Gets the position of the light from the main function
		uniform vec3 lightPos;
		// Gets the position of the camera from the main function
		uniform vec3 camPos;
		uniform float farPlane;

		vec4 pointLight()
		{	
			// used in two variables so I calculate it here to not have to do it twice
			vec3 lightVec = lightPos - crntPos;

			// intensity of light with respect to distance
			float dist = length(lightVec);
			float a = 0.0003f;
			float b = 0.00002f;
			float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

			// ambient lighting
			float ambient = 0.20f;

			// diffuse lighting
			vec3 normal = normalize(Normal);
			vec3 lightDirection = normalize(lightVec);
			float diffuse = max(dot(normal, lightDirection), 0.0f);

			// specular lighting
			float specular = 0.0f;
			if (diffuse != 0.0f)
			{
				float specularLight = 0.50f;
				vec3 viewDirection = normalize(camPos - crntPos);
				vec3 halfwayVec = normalize(viewDirection + lightDirection);
				float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
				specular = specAmount * specularLight;
			};

			// Shadow value
			float shadow = 0.0f;
			vec3 fragToLight = crntPos - lightPos;
			float currentDepth = length(fragToLight);
			float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.0005f); 

			// Not really a radius, more like half the width of a square
			int sampleRadius = 2;
			float offset = 0.02f;
			for(int z = -sampleRadius; z <= sampleRadius; z++)
			{
				for(int y = -sampleRadius; y <= sampleRadius; y++)
				{
					for(int x = -sampleRadius; x <= sampleRadius; x++)
					{
						float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
						// Remember that we divided by the farPlane?
						// Also notice how the currentDepth is not in the range [0, 1]
						closestDepth *= farPlane;
						if (currentDepth > closestDepth + bias)
							shadow += 1.0f;     
					}    
				}
			}
			// Average shadow
			shadow /= pow((sampleRadius * 2 + 1), 3);

			return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;
		}

		vec4 direcLight()
		{
			// ambient lighting
			float ambient = 0.20f;

			// diffuse lighting
			vec3 normal = normalize(Normal);
			vec3 lightDirection = normalize(lightPos);
			float diffuse = max(dot(normal, lightDirection), 0.0f);

			// specular lighting
			float specular = 0.0f;
			if (diffuse != 0.0f)
			{
				float specularLight = 0.50f;
				vec3 viewDirection = normalize(camPos - crntPos);
				vec3 halfwayVec = normalize(viewDirection + lightDirection);
				float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
				specular = specAmount * specularLight;
			};


			// Shadow value
			float shadow = 0.0f;
			// Sets lightCoords to cull space
			vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
			if(lightCoords.z <= 1.0f)
			{
				// Get from [-1, 1] range to [0, 1] range just like the shadow map
				lightCoords = (lightCoords + 1.0f) / 2.0f;
				float currentDepth = lightCoords.z;
				// Prevents shadow acne
				float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

				// Smoothens out the shadows
				int sampleRadius = 2;
				vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
				for(int y = -sampleRadius; y <= sampleRadius; y++)
				{
					for(int x = -sampleRadius; x <= sampleRadius; x++)
					{
						float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
						if (currentDepth > closestDepth + bias)
							shadow += 1.0f;     
					}    
				}
				// Get average shadow
				shadow /= pow((sampleRadius * 2 + 1), 2);

			}

			return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, texCoord).r * specular  * (1.0f - shadow)) * lightColor;
		}

		vec4 spotLight()
		{
			// controls how big the area that is lit up is
			float outerCone = 0.90f;
			float innerCone = 0.95f;

			// ambient lighting
			float ambient = 0.20f;

			// diffuse lighting
			vec3 normal = normalize(Normal);
			vec3 lightDirection = normalize(lightPos - crntPos);
			float diffuse = max(dot(normal, lightDirection), 0.0f);

			// specular lighting
			float specular = 0.0f;
			if (diffuse != 0.0f)
			{
				float specularLight = 0.50f;
				vec3 viewDirection = normalize(camPos - crntPos);
				vec3 halfwayVec = normalize(viewDirection + lightDirection);
				float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
				specular = specAmount * specularLight;
			};

			// calculates the intensity of the crntPos based on its angle to the center of the light cone
			float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
			float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);


			// Shadow value
			float shadow = 0.0f;
			// Sets lightCoords to cull space
			vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
			if(lightCoords.z <= 1.0f)
			{
				// Get from [-1, 1] range to [0, 1] range just like the shadow map
				lightCoords = (lightCoords + 1.0f) / 2.0f;
				float currentDepth = lightCoords.z;
				// Prevents shadow acne
				float bias = max(0.00025f * (1.0f - dot(normal, lightDirection)), 0.000005f);

				// Smoothens out the shadows
				int sampleRadius = 2;
				vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
				for(int y = -sampleRadius; y <= sampleRadius; y++)
				{
					for(int x = -sampleRadius; x <= sampleRadius; x++)
					{
						float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
						if (currentDepth > closestDepth + bias)
							shadow += 1.0f;     
					}    
				}
				// Get average shadow
				shadow /= pow((sampleRadius * 2 + 1), 2);

			}

			return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;
		}


		void main()
		{
			// outputs final color
			FragColor = pointLight();
		}
	)";
	return default_frag;
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
		"uniform vec3 objectColor;"
		"in vec3 vertexColor;"
		"out vec4 FragColor;"
		""
		""
		"void main()"
		"{"
		"   FragColor = vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);"
		"}";
}

const char* getTexturedVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		"layout (location = 2) in vec2 aUV;"
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"out vec2 vertexUV;"
		""
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"   vertexUV = aUV;"
		"}";
}

const char* getTexturedFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"in vec2 vertexUV;"
		"uniform sampler2D textureSampler;"
		""
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   vec4 textureColor = texture( textureSampler, vertexUV );"
		"   FragColor = textureColor ;"
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

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create and bind vertex array object for tennis ball
	glGenVertexArrays(1, &tennisBallVertexArrayObject);
	glBindVertexArray(tennisBallVertexArrayObject);

	return true;
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

// shader variable setters
void SetUniformMat4(GLuint shader_id, const char* uni_name, mat4 uniform_value) {
	glUseProgram(shader_id);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, uni_name),
						1, GL_FALSE, &uniform_value[0][0]);
}


/*
void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {	
	mat4 lower_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	lower_arm = translate(lower_arm, vec3(0.0f, 0.0f, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(45.0f), vec3(0.0f, 0.0f, 1.0f)) *	// 45 angle
				scale(mat4(1.0), vec3(4.0f, 1.0f, 1.0f));
	//lower_arm = rotate(mat4(1.0f), glm::radians(-armTennisAngleZlower), vec3(0.0f, 0.0f, 1.0f)); // adjust angle
	setWorldMatrix(worldMatrixLocation, lower_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 upper_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	upper_arm = translate(upper_arm, vec3(1.3f, 3.1f, 0.0f)) *
		//rotate(mat4(1.0f), glm::radians(armTennisAngleZlower), vec3(0.0f, 0.0f, 1.0f)) * // adjust angle
				scale(mat4(1.0), vec3(1.0f, 4.0f, 1.0f));
	//upper_arm = translate(upper_arm, vec3(-sin(radians(armTennisAngleZlower)) * 2.3f, 0.9f - sin(radians(90 - armTennisAngleZlower)), 0.0f));
	setWorldMatrix(worldMatrixLocation, upper_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw

}

void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_handle = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_handle = translate(tennis_handle, vec3(1.3f, 7.3f, 0.0f)) *
		//rotate(mat4(1.0f), glm::radians(armTennisAngleZlower), vec3(0.0f, 0.0f, 1.0f)) * // adjust angle
					scale(mat4(1.0), vec3(0.8f, 5.0f, 0.8f));
	//tennis_handle = translate(tennis_handle, vec3(-sin(radians(armTennisAngleZlower)) * 8.0f, 0.9f - sin(radians(90 - armTennisAngleZlower)), 0.0f));
	setWorldMatrix(worldMatrixLocation, tennis_handle);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}	

void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 8.0f; i++) {	// |
		mat4 tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(-0.38f + i / 2, 13.6f, 0.0f)) *
			scale(mat4(1.0), vec3(0.2f, 7.0f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
	for (float i = 0.0f; i < 15.0f; i++) {	// --
		mat4 tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(1.35f, 10.0f + i / 2, 0.0f)) *
			scale(mat4(1.0), vec3(4.0f, 0.2f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimBottom = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimBottom = translate(tennis_RimBottom, vec3(1.35f, 10.0f, 0.0f)) *
		scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimBottom);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimLeft = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimLeft =	translate(tennis_RimLeft, vec3(-0.8f, 13.6f, 0.0f)) *
						scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimLeft);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimRight = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimRight =	translate(tennis_RimRight, vec3(3.5f, 13.6f, 0.0f)) *
						scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimRight);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimTop = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimTop =		translate(tennis_RimTop, vec3(1.35f, 17.2f, 0.0f)) *
						scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimTop);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}
*/


void cube(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	original_cube = armTennis;
	original_cube = translate(original_cube, vec3(0.0f, 0.0f, 0.0f)) *
		scale(mat4(1.0), vec3(1.0f, 1.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, original_cube);			// set
	//glDrawArrays(renderMode, 0, 36);						// dont draw original cube
}

void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	lower_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	lower_arm = translate(lower_arm, vec3(0.0f, 0.0f, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(armTennisAngleZlower + armTennisAngleZHigher), vec3(0.0f, 0.0f, 1.0f)) *	
		rotate(mat4(1.0f), glm::radians(armTennisAngleYlower + armTennisAngleYHigher), vec3(0.0f, 1.0f, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(armTennisAngleXlower + armTennisAngleXHigher), vec3(1.0f, 0.0f, 0.0f)) *
		scale(mat4(1.0), vec3(4.0f, 1.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, lower_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	upper_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	upper_arm = translate(upper_arm, vec3(1.2f+ displacementLR+ displacementY1, 2.4f+ displacementUD, 0.0f+displacementX+ displacementY2)) *
	//upper_arm = translate(lower_arm, vec3(0.6f, 2.4f, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(armTennisAngleZHigher), vec3(0.0f, 0.0f, 1.0f)) *	
		rotate(mat4(1.0f), glm::radians(armTennisAngleYHigher), vec3(0.0f, 1.0f, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(armTennisAngleXHigher), vec3(1.0f, 0.0f, 0.0f)) *
		scale(mat4(1.0), vec3(1.0f, 5.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, upper_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw

}

void wrist(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	wrist_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	wrist_arm = translate(wrist_arm, vec3(1.2f + displacementLR, 5.5f + displacementUD, 0.0f+ displacementX2)) *
		scale(mat4(1.0), vec3(1.3f, 1.3f, 1.3f));
	setWorldMatrix(worldMatrixLocation, wrist_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw

}


void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	tennis_handle = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_handle = translate(tennis_handle, vec3(1.2f+ displacementLR, 7.3f+ displacementUD, 0.0f+ displacementX2)) *
		scale(mat4(1.0), vec3(0.8f, 5.0f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_handle);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 8.0f; i++) {	// |
		tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(-0.48f + i / 2 + displacementLR, 13.6f + displacementUD, 0.0f+ displacementX2)) *
			scale(mat4(1.0), vec3(0.2f, 7.0f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
	for (float i = 0.0f; i < 15.0f; i++) {	// --
		tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(1.25f + displacementLR, 10.0f + i / 2 + displacementUD, 0.0f + displacementX2)) *
			scale(mat4(1.0), vec3(4.0f, 0.2f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	tennis_RimBottom = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimBottom = translate(tennis_RimBottom, vec3(1.25f + displacementLR, 10.0f + displacementUD, 0.0f + displacementX2)) *
		scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimBottom);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	tennis_RimLeft = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimLeft = translate(tennis_RimLeft, vec3(-0.9f + displacementLR, 13.6f + displacementUD, 0.0f + displacementX2)) *
		scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimLeft);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	tennis_RimRight = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimRight = translate(tennis_RimRight, vec3(3.4f + displacementLR, 13.6f + displacementUD, 0.0f + displacementX2)) *
		scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimRight);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	tennis_RimTop = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimTop = translate(tennis_RimTop, vec3(1.25f + displacementLR, 17.2f + displacementUD, 0.0f + displacementX2)) *
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
	//updatedViewMatrix = rotate(updatedViewMatrix, glm::radians(angle), axis);
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
			armTennisAngleZ = 0.0f;
			armTennisAngleZlower = 0.0f;
			armTennisAngleZHigher = 0.0f;

			armTennisAngleY = 0.0f;
			armTennisAngleYlower = 0.0f;
			armTennisAngleYHigher = 0.0f;

			armTennisAngleX = 0.0f;
			armTennisAngleXlower = 0.0f;
			armTennisAngleXHigher = 0.0f;
			
			displacementLR = 0.0f;
			displacementUD = 0.0f;
			displacementX = 0.0f;
			displacementX2 = 0.0f;
			displacementY2 = 0.0f;
			displacementY1 = 0.0f;


			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = 0.0f;
			cameraPosition = vec3(0.6f, 6.0f, 25.0f);
			cameraLookAt = vec3(0.0f, 0.0f, -1.0f);

			break;
		default:
			break;
		}
	}

	// toggle texture (x)
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		toggleTexture = !toggleTexture;
	}

}

int createTexturedCubeVertexArrayObject() {	// based on lab 4
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBindVertexArray(0);

	glBindVertexArray(vertexBufferObject);

	return vertexArrayObject;
}

/*
void drawIcosphere(GLuint vertexArrayObject, int vertexCount, int renderMode)	{	// draw ball
	glBindVertexArray(vertexArrayObject);
	glDrawElements(renderMode, vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void ball(const mat4& tennisBall, const GLuint& worldMatrixLocation, int renderMode) {
	mat4 tennis_ball = tennisBall;

	// icosphere vertices 
	int recursionLevel = 3;	// higher = more smooth
	std::vector<glm::vec3> icosphereVertices = generateIcosphereVertices(recursionLevel);

	GLuint vertexBufferObject, indexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, icosphereVertices.size() * sizeof(glm::vec3), icosphereVertices.data(), GL_STATIC_DRAW);

	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// indices, draw triangles
	std::vector<GLuint> indices;
	for (int i = 0; i < icosphereVertices.size(); ++i) {
		indices.push_back(i);
	}

	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// set world matrix and render the tennis ball
	tennis_ball = translate(tennis_ball, vec3(-5.0f, 13.0f, 0.0f)) *
		scale(mat4(1.0), vec3(ballXScale, ballYScale, ballZScale));
	setWorldMatrix(worldMatrixLocation, tennis_ball);
	drawIcosphere(vertexArrayObject, indices.size(), renderMode);

	// clean up
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &indexBufferObject);
	glDeleteVertexArrays(1, &vertexArrayObject);
}
*/

int createSphereObject()	// code based on https://pastebin.com/erWMP4s4
{
	// A vertex is a point on a polygon, it contains positions and other data (eg: colors)
	unsigned int sphereVAO;
	glGenVertexArrays(1, &sphereVAO);

	unsigned int vbo, ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> colors;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount = indices.size();

	std::vector<float> data;
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (colors.size() > 0) {
			data.push_back(colors[i].x);
			data.push_back(colors[i].y);
			data.push_back(colors[i].z);
		}
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	float stride = (3 + 2 + 3 + 3) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
	glBindVertexArray(0); // Unbind to not modify the VAO

	return sphereVAO;
}