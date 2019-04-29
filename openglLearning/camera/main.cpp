//
//  main.cpp
//  texture
//
//  Created by Ashoka on 2018/4/30.
//

#include <iostream>
#include "ShaderProgram.hpp"
#include "glfw3.h"
#include "asLog.h"
#include <unistd.h>
#include <math.h>
#include <vector>
#include <map>
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Camera.hpp"

int createHelloTriangleWindow();
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
void progressInput(GLFWwindow *window);
void mousePositionCallback(GLFWwindow *window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow *window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float fps = 70.0;

float mixValue = 0.2f;

glm::vec3 cameraPos = glm::vec3(0, 0, 3);
glm::vec3 cameraFront = glm::vec3(0, 0, -1);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

/*
 作为我们摄像机系统的一个附加内容，我们还会来实现一个缩放(Zoom)接口。在之前的教程中我们说视野(Field of View)或fov定义了我们可以看到场景中多大的范围。当视野变小时，场景投影出来的空间就会减小，产生放大(Zoom In)了的感觉。我们会使用鼠标的滚轮来放大。
 */
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

float screenWidth = SCR_WIDTH;
float screenHeight = SCR_HEIGHT;

int main(int argc, const char *argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    createHelloTriangleWindow();
    return 0;
}

unsigned int vaoGenerate(float *vertices, unsigned int vertexCount)
{
    // 顶点数组对象
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 顶点缓冲对象
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    // 告诉OpenGL该如何解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 第二个属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 此时可以相关数据已经存到vao中，可以解绑vbo和vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);

    return vao;
}

unsigned int textureGenarate(const char *imagePath)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    // OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。很幸运，stb_image.h能够在图像加载时帮助我们翻转y轴，只需要在加载任何图像前加入以下语句即可：
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    asLog("%s width: %d, height: %d, channels: %d", imagePath, width, height, nrChannels);
    if (data)
    {
        GLenum format = GL_RGB;
        // 当channel == 4 时，说明有alpha通道， == 3 时，只有rgb通道
        if (nrChannels == 4)
        {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        asLog("can't load image at path: %s", imagePath);
    }
    stbi_image_free(data);
    return texture;
}

int createHelloTriangleWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Hello Triangle", NULL, NULL);
    if (window == NULL)
    {
        asLog("Failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        asLog("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    char buffer[1024];
    getcwd(buffer, 1024);
    printf("The current directory is: %s", buffer);

    ///=============================================================================
    /// @name 着色器程序
    ///=============================================================================
    //    ShaderProgram shaderProgram = ShaderProgram("timing.vs", "timing.fs");
    ShaderProgram shaderProgram = ShaderProgram("4.1.1.depth_testing.vs", "4.1.1.depth_testing.fs");

    //    float vertices[] = {
    //        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    //        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    //        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    //        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    //    };
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    float planeVertices[] = {
        // 将 y 轴设置成 -0.501 稍微下移减少深度冲突
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.501f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.501f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.501f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.501f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.501f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.501f, -5.0f, 2.0f, 2.0f};
    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    asLog("vertices size: %d", sizeof(cubeVertices));

    // transparent window locations
    // --------------------------------
    std::vector<glm::vec3> windows
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3( 0.5f, 0.0f, -0.6f)
    };
    // 顶点数组对象
    unsigned int cubeVao = vaoGenerate(cubeVertices, sizeof(cubeVertices) / sizeof(float));
    unsigned int planeVao = vaoGenerate(planeVertices, sizeof(planeVertices) / sizeof(float));
    unsigned int windowVao = vaoGenerate(transparentVertices, sizeof(transparentVertices) / sizeof(float));

    unsigned int cubeTexture = textureGenarate("marble.jpg");
    unsigned int planeTexture = textureGenarate("metal.png");
    unsigned int windowTexture = textureGenarate("blending_transparent_window.png");

    // uncomment this call to draw in wireframe polygons. 线条模式
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 激活着色器程序
    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    //    shaderProgram.setInt("texture2", 1);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    
    float expectedFrameTime = 1 / fps;
    // 当帧数过高时，usleep(diffTime * 1000000) 来降低帧数
    float diffTime = 0.0;
    // 开启深度测试
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float temp = currentFrame - lastFrame;
        if (temp < expectedFrameTime)
        {
            diffTime = expectedFrameTime - temp;
            usleep(diffTime * 1000000);
            temp += diffTime;
            //            asLog("sleep:%f", diffTime);
        }
        deltaTime = temp;
        lastFrame = currentFrame;
        //        asLog("deltaTime:%f", deltaTime);

        progressInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.setFloat("mixValue", mixValue);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view;
        glm::mat4 projection;

        // 创建一个 lookAt 观察矩阵
        //        float radius = 10.0f;
        // camera 随时间转圈
        //        float camX = sin((float)glfwGetTime()) * radius;
        //        float camZ = cos((float)glfwGetTime()) * radius;
        //        view = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        view = camera.viewMatrix();

        // 投影矩阵
        projection = glm::perspective(glm::radians(camera.zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

        shaderProgram.setMatrix4fv("view", view);
        shaderProgram.setMatrix4fv("projection", projection);

        // cubes
        glBindVertexArray(cubeVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shaderProgram.setMatrix4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shaderProgram.setMatrix4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // floor
        glBindVertexArray(planeVao);
        glBindTexture(GL_TEXTURE_2D, planeTexture);
        model = glm::mat4(1.0f);
        shaderProgram.setMatrix4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // windows
        glBindVertexArray(windowVao);
        glBindTexture(GL_TEXTURE_2D, windowTexture);
        std::map<float, glm::vec3> sorted;
        for(size_t i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(camera.cameraPos - windows[i]);
            sorted[distance] = windows[i];
        }

        for(std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shaderProgram.setMatrix4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &cubeVao);
    glDeleteVertexArrays(1, &planeVao);
    glDeleteVertexArrays(1, &windowVao);

    glfwTerminate();
    return 0;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    asLog("current window width: %d, height: %d", width, height);
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void progressInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        asLog("escape pressed");
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01;
        mixValue = mixValue > 1.0f ? 1.0f : mixValue;
        asLog("mix value: %f", mixValue);
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01;
        mixValue = mixValue < 0.0f ? 0.0f : mixValue;
        asLog("mix value: %f", mixValue);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.progressKeyboard(FORWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.progressKeyboard(BACKWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.progressKeyboard(LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.progressKeyboard(RIGHT, deltaTime);
    }
}

float lastXpos = screenWidth / 2;
float lastYpos = screenHeight / 2;
bool firstMouse = true;
float sensitivity = 0.05f;
// 俯仰角
float pitch = 0.0f;
// 偏航角
float yaw = -90.0f; // 初始化成pitch =0, yaw = -90 相当于 camerafront = (0, 0, -1)
void mousePositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastXpos = xpos;
        lastYpos = ypos;
        firstMouse = false;
    }
    asLog("xpos: %f, y: %f", xpos, ypos);
    float offsetX = xpos - lastXpos;
    float offsetY = lastYpos - ypos;
    asLog("offsetX: %f, offsetY: %f", offsetX, offsetY);
    lastXpos = xpos;
    lastYpos = ypos;

    camera.progressMouseMove(offsetX, offsetY);
}

void mouseScrollCallback(GLFWwindow *window, double xpos, double ypos)
{
    asLog("xpos: %f, ypos: %f", xpos, ypos);
    camera.progressScroll(ypos);
}
