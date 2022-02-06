#ifndef ECLIPSEMAP_H
#define ECLIPSEMAP_H

#include <vector>
#include <GL/glew.h>
#include <iostream>
#include "glm/glm/ext.hpp"
#include "Shader.h"
#include <vector>
#include "glm/glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <jpeglib.h>
#include <GL/glew.h>

#define PI 3.14159265359
using namespace std;


class EclipseMap {
private:
    float heightFactor = 80;
    float textureOffset = 0;
    float orbitDegree = 0;
    glm::vec3 lightPos = glm::vec3(0, 4000, 0);
    bool pKeyPressed = false;
    // DISPLAY SETTINGS
    enum displayFormatOptions {
        windowed = 1, fullScreen = 0
    };
    const char *windowName = "Ceng477 - HW3";
    int defaultScreenWidth = 1000;
    int defaultScreenHeight = 1000;
    int screenWidth = defaultScreenWidth;
    int screenHeight = defaultScreenHeight;
    int displayFormat = displayFormatOptions::windowed;
    // CAMERA SETTINGS
    float projectionAngle = 45;
    float aspectRatio = 1;
    float near = 0.1;
    float far = 10000;
    //float startPitch = 180;
    //float startYaw = 90;
    float startPitch = 0;
    float startYaw = 0;
    float startSpeed = 0;
    float pitch = startPitch;
    float yaw = startYaw;
    float speed = startSpeed;
    float currentTime = 0;
    float lastTime = 0;
    float deltaTime = 0;

    glm::vec3 cameraStartPosition = glm::vec3(0, 4000, 4000);//TODO
    //glm::vec3 cameraStartPosition = glm::vec3(0, 1500, 1500);//TODO
    //glm::vec3 cameraStartPosition = glm::vec3(2000, 2000, 0);//right
    glm::vec3 cameraStartDirection = glm::vec3(0, -1, -1);//TODO
    //glm::vec3 cameraStartDirection = glm::vec3(-1, -1, 0);//right
    glm::vec3 cameraStartUp = glm::vec3(0, 0, 1);
    glm::vec3 cameraPosition = cameraStartPosition;
    glm::vec3 cameraDirection = cameraStartDirection;
    glm::vec3 cameraUp = cameraStartUp;
    glm::vec3 cameraLeft = glm::cross(cameraUp, cameraDirection);
public:
    unsigned int textureColor;
    unsigned int textureGrey;
    unsigned int VAO;
    unsigned int VBO, EBO;
    float imageHeight;
    float imageWidth;
    float radius = 600;
    int horizontalSplitCount = 250;
    int verticalSplitCount = 125;

    unsigned int moonTextureColor;
    unsigned int moonVAO;
    unsigned int moonVBO, moonEBO;
    float moonImageHeight;
    float moonImageWidth;
    float moonRadius = 162;

    vector<float> worldVertices;
    vector<unsigned int> worldIndices;

    vector<float> moonVertices;
    vector<unsigned int> moonIndices;

    GLFWwindow *openWindow(const char *windowName, int width, int height);

    void Render(const char *coloredTexturePath, const char *greyTexturePath, const char *moonTexturePath);

    void handleKeyPress(GLFWwindow *window);

    void initColoredTexture(const char *filename, GLuint shader);

    void initGreyTexture(const char *filename, GLuint shader);

    void initMoonColoredTexture(const char *filename, GLuint shader);


    void computeEarthArrays();
    void computeMoonArrays();

};

#endif
