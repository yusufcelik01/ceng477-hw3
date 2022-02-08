SOURCE_FILES = Main.cpp EclipseMap.cpp Shader.cpp EclipseMap.h Shader.h 
hw3: $(SOURCE_FILES)
	g++ $(SOURCE_FILES) -o hw3 -std=c++11 -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage  -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -ljpeg


zip:
	zip e2457703_e2110252_opengl.zip Makefile *.cpp *.h *.vert *.frag  *.jpg -r glm
