#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include <iostream>
#include <vector>

const GLint FIREWORK_PARTICLES = 30;
class Firework
{
public:
	Firework();
	~Firework();
	void blowUp(glm::vec2 lemmingPos);
	void render();
private:
	bool hasExploded;
	GLfloat particleSize;
	GLfloat red;
	GLfloat blue;
	GLfloat green;
	GLfloat alpha;
	static const GLfloat GRAVITY;
	glm::vec2 lemmingPos;
	GLfloat x[FIREWORK_PARTICLES];
	GLfloat y[FIREWORK_PARTICLES];
	GLfloat xSpeed[FIREWORK_PARTICLES];
	GLfloat ySpeed[FIREWORK_PARTICLES];
	ShaderProgram program;
	vector<Quad*> particles;
	void explode();
	void initShader();
	glm::mat4 projection;
	void init(glm::vec2 lemmingPos);
};

