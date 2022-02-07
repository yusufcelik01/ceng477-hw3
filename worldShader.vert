#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTex;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MVP;

uniform sampler2D TexColor;
uniform sampler2D TexGrey;
uniform float textureOffset;

uniform float heightFactor;
uniform float imageWidth;
uniform float imageHeight;

out Data
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} data;


out vec3 LightVector;// Vector from Vertex to Light;
out vec3 CameraVector;// Vector from Vertex to Camera;

void main()
{
    
    vec4 heightMapValue = texture(TexGrey, VertexTex);
    vec3 vertexHeight = VertexNormal*heightFactor* heightMapValue.r;
    vec3 tempCoord = VertexPosition + vertexHeight;


    gl_Position = MVP * vec4(tempCoord, 1.0);
    
    data.Position = (ModelMatrix * vec4(tempCoord, 1.0f)).xyz;
    data.Normal = normalize((ModelMatrix* vec4(VertexNormal, 0.f)).xyz);
    data.TexCoord = VertexTex;

    LightVector = lightPosition - data.Position;
    LightVector = normalize(LightVector);
    CameraVector = cameraPosition - data.Position;
    CameraVector = normalize(CameraVector);
}
