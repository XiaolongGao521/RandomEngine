#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.2f;
const float ZOOM        =  45.0f;

class PerspectiveCamera {
    private:
        glm::vec3 cameraPos;
        glm::vec3 worldUpVec;
        glm::vec3 upVec;
        glm::vec3 frontVec;
        glm::vec3 rightVec;
        float speed;
        float mouseSensitivity;
        float zoom;
        float yaw;
        float pitch;
        void updateVectors();
    public:
        PerspectiveCamera();
        ~PerspectiveCamera();
        glm::mat4x4 getViewMatrix();
        glm::vec3 getViewPos();
        float getZoom();
        void processKeyPress(Direction direction, float deltaTime);
        void processMouseMovement(float xOffset, float yOffset);
        void processMouseScroll(float yOffset);
};

PerspectiveCamera::PerspectiveCamera():
cameraPos(glm::vec3(0.0f, 0.0f, 5.0f)),
worldUpVec(glm::vec3(0.0f, 1.0f, 0.0f)),
upVec(glm::vec3(0.0f, 1.0f, 0.0f)),
frontVec(glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f))),
speed(SPEED),
mouseSensitivity(SENSITIVITY),
zoom(ZOOM),
yaw(YAW),
pitch(PITCH)
{
    updateVectors();
}

PerspectiveCamera::~PerspectiveCamera()
{}

glm::mat4x4 PerspectiveCamera::getViewMatrix() {
    return glm::lookAt(this->cameraPos, this->cameraPos + this->frontVec, this->upVec);
}

glm::vec3 PerspectiveCamera::getViewPos() {
    return this->cameraPos;
}

float PerspectiveCamera::getZoom() {
    return this->zoom;
}

void PerspectiveCamera::processKeyPress(Direction direction, float deltaTime) {
    float velocity = this->speed * deltaTime;
    if (direction == FORWARD)
        this->cameraPos += this->frontVec * velocity;
    if (direction == BACKWARD)
        this->cameraPos -= this->frontVec * velocity;
    if (direction == LEFT)
        this->cameraPos -= this->rightVec * velocity;
    if (direction == RIGHT)
        this->cameraPos += this->rightVec * velocity;
    if (direction == UP)
        this->cameraPos += this->worldUpVec * velocity;
    if (direction == DOWN)
        this->cameraPos += -this->worldUpVec * velocity;
}

void PerspectiveCamera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw += xOffset;
    this->pitch += yOffset;

    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;

    this->updateVectors();
}

void PerspectiveCamera::processMouseScroll(float yOffset) {
    this->zoom -= (float) yOffset;
    if (this->zoom < 1.0f)
        this->zoom = 1.0f;
    if (this->zoom > 45.0f)
        this->zoom = 45.0f;
}

void PerspectiveCamera::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->frontVec = glm::normalize(front);
    this->rightVec = glm::normalize(glm::cross(this->frontVec, this->worldUpVec));
    this->upVec = glm::normalize(glm::cross(this->rightVec, this->frontVec));
}
#endif