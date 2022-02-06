#version 430

out vec4 fragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D TexColor;
uniform sampler2D TexGrey;


void main()
{
    fragColor = texture(TexColor, TexCoord);
}




