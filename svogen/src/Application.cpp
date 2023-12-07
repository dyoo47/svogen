#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "VoxelData.h"
#include "Octree.h"

using namespace svogen;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW did not initialize properly." << std::endl;
    }

    std::ifstream t("src/shaders/chunkgen.comp");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string computeSource = buffer.str();
    const GLchar* source = (const GLchar*)computeSource.c_str();

    std::cout << "Read in file" << std::endl;

    GLuint computeProgram = glCreateProgram();
    GLuint computeProgramShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeProgramShader, 1, &source, 0);
    glCompileShader(computeProgramShader);
    glAttachShader(computeProgram, computeProgramShader);
    glLinkProgram(computeProgram);

    std::cout << "Linked shader" << std::endl;
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexStorage3D(GL_TEXTURE_3D, 1, GL_R8I, 1024, 1024, 1024);
    glBindImageTexture(3, texture, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_R8I);

    VoxelData* voxelData = new VoxelData();
    GLubyte* textureBuffer = voxelData->data;

    std::cout << "Added 3d texture" << std::endl;

    glUseProgram(computeProgram);
    GLuint numGroups = 1024 / 8;
    int origin[3] = { 0, -1024, 0 };
    glUniform1i(1, origin[0]);
    glUniform1i(2, origin[1]);
    glUniform1i(3, origin[2]);

    std::cout << "Added uniforms" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    glDispatchCompute(numGroups, numGroups, numGroups);
    glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_BYTE, textureBuffer);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << glGetError() << std::endl;
    std::cout << "Voxel generation elapsed time: " << duration.count() << "ms" << std::endl;
    //std::cout << "result: " + std::to_string(textureBuffer[0]) << std::endl;

    Octree* octree = new Octree(1024);

    start = std::chrono::high_resolution_clock::now();
    int startPos[] = {0, 0, 0};
    octree->constructOctree(1024, 0, 9, startPos, 0, voxelData);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Octree generation elapsed time: " << duration.count() << "ms" << std::endl;
    printf("memOffset: %d\n", octree->memOffset);

    //octree->printBuffer(0, 100);

    std::ofstream outfile;
    outfile.open("level.svo", std::ios::out | std::ios::binary | std::ios::trunc);
    if (outfile.is_open()) {
        //Convert to big endian (Java format)
        int off = octree->memOffset;
        GLubyte header[4];
        header[0] = (GLubyte)(off >> 24u);
        header[1] = (GLubyte)(off >> 16u);
        header[2] = (GLubyte)(off >> 8u);
        header[3] = (GLubyte)(off >> 0u);
        outfile.write(reinterpret_cast<char*>(header), sizeof(int));
        outfile.write((char*)octree->buffer, 1024 * 1024 * 1024);
        printf("Successfully wrote to file.\n");
    }
    outfile.close();

    std::ifstream infile;
    infile.open("level.svo", std::ios::in | std::ios::binary);
    if (infile.is_open()) {
        infile.read((char*)textureBuffer, 1024 * 1024 * 1024);
        voxelData->printBuffer(0, 4);
    }


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}