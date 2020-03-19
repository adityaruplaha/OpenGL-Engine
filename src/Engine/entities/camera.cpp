#include <GLCore/program.h>

#include "camera.h"

Camera* Camera::current_camera{};

/// HELPER STUFF

glm::vec3 AxisData::front() const {
    return std::move(glm::normalize(glm::cross(up, right)));
}

glm::vec3 CameraOrientation::target() const {
    return std::move(position + focal_dist * axes.front());
}

// These are linked statically to prevent external use. May be exposed later.

static bool operator==(AxisData& a, AxisData& b) {
    return (a.right == b.right) && (a.up == b.up);
}

static bool operator==(CameraOrientation& a, CameraOrientation& b) {
    return (a.position == b.position) && (a.axes == b.axes) && (a.focal_dist == b.focal_dist);
}

/// Camera class methods

Camera::Camera(CameraOrientation& o) : orientation(std::move(o)) {}

void Camera::start() {
    current_camera = this;
    flush();
}

void Camera::stop() {
    current_camera = nullptr;
    flush();
}

void Camera::ypr(float yaw, float pitch, float roll) {
    // Roll
    auto front = orientation.axes.front();
    auto roll_mat = glm::rotate(glm::mat4(1.0f), glm::radians(roll), front);
    orientation.axes.right = roll_mat * orientation.axes.right;
    orientation.axes.up = roll_mat * orientation.axes.up;
    // Yaw
    orientation.axes.right = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), orientation.axes.up) *
                             orientation.axes.right;
    // Pitch
    orientation.axes.up =
        glm::rotate(glm::mat4(1.0f), glm::radians(pitch), orientation.axes.right) *
        orientation.axes.up;
}

void Camera::lookAt(glm::vec3& target) {
    // Get previous roll value in radians
    auto prev_direction = orientation.axes.front();
    auto fk_right = glm::normalize(glm::cross(prev_direction, Directions::UP));
    float roll = acos(glm::dot(fk_right, orientation.axes.right));

    // Get the direction right
    auto direction = glm::normalize(target - orientation.position);
    orientation.axes.right = glm::normalize(glm::cross(direction, Directions::UP));
    orientation.axes.up = glm::normalize(glm::cross(orientation.axes.right, direction));

    // Roll
    auto front = orientation.axes.front();
    auto roll_mat = glm::rotate(glm::mat4(1.0f), roll, front);
    orientation.axes.right = roll_mat * orientation.axes.right;
    orientation.axes.up = roll_mat * orientation.axes.up;
}

void Camera::recompute() {
    viewMatrix = glm::lookAt(orientation.position, orientation.target(), orientation.axes.up);
}

void Camera::flush() {
    if (current_camera) {
        current_camera->recompute();
    }
    setViewMatrix(current_camera);
}

void Camera::setViewMatrix(Camera* c) {
    glm::mat4 mat(1.0f);
    if (c) {
        mat = c->viewMatrix;
    }
    Program::current_program->setMat4(Program::current_program->metadata.view_mat_name, mat);
}

/// Camera movement methods

void Camera::rmoveFD(float amt) {
    orientation.position += amt * orientation.axes.front();
}

void Camera::rmoveBK(float amt) {
    orientation.position += amt * orientation.axes.front();
}

void Camera::rmoveRT(float amt) {
    orientation.position += amt * orientation.axes.right;
}

void Camera::rmoveLT(float amt) {
    orientation.position += amt * orientation.axes.right;
}

void Camera::rmoveUP(float amt) {
    orientation.position += amt * orientation.axes.up;
}

void Camera::rmoveDN(float amt) {
    orientation.position += amt * orientation.axes.up;
}

void Camera::rturnLT(float amt) {
    ypr(amt, 0, 0);
}

void Camera::rturnRT(float amt) {
    ypr(amt, 0, 0);
}
