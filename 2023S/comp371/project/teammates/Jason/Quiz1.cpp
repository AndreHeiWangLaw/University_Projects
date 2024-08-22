//
// COMP 371 Quiz 1
// Jason Liang 40186162
// Credit to Nicolas Bergeron for the lab 3 framework.
//

#include <iostream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// Class used to draw the racket at the specified location
class Racket
{
public:
    int playerNumber; // Will keep track of the current active player
    vec3 centerOfRacket; // Keeps track of the location of the center of the racket

    Racket(GLuint worldMatrixLocation, float worldXAngle, float worldYAngle, vec3 location, GLuint colorLocation, GLenum renderMode, int number, float initialAngle, float armAngle, float elbowAngle, float racketAngle)
    {
        // Set player number to this racket
        playerNumber = number;

        // First let us generate the arm
        mat4 partMatrix = scale(mat4(1.0f), vec3(0.3f, 2.0f, 0.3f));

        // Initial movement to rotate and place the model away from the origin
        mat4 armMatrix = translate(mat4(1.0f), location);
        armMatrix = rotate(armMatrix, radians(armAngle), vec3(0.0f, 1.0f, 0.0f)); //Rotate the arm
        armMatrix = rotate(armMatrix, radians(initialAngle), vec3(0.0f, 0.0f, 1.0f)); // Keeps track of the arm + forearm + racket

        mat4 worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the arm at the location
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.878f, 0.675f, 0.412f)));
        glDrawArrays(renderMode, 0, 36);

        // Now we generate the forearm in respect to the arm's location
        partMatrix = scale(mat4(1.0f), vec3(0.3f, 2.0f, 0.3f));

        // Clamp elbow angle to realistic values
        if (elbowAngle > 100)
        {
            elbowAngle = 100.0f;
        }
        else if (elbowAngle < 0)
        {
            elbowAngle = 0.0f;
        }

        mat4 forearmMatrix = translate(mat4(1.0f), vec3(0.0f, 1.8f, 0.0f));
        forearmMatrix = rotate(forearmMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f)); // Keeps track of the forearm + racket

        // Adjusting for the slight movement to correct for the rotation
        forearmMatrix = forearmMatrix * translate(mat4(1.0f), vec3(-sin(radians(elbowAngle)) * 0.9f, 0.9f - sin(radians(90 - elbowAngle)), 0.0f));

        partMatrix = forearmMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the forearm at the location
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.878f, 0.675f, 0.412f)));
        glDrawArrays(renderMode, 0, 36);

        // Finally we draw the racket, starting with the handle
        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.2f, 0.2f));

        mat4 racketMatrix = translate(mat4(1.0f), vec3(-sin(radians(elbowAngle)) * 2.4f, 0.9f + cos(radians(elbowAngle)) * 2.4f, 0.0f));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the handle at the end of the forearm
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.643f, 0.455f, 0.286f)));
        glDrawArrays(renderMode, 0, 36);

        // Now we draw the racket, the entire racket will be built using this as the center
        centerOfRacket = vec3(-sin(radians(elbowAngle)) * 3.9f, 0.9f + cos(radians(elbowAngle)) * 3.9f, 0.0f);

        // Start with the base
        partMatrix = scale(mat4(1.0f), vec3(1.2f, 0.2f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + 0.8f * sin(radians(elbowAngle)), centerOfRacket.y - 0.8f * cos(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;
         
        // First draw the base
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.643f, 0.455f, 0.286f)));
        glDrawArrays(renderMode, 0, 36);

        // Do the sides of the racket now
        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.8f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x - 0.5f * cos(radians(elbowAngle)), centerOfRacket.y - 0.5f * sin(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the left side of the racket
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.643f, 0.455f, 0.286f)));
        glDrawArrays(renderMode, 0, 36);

        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.8f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + 0.5f * cos(radians(elbowAngle)), centerOfRacket.y + 0.5f * sin(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the right side of the racket
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.643f, 0.455f, 0.286f)));
        glDrawArrays(renderMode, 0, 36);

        // Finish with the top
        partMatrix = scale(mat4(1.0f), vec3(1.2f, 0.2f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x - 0.8f * sin(radians(elbowAngle)), centerOfRacket.y + 0.8f * cos(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the top of the racket
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.643f, 0.455f, 0.286f)));
        glDrawArrays(renderMode, 0, 36);

        // Draw the net of the racket

        for (int i = 0; i < 7; i++)
        {
            partMatrix = scale(mat4(1.0f), vec3(0.05f, 1.6f, 0.05f));
            racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x - (0.3f * cos(radians(elbowAngle))) + 0.1f * i * cos(radians(elbowAngle)), centerOfRacket.y - (0.3f * sin(radians(elbowAngle))) + 0.1f * i * sin(radians(elbowAngle)), centerOfRacket.z));
            racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

            partMatrix = racketMatrix * partMatrix;
            worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

            // Draw the vertical string
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
            glDrawArrays(renderMode, 0, 36);
        }

        for (int i = 0; i < 14; i++)
        {
            partMatrix = scale(mat4(1.0f), vec3(1.0f, 0.05f, 0.05f));
            racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + (0.7f * sin(radians(elbowAngle))) - 0.1f * i * sin(radians(elbowAngle)), centerOfRacket.y - (0.7f * cos(radians(elbowAngle))) + 0.1f * i * cos(radians(elbowAngle)), centerOfRacket.z));
            racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

            partMatrix = racketMatrix * partMatrix;
            worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

            // Draw the horizontal string
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
            glDrawArrays(renderMode, 0, 36);
        }
    }
};

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix;"
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


