#include "EclipseMap.h"

using namespace std;

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;

    vertex() {}

    vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texture) : position(position),
                                                                                           normal(normal),
                                                                                           texture(texture) {}
};

struct triangle {
    int vertex1;
    int vertex2;
    int vertex3;

    triangle() {}

    triangle(const int &vertex1, const int &vertex2, const int &vertex3) : vertex1(vertex1), vertex2(vertex2),
                                                                           vertex3(vertex3) {}
};

//float earthVertices[] = {
//    -0.5f, -0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//     0.0f,  0.5f, 0.0f
//};


//unsigned int indices[] = {
//    0, 1, 2
//};

void createSphereArrays(vertex* vertexArray, vector<triangle>& indexArray, 
        float radius, glm::vec3 center, glm::vec3 up,
        int horizontalSplitCount, int verticalSplitCount)
{
    int numberOfVertices = (verticalSplitCount -1) *horizontalSplitCount  + 2;
    //up = glm::normalize(up);
    up = glm::vec3(0.0f, 0.0f, 1.0f);

    
    //vertexArray = new vertex[numberOfVertices];
    //TODO alloc triangles

    //north pole
    vertexArray[0].position = center + radius * up;
    vertexArray[0].normal = up;
    vertexArray[0].texture = glm::vec2(0.f, 0.f);

    vertexArray[1].position = center - radius * up;
    vertexArray[1].normal = -up;
    vertexArray[1].texture = glm::vec2(0.f, 1.f);

    size_t index = 2;

    for(int i=0; i <= horizontalSplitCount; i++){// for each meridian
        double alpha = 2*PI * ((double)i /horizontalSplitCount);
        for(int j=1; j < verticalSplitCount; j++){
            double beta = PI * ((double)j/verticalSplitCount);
            glm::vec3 ver(radius*sin(beta)*cos(alpha), 
                                radius*sin(beta) * sin(alpha),
                                radius*cos(beta));        
            vertexArray[index].position = ver;

            //calculate normal

            glm::vec3 normal = ver - center;
            vertexArray[index].normal = glm::normalize(normal);
          
            //calculate texture coordinates
            glm::vec2 tex((double)i /horizontalSplitCount, (double)j/verticalSplitCount);
            
            vertexArray[index].texture = tex;

            index++;
        }
    }
    //cout << "index: " << index << endl;


    vector<triangle>* triangles = new vector<triangle>;
    
    for(size_t i=1; i <= horizontalSplitCount; i++){
        size_t index = (verticalSplitCount -1) *i + 2;
        triangles->push_back(triangle(0, index, index- (verticalSplitCount-1)));

        if(false && i == horizontalSplitCount-1){
            triangles->push_back(triangle(0, 2, index));
            //TODO
            triangles->push_back(triangle(index+verticalSplitCount -2, verticalSplitCount, 1));
        } 
        index--;//for south pole
        triangles->push_back(triangle(index, index + (verticalSplitCount-1), 1));
    }
    //triangle t = (*triangles)[triangles->size()-2];
    //cout <<  t.vertex1 << " " << t.vertex2 << " " << t.vertex3 << endl;

    for(size_t i=1; i <=horizontalSplitCount; i++){
        size_t index = i* (verticalSplitCount-1) +2;
        for(size_t j=1; j< verticalSplitCount-1; j++){
            triangles->push_back(triangle(index, index+1, index-(verticalSplitCount-1)));
            triangles->push_back(triangle(index+1, index-verticalSplitCount+2,index- verticalSplitCount +1));

            if(false && i== horizontalSplitCount-1){//TODO
                triangles->push_back(triangle(1+ j, j+2, index));
                triangles->push_back(triangle(j+2, index+1, index));
            }
        
            index++;
        }
    
    }

    
    indexArray = *triangles;
}



