#ifndef CONE_H
#define CONE_H

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

class Cone {
public:
	Cone(float size) {
		buildVertices(size);
		buildIndices();
	}

	~Cone() {
		vertices.clear();
		indices.clear();
	}

	vector<Vertex>       vertices;
	vector<unsigned int> indices;

private:

	void buildVertices(float size) {

	}

	void buildIndices() {
		
	}
};

#endif