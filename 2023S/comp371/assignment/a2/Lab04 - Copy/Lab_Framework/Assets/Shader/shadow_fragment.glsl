#version 330 core
    in vec3 fragPosition;
    in vec3 fragNormal;

    uniform vec3 viewPosition;
    uniform vec3 lightPosition;
    uniform vec3 lightDirection;
    uniform float spotInnerCutoff;
    uniform float spotOuterCutoff;

    out vec4 FragColor;

    vec3 calcSpotLight(vec3 lightColor, vec3 lightDirection, vec3 lightPosition, vec3 viewDirection, vec3 surfaceNormal, float innerCutoff, float outerCutoff)
    {
        vec3 lightDir = normalize(lightPosition - fragPosition);
        float intensity = max(dot(surfaceNormal, -lightDir), 0.0);
        float spotEffect = dot(normalize(lightDirection), -lightDir);
        
        if (spotEffect > innerCutoff) {
            // Within the inner cutoff angle, apply full light intensity
            float attenuation = 1.0;
            if (spotEffect < outerCutoff) {
                // Apply attenuation within the outer cutoff angle
                float t = (spotEffect - outerCutoff) / (innerCutoff - outerCutoff);
                attenuation = 1.0 - t;
            }
            return lightColor * intensity * attenuation;
        }
        return vec3(0.0);
    }

    void main()
    {
        vec3 lightColor = vec3(1.0, 1.0, 1.0); // White light color
        vec3 viewDirection = normalize(viewPosition - fragPosition);
        vec3 surfaceNormal = normalize(fragNormal);

        // Calculate the light intensity using the Phong model with spot light
        vec3 spotLightIntensity = calcSpotLight(lightColor, lightDirection, lightPosition, viewDirection, surfaceNormal, spotInnerCutoff, spotOuterCutoff);

        // Combine the spot light intensity with material properties (ambient, diffuse, specular) and calculate the final color
        // ...

        FragColor = vec4(finalColor, 1.0);
    }