#pragma once

#include <GLCore/base/gl_math_defs.h>
#include "object.h"

struct Orientaion
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = {1, 1, 1};
};

class Instance {
public:
	Instance(Object* obj, Orientaion o = {});
	void render();
	Orientaion orientation;
	
private:
	Object* obj;
	glm::mat4 calculateModelMat();
};