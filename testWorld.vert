#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTex;

uniform mat4 MVP;
uniform sampler2D TexColor;

out vec3 vertexColor;
out vec2 TexCoord;

void main()
{
    //gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);

    gl_Position = MVP * vec4(VertexPosition, 1.0);
    if(VertexPosition.y > 100){
        vertexColor = vec3(0.55f, 0.0f, 1.0f);
    }
    else if(VertexPosition.y < -100){
        vertexColor = vec3(1.0f, 1.0f, 0.0f);
    }
    else{
        vertexColor = vec3(1.0f, 0.0f, 0.0f);
    }

   vertexColor = (VertexPosition + vec3(600.f, 600.f, 600.f))/1200;
   TexCoord = VertexTex;

   if(VertexPosition.x > 590){
       vertexColor = vec3(1.f, 1.f, 1.f);
   }
   else if(VertexPosition.y > 590){
       vertexColor = vec3(1.f, 1.f, 1.f);
   }
   else if(VertexPosition.z > 590){
       vertexColor = vec3(1.f, 1.f, 1.f);
   }

   //TexCoord = vec2(VertexPosition.x, VertexPosition.y);
   
}