void EclipseMap::Render(const char *coloredTexturePath, const char *greyTexturePath, const char *moonTexturePath) {
    // Open window
    GLFWwindow *window = openWindow(windowName, screenWidth, screenHeight);

    // Moon commands
    // Load shaders
    GLuint moonShaderID = initShaders("moonShader.vert", "moonShader.frag");

    initMoonColoredTexture(moonTexturePath, moonShaderID);

    
    // TODO: Set moonVertices
    
    // TODO: Configure Buffers
    

    // World commands
    // Load shaders
    GLuint worldShaderID = initShaders("worldShader.vert", "worldShader.frag");
    //GLuint worldShaderID = initShaders("testWorld.vert", "testWorld.frag");

    initColoredTexture(coloredTexturePath, worldShaderID);
    initGreyTexture(greyTexturePath, worldShaderID);

    // TODO: Set worldVertices
    //size_t numberOfEarthVertices = (verticalSplitCount -1) *horizontalSplitCount  + 2;
    size_t numberOfEarthVertices = (verticalSplitCount -1) *(horizontalSplitCount +1 )+ 2;
    vertex earthVertices[numberOfEarthVertices];
    vertex moonVertices[numberOfEarthVertices];
    //triangle* indices = NULL;
    vector<triangle> indices;
    vector<triangle> moonIndices;

    createSphereArrays( earthVertices, indices, radius, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
                            horizontalSplitCount, verticalSplitCount);
    createSphereArrays( moonVertices, moonIndices, moonRadius, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
                            horizontalSplitCount, verticalSplitCount);

    //for(size_t k=0; k< numberOfEarthVertices; k++){
    //    glm::vec3 v = earthVertices[k].position;
    //    cout << v.x << " " << v.y << " " << v.z << endl;
    //}

    //for(size_t k=0; k< indices.size(); k++){
    //    triangle temps = indices[k];
    //    cout << temps.vertex1 << " " << temps.vertex2 << " " << temps.vertex3 << endl;
    //}

    // moon buffer intilization

    glGenVertexArrays(1, &moonVAO);
    glGenBuffers(1, &moonVBO);
    glGenBuffers(1, &moonEBO);
    
    glBindVertexArray(moonVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, moonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(moonVertices), moonVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moonEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, moonIndices.size()*sizeof(triangle), &moonIndices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (6*sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint moon_MVP_location, moon_Model_location;
    GLuint moonTexColor_location;
    GLuint moonLight_position, moonCamera_position;

    moon_Model_location = glGetUniformLocation(moonShaderID, "ModelMatrix");
    moon_MVP_location = glGetUniformLocation(moonShaderID, "MVP");
    moonTexColor_location = glGetUniformLocation(moonShaderID, "MoonTexColor");
    
    moonTexColor_location = glGetUniformLocation(moonShaderID, "MoonTexColor");

    moonLight_position  = glGetUniformLocation(moonShaderID, "lightPosition");
    moonCamera_position  = glGetUniformLocation(moonShaderID, "cameraPosition");
    



    /* ##############  EARTH VARIABLES AND BUFFES  #################### */
    //GLuint VBO;
    //Earth buffer initilazitons
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);


    ////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(earthVertices), earthVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(triangle) , &indices[0], GL_STATIC_DRAW);






    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (6*sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint earth_MVP_location, earth_Model_location, heightFactor_location;
    GLuint texColor_location, texGrey_location;
    GLuint lightPosition_location, cameraPosition_location;

    earth_Model_location = glGetUniformLocation(worldShaderID, "ModelMatrix");
    earth_MVP_location = glGetUniformLocation(worldShaderID, "MVP");
    heightFactor_location = glGetUniformLocation(worldShaderID, "heightFactor");
    texColor_location = glGetUniformLocation(worldShaderID, "TexColor");
    texGrey_location  = glGetUniformLocation(worldShaderID, "TexGrey");
    lightPosition_location  = glGetUniformLocation(worldShaderID, "lightPosition");
    cameraPosition_location  = glGetUniformLocation(worldShaderID, "cameraPosition");

    glm::vec3 lightPosition = glm::vec3(0.f, 4000.f, 0.f);
    //GLuint VAO;

    //GLuint EBO;

    // TODO: Configure Buffers

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Main rendering loop
    glm::vec3 rotAxis = glm::vec3(0.f, 0.f, 1.f);
    float earthRotationAngle = 0;

    glm::vec3 moonRotAxis = glm::vec3(0.f, 0.f, 1.f);
    glm::vec3 moonOrbitAxis = glm::vec3(0.f, 0.f, -1.f);
    float moonRotationAngle = 0;
    float moonOrbitAngle = 0;

    yaw = 0;
    pitch = 0;
    do {
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        aspectRatio = (double)screenWidth/screenHeight;
        glViewport(0, 0, screenWidth, screenHeight);

        glClearStencil(0);
        glClearDepth(1.0f);
        //glClearColor(1.0f, 1.0f, 0.f, 1);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



        // TODO: Handle key presses
        handleKeyPress(window);

        // TODO: Manipulate rotation variables
        
        
        // TODO: Bind textures
        
        // TODO: Use moonShaderID program
        
        // TODO: Update camera at every frame
        cameraLeft = glm::cross(cameraUp, cameraDirection);
        cameraLeft = glm::normalize(cameraLeft);
        cameraUp = glm::cross(cameraDirection, cameraLeft);
        cameraUp = glm::normalize(cameraUp);

        glm::mat4 pitchMatrix = glm::rotate(pitch, cameraLeft);
        
        cameraDirection = pitchMatrix * glm::vec4(cameraDirection, 1);
        cameraUp = pitchMatrix * glm::vec4(cameraUp, 1);
        

        glm::mat4 yawMatrix = glm::rotate(yaw, cameraUp);

        cameraLeft = yawMatrix * glm::vec4(cameraLeft, 1);
        cameraDirection = yawMatrix * glm::vec4(cameraDirection, 1);

        cameraPosition += cameraDirection * speed;

        pitch = 0;
        yaw = 0;

        glm::mat4 view = glm::lookAt(
                cameraPosition,
                cameraPosition + cameraDirection,
                cameraUp
        );


        glm::mat4 projection = glm::perspective(
                glm::radians(projectionAngle),
                aspectRatio,
                near,
                far
        );


        //CALCULATE MOON MATRICES
        glm::mat4 moonModel = glm::mat4(1.0f);

        glm::mat4 moonRotationMatrix = glm::rotate(moonRotationAngle, moonRotAxis);

        glm::mat4 moonOrbitMatrix = glm::rotate(moonOrbitAngle, moonOrbitAxis);

        glm::mat4 moonTranslationMatrix = glm::translate(glm::mat4(1.0f),
                             glm::vec3(0.f, 2600.f, 0.f));


        moonModel = moonOrbitMatrix * moonTranslationMatrix * moonRotationMatrix;

        glm::mat4 moonMVP = projection * view * moonModel;

        moonRotationAngle += 0.5/horizontalSplitCount * 3*deltaTime * 3;
        moonOrbitAngle += 0.02 *deltaTime * 3;//* 0.05;

        

        // Calculate MVP
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(earthRotationAngle, rotAxis);
        earthRotationAngle += 0.5/horizontalSplitCount * 4*deltaTime * 3;
        //model = glm::rotate(0.f, rotAxis);



        glm::mat4 MVP = projection * view * model;
        
        
        // TODO: Update uniform variables at every frame

        glUseProgram(moonShaderID);
        
        glUniform1i(moonTexColor_location, 0);

        glUniformMatrix4fv(moon_MVP_location, 1, GL_FALSE, &moonMVP[0][0]);
        glUniformMatrix4fv(moon_Model_location, 1, GL_FALSE, &moonModel[0][0]);

        glUniform3fv(moonCamera_position, 1, &cameraPosition[0]);
        glUniform3fv(moonLight_position, 1, &cameraPosition[0]);


        // TODO TODO TODO TODO TODO TODO
        
        // TODO: Bind moon vertex array        
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, moonTextureColor);

        glBindVertexArray(moonVAO);
        glBindBuffer(GL_ARRAY_BUFFER, moonVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moonEBO);

        glDrawElements(GL_TRIANGLES, moonIndices.size()*3, GL_UNSIGNED_INT, 0);


        
        // TODO: Draw moon object
        
        /*************************/
        

        // TODO: Use worldShaderID program
        glUseProgram(worldShaderID);
        
        // TODO: Update camera at every frame

        // TODO: Update uniform variables at every frame

        //bind the uniform samplers to texture units:
        glUniform1i(texColor_location, 0);
        glUniform1i(texGrey_location,  1);

        glUniformMatrix4fv(earth_Model_location, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(earth_MVP_location, 1, GL_FALSE, &MVP[0][0]);
        glUniform1f(heightFactor_location, heightFactor);
        glUniform3fv(cameraPosition_location, 1, &cameraPosition[0]);

        glUniform3fv(lightPosition_location, 1, &lightPosition[0]);

        

        
        // TODO: Bind world vertex array
        
        // TODO: Draw world object

        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, textureColor);
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, textureGrey);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, indices.size()*3, GL_UNSIGNED_INT, 0);//TODO
        

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        
        deltaTime = (currentTime - lastTime) * 20;
        lastTime = currentTime;
        currentTime = glfwGetTime();

    } while (!glfwWindowShouldClose(window));

    // Delete buffers
    glDeleteBuffers(1, &moonVAO);
    glDeleteBuffers(1, &moonVBO);
    glDeleteBuffers(1, &moonEBO);

    
    // Delete buffers
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
   
    glDeleteProgram(moonShaderID);
    glDeleteProgram(worldShaderID);

    // Close window
    glfwTerminate();
}

