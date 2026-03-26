#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLFW/glfw3.h"
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <algorithm>



void processInput(GLFWwindow *window, float delta);


float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

unsigned int indices[] = {
       0, 1, 3, // first triangle
       1, 2, 3  // second triangle
   };

float texCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.5f, 1.0f
};

float blend_amount = 0.5;


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Shader triangleShader("./shaders/triangleshader.glsl","./shaders/triangleshaderfrag.glsl");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int textures[2];
    glGenTextures(2, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);

    stbi_set_flip_vertically_on_load(true);
    int w,h,nCh;
    unsigned char *data = stbi_load("./textures/demo.jpg", &w, &h, &nCh, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, w,h,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    data = stbi_load("./textures/demo2.jpg", &w, &h, &nCh, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, w,h,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE,8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE,8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE,8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    triangleShader.use();
    glUniform1i(glGetUniformLocation(triangleShader.ID, "texture1"),0);
    triangleShader.setInt("texture2",1);

    float delta = 0.0f;
    float lastFrame = 0.0f;
    // Main loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window,delta);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        triangleShader.setFloat("blend_amount", blend_amount);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);

        //glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 6);


        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window,float delta)
{
    float speed = 0.5;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        blend_amount += speed*delta;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        blend_amount -= speed*delta;

    blend_amount = std::clamp(blend_amount, 0.0f, 1.0f);
}
