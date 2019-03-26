#pragma once

#include "../window.h"
#include <GLCore/base/gl_math_defs.h>

// Roll is not implemented.
struct CameraOrientation
{
	glm::vec3 position;
	glm::vec3 direction;
	float roll;

	float focal_dist;

	glm::vec3 target();
};

struct AxisData
{
	glm::vec3 up;
	glm::vec3 right;
};

class Camera {
public:
	Camera(CameraOrientation& o);
	void start();
	void stop();

	CameraOrientation orientation;
	void setDirection(float yaw, float pitch);
	void setDirection(glm::vec3& target);
	void setPosition(glm::vec3& pos);
	void setRoll(float roll);

	// Mostly to be used by flush() but can be used manually.
	void recompute_if_dirty();

	static void flush();
	static Camera* current_camera;

private:
	AxisData axes;
	void recalculateAxes();

	glm::mat4 viewMatrix;

	static void setViewMatrix(Camera* c);

	// Contains the previous orientation
	CameraOrientation previous_orientation;
};

namespace Directions {
	const glm::vec3 UP(0, 1, 0);
	const glm::vec3 DOWN(0, -1, 0);

	const glm::vec3 LEFT(-1, 0, 0);
	const glm::vec3 RIGHT(1, 0, 0);

	const glm::vec3 INTO_SCREEN(0, 0, -1);
	const glm::vec3 OUT_OF_SCREEN(0, 0, 1);
}