const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "uniform vec3 objectColor;"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);"
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


int createVertexBufferObject()
{
    // Cube model
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), // far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), // right - purple
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f)
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


    return vertexBufferObject;
}


int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024x786
    GLFWwindow* window = glfwCreateWindow(1024, 786, "Comp371 - Quiz 1", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Green background
    glClearColor(0.184f, 0.310f, 0.310f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // Camera parameters for view transform
    vec3 cameraPosition(0.0f, 6.0f, 15.0f);
    vec3 cameraLookAt(0.0f, -6.0f, -15.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;

    float FOV = 70.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = perspective(FOV,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center, this is 0.0, 0.0, 0.0 just needed to eqaulize the camera position
        cameraUp); // up

    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    // Define and upload geometry to the GPU here ...
    int vao = createVertexBufferObject();

    // For frame time
    double lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once
    // Backface culling disabled for the skybox
    //glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Intial angle of the world
    float worldXAngle = 0.0f;
    float worldYAngle = 0.0f;

    // Default rendering mode
    GLenum renderMode = GL_TRIANGLES;

    // Player poisition reference and initial positions
    vec3* playerPosition = NULL;
    vec3 playerOneLoc = vec3(-25.0f, 0.5f, 8.0f);
    vec3 playerTwoLoc = vec3(-10.0f, 0.5f, -8.0f);
    vec3 playerThreeLoc = vec3(10.0f, 0.5f, 8.0f);
    vec3 playerFourLoc = vec3(20.0f, 0.5f, -8.0f);

    Racket* currentRacket = NULL;

    // Player colors, contains the possible colors for the player letter
    vec3 playerColors[4][3] = {{vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f)},
                               {vec3(0.8235f, 0.0157f, 0.176f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.25f, 0.25f)},
                               {vec3(1.0f, 1.0f, 0.25f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.753f, 0.0f)},
                               {vec3(0.059f, 1.0f, 0.314f), vec3(0.0f, 0.5f, 0.0f), vec3(0.18f, 0.545f, 0.341f)}};
    
    //intital arm elbow racket, initially planned for some features that I didnt have time to finish
    float armAngles[4][4] = { {0.0f, 90.0f, 0.0f, 0.0f},
                              {0.0f, 90.0f, 0.0f, 0.0f},
                              {0.0f, 90.0f, 0.0f, 0.0f},
                              {0.0f, 90.0f, 0.0f, 0.0f} };
    
    double mousePosX, mousePosY;

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        double dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor and clear Depth Buffer Bit as well
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw geometry
        glBindVertexArray(vao);

        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        // Draw X axis, colored red
        mat4 xAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        xAxis = xAxis * translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &xAxis[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw Y axis, colored green
        mat4 yAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        yAxis = yAxis * translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &yAxis[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw Z axis, colored blue
        mat4 zAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        zAxis = zAxis * translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &zAxis[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the court a 78x36 surface
        mat4 court = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        court = court * translate(mat4(1.0f), vec3(0.0f, -0.1f, 0.0f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 36.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &court[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.522f, 0.4f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the white lines on the court
        // Drawing the single + doubles sidelines
        mat4 lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, -17.9f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, -13.4f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 13.4f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 17.9f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the baselines + service lines
        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(-38.9f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 36.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(-21.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 27.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(21.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 27.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(38.9f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 36.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the center service line
        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(42.0f, 0.1f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lines[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*
        // Drawing the ground model, since its a 78x36 square grid, this is only here in case you want the grid enforced
        mat4 floorLine;
        for (int i = 0; i < 79; ++i)
        {
            // Draw the line on the Z axis
            floorLine = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            floorLine = floorLine * translate(mat4(1.0f), vec3(-39.0f + i * 1.0f, -0.1f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 36.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &floorLine[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 0.0f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (int i = 0; i < 37; ++i)
        {
            // Draw the line on the X axis
            floorLine = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            floorLine = floorLine * translate(mat4(1.0f), vec3(0.0f, -0.1f, -18.0f + i * 1.0f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.1f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &floorLine[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 0.0f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        */

        // Draws the sky box, all object are inside this
        mat4 skyBox = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        skyBox = skyBox * scale(mat4(1.0f), vec3(90.0f, 90.0f, 90.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &skyBox[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.529f, 0.808f, 0.922f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the net, first the posts
        mat4 netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, -19.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &netPost[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, 19.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &netPost[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Now draw the net
        mat4 net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        net = net * translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.4f, 0.4f, 38.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &net[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        net = net * translate(mat4(1.0f), vec3(0.0f, 2.1f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 3.8f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &net[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        for (int i = 0; i < 39; ++i)
        {
            // Draw vertical lines for the net
            net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            net = net * translate(mat4(1.0f), vec3(0.0f, 2.1f, -19.5f + 0.5f * i)) * scale(mat4(1.0f), vec3(0.1f, 3.8f, 0.1f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &net[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            net = net * translate(mat4(1.0f), vec3(0.0f, 2.1f, 0.0f + 0.5f * i)) * scale(mat4(1.0f), vec3(0.1f, 3.8f, 0.1f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &net[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (int i = 0; i < 7; ++i)
        {
            // Draw vertical lines for the net
            net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            net = net * translate(mat4(1.0f), vec3(0.0f, 0.2f + 0.5429f * i, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 38.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &net[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0f, 0.0f, 0.0f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        /*
        Racket playerOne = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerOneLoc, colorLocation, renderMode, 1, armAngles[0][0], armAngles[0][1], armAngles[0][2], armAngles[0][3]);
        mat4 playerOneCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerOneCharacter = playerOneCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f));
        playerOneCharacter = rotate(playerOneCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerOneCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[0][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        playerOneCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerOneCharacter = playerOneCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, -0.15f * cos(radians(armAngles[0][1] + 90.0f))));
        playerOneCharacter = rotate(playerOneCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerOneCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[0][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        Racket playerTwo = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerTwoLoc, colorLocation, renderMode, 2, armAngles[1][0], armAngles[1][1], armAngles[1][2], armAngles[1][3]);
        mat4 playerTwoCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerTwoCharacter = playerTwoCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        playerTwoCharacter = rotate(playerTwoCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerTwoCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[1][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        Racket playerThree = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerThreeLoc, colorLocation, renderMode, 3, armAngles[2][0], armAngles[2][1], armAngles[2][2], armAngles[2][3]);
        mat4 playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, -0.25f * cos(radians(armAngles[2][1] + 90.0f))));
        playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.25f * cos(radians(armAngles[2][1] + 90.0f))));
        playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 2.2f, 0.0f));
        playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.9f, 0.0f));
        playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        Racket playerFour = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerFourLoc, colorLocation, renderMode, 4, armAngles[3][0], armAngles[3][1], armAngles[3][2], armAngles[3][3]);
        mat4 playerFourCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerFourCharacter = playerFourCharacter * translate(mat4(1.0f), playerFourLoc) * translate(mat4(1.0f), playerFour.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, -0.25f * cos(radians(armAngles[3][1] + 90.0f))));
        playerFourCharacter = rotate(playerFourCharacter, radians(armAngles[3][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerFourCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[3][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        playerFourCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerFourCharacter = playerFourCharacter * translate(mat4(1.0f), playerFourLoc) * translate(mat4(1.0f), playerFour.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        playerFourCharacter = rotate(playerFourCharacter, radians(armAngles[3][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        playerFourCharacter = rotate(playerFourCharacter, radians(37.7f), vec3(1.0f, 0.0, 0.0)) * scale(mat4(1.0f), vec3(0.2f, 0.8f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerFourCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[3][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        playerFourCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        playerFourCharacter = playerFourCharacter * translate(mat4(1.0f), playerFourLoc) * translate(mat4(1.0f), playerFour.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.25 * cos(radians(armAngles[3][1] + 90.0f))));
        playerFourCharacter = rotate(playerFourCharacter, radians(armAngles[3][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerFourCharacter[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[3][1])));
        glDrawArrays(GL_TRIANGLES, 0, 36);*/
        
        // Now generate characters and arms/rackets using default player positions and the center of the racket value
        // Start with player one's racket
        Racket playerOne = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerOneLoc, colorLocation, renderMode, 1, armAngles[0][0], armAngles[0][1], armAngles[0][2], armAngles[0][3]);
        for (int i = 0; i < 3; i++)
        {
            // Generate 3 characters (L) 0.4f apart and use different colors for each, player 1 is blue
            // First draw the bottom line
            mat4 playerOneCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerOneCharacter = playerOneCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[0][1] + 90.0f)), 1.5f, sin(radians(armAngles[0][1] + 90.0f)) * 0.4f * i - 0.4f * sin(radians(armAngles[0][1] + 90.0f))));
            playerOneCharacter = rotate(playerOneCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerOneCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[0][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Then the vertical line
            playerOneCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerOneCharacter = playerOneCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[0][1] + 90.0f)), 1.875f, -0.15f * cos(radians(armAngles[0][1] + 90.0f)) + sin(radians(armAngles[0][1] + 90.0f)) * 0.4f * i - 0.4f * sin(radians(armAngles[0][1] + 90.0f))));
            playerOneCharacter = rotate(playerOneCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerOneCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[0][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Second is player 2's racket
        Racket playerTwo = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerTwoLoc, colorLocation, renderMode, 2, armAngles[1][0], armAngles[1][1], armAngles[1][2], armAngles[1][3]);
        for (int i = 0; i < 3; i++)
        {
            // Generate 3 characters (I) 0.4f apart and use different colors for each, player 2 is red
            // Only one line needed for I
            mat4 playerTwoCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerTwoCharacter = playerTwoCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[1][1] + 90.0f)), 1.875f, sin(radians(armAngles[1][1] + 90.0f)) * 0.4f * i - 0.4f * sin(radians(armAngles[1][1] + 90.0f))));
            playerTwoCharacter = rotate(playerTwoCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerTwoCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[1][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Third is player 3's racket
        Racket playerThree = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerThreeLoc, colorLocation, renderMode, 3, armAngles[2][0], armAngles[2][1], armAngles[2][2], armAngles[2][3]);
        for (int i = 0; i < 3; i++)
        {
            // Generate 3 characters (A) 0.4f apart and use different colors for each, player 3 is yellow
            // Right vertical line
            mat4 playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[2][1] + 90.0f)), 1.875f, -0.25f * cos(radians(armAngles[2][1] + 90.0f)) + 0.4f * i * sin(radians(armAngles[2][1] + 90.0f)) - 0.4f * sin(radians(armAngles[2][1] + 90.0f))));
            playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Left vertical line
            playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[2][1] + 90.0f)), 1.875f, 0.25f * cos(radians(armAngles[2][1] + 90.0f)) + 0.4f * i * sin(radians(armAngles[2][1] + 90.0f)) - 0.4f * sin(radians(armAngles[2][1] + 90.0f))));
            playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Upper horizontal line
            playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[2][1] + 90.0f)), 2.2f, sin(radians(armAngles[2][1] + 90.0f)) * 0.4f * i - 0.4f * sin(radians(armAngles[2][1] + 90.0f))));
            playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Lower horizontal line
            playerThreeCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerThreeCharacter = playerThreeCharacter * translate(mat4(1.0f), playerThreeLoc) * translate(mat4(1.0f), playerThree.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[2][1] + 90.0f)), 1.9f, sin(radians(armAngles[2][1] + 90.0f)) * 0.4f * i - 0.4f * sin(radians(armAngles[2][1] + 90.0f))));
            playerThreeCharacter = rotate(playerThreeCharacter, radians(armAngles[2][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerThreeCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[2][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Fourth is player 4's racket
        Racket playerFour = Racket(worldMatrixLocation, worldXAngle, worldYAngle, playerFourLoc, colorLocation, renderMode, 4, armAngles[3][0], armAngles[3][1], armAngles[3][2], armAngles[3][3]);
        for (int i = 0; i < 3; i++)
        {
            // Generate 3 characters (N) 0.4f apart and use different colors for each, player 4 is green
            // Right vertical line
            mat4 playerFourCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerFourCharacter = playerFourCharacter * translate(mat4(1.0f), playerFourLoc) * translate(mat4(1.0f), playerFour.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[3][1] + 90.0f)), 1.875f, -0.25f * cos(radians(armAngles[3][1] + 90.0f)) + 0.4f * i * sin(radians(armAngles[3][1] + 90.0f)) - 0.4f * sin(radians(armAngles[3][1] + 90.0f))));
            playerFourCharacter = rotate(playerFourCharacter, radians(armAngles[3][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerFourCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[3][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Diagonal line
            playerFourCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerFourCharacter = playerFourCharacter * translate(mat4(1.0f), playerFourLoc) * translate(mat4(1.0f), playerFour.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[3][1] + 90.0f)) + 0.4f * i, 1.875f, sin(radians(armAngles[3][1] + 90.0f)) * 0.4f * i - 0.4f * sin(radians(armAngles[3][1] + 90.0f))));
            playerFourCharacter = rotate(playerFourCharacter, radians(-37.7f), vec3(1.0f, 0.0, 0.0));
            playerFourCharacter = rotate(playerFourCharacter, radians(armAngles[3][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f))* scale(mat4(1.0f), vec3(0.2f, 0.8f, 0.2f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerFourCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[3][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Left vertical lines
            playerFourCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            playerFourCharacter = playerFourCharacter * translate(mat4(1.0f), playerFourLoc) * translate(mat4(1.0f), playerFour.centerOfRacket) * translate(mat4(1.0f), vec3(0.4f * i * cos(radians(armAngles[3][1] + 90.0f)), 1.875f, 0.25f * cos(radians(armAngles[3][1] + 90.0f)) + 0.4f * i * sin(radians(armAngles[3][1] + 90.0f)) - 0.4f * sin(radians(armAngles[3][1] + 90.0f))));
            playerFourCharacter = rotate(playerFourCharacter, radians(armAngles[3][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &playerFourCharacter[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(playerColors[3][i])));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Camera lookat and side vectors to update positions with ASDW
        if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) & (playerPosition != NULL))
        {
            // Uppercase A - move camera to the left
            if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) | glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS)
            {
                // Movement is revered for players 1 and 2 on the other side of the net
                if ((*currentRacket).playerNumber < 3)
                {
                    (*playerPosition).z += 1.0f * dt;
                }
                else
                {
                    (*playerPosition).z -= 1.0f * dt;
                }
            }
            // Lowercase A - Rotate left
            else
            {
                armAngles[(*currentRacket).playerNumber-1][1] += 10.0f * dt;
            }
        }

        if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) & (playerPosition != NULL)) // D - move camera to the right
        {
            // Uppercase D - move camera to the right
            if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) | glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS)
            {
                // Movement is revered for players 1 and 2 on the other side of the net
                if ((*currentRacket).playerNumber < 3)
                {
                    (*playerPosition).z -= 1.0f * dt;
                }
                else
                {
                    (*playerPosition).z += 1.0f * dt;
                }
            }
            // Lowercase D - Rotate right
            else
            {
                armAngles[(*currentRacket).playerNumber-1][1] -= 10.0f * dt;
            }
        }

        if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) & (playerPosition != NULL)) // S - move camera up
        {
            // Movement is revered for players 1 and 2 on the other side of the net
            if ((*currentRacket).playerNumber < 3)
            {
                (*playerPosition).x += 1.0f * dt;
            }
            else
            {
                (*playerPosition).x -= 1.0f * dt;
            }
        }

        if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) & (playerPosition != NULL)) // W - move camera down
        {
            // Movement is revered for players 1 and 2 on the other side of the net
            if ((*currentRacket).playerNumber < 3)
            {
                (*playerPosition).x -= 1.0f * dt;
            }
            else
            {
                (*playerPosition).x += 1.0f * dt;
            }
        }

        // Arrow keys
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) // Left arrow - Rotate world in the postive x axis
        {
            worldXAngle -= 2.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Up arrow - Rotate world in the positive y axis
        {
            worldYAngle -= 2.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // Down arrow - Rotate world in the negative y axis
        {
            worldYAngle += 2.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Right arrow - Rotate world in the negative x axis
        {
            worldXAngle += 2.0f;;
        }

        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // HOME - Reset camera to default location, also removes current player
        {
            // Reset world rotation
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;

            // Return zoom to normal
            projectionMatrix = perspective(70.0f, 800.0f / 600.0f, 0.01f, 100.0f);
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

            // Return to looking at the original spot
            cameraPosition = vec3(0.0f, 6.0f, 15.0f);
            cameraLookAt = vec3(0.0f, -6.0f, -15.0f);

            // Return current player to default null value
            playerPosition = NULL;
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // 1 - Set player 1 as the current player
        {
            // Change current player position reference to player 1
            playerPosition = &playerOneLoc;

            // Change the reference of the camera (using the current camera position because the lookAt is a function of the two
            cameraLookAt = playerOneLoc - cameraPosition;

            // Place the camera in front to allow a proper view
            cameraPosition = vec3((*playerPosition).x + 15.0f, (*playerPosition).y + 6.0f, (*playerPosition).z);

            // Assign current racket as the first players racket
            currentRacket = &playerOne;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // 2 - Set player 2 as the current player
        {
            // Change current player position reference to player 2
            playerPosition = &playerTwoLoc;

            // Change the reference of the camera (using the current camera position because the lookAt is a function of the two
            cameraLookAt = playerTwoLoc - cameraPosition;

            // Place the camera in front to allow a proper view
            cameraPosition = vec3((*playerPosition).x + 15.0f, (*playerPosition).y + 6.0f, (*playerPosition).z);

            // Assign current racket as the second players racket
            currentRacket = &playerTwo;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // 3 - Set player 3 as the current player
        {
            // Change current player position reference to player 3
            playerPosition = &playerThreeLoc;

            // Change the reference of the camera (using the current camera position because the lookAt is a function of the two
            cameraLookAt = playerThreeLoc - cameraPosition;

            // Place the camera in front to allow a proper view
            cameraPosition = vec3((*playerPosition).x - 15.0f, (*playerPosition).y + 6.0f, (*playerPosition).z);

            // Assign current racket as the third players racket
            currentRacket = &playerThree;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // 4 - Set player 4 as the current player
        {
            // Change current player position reference to player 4
            playerPosition = &playerFourLoc;
            
            // Change the reference of the camera (using the current camera position because the lookAt is a function of the two
            cameraLookAt = playerFourLoc - cameraPosition;

            // Place the camera in front to allow a proper view
            cameraPosition = vec3((*playerPosition).x - 15.0f, (*playerPosition).y + 6.0f, (*playerPosition).z);

            // Assign current racket as the fourth players racket
            currentRacket = &playerFour;
        }

        // Different rendering modes
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // P - Render model as points
        {
            renderMode = GL_POINTS;
        }

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // L - Render model as lines
        {
            renderMode = GL_LINES;
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // T - Render model as triangles
        {
            renderMode = GL_TRIANGLES;
        }
        // Removed feature due to time constraints
        /*
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) // Flex the arm
        {
            armAngles[(*currentRacket).playerNumber-1][2] += dt * 5.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // Unflex the arm
        {
            armAngles[(*currentRacket).playerNumber-1][2] -= dt * 5.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // Rotate the arm in the +Z axis
        {
            armAngles[(*currentRacket).playerNumber-1][0] += dt * 5.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // Rotate the arm in the -Z axis
        {
            armAngles[(*currentRacket).playerNumber-1][0] -= dt * 5.0f;
        }*/

        // Mouse buttons
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // Pressing right mouse button and moving pans the camera
        {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dx = mousePosX - lastMousePosX;

            cameraLookAt.x += dx * 0.01;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) // Pressing middle mouse button and moving tilts the camera
        {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dy = mousePosY - lastMousePosY;

            cameraLookAt.y += dy * 0.01;

            // Clamp Y values so nothing wacky happens
            if (cameraLookAt.y < -2.0)
            {
                cameraLookAt.y = -2.0f;
            }
            else if (cameraLookAt.y > 2.0)
            {
                cameraLookAt.y = 2.0f;
            }
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) // Pressing left mouse button and moving zooms in/out
        {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dy = mousePosY - lastMousePosY;

            FOV += dy * 0.01;

            // Clamp FOV values so nothing wacky happens
            if (FOV < 69.3)
            {
                FOV = 69.3f;
            }
            else if (FOV > 71.8)
            {
                FOV = 71.8f;
            }

            projectionMatrix = perspective(FOV, 800.0f / 600.0f, 0.01f, 100.0f);
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        }

        // Set the view matrix
        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

        glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}