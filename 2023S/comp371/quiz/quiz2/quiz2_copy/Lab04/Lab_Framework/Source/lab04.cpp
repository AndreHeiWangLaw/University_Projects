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
const char* getTexturedFragmentShaderSourceTransparent();
bool initContext();						// setup GLFW and OpenGl version (based on lab2)
//int createVertexArrayObject(vec3 color);// 3d cube model
int createTexturedCubeVertexArrayObject();	// 3d model, lab4
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

// ================================================================ //
//							Arm + Racket							//
// ================================================================ //

// render arm and tennis racket
void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float angleZL, float angleZH);
void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD, float angleZH);
void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD);
void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD);
void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD);
void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD);
void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD);
void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD);

// player 1 parameters
vec3 armTennisPosition1(0.0f, 0.0f, 15.0f);
vec3 armTennisLookAt1(1.0f, 0.0f, 0.0f);
vec3 armTennisUp1(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector1 = cross(armTennisLookAt1, armTennisUp1);
float armTennisScale1 = 1.0f;
float armTennisAngleZ1 = 0.0f;
float armTennisAngleY1 = 0.0f;
float armTennisAngleX1 = 0.0f;
float armTennisAngleZlower1 = 0.0f;
float armTennisAngleZHigher1 = 0.0f;
float displacementLR1 = 0.0f;
float displacementUD1 = 0.0f;

// player 2 parameters
vec3 armTennisPosition2(0.0f, 0.0f, -15.0f);
vec3 armTennisLookAt2(1.0f, 0.0f, 0.0f);
vec3 armTennisUp2(0.0f, 1.0f, 0.0f);
vec3 armTennisSideVector2 = cross(armTennisLookAt2, armTennisUp2);
float armTennisScale2 = 1.0f;
float armTennisAngleZ2 = 0.0f;
float armTennisAngleY2 = 0.0f;
float armTennisAngleX2 = 0.0f;
float armTennisAngleZlower2 = 0.0f;
float armTennisAngleZHigher2 = 0.0f;
float displacementLR2 = 0.0f;
float displacementUD2 = 0.0f;

// general racket parameters
float rotateSpeed = 2.0f;
float armTennisScaleAmount = 0.05f;
float armTennisSpeed = 20.0f;
int currentNumber = 1;	// default 1, used to swtich racket
bool isCamera1 = true; // camera is 1, defaultt
bool isCamera2 = false; // camera is 2
bool isCamera3 = false; // camera is 3
static bool keyPressed = false;	// check if key m is pressed and hold

// ================================================================ //
//							Tennis Court							//
// ================================================================ //

void pole(const mat4& poles, const GLuint& worldMatrixLocation, char renderMode, vec3 polePos);
void poleTopRow(const mat4& poles, const GLuint& worldMatrixLocation, char renderMode, vec3 polePos);
void drawFloor(const mat4& floorGrid, const GLuint& worldMatrixLocation, char renderMode);

// world grid parameters
float gridLength = 78.0f;
float gridWidth = 36.0f;

// tennis net parameters
float tennisNetWidth = gridWidth;
float tennisNetHeight = 8.0f;

// pole parameters
vec3 polePos1(-gridWidth/4, 2.25f, 0.0f);
vec3 polePos2(0.0f, 2.25f, 0.0f);
vec3 polePos3(gridWidth/4, 2.25f, 0.0f);
vec3 topRow(0.0f, 4.0f, 0.0f);

// ================================================================ //
//							alphabet								//
// ================================================================ //

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
float alphThinness = 0.75f;
float alphWidth = 0.8f;

// ================================================================ //
//							camera									//
// ================================================================ //

// Camera parameters  
vec3 cameraPosition(0.0f, 15.0f, 45.0f);		// default camera view
vec3 cameraLookAt(0.0f, -0.1f, -1.0f);
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
bool toggleFirstPersonView = false;		// true = 1st person, false = 3rd person
vec3 offset(0.0f, 10.0f, -1.5f);	// 1st person view
vec3 offset2(0.0f, 10.0f, 1.5f);	// 1st person view (mirror)

// Set initial view matrix
mat4 worldMatrix = mat4(1.0f);			// Initial world orientation
mat4 updatedViewMatrix;

// toggle
bool toggleTexture = true;		// texture, true = on, false = off

// ================================================================ //
//			assignment 2 additional code - tennis ball				//
// ================================================================ //

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
//void drawIcosphere(GLuint vertexArrayObject, int vertexCount, int renderMode);	// draw ball
//void ball(const mat4& tennisBall, const GLuint& worldMatrixLocation, int renderMode);	// render
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
//							main									//
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
	//GLuint greenTextureID = loadTexture("../Assets/Textures/green1.jpg");

	// ground
	GLuint clayTextureID = loadTexture("../Assets/Textures/clay.jpg");
	GLuint greenTextureID = loadTexture("../Assets/Textures/green.jpg");

	// racket + arm
	GLuint glossTextureID = loadTexture("../Assets/Textures/gloss.jpg");
	GLuint tattooTextureID = loadTexture("../Assets/Textures/tattoo.jpeg");

	// sky
	GLuint skyTextureID = loadTexture("../Assets/Textures/sky.jpg");

	// net poles
	GLuint metalPoleTextureID = loadTexture("../Assets/Textures/metalPole.jpg");
	GLuint topRowPoleTextureID = loadTexture("../Assets/Textures/cloth.jpg");

	// alphabet
	GLuint letterPurpleTextureID = loadTexture("../Assets/Textures/letterPurple.jpg");
	GLuint letterOrangeTextureID = loadTexture("../Assets/Textures/brick.jpg");
	GLuint letterBlueTextureID = loadTexture("../Assets/Textures/letterBlue.jpg");
	GLuint letterRedTextureID = loadTexture("../Assets/Textures/letterRed.png");

	//GLuint shaderShadow = loadSHADER("../Assets/Shader/shadow_vertex.glsl", "../Assets/Shader/shadow_fragment.glsl");
	//GLuint shaderScene = loadSHADER("../Assets/Shader/scene_vertex.glsl","../Assets/Shader/scene_fragment.glsl");
#endif

	// background color
	//glClearColor(0.0f, 0.2f, 0.0f, 1.0f);

	// Compile and link shaders here ...
	int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
	int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
	int texturedShaderProgramTransparent = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSourceTransparent());

	
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
	setViewMatrix(texturedShaderProgramTransparent, viewMatrix);

	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgramTransparent, projectionMatrix);

	//SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);
	//SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // resize window

	// Define and upload geometry to the GPU here ...
	//int vao = createVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));
	int texturedCubeVAO = createTexturedCubeVertexArrayObject(); // texture

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

	// enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

		
		//glUseProgram(colorShaderProgram);

		// ================================================================ //
		//							draw									//
		// ================================================================ //
		
		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
		//glBindTexture(GL_TEXTURE_2D, cementTextureID);
		glUniform1i(textureLocation, 0);   // Set our Texture sampler to user Texture Unit 0

		// Floor Grid
		mat4 worldGridMatrix;
		worldGridMatrix = translate(mat4(1.0f), vec3(0.0f,0.0f,0.0f));
		// toggle tennis racket texture for player 1
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, greenTextureID);
			drawFloor(worldGridMatrix, texturedShaderProgram, renderMode);
		}
		else {	// off
			// render 
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 1.0f)));
			drawFloor(worldGridMatrix, colorShaderProgram, renderMode);

		}

		if (toggleFloorGrid)	// true = draw floor grid
		{
			//createVertexArrayObject(vec3(0.5f, 1.0f, 0.0f));	// grass color rgb, 124, 252, 0
			for (int i = 0; i <= gridLength; ++i)	{
				worldGridMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -gridWidth + i * 1.0f - 3.0f)) * scale(mat4(1.0f), vec3(gridWidth, 0.2f, 0.1f));
				// toggle texture
				if (toggleTexture) {
					glBindTexture(GL_TEXTURE_2D, greenTextureID);
					setWorldMatrix(texturedShaderProgram, worldGridMatrix);
				}
				else {
					setWorldMatrix(colorShaderProgram, worldGridMatrix);
				}
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5f, 1.0f, 0.0f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int i = 0; i <= gridWidth; ++i) {
				worldGridMatrix = translate(mat4(1.0f), vec3(-gridWidth / 2 + i * 1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.2f, gridLength));
				// toggle texture
				if (toggleTexture) {
					glBindTexture(GL_TEXTURE_2D, greenTextureID);
					setWorldMatrix(texturedShaderProgram, worldGridMatrix);
				}
				else {
					setWorldMatrix(colorShaderProgram, worldGridMatrix);
				}
				glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5f, 1.0f, 0.0f)));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		

		// draw large cube - blue sky - textured
		glBindTexture(GL_TEXTURE_2D, skyTextureID);
		mat4 largeCube = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(90.0f, 90.0f, 90.0f));
		setWorldMatrix(texturedShaderProgram, largeCube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw colored geometry
		//glUseProgram(colorShaderProgram);
			   
		// Draw axis 
		mat4 axisLines;
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
		//							Tennis Net								//
		// ================================================================ //

		// Floor Grid
		mat4 tennisNet;
		glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f))); // color of net
		for (int i = 0; i <= tennisNetHeight; ++i)	// length-wise grid, --
		{
			tennisNet = translate(mat4(1.0f), vec3(0.0f, i * 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(tennisNetWidth, 0.1f, 0.1f));
			setWorldMatrix(colorShaderProgram, tennisNet);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		for (int i = 0; i <= tennisNetWidth; ++i)	// width-wise grid, |
		{
			tennisNet = translate(mat4(1.0f), vec3(-tennisNetWidth / 2 + i * 1.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, tennisNetHeight, 0.1f));
			setWorldMatrix(colorShaderProgram, tennisNet);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		mat4 tennisNetPole1 = translate(mat4(1.0f), polePos1);	// pole 1
		mat4 tennisNetPole2 = translate(mat4(1.0f), polePos2);	// pole 2
		mat4 tennisNetPole3 = translate(mat4(1.0f), polePos3);	// pole 3
		mat4 tennisNetPoleTopRow = translate(mat4(1.0f), topRow);	// top row



		// toggle tennis racket texture for player 1
		if (toggleTexture) {	// on
			// render 
			glBindTexture(GL_TEXTURE_2D, metalPoleTextureID);	// texture
			pole(tennisNetPole1, texturedShaderProgram, renderMode, polePos1);	// render pole 1
			pole(tennisNetPole2, texturedShaderProgram, renderMode, polePos2);	// render pole 2
			pole(tennisNetPole3, texturedShaderProgram, renderMode, polePos3);	// render pole 3

			glBindTexture(GL_TEXTURE_2D, topRowPoleTextureID);	// texture
			poleTopRow(tennisNetPoleTopRow, texturedShaderProgram, renderMode, topRow);	// render top row
		}
		else {	// off
			// render 
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
			pole(tennisNetPole1, colorShaderProgram, renderMode, polePos1);	// render pole 1
			pole(tennisNetPole2, colorShaderProgram, renderMode, polePos2);	// render pole 2
			pole(tennisNetPole3, colorShaderProgram, renderMode, polePos3);	// render pole 3

			glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
			poleTopRow(tennisNetPoleTopRow, colorShaderProgram, renderMode, topRow);	// render top row
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
		mat4 racket1 = translate(mat4(1.0f), armTennisPosition1);		// origin xyz position
		mat4 racket2 = translate(mat4(1.0f), armTennisPosition2);		// origin xyz position

		
		// player 1
		racket1 = translate(racket1, armTennisPosition1);				// wasd movement
		racket1 = scale(racket1, vec3(armTennisScale1, armTennisScale1, armTennisScale1));	// u,j scale
		racket1 = rotate(racket1, glm::radians(armTennisAngleZ1), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		racket1 = rotate(racket1, glm::radians(armTennisAngleY1), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis		
		racket1 = rotate(racket1, glm::radians(armTennisAngleX1), vec3(1.0f, 0.0f, 0.0f));		// z,c y axis		
		glUniformMatrix4fv(texturedShaderProgram, 1, GL_FALSE, &racket1[0][0]);
		

		
		// player 2
		racket2 = translate(racket2, armTennisPosition2);				// wasd movement
		racket2 = scale(racket2, vec3(armTennisScale2, armTennisScale2, armTennisScale2));	// u,j scale
		racket2 = rotate(racket2, glm::radians(armTennisAngleZ2), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		racket2 = rotate(racket2, glm::radians(armTennisAngleY2), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis		
		racket2 = rotate(racket2, glm::radians(armTennisAngleX2), vec3(1.0f, 0.0f, 0.0f));		// z,c y axis		
		glUniformMatrix4fv(texturedShaderProgram, 1, GL_FALSE, &racket2[0][0]);
		

		
		// toggle tennis racket texture for player 1
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, tattooTextureID);	// texture

			// render arm
			lowerArm(racket1, texturedShaderProgram, renderMode, armTennisAngleZlower1, armTennisAngleZHigher1);			// render loawer arm
			upperArm(racket1, texturedShaderProgram, renderMode, displacementLR1, displacementUD1, armTennisAngleZHigher1);			// render upper arm

			glBindTexture(GL_TEXTURE_2D, glossTextureID);	// texture

			// render tennis head
			tennisHead(racket1, texturedShaderProgram, renderMode, displacementLR1, displacementUD1);		// render tennis head

			// render tennis body
			tennisHandle(racket1, texturedShaderProgram, renderMode, displacementLR1, displacementUD1);		// render tennis handle
			tennisRimBottom(racket1, texturedShaderProgram, renderMode, displacementLR1, displacementUD1);	// render tennis bottom rim
			tennisRimLeft(racket1, texturedShaderProgram, renderMode, displacementLR1, displacementUD1);	// render tennis left rim
			tennisRimRight(racket1, texturedShaderProgram, renderMode, displacementLR1, displacementUD1);	// render tennis right rim
			tennisRimTop(racket1, texturedShaderProgram, renderMode, displacementLR1, displacementUD1);		// render tennis top rim
		}
		else {	// off

			// render arm
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.9f, 0.75f, 0.67f)));
			lowerArm(racket1, colorShaderProgram, renderMode, armTennisAngleZlower1, armTennisAngleZHigher1);			// render loawer arm
			upperArm(racket1, colorShaderProgram, renderMode, displacementLR1, displacementUD1, armTennisAngleZHigher1);			// render upper arm

			// render tennis head
			glUniform3fv(colorLocation, 1, value_ptr(vec3(00.0f, 0.4f, 0.0f)));
			tennisHead(racket1, colorShaderProgram, renderMode, displacementLR1, displacementUD1);		// render tennis head

			// render tennis body
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.4f, 0.0f, 0.0f)));
			tennisHandle(racket1, colorShaderProgram, renderMode, displacementLR1, displacementUD1);		// render tennis handle
			tennisRimBottom(racket1, colorShaderProgram, renderMode, displacementLR1, displacementUD1);	// render tennis bottom rim
			tennisRimLeft(racket1, colorShaderProgram, renderMode, displacementLR1, displacementUD1);	// render tennis left rim
			tennisRimRight(racket1, colorShaderProgram, renderMode, displacementLR1, displacementUD1);	// render tennis right rim
			tennisRimTop(racket1, colorShaderProgram, renderMode, displacementLR1, displacementUD1);		// render tennis top rim
		}
		

		
		// toggle tennis racket texture for player 2
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, tattooTextureID);	// texture

			// render arm
			lowerArm(racket2, texturedShaderProgram, renderMode, armTennisAngleZlower2, armTennisAngleZHigher2);			// render loawer arm
			upperArm(racket2, texturedShaderProgram, renderMode, displacementLR2, displacementUD2, armTennisAngleZHigher2);			// render upper arm
			
			glBindTexture(GL_TEXTURE_2D, glossTextureID);	// texture

			// render tennis head
			tennisHead(racket2, texturedShaderProgram, renderMode, displacementLR2, displacementUD2);		// render tennis head

			// render tennis body
			tennisHandle(racket2, texturedShaderProgram, renderMode, displacementLR2, displacementUD2);		// render tennis handle
			tennisRimBottom(racket2, texturedShaderProgram, renderMode, displacementLR2, displacementUD2);	// render tennis bottom rim
			tennisRimLeft(racket2, texturedShaderProgram, renderMode, displacementLR2, displacementUD2);	// render tennis left rim
			tennisRimRight(racket2, texturedShaderProgram, renderMode, displacementLR2, displacementUD2);	// render tennis right rim
			tennisRimTop(racket2, texturedShaderProgram, renderMode, displacementLR2, displacementUD2);		// render tennis top rim
		}
		else {	// off
			//glUseProgram(colorShaderProgram);	// set color

			// render arm
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.9f, 0.75f, 0.67f)));
			lowerArm(racket2, colorShaderProgram, renderMode, armTennisAngleZlower2, armTennisAngleZHigher2);			// render loawer arm
			upperArm(racket2, colorShaderProgram, renderMode, displacementLR2, displacementUD2, armTennisAngleZHigher2);			// render upper arm

			// render tennis head
			glUniform3fv(colorLocation, 1, value_ptr(vec3(00.0f, 0.4f, 0.0f)));
			tennisHead(racket2, colorShaderProgram, renderMode, displacementLR2, displacementUD2);		// render tennis head

			// render tennis body
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.4f, 0.0f, 0.0f)));
			tennisHandle(racket2, colorShaderProgram, renderMode, displacementLR2, displacementUD2);		// render tennis handle
			tennisRimBottom(racket2, colorShaderProgram, renderMode, displacementLR2, displacementUD2);	// render tennis bottom rim
			tennisRimLeft(racket2, colorShaderProgram, renderMode, displacementLR2, displacementUD2);	// render tennis left rim
			tennisRimRight(racket2, colorShaderProgram, renderMode, displacementLR2, displacementUD2);	// render tennis right rim
			tennisRimTop(racket2, colorShaderProgram, renderMode, displacementLR2, displacementUD2);		// render tennis top rim
		}
		
		// ================================================================ //
		//							Four Alphabets							//
		// ================================================================ //

		// Full Name: Andre Hei Wang Law
		// Last four alphabets:
		//		1: W
		//		2: A
		//		3: L
		//		4: G

		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocationTransparent = glGetUniformLocation(texturedShaderProgramTransparent, "textureSamplerTransparent");
		glUniform1i(textureLocationTransparent, 0);   // Set our Texture sampler to user Texture Unit 0

		// toggle tennis racket texture for player 2
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, letterPurpleTextureID);	// texture
			// render alphabet W
			alphabetWBottom(racket1, texturedShaderProgramTransparent, renderMode);
			alphabetWLeft(racket1, texturedShaderProgramTransparent, renderMode);
			alphabetWMiddle(racket1, texturedShaderProgramTransparent, renderMode);
			alphabetWRight(racket1, texturedShaderProgramTransparent, renderMode);

			glBindTexture(GL_TEXTURE_2D, letterOrangeTextureID);	// texture
			// render alphabet A
			alphabetALeft(racket1, texturedShaderProgramTransparent, renderMode);
			alphabetARight(racket1, texturedShaderProgramTransparent, renderMode);
			alphabetATop(racket1, texturedShaderProgramTransparent, renderMode);
			alphabetAMiddle(racket1, texturedShaderProgramTransparent, renderMode);

			glBindTexture(GL_TEXTURE_2D, letterBlueTextureID);	// texture
			// render alphabet L
			alphabetLBottom(racket2, texturedShaderProgramTransparent, renderMode);
			alphabetLleft(racket2, texturedShaderProgramTransparent, renderMode);

			glBindTexture(GL_TEXTURE_2D, letterRedTextureID);	// texture
			// render alphabet G
			alphabetGBottom(racket2, texturedShaderProgramTransparent, renderMode);
			alphabetGLeft(racket2, texturedShaderProgramTransparent, renderMode);
			alphabetGRightBottom(racket2, texturedShaderProgramTransparent, renderMode);;
			alphabetGRightMiddle(racket2, texturedShaderProgramTransparent, renderMode);
			alphabetGTop(racket2, texturedShaderProgramTransparent, renderMode);
			alphabetGRightTop(racket2, texturedShaderProgramTransparent, renderMode);
		}
		else {	// off
			// render alphabet W
			glUniform3fv(colorLocation, 1, value_ptr(vec4(0.4f, 0.0f, 0.0f, 1.0f)));
			alphabetWBottom(racket1, colorShaderProgram, renderMode);
			alphabetWLeft(racket1, colorShaderProgram, renderMode);
			alphabetWMiddle(racket1, colorShaderProgram, renderMode);
			alphabetWRight(racket1, colorShaderProgram, renderMode);

			// render alphabet A
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.4f, 0.4f, 0.0f)));
			alphabetALeft(racket1, colorShaderProgram, renderMode);
			alphabetARight(racket1, colorShaderProgram, renderMode);
			alphabetATop(racket1, colorShaderProgram, renderMode);
			alphabetAMiddle(racket1, colorShaderProgram, renderMode);

			// render alphabet L
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.4f, 0.0f, 0.4f)));
			alphabetLBottom(racket2, colorShaderProgram, renderMode);
			alphabetLleft(racket2, colorShaderProgram, renderMode);

			// render alphabet G
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.4f, 0.0f)));
			alphabetGBottom(racket2, colorShaderProgram, renderMode);
			alphabetGLeft(racket2, colorShaderProgram, renderMode);
			alphabetGRightBottom(racket2, colorShaderProgram, renderMode);;
			alphabetGRightMiddle(racket2, colorShaderProgram, renderMode);
			alphabetGTop(racket2, colorShaderProgram, renderMode);
			alphabetGRightTop(racket2, colorShaderProgram, renderMode);
		}

		// ================================================================ //
		//						Tennis Ball Sphere							//
		// ================================================================ //

		/*
		mat4 tennisBall = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));		// origin xyz position
		// toggle texture, tennis ball texture
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, greenTextureID);	// green texture
			//glUseProgram(texturedShaderProgram);		// set texture
			ball(racket1, texturedShaderProgram, renderMode);	// render, move together w/ racket 1
		}
		else {	// off
			//glUseProgram(colorShaderProgram);	// set color
			glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));	// color
			ball(racket1, colorShaderProgram, renderMode);	// render, move together w/ racket 1
		}
		*/

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

			// lower model rotation around z-axis (f and g) 
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {		// rotate left
				armTennisAngleZlower1 = armTennisAngleZlower1 - rotateSpeed;
				armTennisAngleZ1 = armTennisAngleZ1 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {		// rotate right
				armTennisAngleZlower1 = armTennisAngleZlower1 + rotateSpeed;
				armTennisAngleZ1 = armTennisAngleZ1 - rotateSpeed;
			}

			// lower model rotation around z-axis (v and b) 
			if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {		// rotate left
				armTennisAngleZHigher1 = armTennisAngleZHigher1 - rotateSpeed;
				armTennisAngleZ1 = armTennisAngleZ1 + rotateSpeed;
				displacementLR1 = displacementLR1 + 0.1f;
				displacementUD1 = displacementUD1 - 0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {		// rotate right
				armTennisAngleZHigher1 = armTennisAngleZHigher1 + rotateSpeed;
				armTennisAngleZ1 = armTennisAngleZ1 - rotateSpeed;
				displacementLR1 = displacementLR1 - 0.1f;
				displacementUD1 = displacementUD1 + 0.05f;
			}

			// model rotation around x-axis (n and m) 
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {		// learn forward
				armTennisAngleX1 = armTennisAngleX1 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {		// lean backwards
				armTennisAngleX1 = armTennisAngleX1 - rotateSpeed;
			}
			break;
		case 2:
			// Update Model position (wasd) 
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {		// Move right
				armTennisPosition2 += armTennisLookAt2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {		// Move left
				armTennisPosition2 -= armTennisLookAt2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {		// Move up
				armTennisPosition2 -= armTennisSideVector2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset2; // offset to be in front of racket
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		// Move down
				armTennisPosition2 += armTennisSideVector2 * armTennisSpeed * dt;
				if (toggleFirstPersonView)
					cameraPosition = armTennisPosition2 * 2.0f + offset2; // offset to be in front of racket
			}
			// model rotation around z-axis (q and e) 
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {		// rotate left
				armTennisAngleZ2 = armTennisAngleZ2 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {		// rotate right
				armTennisAngleZ2 = armTennisAngleZ2 - rotateSpeed;
			}

			// model rotation around y-axis (z and c) 
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {		// rotate left
				armTennisAngleY2 = armTennisAngleY2 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		// rotate right
				armTennisAngleY2 = armTennisAngleY2 - rotateSpeed;
			}
			// lower model rotation around z-axis (f and g) 
			if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {		// rotate left
				armTennisAngleZlower2 = armTennisAngleZlower2 - rotateSpeed;
				armTennisAngleZ2 = armTennisAngleZ2 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {		// rotate right
				armTennisAngleZlower2 = armTennisAngleZlower2 + rotateSpeed;
				armTennisAngleZ2 = armTennisAngleZ2 - rotateSpeed;
			}

			// lower model rotation around z-axis (v and b) 
			if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {		// rotate left
				armTennisAngleZHigher2 = armTennisAngleZHigher2 - rotateSpeed;
				armTennisAngleZ2 = armTennisAngleZ2 + rotateSpeed;
				displacementLR2 = displacementLR2 + 0.1f;
				displacementUD2 = displacementUD2 - 0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {		// rotate right
				armTennisAngleZHigher2 = armTennisAngleZHigher2 + rotateSpeed;
				armTennisAngleZ2 = armTennisAngleZ2 - rotateSpeed;
				displacementLR2 = displacementLR2 - 0.1f;
				displacementUD2 = displacementUD2 + 0.05f;
			}

			// model rotation around x-axis (n and m) 
			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {		// learn forward
				armTennisAngleX2 = armTennisAngleX2 + rotateSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {		// lean backwards
				armTennisAngleX2 = armTennisAngleX2 - rotateSpeed;
			}
			break;
		}
		

		/*
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

		// lower model rotation around z-axis (f and g) 
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {		// rotate left
			armTennisAngleZlower = armTennisAngleZlower - rotateSpeed;
			armTennisAngleZ = armTennisAngleZ + rotateSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {		// rotate right
			armTennisAngleZlower = armTennisAngleZlower + rotateSpeed;
			armTennisAngleZ = armTennisAngleZ - rotateSpeed;
		}

		// lower model rotation around z-axis (v and b) 
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {		// rotate left
			armTennisAngleZHigher = armTennisAngleZHigher - rotateSpeed;
			armTennisAngleZ = armTennisAngleZ + rotateSpeed;
			displacementLR = displacementLR + 0.1f;
			displacementUD = displacementUD - 0.05f;
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {		// rotate right
			armTennisAngleZHigher = armTennisAngleZHigher + rotateSpeed;
			armTennisAngleZ = armTennisAngleZ - rotateSpeed;
			displacementLR = displacementLR - 0.1f;
			displacementUD = displacementUD + 0.05f;
		}

		// model rotation around y-axis (z and c) 
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		// rotate left
			armTennisAngleY = armTennisAngleY + rotateSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {		// rotate right
			armTennisAngleY = armTennisAngleY - rotateSpeed;
		}

		// model rotation around x-axis (n and m) 
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {		// learn forward
			armTennisAngleX = armTennisAngleX + rotateSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {		// lean backwards
			armTennisAngleX = armTennisAngleX - rotateSpeed;
		}
		*/

		// ================================================================ //
		//						Limit Movement Control						//
		// ================================================================ //
		
		
		// limit angle values, realistic movements
		float limitRacket = 10.0f;
		if (armTennisAngleZlower1 > limitRacket)	// upper arm, racket movement, left right
		{
			armTennisAngleZlower1 = limitRacket;
		}
		else if (armTennisAngleZlower1 < -limitRacket)
		{
			armTennisAngleZlower1 = -limitRacket;
		}
		
		if (armTennisAngleZ1 > limitRacket)	// movement of all parts, left right
		{
			armTennisAngleZ1 = limitRacket;
		}
		else if (armTennisAngleZ1 < -limitRacket)
		{
			armTennisAngleZ1 = -limitRacket;
		}

		if (armTennisAngleZHigher1 > limitRacket)		// racket movement only, left right
		{
			armTennisAngleZHigher1 = limitRacket;
		}
		else if (armTennisAngleZHigher1 < -limitRacket)
		{
			armTennisAngleZHigher1 = -limitRacket;
		}

		float limitRacket_lean = 85.0f;
		if (armTennisAngleX1 > limitRacket_lean)	// movement of all parts, lean front back
		{
			armTennisAngleX1 = limitRacket_lean;
		}
		else if (armTennisAngleX1 < -limitRacket_lean)
		{
			armTennisAngleX1 = -limitRacket_lean;
		}

		// limit readjust displacement values, realistic movements
		float limitRacket_dispLR = 0.5f;	// left and right
		if (displacementLR1 > limitRacket_dispLR)
		{
			displacementLR1 = limitRacket_dispLR;
		}
		else if (displacementLR1 < -limitRacket_dispLR)
		{
			displacementLR1 = -limitRacket_dispLR;
		}

		float limitRacket_dispUD = 0.2f;	// up an down
		if (displacementUD1 > limitRacket_dispUD)
		{
			displacementUD1 = limitRacket_dispUD;
		}
		else if (displacementUD1 < -limitRacket_dispUD)
		{
			displacementUD1 = -limitRacket_dispUD;
		}


		// limit angle values, realistic movements
		if (armTennisAngleZlower2 > limitRacket)	// upper arm, racket movement, left right
		{
			armTennisAngleZlower2 = limitRacket;
		}
		else if (armTennisAngleZlower2 < -limitRacket)
		{
			armTennisAngleZlower2 = -limitRacket;
		}

		if (armTennisAngleZ2 > limitRacket)	// movement of all parts, left right
		{
			armTennisAngleZ2 = limitRacket;
		}
		else if (armTennisAngleZ2 < -limitRacket)
		{
			armTennisAngleZ2 = -limitRacket;
		}

		if (armTennisAngleZHigher2 > limitRacket)		// racket movement only, left right
		{
			armTennisAngleZHigher2 = limitRacket;
		}
		else if (armTennisAngleZHigher2 < -limitRacket)
		{
			armTennisAngleZHigher2 = -limitRacket;
		}

		if (armTennisAngleX2 > limitRacket_lean)	// movement of all parts, lean front back
		{
			armTennisAngleX2 = limitRacket_lean;
		}
		else if (armTennisAngleX2 < -limitRacket_lean)
		{
			armTennisAngleX2 = -limitRacket_lean;
		}

		// limit readjust displacement values, realistic movements
		if (displacementLR2 > limitRacket_dispLR)
		{
			displacementLR2 = limitRacket_dispLR;
		}
		else if (displacementLR2 < -limitRacket_dispLR)
		{
			displacementLR2 = -limitRacket_dispLR;
		}

		if (displacementUD2 > limitRacket_dispUD)
		{
			displacementUD2 = limitRacket_dispUD;
		}
		else if (displacementUD2 < -limitRacket_dispUD)
		{
			displacementUD2 = -limitRacket_dispUD;
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
		//glDrawArrays(renderMode, 0, 36);	// rotate light around camera circle






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

		/*
		// camera control
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)	// camera 1
		{
			if (toggleFirstPersonView) {
				cameraPosition = armTennisPosition1 * 2.0f + offset;
				cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
			}
			else
				toggleFirstPersonView = true;
		}

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)	// camera 2
		{
			if (toggleFirstPersonView) {
				cameraPosition = armTennisPosition2 * 2.0f + offset2;
				cameraLookAt = vec3(0.0f, 0.0f, 1.0f);
			}
			else
				toggleFirstPersonView = true;
		}
		*/

		

		if (keyPressed) {
			// ratate betwwen cameras
			if (isCamera1) {
				toggleFirstPersonView = false;

				cameraPosition = vec3(0.6f, 10.0f, 45.0f);
				cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
			}
			else if (isCamera2) {
				if (toggleFirstPersonView) {
					cameraPosition = armTennisPosition2 * 2.0f + offset2;
					cameraLookAt = vec3(0.0f, 0.0f, 1.0f);
				}
				else
					toggleFirstPersonView = true;

				currentNumber = 2;
			}
			else if (isCamera3) {
				
				if (toggleFirstPersonView) {
					cameraPosition = armTennisPosition1 * 2.0f + offset;
					cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
				}
				else
					toggleFirstPersonView = true;

				currentNumber = 1;
			}
			keyPressed = true;
		}


		// update view
		updatedViewMatrix = viewMatrix * worldMatrix;	// update 
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

		setViewMatrix(colorShaderProgram, updatedViewMatrix);	// set 
		setViewMatrix(texturedShaderProgram, updatedViewMatrix);	// set 
		setViewMatrix(texturedShaderProgramTransparent, updatedViewMatrix);	// set 


		mat4 projectionMatrix = perspective(camera_fov, WINDOW_WIDTH/ WINDOW_HEIGHT, 0.1f, 100.0f);
		setProjectionMatrix(colorShaderProgram, projectionMatrix);	// set new fov
		setProjectionMatrix(texturedShaderProgram, projectionMatrix);	// set 
		setProjectionMatrix(texturedShaderProgramTransparent, projectionMatrix);	// set 

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
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// min: scaled down, linear: smooth (reduce pixelation)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // max, scaled up, linear: smooth (reduce pixelation)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// nearest: blocky pixely, faster filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// repeat texture parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrapping mode to repeat (horizontal axis)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrapping mode to repeat (vertical axis)


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
		//"FragColor = vec4(textureColor.rgb, textureColor.a * 0.2f);"
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

void lowerArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float angleZL, float angleZH) {
	mat4 lower_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	lower_arm = translate(lower_arm, vec3(0.0f, 0.0f, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(45.0f + angleZL + angleZH), vec3(0.0f, 0.0f, 1.0f)) *	// 45 angle
		scale(mat4(1.0), vec3(4.0f, 1.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, lower_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD, float angleZH) {
	mat4 upper_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	upper_arm = translate(upper_arm, vec3(1.2f+ dispLR, 3.2f+ dispUD, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(angleZH), vec3(0.0f, 0.0f, 1.0f)) *	// 45 angle
		scale(mat4(1.0), vec3(1.0f, 4.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, upper_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw

}

void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD) {
	mat4 tennis_handle = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_handle = translate(tennis_handle, vec3(1.2f+ dispLR, 7.3f+ dispUD, 0.0f)) *
		scale(mat4(1.0), vec3(0.8f, 5.0f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_handle);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD) {
	for (float i = 0.0f; i < 8.0f; i++) {	// |
		mat4 tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(-0.48f + i / 2 + dispLR, 13.6f + dispUD, 0.0f)) *
			scale(mat4(1.0), vec3(0.2f, 7.0f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
	for (float i = 0.0f; i < 15.0f; i++) {	// --
		mat4 tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(1.25f + dispLR, 10.0f + i / 2 + dispUD, 0.0f)) *
			scale(mat4(1.0), vec3(4.0f, 0.2f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD) {
	mat4 tennis_RimBottom = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimBottom = translate(tennis_RimBottom, vec3(1.25f + dispLR, 10.0f + dispUD, 0.0f)) *
		scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimBottom);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD) {
	mat4 tennis_RimLeft = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimLeft = translate(tennis_RimLeft, vec3(-0.9f + dispLR, 13.6f + dispUD, 0.0f)) *
		scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimLeft);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD) {
	mat4 tennis_RimRight = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimRight = translate(tennis_RimRight, vec3(3.4f + dispLR, 13.6f + dispUD, 0.0f)) *
		scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimRight);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode, float dispLR, float dispUD) {
	mat4 tennis_RimTop = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimTop = translate(tennis_RimTop, vec3(1.25f + dispLR, 17.2f + dispUD, 0.0f)) *
		scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimTop);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void pole(const mat4& poles, const GLuint& worldMatrixLocation, char renderMode, vec3 polePos) {
	mat4 pol = poles;
	pol = translate(pol, polePos) *
		scale(mat4(1.0), vec3(1.0f, 9.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, pol);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void poleTopRow(const mat4& poles, const GLuint& worldMatrixLocation, char renderMode, vec3 polePos) {
	mat4 polTop = poles;
	polTop = translate(polTop, polePos) *
		scale(mat4(1.0), vec3(gridWidth, 1.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, polTop);		// set
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
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		switch (currentNumber) {
		case 1:
			armTennisScale1 += armTennisScaleAmount;	// racket 1
			break;
		case 2:
			armTennisScale2 += armTennisScaleAmount;	// racket 2
			break;
		}
	}
	// Scale down (j)
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		switch (currentNumber) {
		case 1:
			if (armTennisScale1 <= 0.05f) {	// Limit the scale 
				armTennisScale1 = 0.05f;
			}
			else
				armTennisScale1 -= armTennisScaleAmount;	// racket 1
			break;
		case 2:
			if (armTennisScale2 <= 0.05f) {	// Limit the scale 
				armTennisScale2 = 0.05f;
			}
			else
				armTennisScale2 -= armTennisScaleAmount;	// racket 2
			break;
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
		case GLFW_KEY_R:
			resetWorldOrientation();	// reset world position and orientation
			camera_fov = 70.0f;		// reset fov
			toggleFirstPersonView = false;

			//cameraPosition = vec3(0.6f, 10.0f, 45.0f);		// default camera view
			//cameraLookAt = vec3(0.0f, 0.0f, -1.0f);

			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = 0.0f;

			cameraPosition = vec3(0.0f, 15.0f, 45.0f);		// default camera view
			cameraLookAt = vec3(0.0f, -0.1f, -1.0f);
			break;
		default:
			break;
		}
	}

	// toggle texture (x)
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		toggleTexture = !toggleTexture;
	}
	
	if (action == GLFW_PRESS) {		// choose which 1st person view racket
		switch (key) {
		case GLFW_KEY_1:
			currentNumber = 1;	// racket 1
			break;
		case GLFW_KEY_2:
			currentNumber = 2;	// racket 2
			break;
		}
	}

	if (key == GLFW_KEY_M) {
		if (action == GLFW_PRESS) {
			// Set the flag to true when 'M' key is pressed
			keyPressed = true;
		}
		else if (action == GLFW_RELEASE) {
			// Reset the flag when 'M' key is released
			keyPressed = false;
			if (isCamera1) {
				isCamera1 = !isCamera1;
				isCamera2 = !isCamera2;
			} else if (isCamera2){
				isCamera2 = !isCamera2;
				isCamera3 = !isCamera3;
			} else if (isCamera3){
				isCamera3 = !isCamera3;
				isCamera1 = !isCamera1;
			}
		}
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
*/

/*
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


// render floor
void drawFloor(const mat4& floorGrid, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 floor = floorGrid;
	floor = translate(floor, vec3(0.0f, 0.0f, 0.0f)) *
		scale(mat4(1.0), vec3(gridWidth, 0.1f, gridLength));
	setWorldMatrix(worldMatrixLocation, floor);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

// ================================================================ //
//							Alphabet								//
// ================================================================ //

// render alphabet W
void alphabetWBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WBottom = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WBottom = translate(alphabet_WBottom, vec3(1.35f - 3.0f, 19.0f, 0.5f)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WBottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetWLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WLeft = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WLeft = translate(alphabet_WLeft, vec3(3.5f - 3.0f, 21.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WLeft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetWMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WMiddle = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WMiddle = translate(alphabet_WMiddle, vec3(1.5f - 3.0f, 21.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WMiddle);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetWRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_WRight = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.8f - i / 10, 0.0f));    // color 
		alphabet_WRight = translate(alphabet_WRight, vec3(-0.8f - 3.0f, 21.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_WRight);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}


// render alphabet A
void alphabetALeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ALeft = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_ALeft = translate(alphabet_ALeft, vec3(3.5f + 3.0f, 21.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ALeft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetARight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ARight = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_ARight = translate(alphabet_ARight, vec3(-0.8f + 3.0f, 21.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ARight);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetATop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ATop = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_ATop = translate(alphabet_ATop, vec3(1.35f + 3.0f, 23.9f, 0.5f)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ATop);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetAMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_AMiddle = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.2f + i / 10, 0.8f - i / 10));    // color 
		alphabet_AMiddle = translate(alphabet_AMiddle, vec3(1.35f + 3.0f, 21.2f, 0.5f)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_AMiddle);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}


// render alphabet L
void alphabetLBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_ABottom = armTennis;
		//createVertexArrayObject(vec3(1.0f, 0.8f - i / 10, 0.2f + i / 10));    // color 
		alphabet_ABottom = translate(alphabet_ABottom, vec3(1.35f-3.0f, 19.0f, 0.5f)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_ABottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetLleft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_Aleft = armTennis;
		//createVertexArrayObject(vec3(1.0f, 0.8f - i / 10, 0.2f + i / 10));    // color 
		alphabet_Aleft = translate(alphabet_Aleft, vec3(3.5f - 3.0f, 21.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_Aleft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}


// render alphabet G
void alphabetGBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GBottom = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GBottom = translate(alphabet_GBottom, vec3(1.35f+3.0f, 19.0f, 0.5f)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GBottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetGLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GLeft = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GLeft = translate(alphabet_GLeft, vec3(3.5f + 3.0f, 21.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GLeft);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetGRightBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GRightBottom = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GRightBottom = translate(alphabet_GRightBottom, vec3(-0.8f + 3.0f, 20.3f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong / 2, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GRightBottom);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetGRightMiddle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GRightMiddle = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GRightMiddle = translate(alphabet_GRightMiddle, vec3(0.0f + 3.0f, 21.2f, 0.5f)) *
			scale(mat4(1.0), vec3(alphLong / 2, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GRightMiddle);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetGTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GTop = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GTop = translate(alphabet_GTop, vec3(1.35f + 3.0f, 23.9f, 0.5f)) *
			scale(mat4(1.0), vec3(alphLong, alphWidth, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GTop);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

void alphabetGRightTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	//for (float i = 0.0f; i < 3.0f; i++) {
		mat4 alphabet_GRightTop = armTennis;
		//createVertexArrayObject(vec3(0.8f - i / 10, 0.2f + i / 10, 1.0f));    // color 
		alphabet_GRightTop = translate(alphabet_GRightTop, vec3(-0.8f + 3.0f, 23.5f, 0.5f)) *
			scale(mat4(1.0), vec3(alphWidth, alphLong / 3, alphThinness));
		setWorldMatrix(worldMatrixLocation, alphabet_GRightTop);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	//}
}

const char* getTexturedFragmentShaderSourceTransparent()	// transparent fragment
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"in vec2 vertexUV;"
		"uniform sampler2D textureSamplerTransparent;"
		""
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   vec4 textureColor = texture( textureSamplerTransparent, vertexUV );"
		//"   FragColor = textureColor ;"
		"FragColor = vec4(textureColor.rgb, textureColor.a * 0.3f);"	// transparency level (alpha)
		"}";
}




