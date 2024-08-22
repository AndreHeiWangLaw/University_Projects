//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace std;
using namespace glm;

float drawGrid = true;

// Camera parameters
float camera_fov = 70.0f;
float dt = 0.0f;

// Camera parameters for view transform -- taken from lab 
vec3 cameraPosition(0.6f, 1.0f, 20.0f);
vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
vec3 cameraSideVector = cross(cameraLookAt, cameraUp);

// Camera movement
float cameraRotateX = 0.0f;
float cameraRotateY = 0.0f;
float cameraRotationSpeed = 5.0f;
float cameraRotationAmount = 1.0f;

float cameraHorizontalAngle = 180.0f - degrees(acos(dot(normalize(vec3(1, 0, 0)), normalize(cameraPosition))));
float cameraVerticalAngle = -degrees(acos(dot(normalize(vec3(1, 0, 1)), normalize(cameraPosition))));

float cameraPanningSpeed = 1.0f;
float cameraTiltingSpeed = 1.0f;
float cameraFovChangeSpeed = 0.01f;

// Cursor position
double lastMousePosX;
double lastMousePosY;

// Mouse button states
bool isLeftMouseBtnHeld = false;
bool isRightMouseBtnHeld = false;
bool isMiddleMouseBtnHeld = false;

// Controls
bool moveForwardAndBack = false;
bool moveLeftRight = false;

vec3 olafPosition(0.0f, 0.0f, 0.0f);
vec3 olafLookAt(1.0f, 0.0f, 0.0f);
vec3 olafUp(0.0f, 1.0f, 0.0f);
vec3 olafSideVector = cross(olafLookAt, olafUp);

float olafSpeed = 5.0f;
float olafScale = 1.0f;
float olafScaleAmount = 0.05f;

const char* getVertexShaderSource()
{
    // Insert Shaders here ...
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files

    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix = mat4(1.0);"
        "uniform mat4 viewMatrix = mat4(1.0);"
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

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "}";
}

int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------


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

    return shaderProgram;
}

int createLineVertexArrayObject()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    vec3 vertexArray[] = {
        // Position              Color
        vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // line in x
        vec3(5.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), // line in y
        vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), // line in z
        vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), // gridline in x
        vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), // gridline in z
        vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f)
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

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
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

int createCubeVertexArrayObject(vec3 color)
{
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    glBindVertexArray(vertexBufferObject);

    return vertexArrayObject;
}

