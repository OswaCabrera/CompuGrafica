#ifndef PIRAMIDE_H
#define PIRAMIDE_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vertex.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;

class Piramide {
public:
	Piramide() {
		buildVertices();
		buildIndices();
	}

	~Piramide() {
		vertices.clear();
		indices.clear();
	}

	vector<Vertex>       vertices;
	vector<unsigned int> indices;

private:

	void buildVertices() {
		Vertex  v;
		v.Position.x = 1.0f; v.Position.y = 1.0f; v.Position.z = 0.0f;
		v.Color.r = 0.0f; v.Color.g = 1.0f; v.Color.b = 1.0f; v.Color.a = 1.0f;
		vertices.push_back(v);

		v.Position.x = -1.0f; v.Position.y = 1.0f; v.Position.z = 0.0f;
		v.Color.r = 1.0f; v.Color.g = 0.0f; v.Color.b = 0.0f; v.Color.a = 1.0f;
		vertices.push_back(v);

		v.Position.x = -1.0f; v.Position.y = -1.0f; v.Position.z = 0.0f;
		v.Color.r = 1.0f; v.Color.g = 1.0f; v.Color.b = 0.0f; v.Color.a = 1.0f;
		vertices.push_back(v);

		v.Position.x = 1.0f; v.Position.y = -1.0f; v.Position.z = 0.0f;
		v.Color.r = 1.0f; v.Color.g = 0.0f; v.Color.b = 1.0f; v.Color.a = 1.0f;
		vertices.push_back(v);

		v.Position.x = 0.0f; v.Position.y = 0.0f; v.Position.z = 2.0f;
		v.Color.r = 1.0f; v.Color.g = 1.0f; v.Color.b = 1.0f; v.Color.a = 1.0f;
		vertices.push_back(v);

		/*for (int theta = 0; theta < 360; theta += 120) {
			Vertex  v;
			double rdeg = glm::radians((double)theta);
			v.Position.x = size * (float)cos(rdeg);
			v.Position.y = size * (float)sin(rdeg);
			v.Position.z = 0.0f;
			if (theta == 0) { v.Color.r = 0.0f; v.Color.g = 1.0f; v.Color.b = 1.0f; }
			if (theta == 120) { v.Color.r = 1.0f; v.Color.g = 0.0f; v.Color.b = 1.0f; }
			if (theta == 240) { v.Color.r = 0.0f; v.Color.g = 0.0f; v.Color.b = 1.0f; }
			v.Color.a = 1.0f;
			vertices.push_back(v);
		}*/
	}

	void buildIndices() {
		indices.push_back(0); indices.push_back(3); indices.push_back(4);
		indices.push_back(0); indices.push_back(1); indices.push_back(4);
		indices.push_back(1); indices.push_back(2); indices.push_back(4);
		indices.push_back(2); indices.push_back(3); indices.push_back(4);
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
	}
};

#endif