void EclipseMap::handleFullScreenToggle() {
    /* 
       notice that this function actually simulates a 
       finite state machine
     */
    
    if(pKeyPressed == false && displayFormat == windowed) {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            pKeyPressed = true;
            displayFormat = fullScreen;

            glfwGetWindowSize(window, &prevWindowWidth, &prevWindowHeight);
            glfwGetWindowPos(window, &xpos, &ypos);

            //glfwGetMonitorWorkarea(monitor, &monitorXpos, &monitorYpos, &monitorWidth, &monitorHeight);
            screenWidth = monitorWidth;
            screenHeight = monitorHeight;


            glfwSetWindowMonitor(window, monitor, 1, 31, screenWidth, screenHeight, mode->refreshRate);//TODO
        }
    }
    else if (pKeyPressed == true && displayFormat == fullScreen) {
        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
            pKeyPressed = false;
        }
    }
    else if (pKeyPressed == false && displayFormat == fullScreen) {
        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            pKeyPressed = true;
            displayFormat = windowed;

            //glfwSetWindowMonitor(window, NULL, 1, 31, screenWidth, screenHeight, mode->refreshRate);//TODO

            screenWidth  = prevWindowWidth;
            screenHeight = prevWindowHeight;

            //cout << "xpos: " << xpos <<" ypos: "<< ypos << endl;
            //if(xpos == NULL)
            //if(ypos == NULL)
            //{xpos = 1;}
            //{ypos = 31;}

            glfwSetWindowMonitor(window, NULL, xpos, ypos, screenWidth, screenHeight, mode->refreshRate);//TODO
        }
    } 
    else { //pKeyPressed == true && displayFormat == windowed
        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
            pKeyPressed = false;
        }
    }

}

