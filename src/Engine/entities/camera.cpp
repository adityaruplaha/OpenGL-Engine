#include <GLCore/program.h>

#include "camera.h"

Camera* Camera::current_camera{};

// Helper function prototype
bool operator == (CameraOrientation& a, CameraOrientation& b);

Camera::Camera(CameraOrientation& o) : orientation(std::move(o)) {}

void Camera::start() {
	current_camera = this;
	flush();
}

void Camera::stop() {
	current_camera = nullptr;
	flush();
}

void Camera::setDirection(float yaw, float pitch)
{
	orientation.direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	orientation.direction.y = sin(glm::radians(pitch));
	orientation.direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	orientation.direction *= -1;
}

void Camera::setDirection(glm::vec3& target)
{
	orientation.direction = glm::normalize(target - orientation.position);
}

void Camera::setPosition(glm::vec3& pos)
{
	orientation.position = pos;
}

void Camera::setRoll(float roll)
{
	orientation.roll = roll;
}

void Camera::recompute_if_dirty()
{
	if (orientation == previous_orientation)
	{
		return;
	}
	recalculateAxes();
	viewMatrix = glm::lookAt(orientation.position, orientation.target(), axes.up);
	previous_orientation = orientation;
}

void Camera::flush()
{
	if (current_camera)
	{
		current_camera->recompute_if_dirty();
	}	
	setViewMatrix(current_camera);
}

void Camera::recalculateAxes()
{
	axes.right = glm::normalize(glm::cross(Directions::UP, -orientation.direction));
	axes.up = glm::normalize(glm::cross(-orientation.direction, axes.right));
}

void Camera::setViewMatrix(Camera* c)
{
	glm::mat4 mat(1.0f);
	if (c)
	{
		mat = c->viewMatrix;
	}
	Program::current_program->setMat4(Program::current_program->metadata.view_mat_name, mat);
}

// HELPER STUFF

glm::vec3 CameraOrientation::target()
{
	return std::move(position + focal_dist * direction);
}

bool operator == (CameraOrientation& a, CameraOrientation& b)
{
	return (a.position == b.position) &&
		(a.direction == b.direction) &&
		(a.roll == b.roll) &&
		(a.focal_dist == b.focal_dist);
}