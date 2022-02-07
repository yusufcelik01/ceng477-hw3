#version 430

in Data
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} data;
in vec3 LightVector;
in vec3 CameraVector;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform sampler2D MoonTexColor;
//uniform sampler2D TexColor;
//uniform sampler2D TexGrey;
uniform float textureOffset;

out vec4 FragColor;

vec3 ambientReflectenceCoefficient = vec3(0.5f, 0.5f, 0.5f);
vec3 ambientLightColor = vec3(0.6f, 0.6f, 0.6f);

vec3 diffuseReflectenceCoefficient= vec3(1.0f, 1.0f, 1.0f);
vec3 diffuseLightColor = vec3(1.0f, 1.0f, 1.0f);

vec3 specularReflectenceCoefficient = vec3(1.0f, 1.0f, 1.0f);
vec3 specularLightColor = vec3(1.0f, 1.0f, 1.0f);
float SpecularExponent = 10;


void main()
{
    // Calculate texture coordinate based on data.TexCoord
    vec3 N = normalize(data.Normal);

    vec3 halfVector = normalize(LightVector + CameraVector);
    float NdotL = dot(LightVector, N);
    float NdotH = dot(halfVector, N);

    vec4 pixelColor = texture(MoonTexColor, data.TexCoord);
    diffuseReflectenceCoefficient = pixelColor.xyz;


    vec3 ambient = ambientLightColor * ambientReflectenceCoefficient;

    vec3 diffuse = diffuseLightColor * diffuseReflectenceCoefficient
                        * max(0, NdotL);

    vec3 spec = specularLightColor * specularReflectenceCoefficient 
                    * pow(max(0, NdotH), SpecularExponent);
    //vec3 ambient = vec3(0, 0, 0);    
    //vec3 diffuse = vec3(0, 0, 0);
    //vec3 spec = vec3(0, 0, 0);

    FragColor = vec4(ambient+diffuse+spec, 1.0f);
    //FragColor = texture(MoonTexColor, data.TexCoord);
    FragColor = vec4(pixelColor.xyz * FragColor.xyz, 1.0f);


}
