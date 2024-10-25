#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>
 
#include <stdlib.h>
#include <stdio.h>

#include "lib/Program.hpp"
#include "lib/Camera.hpp"
#include "lib/CubeMesh.hpp"

#include "main/App.hpp"

// AppImpl

const int SCREEN_WITDH = 720;
const int SCREEN_HEIGHT = 1280;

class AppImpl {
    private:
        float deltaTime;
        float lastFrame;
        float screenWidth;
        float screenHeight;
        float lastMouseXPos;
        float lastMouseYPos;
        bool firstMouse;
        std::unique_ptr<PerspectiveCamera> camera;

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        static void error_callback(int error, const char* description);
        void processKeyInput(GLFWwindow* window, int key);
        void processMouseInput(GLFWwindow* window, double xposIn, double yposIn);
        void processScrollInput(GLFWwindow* window, double xoffset, double yoffset);
    public:
        AppImpl();
        ~AppImpl();
        void mainLoop();
};

void AppImpl::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    AppImpl* app = static_cast<AppImpl*>(glfwGetWindowUserPointer(window));
    app->processKeyInput(window, key);
}

void AppImpl::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    AppImpl* app = static_cast<AppImpl*>(glfwGetWindowUserPointer(window));
    app->processMouseInput(window, xposIn, yposIn);
}

void AppImpl::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    AppImpl* app = static_cast<AppImpl*>(glfwGetWindowUserPointer(window));
    app->processScrollInput(window, xoffset, yoffset);
}

void AppImpl::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void AppImpl::processKeyInput(GLFWwindow* window, int key) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->camera->processKeyPress(FORWARD, this->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->camera->processKeyPress(BACKWARD, this->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->camera->processKeyPress(LEFT, this->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->camera->processKeyPress(RIGHT, this->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        this->camera->processKeyPress(UP, this->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        this->camera->processKeyPress(DOWN, this->deltaTime);
}

void AppImpl::processMouseInput(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastMouseXPos = xpos;
        lastMouseYPos = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastMouseXPos;
    float yoffset = lastMouseYPos - ypos; // reversed since y-coordinates go from bottom to top

    lastMouseXPos = xpos;
    lastMouseYPos = ypos;

    this->camera->processMouseMovement(xoffset, yoffset);
}

void AppImpl::processScrollInput(GLFWwindow* window, double xoffset, double yoffset) {
    this->camera->processMouseScroll(static_cast<float>(yoffset));
}

AppImpl::AppImpl():
deltaTime(0.0f),
lastFrame(0.0f),
screenWidth(SCREEN_WITDH),
screenHeight(SCREEN_HEIGHT),
lastMouseXPos(SCREEN_WITDH / 2.0f),
lastMouseYPos(SCREEN_HEIGHT / 2.0f),
firstMouse(true),
camera(std::make_unique<PerspectiveCamera>())
{}

AppImpl::~AppImpl()
{}

void AppImpl::mainLoop() {
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Cube app", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowUserPointer(window, this);

    // init
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    GLenum error = glewInit();
    if (GLEW_OK != error) {
        glfwDestroyWindow(window);
        throw std::runtime_error(
            std::string("Error initializing glew: ") +
            reinterpret_cast<const char *>(glewGetErrorString(error)));
    }

    glfwSwapInterval(1);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    const float ratio = static_cast<float>(width) / static_cast<float>(height);

    glEnable(GL_DEPTH_TEST);  

    // init shader program
    const std::string vertexShaderPath = "./resources/shader/vert.glsl";
    const std::string fragmentShaderPath = "./resources/shader/frag.glsl";

    Program program(vertexShaderPath, fragmentShaderPath);
    CubeMesh cubeMesh;

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.useProgram();

        glm::mat4x4 model = glm::mat4x4(1.0f);
        model = glm::scale(model, glm::vec3(2.0f));
        const glm::mat4x4 view = camera->getViewMatrix();
        const glm::mat4x4 projection = glm::perspective(glm::radians(this->camera->getZoom()), ratio, 0.1f, 100.0f);

        program.setMat4x4("model", model);
        program.setMat4x4("view", view);
        program.setMat4x4("projection", projection);

        glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 5.0f);

        program.setVec3("lightPos", lightPos);
        program.setVec3("viewPos", camera->getViewPos());

        cubeMesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// App

App::App():
appImpl(std::make_unique<AppImpl>())
{}

App::~App()
{}

void App::mainLoop() {
    this->appImpl->mainLoop();
}