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
// Icosphere logic based on these online ressources:
// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
// https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
// http://www.songho.ca/opengl/gl_sphere.html
//
// Lighting based on these online ressources:
// https://learnopengl.com/Lighting/Basic-Lighting
// 

#include <iostream>
#define GLEW_STATIC 1	// This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>	// Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>	// Cross-platform interface (graphics, init OpenGl and binding inputs)
#include <glm/glm.hpp>	// GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <random>
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
void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode);
mat4 original_cube;
mat4 lower_arm;
mat4 upper_arm;
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
float armTennisAngleZ = 0.0f;
float armTennisAngleY = 0.0f;
float armTennisAngleX = 0.0f;
float armTennisAngleZlower = 0.0f;
float armTennisAngleZHigher = 0.0f;
float displacementLR = 0.0f;
float displacementUD = 0.0f;

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
//			assignment 2 additional code - lighting					//
// ================================================================ //


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
	GLuint greenTextureID = loadTexture("../Assets/Textures/green1.jpg");
	GLuint clayTextureID = loadTexture("../Assets/Textures/clay.jpg");
	GLuint glossTextureID = loadTexture("../Assets/Textures/gloss.jpg");
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

	// light
	// build and compile our shader zprogram
	








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
			glUseProgram(texturedShaderProgram);
		}
		else {
			glUseProgram(colorShaderProgram);
		}
		glUseProgram(colorShaderProgram);

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
		glUseProgram(colorShaderProgram);
			   
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
		original_cube = translate(mat4(1.0f), vec3(0.0f, 1.7f, 0.0f));		// origin xyz position
		original_cube = translate(original_cube, armTennisPosition);				// wasd movement
		original_cube = scale(original_cube, vec3(armTennisScale, armTennisScale, armTennisScale));	// u,j scale
		original_cube = rotate(original_cube, glm::radians(armTennisAngleZ), vec3(0.0f, 0.0f, 1.0f));		// q,e z-axis 
		original_cube = rotate(original_cube, glm::radians(armTennisAngleY), vec3(0.0f, 1.0f, 0.0f));		// z,c y axis		
		original_cube = rotate(original_cube, glm::radians(armTennisAngleX), vec3(1.0f, 0.0f, 0.0f));		// z,c y axis		
		glUniformMatrix4fv(texturedShaderProgram, 1, GL_FALSE, &original_cube[0][0]);
		

		//lower_arm = original_cube;

		// toggle tennis racket texture
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, glossTextureID);	// green texture
			glUseProgram(texturedShaderProgram);		// set texture

			// render arm
			lowerArm(original_cube, texturedShaderProgram, renderMode);			// render loawer arm
			upperArm(original_cube, texturedShaderProgram, renderMode);			// render upper arm

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
			glUseProgram(colorShaderProgram);	// set color

			// render arm
			glUniform3fv(colorLocation, 1, value_ptr(vec3(0.9f, 0.75f, 0.67f)));
			lowerArm(original_cube, colorShaderProgram, renderMode);			// render loawer arm
			upperArm(original_cube, colorShaderProgram, renderMode);			// render upper arm

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

		mat4 tennisBall = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));		// origin xyz position
		// toggle texture, tennis ball texture
		if (toggleTexture) {	// on
			glBindTexture(GL_TEXTURE_2D, greenTextureID);	// green texture
			glUseProgram(texturedShaderProgram);		// set texture
			ball(original_cube, texturedShaderProgram, renderMode);	// render, original_cube means move together
		}
		else {	// off
			glUseProgram(colorShaderProgram);	// set color
			glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));	// color
			ball(original_cube, colorShaderProgram, renderMode);	// render, original_cube means move together
		}

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

		// model rotation around x-axis (z and c) 
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {		// learn forward
			armTennisAngleX = armTennisAngleX + rotateSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {		// lean backwards
			armTennisAngleX = armTennisAngleX - rotateSpeed;
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

		float limit2 = 85.0f;
		if (armTennisAngleX > limit2)	// movement of all parts, lean front back
		{
			armTennisAngleX = limit2;
		}
		else if (armTennisAngleX < -limit2)
		{
			armTennisAngleX = -limit2;
		}

		// limit readjust displacement values, realistic movements
		float limitDispl1 = 0.5f;	// left and right
		if (displacementLR > limitDispl1)
		{
			displacementLR = limitDispl1;
		}
		else if (displacementLR < -limitDispl1)
		{
			displacementLR = -limitDispl1;
		}

		float limitDispl2 = 0.2f;	// up an down
		if (displacementUD > limitDispl2)
		{
			displacementUD = limitDispl2;
		}
		else if (displacementUD < -limitDispl2)
		{
			displacementUD = -limitDispl2;
		}

		// ================================================================ //
		//			Light	(based on lab 8 and online ressource)			//
		// ================================================================ //
		
		

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
	mat4 lower_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	lower_arm = translate(lower_arm, vec3(0.0f, 0.0f, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(45.0f + armTennisAngleZlower + armTennisAngleZHigher), vec3(0.0f, 0.0f, 1.0f)) *	// 45 angle
		scale(mat4(1.0), vec3(4.0f, 1.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, lower_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void upperArm(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 upper_arm = armTennis;
	//createVertexArrayObject(vec3(0.9f, 0.75f, 0.67f));    // color 
	upper_arm = translate(upper_arm, vec3(1.2f+ displacementLR, 3.2f+ displacementUD, 0.0f)) *
		rotate(mat4(1.0f), glm::radians(armTennisAngleZHigher), vec3(0.0f, 0.0f, 1.0f)) *	// 45 angle
		scale(mat4(1.0), vec3(1.0f, 4.0f, 1.0f));
	setWorldMatrix(worldMatrixLocation, upper_arm);			// set
	glDrawArrays(renderMode, 0, 36);						// draw

}

void tennisHandle(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_handle = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_handle = translate(tennis_handle, vec3(1.2f+ displacementLR, 7.3f+ displacementUD, 0.0f)) *
		scale(mat4(1.0), vec3(0.8f, 5.0f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_handle);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisHead(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	for (float i = 0.0f; i < 8.0f; i++) {	// |
		mat4 tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(-0.48f + i / 2 + displacementLR, 13.6f + displacementUD, 0.0f)) *
			scale(mat4(1.0), vec3(0.2f, 7.0f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
	for (float i = 0.0f; i < 15.0f; i++) {	// --
		mat4 tennis_head = armTennis;
		//createVertexArrayObject(vec3(0.0f, 0.4f, 0.0f));    // color 
		tennis_head = translate(tennis_head, vec3(1.25f + displacementLR, 10.0f + i / 2 + displacementUD, 0.0f)) *
			scale(mat4(1.0), vec3(4.0f, 0.2f, 0.1f));
		setWorldMatrix(worldMatrixLocation, tennis_head);		// set
		glDrawArrays(renderMode, 0, 36);						// draw
	}
}

void tennisRimBottom(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimBottom = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimBottom = translate(tennis_RimBottom, vec3(1.25f + displacementLR, 10.0f + displacementUD, 0.0f)) *
		scale(mat4(1.0), vec3(4.0f, 0.4f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimBottom);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimLeft(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimLeft = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimLeft = translate(tennis_RimLeft, vec3(-0.9f + displacementLR, 13.6f + displacementUD, 0.0f)) *
		scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimLeft);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimRight(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimRight = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimRight = translate(tennis_RimRight, vec3(3.4f + displacementLR, 13.6f + displacementUD, 0.0f)) *
		scale(mat4(1.0), vec3(0.4f, 7.6f, 0.8f));
	setWorldMatrix(worldMatrixLocation, tennis_RimRight);		// set
	glDrawArrays(renderMode, 0, 36);						// draw
}

void tennisRimTop(const mat4& armTennis, const GLuint& worldMatrixLocation, char renderMode) {
	mat4 tennis_RimTop = armTennis;
	//createVertexArrayObject(vec3(0.4f, 0.0f, 0.0f));    // color 
	tennis_RimTop = translate(tennis_RimTop, vec3(1.25f + displacementLR, 17.2f + displacementUD, 0.0f)) *
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

