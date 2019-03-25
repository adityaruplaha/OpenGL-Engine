#include <GLCore/program.h>

#include "camera.h"

Camera* Camera::current_camera{};

Camera::Camera(CameraOrientation& o) :
	orientation(std::move(o)), previous_orientation(orientation.load()),
	watcher(this, Camera::camera_watcher) {}

void Camera::start() {
	recalculateAxes();
	watcher.start();
	setViewMatrix(this);
	current_camera = this;
}

void Camera::stop() {
	watcher.stop();
	setViewMatrix(nullptr);	
	current_camera = nullptr;
}

void Camera::setDirection(float yaw, float pitch)
{
	auto temp = orientation.load();
	temp.direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	temp.direction.y = sin(glm::radians(pitch));
	temp.direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	temp.direction *= -1;
	orientation.store(temp);
}

void Camera::setDirection(glm::vec3& target)
{
	auto temp = orientation.load();
	temp.direction = glm::normalize(target - temp.position);
	orientation.store(temp);
}

void Camera::setPosition(glm::vec3& pos)
{
	auto temp = orientation.load();
	temp.position = pos;
	orientation.store(temp);
}

void Camera::setRoll(float roll)
{
	auto temp = orientation.load();
	temp.roll = roll;
	orientation.store(temp);
}

void Camera::recalculateAxes()
{
	auto inv_dir = glm::normalize(-orientation.load().direction);
	axes.right = glm::normalize(glm::cross(Directions::UP, inv_dir));
	axes.up = glm::normalize(glm::cross(inv_dir, axes.right));
}

void Camera::setViewMatrix(Camera* c)
{
	glm::mat4 mat(1.0f);
	if (c)
	{
		auto o = c->orientation.load();
		mat = glm::lookAt(o.position, o.target(), c->axes.up);
	}

	Program::current_program->setMat4(Program::current_program->metadata.view_mat_name, mat);
}
