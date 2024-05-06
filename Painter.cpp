#include "Painter.h"
#include "Obj.h"
#include <algorithm>
#include <vector>

bool Painter::compareByZ(obj::GVertices &a, obj::GVertices &b)
{
	float fa = Painter::meanZ(a);
	float fb = Painter::meanZ(b);

	return fa < fb;
}

void Painter::init()
{
	object = new Obj("./obj/lpbull.obj");
	projectionMatrix = glm::perspective(glm::radians(45.0f), /*(float)w / (float)h*/ 1.0f, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(glm::vec3(0, -2, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(1, 0, 0));
}

void Painter::paint(HDC hdc)
{
	if (w < 0 || h < 0) return;

	Graphics graphics(hdc);
	graphics.Clear(Color(255, 255, 255));
	Pen pen(Color(255, 0, 0, 0));
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	glm::vec4 vertexWorldPos;
	glm::vec4 glPosition;
	std::map<std::string, Object *>::iterator it;
	static int countFace = 0;
	std::vector<obj::GVertices> verticesList;
	for (it = object->objects.begin(); it != object->objects.end(); it++) {

		for (size_t i = 0; i < it->second->faces.size(); i++) {
			Face *f = it->second->faces[i];
			int sz = (int)f->vertices.size();
			countFace++;
			obj::GVertex *vertices = new obj::GVertex[sz];

			for (int j = 0; j < sz; j++) {
				Vertex *v = f->vertices[j];
				vertexWorldPos.x = v->pos.x;
				vertexWorldPos.y = v->pos.y;
				vertexWorldPos.z = v->pos.z;
				vertexWorldPos.w = 1;

				// read from right to left
				glPosition = projectionMatrix * viewMatrix * modelMatrix * vertexWorldPos;
				glPosition /= glPosition.w;

				vertices[j].x = (float)MIN(w - 1, (glPosition.x + 1) * 0.5 * w);
				vertices[j].y = (float)MIN(h - 1, (glPosition.y /** 1.333*/ + 1) * 0.5 * h);
				vertices[j].z = (float)vertexWorldPos.z;
			}

			obj::GVertices gv = {vertices, (size_t)sz};
			verticesList.push_back(gv);
		}
	}

	
	std::sort(verticesList.begin(), verticesList.end(), compareByZ);
	for (auto v = verticesList.begin(); v != verticesList.end(); v++) {
		obj::GVertices gv = *v;
		Point *verticeArray = new Point[gv.sz];
		for (int i = 0; i < gv.sz; i++) {
			verticeArray[i] = Point((int)gv.vertices[i].x, (int)gv.vertices[i].y);
		}

		HatchBrush myHatchBrush(Gdiplus::HatchStyleLightDownwardDiagonal, Gdiplus::Color::Black, Gdiplus::Color::White);
		graphics.FillPolygon(&myHatchBrush, verticeArray, (int)gv.sz);
		graphics.DrawPolygon(&pen, verticeArray, (int)gv.sz);
	}
}

void Painter::close()
{
	delete object;
}

void Painter::rotateX(float angle)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1, 0, 0));
}

void Painter::rotateY(float angle)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0, 1, 0));
}

void Painter::rotateObjectY(Obj &o, float angle)
{
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	glm::vec4 v;
	for (size_t i = 0; i < o.vertices.size(); i++) {
		Vertex *vertex = o.vertices[i];
		v.x = vertex->pos.x;
		v.y = vertex->pos.y;
		v.z = vertex->pos.z;
		v = rotation * v;
		vertex->pos.x = v.x;
		vertex->pos.y = v.y;
		vertex->pos.z = v.z;
	}

	std::map<std::string, Object *>::iterator it;
	for (it = o.objects.begin(); it != o.objects.end(); it++) {
		for (size_t i = 0; i < it->second->faces.size(); i++) {
			Face *face = it->second->faces[i];
			for (size_t j = 0; j < face->normals.size(); j++) {
				v.x = face->normals[j].x;
				v.y = face->normals[j].y;
				v.z = face->normals[j].z;
				v = rotation * v;
				face->normals[j].x = v.x;
				face->normals[j].y = v.y;
				face->normals[j].z = v.z;
			}
		}
	}
}

float Painter::meanZ(obj::GVertices &v)
{
	float sum = 0;
	for (int i = 0; i < v.sz; i++) {
		sum += v.vertices[i].z;
	}
	return sum / v.sz;
}
