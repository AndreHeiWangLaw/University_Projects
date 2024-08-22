#version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 normal;

    uniform mat4 modelMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 lightProjViewMatrix;

    out vec3 fragPosition;
    out vec3 fragNormal;

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
        fragPosition = vec3(modelMatrix * vec4(position, 1.0));
        fragNormal = mat3(transpose(inverse(modelMatrix))) * normal;

        // Transform vertex position to light space
        vec4 lightSpacePos = lightProjViewMatrix * modelMatrix * vec4(position, 1.0);
        gl_Position = lightSpacePos;
    }
