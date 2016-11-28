/*
 The MIT License (MIT)
 Copyright (c) 2016 Erick Mendez
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define GLEW_STATIC
#ifdef __APPLE__
#   include <GL/glew.h>
#   include <OpenGL/gl.h>
#   include <OpenGL/glext.h>
#else
#   include <GL/glew.h>
#   include <GL/gl.h>
#   include <GL/glext.h>
#endif

#include <iostream>
#include <SDL2/SDL.h>

/// These are the variables used for OpenGL
/// structures, buffers, arrays, etc.
GLuint arrayBufferVertexPositions = 0;
GLuint arrayBufferTexCoords = 0;
GLuint vertexArrayCube = 0;
GLuint elementBufferIndices = 0;
GLuint shaderProgram = 0;
GLint uniLocProjectionMatrix = 0;
GLint uniLocModelViewMatrix = 0;
GLint uniLocTexture = 0;
GLint attribLocVertexPosition = 0;
GLint attribLocVertexTexCoords = 0;

/// The Shader source code
/// The escaped new line on the #version lines is needed on Linux
const GLchar* vertexShaderSource ="\
#version 150 core\n \
uniform mat4 projectionMatrix; \
uniform mat4 modelviewMatrix; \
in vec3 vertexPosition; \
in vec2 vertexTexCoord; \
out vec2 fragTexCoord; \
void main() { \
    fragTexCoord = vertexTexCoord; \
    gl_Position = projectionMatrix * modelviewMatrix  * vec4(vertexPosition, 1.0); \
}";

const GLchar* fragmentShaderSource ="\
#version 150 core\n \
uniform sampler2D tex; \
in vec2 fragTexCoord; \
out vec4 outColor; \
void main() { \
    outColor = texture(tex, fragTexCoord); \
}";


/// Here is the geometry of the cube including vertices, colors and indices
/// as well as a hard coded modelview projection matrix
const GLfloat projectionMatrix[16] = { 4.48182, 0, 0, 0, 0, 5.97576, 0, 0, 0, 0, -1.0002, -1, 0, 0,-0.20002, 0};
const GLfloat modelviewMatrix[16] = { -0.707107, -0.372046, 0.601317, 0, 0, 0.85039, 0.526152, 0, -0.707107, 0.372046, -0.601317, 0, 0,4.76837e-07, -13.3041, 1 };

/// Vertices of the cube. Each row is a triangle. Each pair of rows is a face.
/// 6 faces x 2 triangles (per face) x 3 vertices (per triangle)  x 3 values (per vertex)= 108 floats
const size_t numDataVertexPositions = 6*2*3*3;
const GLfloat dataVertexPositions[] = {
    // clang-format off
    -1.00f, -1.00f, -1.00f,     +1.00f, -1.00f, -1.00f,     +1.00f, +1.00f, -1.00f,
    +1.00f, +1.00f, -1.00f,     -1.00f, +1.00f, -1.00f,     -1.00f, -1.00f, -1.00f,
    
    +1.00f, -1.00f, -1.00f,     +1.00f, -1.00f, +1.00f,     +1.00f, +1.00f, +1.00f,
    +1.00f, +1.00f, +1.00f,     +1.00f, +1.00f, -1.00f,     +1.00f, -1.00f, -1.00f,
    
    +1.00f, -1.00f, +1.00f,     -1.00f, -1.00f, +1.00f,     -1.00f, +1.00f, +1.00f,
    -1.00f, +1.00f, +1.00f,     +1.00f, +1.00f, +1.00f,     +1.00f, -1.00f, +1.00f,
    
    -1.00f, -1.00f, +1.00f,     -1.00f, -1.00f, -1.00f,     -1.00f, +1.00f, -1.00f,
    -1.00f, +1.00f, -1.00f,     -1.00f, +1.00f, +1.00f,     -1.00f, -1.00f, +1.00f,
    
    -1.00f, +1.00f, -1.00f,     +1.00f, +1.00f, -1.00f,     +1.00f, +1.00f, +1.00f,
    +1.00f, +1.00f, +1.00f,     -1.00f, +1.00f, +1.00f,     -1.00f, +1.00f, -1.00f,
    
    -1.00f, -1.00f, +1.00f,     +1.00f, -1.00f, +1.00f,     +1.00f, -1.00f, -1.00f,
    +1.00f, -1.00f, -1.00f,     -1.00f, -1.00f, -1.00f,     -1.00f, -1.00f, +1.00f
    // clang-format on
};

/// Texture Coordinates, each row is the texcoord of a triangle.
/// 6 faces x 2 triangles (per face) x 3 vertices (per triangle)  x 2 values (per vertex)= 72 floats
const size_t numDataVertexTexCoords = 6*2*3*2;
const GLfloat dataVertexTexCoords[] = {
    // clang-format off
    0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,
    1.0f, 1.0f,     0.0f, 1.0f,     0.0f, 0.0f,
    
    0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,
    1.0f, 1.0f,     0.0f, 1.0f,     0.0f, 0.0f,
    
    0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,
    1.0f, 1.0f,     0.0f, 1.0f,     0.0f, 0.0f,
    
    0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,
    1.0f, 1.0f,     0.0f, 1.0f,     0.0f, 0.0f,
    
    0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,
    1.0f, 1.0f,     0.0f, 1.0f,     0.0f, 0.0f,
    
    0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f,
    1.0f, 1.0f,     0.0f, 1.0f,     0.0f, 0.0f
    // clang-format on
};

/// There are two triangles for each of the 6 faces
const size_t numDataIndices = 6*2*3*1;
const GLuint dataIndices[] = {
    // clang-format off
    0,  1,  2,      3,  4,  5,
    6,  7,  8,      9, 10, 11,
    12, 13, 14,     15, 16, 17,
    18, 19, 20,     21, 22, 23,
    24, 25, 26,     27, 28, 29,
    30, 31, 32,     33, 34, 35
    // clang-format on
};

/// Hard coded red-green checkers texture 4 x 4
/// This example doesnt show you how to open a file and load it to a texture
const unsigned char dataTexture[] = {
    // clang-format off
    255, 000, 000,  255, 000, 000,  255, 000, 000,  255, 000, 000,      000, 255, 000,  000, 255, 000,  000, 255, 000,  000, 255, 000,
    255, 000, 000,  255, 000, 000,  255, 000, 000,  255, 000, 000,      000, 255, 000,  000, 255, 000,  000, 255, 000,  000, 255, 000,
    255, 000, 000,  255, 000, 000,  255, 000, 000,  255, 000, 000,      000, 255, 000,  000, 255, 000,  000, 255, 000,  000, 255, 000,
    255, 000, 000,  255, 000, 000,  255, 000, 000,  255, 000, 000,      000, 255, 000,  000, 255, 000,  000, 255, 000,  000, 255, 000,
    
    000, 000, 255,  000, 000, 255,  000, 000, 255,  000, 000, 255,      255, 255, 000,  255, 255, 000,  255, 255, 000,  255, 255, 000,
    000, 000, 255,  000, 000, 255,  000, 000, 255,  000, 000, 255,      255, 255, 000,  255, 255, 000,  255, 255, 000,  255, 255, 000,
    000, 000, 255,  000, 000, 255,  000, 000, 255,  000, 000, 255,      255, 255, 000,  255, 255, 000,  255, 255, 000,  255, 255, 000,
    000, 000, 255,  000, 000, 255,  000, 000, 255,  000, 000, 255,      255, 255, 000,  255, 255, 000,  255, 255, 000,  255, 255, 000
    // clang-format on
};

/// Simple error checking function, only prints the error
void CheckForGLErrors(const char* msg)
{
    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR)
        std::cout << "glError [" << msg << "]: " << glErr << "\n";
}

int main()
{
    /// This section creates the window
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to init SDL\n";
        return -1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_Window *sdl_Window = SDL_CreateWindow("Textured Cube",
                                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    if (sdl_Window == NULL)
    {
        std::cout << "Unable to create window\n";
        return -1;
    }
    
    SDL_GLContext sdl_Context = SDL_GL_CreateContext(sdl_Window);
    SDL_GL_MakeCurrent(sdl_Window, sdl_Context);
    
    /// At this point the window has been created
    /// We now initialize Glew and set some basic OpenGL settings
    glewExperimental = GL_TRUE; // Needed on Linux, harmless on macOS
    if(glewInit()!=GLEW_OK)
    {
        std::cout << "Glew failed \n";
        return -1;
    }
    CheckForGLErrors("After glew");
    
    /// Clear color is a tad blue, useful for debugging
    /// if it shows full black then something went wrong with the context or the window
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    CheckForGLErrors("After basic GL state settings");
    
    /// Upload the texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, &dataTexture[0]);
    glBindTexture(GL_TEXTURE_2D, 0);
    CheckForGLErrors("Creating the texture");
    
    
    /// We now create all the buffers and arrays
    glGenVertexArrays(1, &vertexArrayCube);
    glBindVertexArray(vertexArrayCube);
    CheckForGLErrors("After creating the vertex array where the cube is");
    
    glGenBuffers(1, &arrayBufferVertexPositions);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferVertexPositions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numDataVertexPositions, dataVertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckForGLErrors("Creating the Vertex Buffer of the cube");
    
    glGenBuffers(1, &arrayBufferTexCoords);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTexCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numDataVertexTexCoords, dataVertexTexCoords, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckForGLErrors("Creating the TexCoord Buffer of the cube");
    
    glGenBuffers(1, &elementBufferIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numDataIndices, dataIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    CheckForGLErrors("Creating the indices buffer of the cube");
    
    /// We now create our shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    CheckForGLErrors("Compiling shaders");
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    CheckForGLErrors("Linking shaders and creating the program");
    
    /// Get the attributes and uniforms locations
    uniLocProjectionMatrix = glGetUniformLocation(shaderProgram, "projectionMatrix");
    uniLocModelViewMatrix = glGetUniformLocation(shaderProgram, "modelviewMatrix");
    attribLocVertexPosition = glGetAttribLocation(shaderProgram, "vertexPosition");
    attribLocVertexTexCoords = glGetAttribLocation(shaderProgram, "vertexTexCoord");
    uniLocTexture = glGetUniformLocation(shaderProgram, "tex");
    CheckForGLErrors("Getting the uniform and attibutes locations");
    glUseProgram(0);
    
    /// We are ready to do our render loop
    bool leaveASAP = false;
    while(!leaveASAP)
    {
        /// Very simple check to see if we need to close the window
        SDL_Event sdl_Event;
        while (SDL_PollEvent(&sdl_Event))
        {
            if (sdl_Event.type == SDL_QUIT)
                leaveASAP = true;
            else if (sdl_Event.type == SDL_KEYUP && sdl_Event.key.keysym.sym == SDLK_ESCAPE)
                leaveASAP = true;
            else if (sdl_Event.type == SDL_WINDOWEVENT && sdl_Event.window.event == SDL_WINDOWEVENT_CLOSE)
                leaveASAP = true;
        }
        
        glUseProgram(shaderProgram);
        /// This is the actual display block
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniformMatrix4fv(uniLocProjectionMatrix, 1, GL_FALSE, &projectionMatrix[0]);
        glUniformMatrix4fv(uniLocModelViewMatrix, 1, GL_FALSE, &modelviewMatrix[0]);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(uniLocTexture, 0);

        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferVertexPositions);
        glEnableVertexAttribArray(attribLocVertexPosition);
        glVertexAttribPointer(attribLocVertexPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferTexCoords);
        glEnableVertexAttribArray(attribLocVertexTexCoords);
        glVertexAttribPointer(attribLocVertexTexCoords, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        // Comment or uncomment any of the two lines below to see either arrays or elements drawing
        glDrawArrays(GL_TRIANGLES, 0, numDataIndices);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferIndices);
//        glDrawElements(GL_TRIANGLES, numDataIndices, GL_UNSIGNED_INT, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(attribLocVertexPosition);
        glDisableVertexAttribArray(attribLocVertexTexCoords);
        glUseProgram(0);
        CheckForGLErrors("Drawing");
        SDL_GL_SwapWindow(sdl_Window);
    }
    
    SDL_GL_DeleteContext(sdl_Context);
    SDL_DestroyWindow(sdl_Window);
}
