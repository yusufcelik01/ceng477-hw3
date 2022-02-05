#version 430

out vec4 fragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D TexColor;


void main()
{
    //fragColor = vec4(0.0f, 0.4f, 0.8f, 1.0f);
    //fragColor = vertexColor;
    //fragColor = texture(TexColor, TexCoord);
    if(TexCoord.s < 0.1f && TexCoord.t < 0.1f){
        fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else if(TexCoord.s > 0.9f && TexCoord.t < 0.1f){
        fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else if(TexCoord.s > 0.4f && TexCoord.t > 0.7f)
    {
        fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }
    else{
        fragColor = vec4(0.55f, 0.0f, 0.9f, 1.0f);//purple
    }

    //fragColor = texture(TexColor, vec2(0.5f, 0.4f));
    //fragColor = texture(TexColor, TexCoord);
    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    fragColor = vec4(vertexColor, 1.0f);
    fragColor = texture(TexColor, TexCoord);
}




