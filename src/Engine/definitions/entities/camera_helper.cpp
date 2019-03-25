#include "camera.h"

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

bool operator != (CameraOrientation a, CameraOrientation b)
{
	return !(a == b);
}

void (*Camera::camera_watcher)(Camera*) = [](Camera* cam)
{
	if (cam->orientation.load() != cam->previous_orientation.load())
	{
		cam->recalculateAxes();
		setViewMatrix(cam);
		cam->previous_orientation.store(cam->orientation);
	}
};