#pragma once

#include <objidl.h>
#include <gdiplus.h>
#include "Obj.h"
using namespace Gdiplus;
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

class Painter {
  private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;
	static bool compareByZ(obj::GVertices &a, obj::GVertices &b);
  public:
	Obj *object;
	int w = -1, h = -1;
	void init();
	void paint(HDC hdc);
	void close();
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateObjectY(Obj &o, float angle);
	static float meanZ(obj::GVertices &v);
};
