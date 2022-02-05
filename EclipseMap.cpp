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
    vertexArray[0].texture = glm::vec2(0.f, 0.f);

    vertexArray[1].position = center - radius * up;
    vertexArray[1].texture = glm::vec2(0.f, 1.f);

    size_t index = 2;
    //for(int i=0 ; i < verticalSplitCount; i++){
    //    double beta = PI * ((double)i/ verticalSplitCount);
    //    for(int j=1; j< horizontalSplitCount; j++){
    //        double alpha = 2*PI * ((double)j/ horizontalSplitCount); 
    //        glm::vec3 ver(radius*sin(beta)*cos(alpha), 
    //                            radius*sin(beta) * sin(alpha),
    //                            radius*cos(beta));        
    //        vertexArray[index++].position = ver;
    //    }
    //}

    for(int i=0; i < horizontalSplitCount; i++){// for each meridian
        double alpha = 2*PI * ((double)i /horizontalSplitCount);
        for(int j=1; j < verticalSplitCount; j++){
            double beta = PI * ((double)j/verticalSplitCount);
            glm::vec3 ver(radius*sin(beta)*cos(alpha), 
                                radius*sin(beta) * sin(alpha),
                                radius*cos(beta));        
            vertexArray[index].position = ver;

            glm::vec2 tex((double)i /horizontalSplitCount, (double)j/verticalSplitCount);
            
            vertexArray[index].texture = tex;

            index++;
        }
    }
    //cout << "index: " << index << endl;


    vector<triangle>* triangles = new vector<triangle>;
    
    for(size_t i=1; i < horizontalSplitCount; i++){
        size_t index = (verticalSplitCount -1) *i + 2;
        if(i == horizontalSplitCount-1){
            triangles->push_back(triangle(0, 2, index));
            //TODO
            triangles->push_back(triangle(index+verticalSplitCount -2, verticalSplitCount, 1));
        } 
        triangles->push_back(triangle(0, index, index- (verticalSplitCount-1)));

        index--;//for south pole
        triangles->push_back(triangle(index, index + (verticalSplitCount-1), 1));
    }

    for(size_t i=1; i <horizontalSplitCount; i++){
        size_t index = i* (verticalSplitCount-1) +2;
        for(size_t j=1; j< verticalSplitCount; j++){
            triangles->push_back(triangle(index, index+1, index-(verticalSplitCount-1)));
            triangles->push_back(triangle(index+1, index-verticalSplitCount+2,index- verticalSplitCount +1));

            if(i== horizontalSplitCount-1){
                triangles->push_back(triangle(j, j+1, index));
                triangles->push_back(triangle(j+1, index+1, index));
                //cout << "edge case meridian \n";
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
    //GLuint worldShaderID = initShaders("worldShader.vert", "worldShader.frag");
    GLuint worldShaderID = initShaders("testWorld.vert", "testWorld.frag");

    initColoredTexture(coloredTexturePath, worldShaderID);
    initGreyTexture(greyTexturePath, worldShaderID);

    // TODO: Set worldVertices
    size_t numberOfEarthVertices = (verticalSplitCount -1) *horizontalSplitCount  + 2;
    vertex earthVertices[numberOfEarthVertices];
    //triangle* indices = NULL;
    vector<triangle> indices;

    createSphereArrays( earthVertices, indices, radius, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
                            horizontalSplitCount, verticalSplitCount);

    //for(size_t k=0; k< numberOfEarthVertices; k++){
    //    glm::vec3 v = earthVertices[k].position;
    //    cout << v.x << " " << v.y << " " << v.z << endl;
    //}

    //for(size_t k=0; k< indices.size(); k++){
    //    triangle temps = indices[k];
    //    cout << temps.vertex1 << " " << temps.vertex2 << " " << temps.vertex3 << endl;
    //}


    //vertex earthVertices[3];
    ////earthVertices[0].position = glm::vec3(-0.5f, -0.5f, 0.0f);
    ////earthVertices[1].position = glm::vec3( 0.5f, -0.5f, 0.0f);
    ////earthVertices[2].position = glm::vec3( 0.0f,  0.5f, 0.0f);

    //earthVertices[0].position = glm::vec3(-1000.f, -1000.f, 0.0f);
    //earthVertices[1].position = glm::vec3( 1000.f, -1000.f, 0.0f);
    //earthVertices[2].position = glm::vec3( 0.0f,  1000.f, 0.0f);

    //earthVertices[0].texture = glm::vec2(0.0f, 0.0f);
    //earthVertices[1].texture = glm::vec2(1.0f, 0.0f);
    //earthVertices[2].texture = glm::vec2(0.5f, 1.0f);

    //

    //triangle indices[1];
    //indices[0].vertex1 = 0;
    //indices[0].vertex2 = 1;
    //indices[0].vertex3 = 2;

    //GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(earthVertices), earthVertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);




    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(earthVertices[0]), (void*) 0);
    //glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(earthVertices[0]), (void*) (3*sizeof(float)));
    //glEnableVertexAttribArray(1);

    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(earthVertices[0]), (void*) (6*sizeof(float)));
    //glEnableVertexAttribArray(2);

    ////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*((horizontalSplitCount-1) * verticalSplitCount +2), earthVertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(earthVertices), earthVertices, GL_STATIC_DRAW);

    //cout << "sizeof(earthVertices)= " << sizeof(earthVertices) << endl;
    //cout << "sizeof(vertex)" << sizeof(vertex) << endl;
    //cout << "numberOfEarthVertices" << numberOfEarthVertices << endl;
    //cout << "indices.size()" << indices.size() << endl;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(triangle) , &indices[0], GL_STATIC_DRAW);




    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (6*sizeof(float)));
    glEnableVertexAttribArray(2);
    //GLuint VAO;


    //GLuint EBO;
    
    // TODO: Configure Buffers
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Main rendering loop
    glm::vec3 rotAxis = glm::vec3(0.f, 1.f, -1.f);
    float angle = 180;
    do {
        glViewport(0, 0, screenWidth, screenHeight);

        glClearStencil(0);
        glClearDepth(1.0f);
        glClearColor(0, 0.0f, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



        // TODO: Handle key presses
        handleKeyPress(window);

        // TODO: Manipulate rotation variables
        
        // TODO: Bind textures
        
        // TODO: Use moonShaderID program
        
        // TODO: Update camera at every frame

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(angle, rotAxis);
        angle += 0.0003f;
        //angle = 180;
        //model = glm::scale(glm::mat4(1.0f), 
        //               glm::vec3(1000.0f, 1000.0f, 1000.0f));

        glm::mat4 view = glm::lookAt(
                cameraPosition,
                cameraPosition + cameraDirection,
                cameraUp
                //glm::vec3(0, 1, 0)
        );

        //glm::mat4 view = glm::lookAt(
        //        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        //        glm::vec3(0,0,0), // and looks at the origin
        //        glm::vec3(0,1,0)
        //);

        glm::mat4 projection = glm::perspective(
                glm::radians(projectionAngle),
                aspectRatio,
                near,
                far
        );

        glm::mat4 MVP = projection * view * model;
        
        
        // TODO: Update uniform variables at every frame





        // TODO TODO TODO TODO TODO TODO
        
        // TODO: Bind moon vertex array        

        // TODO: Draw moon object
        
        /*************************/

        // TODO: Use worldShaderID program
        glUseProgram(worldShaderID);
        
        // TODO: Update camera at every frame

        // TODO: Update uniform variables at every frame
        GLuint MVP_location;
        MVP_location = glGetUniformLocation(worldShaderID, "MVP");

        glUniformMatrix4fv(MVP_location, 1, GL_FALSE, &MVP[0][0]);
        


        

        
        // TODO: Bind world vertex array
        
        // TODO: Draw world object
        glBindTexture(GL_TEXTURE_2D, textureColor);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, indices.size()*3, GL_UNSIGNED_INT, 0);

        //cout << "triangles drawn: " << indices.size() << endl;

        //cout << indices.size() << endl;//TODO debug
        //cout << verticalSplitCount * ((horizontalSplitCount-2)*2 +2) <<endl;
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
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

void EclipseMap::handleKeyPress(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

}

GLFWwindow *EclipseMap::openWindow(const char *windowName, int width, int height) {
    if (!glfwInit()) {
        getchar();
        return 0;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, windowName, NULL, NULL);
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
