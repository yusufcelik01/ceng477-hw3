#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTex;

uniform mat4 MVP;

uniform sampler2D TexColor;
uniform sampler2D TexGrey;
uniform float textureOffset;

uniform float heightFactor;
uniform float imageWidth;
uniform float imageHeight;

out Data
{
    vec3 Posisiton;
    vec3 Normal;
    vec2 TexCoord;
}data;

void main()
{
    //gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);

    vec4 heightMapValue = texture(TexGrey, VertexTex);
    vec3 vertexHeight = VertexNormal*heightFactor* heightMapValue.r;
    vec3 tempCoord = VertexPosition + vertexHeight;


    gl_Position = MVP * vec4(tempCoord, 1.0);

    data.TexCoord = VertexTex;
}