void EclipseMap::handleKeyPress(GLFWwindow *window) {
    
    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    //pitch and yaw
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        yaw += 0.05*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        yaw -= 0.05*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        pitch -= 0.05*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        pitch += 0.05*deltaTime;
    }
    //camera speed
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        speed += 0.01*deltaTime * 40;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        speed -= 0.01*deltaTime * 40;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        speed = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        pitch = startPitch;
        yaw = startYaw;
        speed = startSpeed;

        cameraPosition = cameraStartPosition;
        cameraUp = cameraStartUp;
        cameraDirection = cameraStartDirection;
        cameraLeft = glm::cross(cameraUp, cameraDirection);
    }
    

    //height map
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        heightFactor +=10 *deltaTime *3;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        heightFactor -=10 *deltaTime *3;
    }


    //handle full-screen toggle
    handleFullScreenToggle();

}

GLFWwindow *EclipseMap::openWindow(const char *windowName, int width, int height) {
    if (!glfwInit()) {
        getchar();
        return 0;
    }

    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);

    //TODO
    monitorWidth = mode->width;
    monitorHeight = mode->height;

    //TODO

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    glfwSetWindowMonitor(window, NULL, 1, 31, screenWidth, screenHeight, mode->refreshRate);

    if (window == NULL) {
        getchar();
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        getchar();
        glfwTerminate();
        return 0;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0, 0, 0, 0);

    return window;
}


void EclipseMap::initColoredTexture(const char *filename, GLuint shader) {
    int width, height;
    glGenTextures(1, &textureColor);
    cout << shader << endl;
    glBindTexture(GL_TEXTURE_2D, textureColor);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
   

    imageWidth = width;
    imageHeight = height;

    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "TexColor"), 0);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);

}

void EclipseMap::initGreyTexture(const char *filename, GLuint shader) {

    glGenTextures(1, &textureGrey);
    glBindTexture(GL_TEXTURE_2D, textureGrey);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
  



    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "TexGrey"), 1);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);

}

void EclipseMap::initMoonColoredTexture(const char *filename, GLuint shader) {
    int width, height;
    glGenTextures(1, &moonTextureColor);
    cout << shader << endl;
    glBindTexture(GL_TEXTURE_2D, moonTextureColor);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *raw_image = NULL;
    int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
    int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    unsigned long location = 0;
    int i = 0, j = 0;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return;
    }
    printf("Texture filename = %s\n", filename);

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);
    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char *) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++)
            raw_image[location++] = row_pointer[0][i];
    }

    height = cinfo.image_height;
    width = cinfo.image_width;


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
   

    imageWidth = width;
    imageHeight = height;

    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:

    glUniform1i(glGetUniformLocation(shader, "MoonTexColor"), 2);
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(raw_image);
    fclose(infile);

}