void renderOlaf(const mat4& olaf, const GLuint& worldMatrixLocation, char renderMode)
{
    // Olaf left leg
    mat4 leftLeg_olaf = olaf;
    leftLeg_olaf = translate(leftLeg_olaf, vec3(-0.5f, 0.25f, 0.0f)) * scale(mat4(1.0), vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leftLeg_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf right leg
    mat4 rightLeg_olaf = olaf;
    rightLeg_olaf = translate(rightLeg_olaf, vec3(0.5f, 0.25f, 0.0f)) * scale(mat4(1.0), vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rightLeg_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf body
    mat4 body_olaf = olaf;
    body_olaf = translate(body_olaf, vec3(0.0f, 1.25f, 0.0f)) * scale(mat4(1.0), vec3(2.0f, 1.5f, 1.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &body_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf torso
    mat4 torso_olaf = olaf;
    torso_olaf = translate(torso_olaf, vec3(0.0f, 2.25f, 0.0f)) * scale(mat4(1.0), vec3(1.5f, 0.5f, 1.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &torso_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf arms
    mat4 arms_olaf = olaf;
    arms_olaf = translate(arms_olaf, vec3(0.0f, 2.25f, 0.0f)) * scale(mat4(1.0), vec3(4.5f, 0.20f, 0.5f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &arms_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf head
    mat4 head_olaf = olaf;
    head_olaf = translate(head_olaf, vec3(0.0f, 2.75f, 0.0f)) * scale(mat4(1.0), vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &head_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);
}

void renderOlafNose(const mat4& olaf, const GLuint& worldMatrixLocation, char renderMode)
{
    mat4 nose_olaf = olaf;
    nose_olaf = translate(nose_olaf, vec3(0.0f, 2.8f, 0.6f)) * scale(mat4(1.0), vec3(0.1f, 0.1f, 0.5f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &nose_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);
}

void renderOlafEyesMouth(const mat4& olaf, const GLuint& worldMatrixLocation, char renderMode)
{
    // Olaf mouth
    mat4 mouth_olaf = olaf;
    mouth_olaf = translate(mouth_olaf, vec3(0.0f, 2.65f, 0.5f)) * scale(mat4(1.0), vec3(0.20f, 0.05f, 0.1f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mouth_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf left eye
    mat4 leftEye_olaf = olaf;
    leftEye_olaf = translate(leftEye_olaf, vec3(-0.25f, 3.0f, 0.5f)) * scale(mat4(1.0), vec3(0.20f, 0.2f, 0.1f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leftEye_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf right eye
    mat4 rightEye_olaf = olaf;
    rightEye_olaf = translate(rightEye_olaf, vec3(0.25f, 3.0f, 0.5f)) * scale(mat4(1.0), vec3(0.20f, 0.2f, 0.1f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rightEye_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);
}

void renderOlafHair(const mat4& olaf, const GLuint& worldMatrixLocation, char renderMode)
{
    // Olaf hair1
    mat4 hair1_olaf = olaf;
    hair1_olaf = translate(hair1_olaf, vec3(-0.15f, 3.5f, 0.0f)) * scale(mat4(1.0), vec3(0.05f, 0.5f, 0.05f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &hair1_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf hair2
    mat4 hair2_olaf = olaf;
    hair2_olaf = translate(hair2_olaf, vec3(0.0f, 3.5f, 0.0f)) * scale(mat4(1.0), vec3(0.05f, 0.5f, 0.05f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &hair2_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

    // Olaf hair3
    mat4 hair3_olaf = olaf;
    hair3_olaf = translate(hair3_olaf, vec3(0.15f, 3.5f, 0.0f)) * scale(mat4(1.0), vec3(0.05f, 0.5f, 0.05f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &hair3_olaf[0][0]);
    glDrawArrays(renderMode, 0, 36);

}

float randomFloat(float start, float end) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = end - start;
    float offset = random * diff;
    return start + offset;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    // Reset olaf position
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        olafPosition = vec3(0.0f, 0.0f, 0.0f);
        olafScale = 1.0f;
    }

    // Random reposition olaf
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        olafPosition = vec3(randomFloat(-(float)50 / 2, (float)50 / 2),
            0.0f,
            randomFloat(-(float)50 / 2, (float)50 / 2));
        //cameraPosition = olafPosition + vec3(0.6f, 1.0f, 20.0f);
        //cameraLookAt = olafPosition + vec3(0.0f, 0.0f, -1.0f);

        camera_fov = 70.0f;
        cameraRotateX = 0.0f;
        cameraRotateY = 0.0f;
    }

    // Scale up olaf
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        olafScale += olafScaleAmount;
    }

    // Scale down olaf
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        olafScale -= olafScaleAmount;

        // Limit the scale of Olaf
        if (olafScale <= 0.05f) {
            olafScale = 0.05f;
        }
    }
}

int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Assignment 01", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Make the window's content current
    glfwMakeContextCurrent(window);

    // Key press callback
    glfwSetKeyCallback(window, keyCallback);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    //taken from lab
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    //taken from lab
    mat4 worldMatrix = mat4(1.0);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);

    //taken from lab
    mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

    //taken from lab
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    // Define and upload geometry to the GPU here ...
    int vao = createLineVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    //artifact from assignment
    char renderMode = GL_TRIANGLES;
    float fov = 70.9f;

    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // taken from lab
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        // Draw axis lines
        glDrawArrays(GL_LINES, 0, 2);
        glDrawArrays(GL_LINES, 2, 2);
        glDrawArrays(GL_LINES, 4, 2);

        //Drawing floor grid
        mat4 gridWorldMatrix;
        if (drawGrid)
        {
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
            for (int i = 0; i <= 100; ++i)
            {
                gridWorldMatrix = translate(mat4(1.0f), vec3(-50.0f, -0.1f, -50.0f + i * 1.0f)) * scale(mat4(1.0f), vec3(100.0f, 1.0f, 1.0f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
                glDrawArrays(GL_LINES, 6, 2);

                gridWorldMatrix = translate(mat4(1.0f), vec3(-50.0f + i * 1.0f, -0.1f, -50.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 100.0f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
                glDrawArrays(GL_LINES, 8, 2);
            }
        }

        // Draw ground
        createCubeVertexArrayObject(vec3(0.85f, 0.9f, 0.9f));
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.3f, 0.0f)) * scale(mat4(1.0f), vec3(100.0f, 0.1f, 100.0f));
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // renderMode: triangle, point or line -- artefact from assignment
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            renderMode = GL_TRIANGLES;
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            renderMode = GL_POINTS;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            renderMode = GL_LINES;

        // Draw olaf body
        createCubeVertexArrayObject(vec3(0.85f, 0.85f, 0.85f));
        mat4 olaf = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
        olaf = translate(olaf, olafPosition);
        olaf = scale(olaf, vec3(olafScale, olafScale, olafScale));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olaf[0][0]);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        renderOlaf(olaf, worldMatrixLocation, renderMode);

        // Draw olaf face features
        // Olaf nose
        createCubeVertexArrayObject(vec3(0.9f, 0.6f, 0.4f));
        renderOlafNose(olaf, worldMatrixLocation, renderMode);

        // Olaf mouth, eyes, hair
        createCubeVertexArrayObject(vec3(0.0f, 0.0f, 0.0f));
        renderOlafEyesMouth(olaf, worldMatrixLocation, renderMode);
        renderOlafHair(olaf, worldMatrixLocation, renderMode);

        //glBindVertexArray(0);

        // End frame
        glfwSwapBuffers(window);

        // Detect inputs
        glfwPollEvents();

        // Close window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Display grid
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            drawGrid = false;
        }

        // Used to translate Olaf
        moveForwardAndBack = false;
        moveLeftRight = false;

        // Move olaf right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            olafPosition += olafLookAt * olafSpeed * dt;
            moveForwardAndBack = true;
        }
        // Move olaf left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            olafPosition -= olafLookAt * olafSpeed * dt;
            moveForwardAndBack = true;
        }
        // Move olaf up
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            olafPosition -= olafSideVector * olafSpeed * dt;
            moveLeftRight = true;

        }
        // Move olaf down
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            olafPosition += olafSideVector * olafSpeed * dt;
            moveLeftRight = true;
        }

        // Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

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

        // Allow panning if right mouse btn is held
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            cameraHorizontalAngle -= dx * cameraPanningSpeed * dt;
        }

        // Allow tilting if middle mouse btn is held
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
            cameraVerticalAngle -= dy * cameraTiltingSpeed * dt;
        }

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        // Allow zomming if left mouse btn is held
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            camera_fov += cameraFovChangeSpeed * (float)dy;

            // Limit the camera_fov
            if (camera_fov == 90.0f) {
                camera_fov = 70.0f;
            }
        }

        // use camera lookat and side vectors to update positions with ASDW
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera up
        {
            cameraRotateY += cameraRotationAmount;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera down
        {
            cameraRotateY -= cameraRotationAmount;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera to the left
        {
            cameraRotateX -= cameraRotationAmount;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera to the right
        {
            cameraRotateX += cameraRotationAmount;
        }

        //camera reset - lab
        if (glfwGetKey(window, GLFW_KEY_HOME) || glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            camera_fov = 70.0f;

            cameraPosition = vec3(0.6f, 1.0f, 20.0f);
            cameraLookAt = vec3(0.0f, 0.0f, -1.0f);
            cameraUp = vec3(0.0f, 1.0f, 0.0f);

            cameraRotateX = 0.0f;
            cameraRotateY = 0.0f;
        }

        // Set the view matrix for first camera
        mat4 viewMatrix = mat4(1.0);
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        viewMatrix = rotate(viewMatrix, radians(cameraRotationSpeed * cameraRotateX), cameraUp);

        mat4 projectionMatrix = perspective(camera_fov, (float)1024 / 768, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        float cameraRotationAngle = cameraRotationSpeed * (cameraRotateX - 45.0f);
        vec3 cameraRotationPos = vec3(-cos(radians(cameraRotationAngle)), 0.0f, -sin(radians(cameraRotationAngle)));
        viewMatrix = rotate(viewMatrix, radians(cameraRotationSpeed * cameraRotateY), cameraRotationPos);

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    }
    // Shutdown GLFW
    glfwTerminate();
    return 0;
}
