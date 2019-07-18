//
//  main.cpp
//  light
//
//  Created by ashoka on 2019/3/19.
//

#include <iostream>
#include <sstream>
#include <string>
#include "ShaderProgram.hpp"
#include "glfw3.h"
#include "asLog.h"
#include <unistd.h>
#include <math.h>
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Camera.hpp"
#include "openGlHelper.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float FPS = 70.0f;

float screenWidth = SCR_WIDTH;
float screenHeight = SCR_HEIGHT;
GLFWwindow *window;
Camera camera = Camera(glm::vec3(0.5f, 0.5f, 5.0f));
float deltaTime = 0.0f;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

int createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // 多重采样缓冲，抗锯齿用
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(screenWidth, screenHeight, "Light", NULL, NULL);
    if (window == NULL)
    {
        asLog("failed to create window");
        glfwTerminate();
        return -1;
    }
    return 1;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

bool firstCursor = true;
float lastXpos = screenWidth / 2;
float lastYpos = screenHeight / 2;
void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstCursor)
    {
        lastXpos = xpos;
        lastYpos = ypos;
        firstCursor = false;
    }
    asLog("xpos: %f, y: %f", xpos, ypos);
    float offsetX = xpos - lastXpos;
    float offsetY = lastYpos - ypos;
    asLog("offsetX: %f, offsetY: %f", offsetX, offsetY);
    lastXpos = xpos;
    lastYpos = ypos;
    camera.progressMouseMove(offsetX, offsetY);
}

void scrollCallback(GLFWwindow *window, double xpos, double ypos)
{
    camera.progressScroll(ypos);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
    else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void setupWindow()
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

unsigned int genLampVAO(float *vertices, unsigned int length)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    return vao;
}

unsigned int genCubeVAO(float *vertices, unsigned int length)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    return vao;
}

int initOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }
    return 1;
}
glm::vec3 lampPos(1.2f, 1.0f, 2.0f);
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};
void drawLamp(ShaderProgram &lampShader, unsigned int vao)
{
    lampShader.use();
    glBindVertexArray(vao);
    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), screenWidth / screenHeight, 0.1f, 100.0f);
    lampShader.setMatrix4fv("view", view);
    lampShader.setMatrix4fv("projection", projection);
    for (int i = 0; i < 4; i++)
    {
        glm::vec3 position = pointLightPositions[i];
        glm::mat4 model = glm::mat4();
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMatrix4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

unsigned int diffuseMap = 0;
unsigned int specularMap = 0;
unsigned int emissionMap = 0;
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};
void drawCube(ShaderProgram &cubeShader, unsigned int vao)
{
    cubeShader.use();
    glBindVertexArray(vao);
    glm::vec3 cubPos(0.0f, 0.0f, 0.0f);
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, cubPos);
    //    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.5, 0.3, 0.5));
    glm::mat4 view = camera.viewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), screenWidth / screenHeight, 0.1f, 100.0f);
    cubeShader.setMatrix4fv("model", model);
    cubeShader.setMatrix4fv("view", view);
    cubeShader.setMatrix4fv("projection", projection);
    cubeShader.setVec3("cubeColor", 1.0f, 0.5f, 0.31f);
    cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("lampPos", lampPos.x, lampPos.y, lampPos.z);
    cubeShader.setVec3("viewPos", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
    // cubeShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    // cubeShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    // cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    cubeShader.setFloat("material.shininess", 32.0f);
    cubeShader.setInt("material.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    cubeShader.setInt("material.specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    cubeShader.setInt("material.emission", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissionMap);

    glm::vec3 lightColor = glm::vec3(1);
    // lightColor.x = sin(glfwGetTime() * 2.0f);
    // lightColor.y = sin(glfwGetTime() * 0.7f);
    // lightColor.z = sin(glfwGetTime() * 1.3f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.05f);
    cubeShader.setVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
    cubeShader.setVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
    cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    cubeShader.setFloat("light.constant", 1.0f);
    cubeShader.setFloat("light.linear", 0.09f);
    cubeShader.setFloat("light.quadratic", 0.032f);
    cubeShader.setVec3("light.position", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
    cubeShader.setVec3("light.direction", camera.cameraFront.x, camera.cameraFront.y, camera.cameraFront.z);
    cubeShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    cubeShader.setFloat("light.outterCutOff", glm::cos(glm::radians(17.5f)));

    // 定向光
    cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    cubeShader.setVec3("dirLight.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
    cubeShader.setVec3("dirLight.diffuse", diffuseColor.x / 2, diffuseColor.y / 2, diffuseColor.z / 2);
    cubeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // 4 个点光源
    for (int i = 0; i < 4; i++)
    {
        glm::vec3 position = pointLightPositions[i];
        std::string prefix = std::string("pointLights[") + std::to_string(i) + std::string("].");
        cubeShader.setVec3(prefix + std::string("position"), position.x, position.y, position.z);
        cubeShader.setVec3(prefix + std::string("ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
        cubeShader.setVec3(prefix + std::string("diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
        cubeShader.setVec3(prefix + std::string("specular"), 0.0f, 1.0f, 0.0f);
        cubeShader.setFloat(prefix + std::string("constant"), 1.0f);
        cubeShader.setFloat(prefix + std::string("linear"), 0.09f);
        cubeShader.setFloat(prefix + std::string("quadratic"), 0.032f);
    }

    // 聚光
    cubeShader.setVec3("spotLight.position", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
    cubeShader.setVec3("spotLight.direction", camera.cameraFront.x, camera.cameraFront.y, camera.cameraFront.z);
    cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    cubeShader.setFloat("spotLight.outterCutOff", glm::cos(glm::radians(17.5f)));
    cubeShader.setVec3("spotLight.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
    cubeShader.setVec3("spotLisht.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
    cubeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    cubeShader.setFloat("spotLight.constant", 1.0f);
    cubeShader.setFloat("spotLight.linear", 0.09f);
    cubeShader.setFloat("spotLight.quadratic", 0.032f);

    for (int i = 0; i < 10; i += 1)
    {
        glm::vec3 cubPos = cubePositions[i];
        glm::mat4 model = glm::mat4();
        model = glm::translate(model, cubPos);
        float angel = 20.0f * i;
        model = glm::rotate(model, glm::radians(angel), glm::vec3(1.0f, 0.3f, 0.5f));
        cubeShader.setMatrix4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    //    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void draw()
{
    glEnable(GL_MULTISAMPLE);
    ShaderProgram cubeShader = ShaderProgram("lightMaps.vs", "lightMaps.fs");
    ShaderProgram lampShader = ShaderProgram("cube.vs", "lamp.fs");
    unsigned int lampVAO = genLampVAO(vertices, sizeof(vertices) / sizeof(float));
    unsigned int cubeVAO = genCubeVAO(vertices, sizeof(vertices) / sizeof(float));
    diffuseMap = textureGenarate("container2.png");
    specularMap = textureGenarate("container2_specular.png");
    emissionMap = textureGenarate("matrix.jpg");
    float fps = FPS;
    float expectedFrameTime = 1.0f / fps;
    float lastFrameTime = 0.0f;
    bool firstRender = true;
    float diffTime = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        if (firstRender)
        {
            lastFrameTime = currentTime;
            firstRender = false;
        }
        float temp = currentTime - lastFrameTime;
        if (temp < expectedFrameTime)
        {
            diffTime = expectedFrameTime - temp;
            usleep(diffTime * 1000000);
            temp = expectedFrameTime;
        }
        deltaTime = temp;
        lastFrameTime = currentTime;
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        drawLamp(lampShader, lampVAO);
        drawCube(cubeShader, cubeVAO);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glfwTerminate();
}

int main(int argc, const char *argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    if (1 != createWindow())
    {
        return -1;
    }
    setupWindow();
    if (1 == initOpenGL())
    {
        draw();
    }
    return 0;
}
