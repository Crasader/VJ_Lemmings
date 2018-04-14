#include "Firework.h"
#include "common_defs.h"
#include <ctime>


const GLfloat Firework::GRAVITY = 0.05f;

Firework::Firework()
{
	hasExploded = false;
}

Firework::~Firework()
{
	hasExploded = false;
}

void Firework::init(glm::vec2 lemmingPos) {
	initShader();
	blue = (float)(rand()) / (float)(RAND_MAX);
	float xSpeedVal;
	float ySpeedVal;
	red = (float)(rand()) / (float)(RAND_MAX);
	alpha = 1.0f;
	green = (float)(rand()) / (float)(RAND_MAX);
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX);
	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{	
		xSpeedVal = -1 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2;
		ySpeedVal = -2 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2;
		x[loop] =lemmingPos.x;
		y[loop] = lemmingPos.y; // Push the particle location down off the bottom of the screen
		xSpeed[loop] = xSpeedVal;
		ySpeed[loop] = ySpeedVal;
		particles.push_back(Quad::createQuad(0, 0, particleSize, particleSize , program));
	}
	
	
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

}

void Firework::blowUp(glm::vec2 lemmingPos)
{
	hasExploded = true;
	init(lemmingPos);
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
		alpha -= 0.005f;
	}
}

void Firework::render() {
	if (hasExploded) {
		glm::mat4 modelview;
		
		for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
		{
			program.use();
			program.setUniformMatrix4f("projection", projection);
			program.setUniform4f("color", red, green, blue, alpha);
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(x[particleLoop], y[particleLoop], 0.f));
			program.setUniformMatrix4f("modelview", modelview);
			particles[particleLoop]->render();
			
			
		}
		explode();

	}
}

void Firework::initShader() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if (!program.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
}
