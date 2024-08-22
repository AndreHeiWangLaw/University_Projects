//
// Assignment 1
// Jason Liang 40186162
// Credit to Nicolas Bergeron for the framework from lab 3.
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
    GLFWwindow* window = glfwCreateWindow(1024, 786, "Comp371 - Assignment 1", NULL, NULL);
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
    vec3 cameraPosition(0.0f, 2.0f, 15.0f);
    vec3 cameraLookAt(0.0f, -2.0f, -15.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;

    // Used to modify the FOV later
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
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Initial angle for the arm/racket and location
    float initialAngle = 0.0f;
    float armAngle = 0.0f;
    float elbowAngle = 0.0f;
    float racketAngle = 0.0f;
    vec3 defaultLocation = vec3(3.0f, 3.0f, 2.0f);

    // Initial scale factor values
    float scaleFactor = 1.00f;
    float ds = 1.00;

    // Initial angle for the world
    float worldXAngle = 0.0f;
    float worldYAngle = 0.0f;

    // Default rendering mode
    GLenum renderMode = GL_TRIANGLES;

    double mousePosX, mousePosY;

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        double dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Scaling modification
        scaleFactor = scaleFactor * ds;
        ds = 1.00; // Reset the change in scale

        // Each frame, reset color of each pixel to glClearColor and clear Depth Buffer Bit as well
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw geometry
        glBindVertexArray(vao);

        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        // Draw X axis, colored red
        mat4 xAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        xAxis = xAxis * translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &xAxis[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 0.0f, 1.0f)));
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

        // Draw the ground model, since its a 100x100 square grid, 101 lines on both sides are needed
        mat4 floorLine;
        for (int i = 0; i < 101; ++i)
        {
            // Draw the line on the Z axis
            floorLine = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            floorLine = floorLine * translate(mat4(1.0f), vec3(-50.0f + i * 1.0f, -0.1f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 100.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &floorLine[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 0.0f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw the line on the X axis
            floorLine = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            floorLine = floorLine * translate(mat4(1.0f), vec3(0.0f, -0.1f, -50.0f + i * 1.0f)) * scale(mat4(1.0f), vec3(100.0f, 0.1f, 0.1f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &floorLine[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0f, 1.0f, 0.0f)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // First let us generate the arm
        mat4 partMatrix = scale(mat4(1.0f), vec3(0.3f, 2.0f, 0.3f));

        // Initial movement to rotate and place the model away from the origin
        mat4 armMatrix = scale(mat4(1.0f), vec3(1.0f * scaleFactor, 1.0f * scaleFactor, 1.0f * scaleFactor)) * translate(mat4(1.0f), defaultLocation);
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
        vec3 centerOfRacket = vec3(-sin(radians(elbowAngle)) * 3.9f, 0.9f + cos(radians(elbowAngle)) * 3.9f, 0.0f);

        // Start with the base
        partMatrix = scale(mat4(1.0f), vec3(1.2f, 0.2f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + 0.8f * sin(radians(elbowAngle)), centerOfRacket.y - 0.8f * cos(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the base
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.643f, 0.455f, 0.286f)));
        glDrawArrays(renderMode, 0, 36);

        // Do the sides of the racket now
        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.8f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x - 0.5f * cos(radians(elbowAngle)), centerOfRacket.y - 0.5f * sin(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the left side
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.643f, 0.455f, 0.286f)));
        glDrawArrays(renderMode, 0, 36);

        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.8f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + 0.5f * cos(radians(elbowAngle)), centerOfRacket.y + 0.5f * sin(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the right side
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

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Camera lookat and side vectors to update positions with ASDW
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            // Uppercase A - move camera to the left
            if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) | glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS)
            {
                defaultLocation.x -= 1.0f * dt;
            }
            // Lowercase A - Rotate left
            else
            {
                armAngle += 5.0f * dt;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            // Uppercase D - move camera to the right
            if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) | glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS)
            {
                defaultLocation.x += 1.0f * dt;
            }
            // Lowercase D - Rotate right
            else
            {
                armAngle -= 5.0f * dt;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // S - move camera up
        {
            defaultLocation.y -= 1.0f * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // W - move camera down
        {
            defaultLocation.y += 1.0f * dt;
        }

        // Arrow keys
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // Left arrow - Rotate world in the postive x axis
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

        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // HOME - Reset camera to default location
        {
            // Reset world rotation
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;

            // Return zoom to normal
            projectionMatrix = perspective(70.0f, 800.0f / 600.0f, 0.01f, 100.0f);
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

            // Return to looking at the original spot
            cameraLookAt.x = 0.0f;
            cameraLookAt.y = 0.0f;
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

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //  Spacebar - Move model to random location on the grid
        {
            //srand(time(NULL));
            int randomX = rand() % 100;
            int randomY = rand() % 4;
            int randomZ = rand() % 100;
            defaultLocation = vec3(randomX * 1.0f - 50.0f, randomY * 1.0f + 2.0f, randomZ * 1.0f - 50.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // U - Increase model size
        {
            ds += ds * 0.01f;
        }

        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // J - Decrease model size
        {
            ds -= ds * 0.01f;
        }

        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) // Y - Flex the arm
        {
            elbowAngle += dt * 5.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // H - Unflex the arm
        {
            elbowAngle -= dt * 5.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // T - Rotate the arm in the +Z axis
        {
            initialAngle += dt * 5.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // G - Rotate the arm in the -Z axis
        {
            initialAngle -= dt * 5.0f;
        }

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

            // Clamp FOV values so nothing wacky happens
            if (cameraLookAt.y < -1.5)
            {
                cameraLookAt.y = -1.5f;
            }
            else if (cameraLookAt.y > 1.5)
            {
                cameraLookAt.y = 1.5f;
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