#include "Firework.h"


const GLfloat Firework::GRAVITY = 0.05f;

Firework::Firework()
{
	hasExploded = false;
}

Firework::~Firework()
{
}

void Firework::init(glm::vec2 lemmingPos) {
	this->lemmingPos = lemmingPos;
	float xSpeedVal = -2 + ((float)rand() / (float)RAND_MAX) * 4.0f;
	float ySpeedVal = -4.0f + ((float)rand() / (float)RAND_MAX) * -4.0f;
	red = ((float)rand() / (float)RAND_MAX);
	green = ((float)rand() / (float)RAND_MAX);
	blue = ((float)rand() / (float)RAND_MAX);
	alpha = 1.0f;
	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{
		x[loop] =lemmingPos.x;
		y[loop] = 610.0f; // Push the particle location down off the bottom of the screen
		xSpeed[loop] = xSpeedVal;
		ySpeed[loop] = ySpeedVal;
	}
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;
	hasExploded = false;

}

void Firework::blowUp()
{
	hasExploded = true;
}

void Firework::explode()
{
	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{
		// Dampen the horizontal speed by 1% per frame
		xSpeed[loop] *= 0.99;

		// Move the particle
		x[loop] += xSpeed[loop];
		y[loop] += ySpeed[loop];

		// Apply gravity to the particle's speed
		ySpeed[loop] += Firework::GRAVITY;
	}

	// Fade out the particles (alpha is stored per firework, not per particle)
	if (alpha > 0.0f)
	{
		alpha -= 0.01f;
	}
}

void Firework::render() {
	if (hasExploded) {
		glAccum(GL_RETURN, 1.0f);

		// Clear the accumulation buffer (don't worry, we re-grab the screen into the accumulation buffer after drawing our current frame!)
		glClear(GL_ACCUM_BUFFER_BIT);

		// Set ModelView matrix mode and reset to the default identity matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Displacement trick for exact pixelisation
		glTranslatef(0.375, 0.375, 0);

		// Draw our fireworks

		for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
		{

			// Set the point size of the firework particles (this needs to be called BEFORE opening the glBegin(GL_POINTS) section!)
			glPointSize(particleSize);

			glBegin(GL_POINTS);
			// Set colour to yellow on the way up, then whatever colour firework should be when exploded
			glColor4f(red, green, blue, alpha);


			// Draw the point
			glVertex2f(x[particleLoop], y[particleLoop]);
			glEnd();
		}

		// Move the firework appropriately depending on its explosion state
		if (hasExploded == true)
		{
			explode();

		}
	}
}


