#ifndef TRIANGLE_H
#define TRIANGLE_H

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

class Triangle {
public:
	Triangle(float size) {
		buildVertices(size);
		buildIndices();
	}

	~Triangle() {
		vertices.clear();
		indices.clear();
	}

	vector<Vertex>       vertices;
	vector<unsigned int> indices;

private:

	void buildVertices(float size) {

		/*Vertex v1, v2, v3;

		v1.Position.x = 0.0f; v1.Position.y = 0.0f; v1.Position.z = 0.0f;
		v2.Position.x = 1.0f; v2.Position.y = 0.0f; v2.Position.z = 0.0f;
		v3.Position.x = 0.5f; v3.Position.y = 1.0f; v3.Position.z = 0.0f;

		v1.Color.x = 1.0f; v1.Color.y = 0.0f; v1.Color.z = 0.0f;
		v2.Color.x = 0.0f; v2.Color.y = 1.0f; v2.Color.z = 0.0f;
		v3.Color.x = 0.0f; v3.Color.y = 0.0f; v3.Color.z = 1.0f;

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);*/

		Vertex v0;
		v0.Position.x = 0.0f; v0.Position.y = 0.0f; v0.Position.z = 0.0f;
		v0.Color.x = 1.0f; v0.Color.y = 0.0f; v0.Color.z = 0.0f;
		vertices.push_back(v0);

		for (int theta = 45; theta <= 405; theta += 90) {
			Vertex  v;
			double rdeg = glm::radians((double)theta);
			v.Position.x = 1.0f * (float)cos(rdeg);
			v.Position.y = 1.0f * (float)sin(rdeg);
			v.Position.z = 0.0f;
			if (theta == 45) { v.Color.r = 0.0f; v.Color.g = 1.0f; v.Color.b = 1.0f; }
			if (theta == 72) { v.Color.r = 1.0f; v.Color.g = 0.0f; v.Color.b = 1.0f; }
			if (theta == 144) { v.Color.r = 0.0f; v.Color.g = 0.0f; v.Color.b = 1.0f; }
			if (theta == 216) { v.Color.r = 0.0f; v.Color.g = 1.0f; v.Color.b = 1.0f; }
			if (theta == 288) { v.Color.r = 1.0f; v.Color.g = 1.0f; v.Color.b = 0.0f; }
			v.Color.a = 1.0f;
			vertices.push_back(v);
		}
	}

	void buildIndices() {
		/*indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);*/
		for ( int i = 0; i < 6; i++ )
			indices.push_back(i);
			
	}
};

#endif