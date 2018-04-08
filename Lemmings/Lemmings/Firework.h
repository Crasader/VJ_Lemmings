#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>

const GLint FIREWORK_PARTICLES = 70;
class Firework
{
public:
	Firework();
	~Firework();
	void init(glm::vec2 lemmingPos);
	void blowUp();
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

	void explode();
};

