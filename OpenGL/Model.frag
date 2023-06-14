#version 330 core

//Output Variables
out vec4 FragColor;

//Variables That Came From Earlier On In The Shader Pipeline (Vertex Shader In This Case)
in vec3 localPosition;
in vec3 worldPosition;

in vec3 vertexNormals;
in vec2 textureCoordinates;
in vec4 vertexColor;
in vec3 worldNormal;


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cameraPosition;

vec3 GetAmbientLight(vec3 normalDir, vec3 worldUp, vec3 groundCol, vec3 equatorCol, vec3 skyCol, float strength) //Get single color ambient in aswell
{
    vec3 ambient;

    float skyGroundDotMul = 2.5f;
    float minEquatorMix = 0.5f;
    float equatorColorBlur = 0.33f;

    float upDot = dot(normalDir, worldUp);

    //Fade between a flat lerp from sky to ground and a 3 way lerp based on how bright the equator light is.
    //This simulates how directional lights get blurred using spherical harmonics
     
     //Work out color from ground and sky, ignoring equator
     float adjustedDot = upDot * skyGroundDotMul;
     vec3 skyGroundColor = mix(groundCol, skyCol, clamp((adjustedDot + 1.0) * 0.5f, 0.0f, 1.0f));
     
     //Work out equator lights brightness
     float equatorBright = clamp(dot(equatorCol, equatorCol), 0.0f, 1.0f);
     
     //Blur equator color with sky and ground colors based on how bright it is.
     vec3 equatorBlurredColor = mix(equatorCol, clamp(equatorCol + groundCol + skyCol, 0.0f, 1.0f), equatorBright * equatorColorBlur);
     
     //Work out 3 way lerp inc equator light
     float smoothDot = pow(abs(upDot), 1.0f);
     vec3 equatorColor = mix(equatorBlurredColor, groundCol, smoothDot) * step(upDot, 0.0f) + mix(equatorBlurredColor, skyCol, smoothDot) * step(0.0f, upDot);
     
     ambient = mix(skyGroundColor, equatorCol, clamp(equatorBright + minEquatorMix, 0.0f, 1.0f)) * 0.75f;
     ambient *= strength;

     return(ambient);
}

float GetDiffuseLight(vec3 normalDir, vec3 lightPos, float strength)
{
    float diffuse;

    vec3 normal = normalize(normalDir);
    vec3 lightDirection = normalize(lightPos - worldPosition);

    diffuse = max(dot(normal, lightDirection), 0.0f);
    diffuse *= strength;

    return(diffuse);
}

float GetSpecularLight(vec3 normalDir, vec3 lightPos, vec3 camPos, float concentration, float strength)
{
    float specular;

    vec3 lightDirection = normalize(lightPos - worldPosition);
    vec3 viewDirection = normalize(camPos - worldPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normalDir);
    specular = pow(max(dot(viewDirection, reflectionDirection), 0.0f), concentration);
    specular *= strength;

    return(specular);
}

void main()
{
    vec3 skyAmbientCol = vec3(0.55f, 0.59f, 0.7f);
    vec3 equatorAmbientCol = vec3(0.34f, 0.36f, 0.43f);
    vec3 groundAmbientCol = vec3(0.17f, 0.18f, 0.21f);

    vec3 ambientLight = GetAmbientLight(worldNormal, vec3(0.0f, 1.0f, 0.0f), groundAmbientCol, equatorAmbientCol, skyAmbientCol, 1.0f);

    float diffuseLight = GetDiffuseLight(worldNormal, lightPosition, 1.0f);
    float specularLight = GetSpecularLight(worldNormal, lightPosition, cameraPosition, 25.0f, 0.0f);
    vec3 directLight = (diffuseLight + specularLight) * lightColor;

    vec3 light = ambientLight + directLight;
    
    FragColor = texture(texture1, worldPosition.xy) * vec4(light, 1.0f);
}