#pragma once

#include "../window.h"
#include <GLCore/base/gl_math_defs.h>

struct AxisData
{
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 front() const;
};

struct CameraOrientation
{
	glm::vec3 position;
	AxisData axes;

	float focal_dist;

	glm::vec3 target() const;
};

class Camera {
public:
	Camera(CameraOrientation& o);
	void start();
	void stop();

	CameraOrientation orientation;
	
	// Yaw, pitch by a certain amount in degrees. Incremental.
	// +ve yaw: towards left of axes.front()
	//   OR anticlockwise along axes.up (right hand thumb rule)
	// +ve pitch: upwards ( axes.up --> see Camera::axes)
	//   OR anticlockwise along axes.right (right hand thumb rule)
	// +ve roll: Anticlockwise along orientation.direction (right hand thumb rule)       
	void ypr(float yaw, float pitch, float roll);
	// Set a target to look at.
	void lookAt(glm::vec3& target);

	// Mostly to be used by flush() but can be used manually.
	void recompute();

	static void flush();
	static Camera* current_camera;

	// Move methods
	void rmoveFD(float amt = FDBK);
	void rmoveBK(float amt = -FDBK);
	void rmoveRT(float amt = LTRT);
	void rmoveLT(float amt = -LTRT);
	void rmoveUP(float amt = UPDN);
	void rmoveDN(float amt = -UPDN);
	void rturnRT(float amt = YAWM);
	void rturnLT(float amt = -YAWM);

private:
	glm::mat4 viewMatrix;

	static void setViewMatrix(Camera* c);

	/// Default movement weights

	constexpr static float UPDN = 0.05f;
	constexpr static float LTRT = 0.075f;
	constexpr static float FDBK = 0.075f;

	// This should always be -ve for directions to make sense.
	constexpr static float YAWM = -1.0f;
};

namespace Directions {
	const glm::vec3 UP(0, 1, 0);
	const glm::vec3 DOWN(0, -1, 0);

	const glm::vec3 LEFT(-1, 0, 0);
	const glm::vec3 RIGHT(1, 0, 0);

	const glm::vec3 INTO_SCREEN(0, 0, -1);
	const glm::vec3 OUT_OF_SCREEN(0, 0, 1);
}