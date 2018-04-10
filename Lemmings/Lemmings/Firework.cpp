#include "Firework.h"
#include "common_defs.h"


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
	this->lemmingPos = lemmingPos;
	float xSpeedVal = -2 + ((float)rand() / (float)RAND_MAX) * 4.0f;
	float ySpeedVal = 4.0f + ((float)rand() / (float)RAND_MAX) * 4.0f;
	red = ((float)rand() / (float)RAND_MAX);
	green = ((float)rand() / (float)RAND_MAX);
	blue = ((float)rand() / (float)RAND_MAX);
	alpha = 1.0f;
	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{
		x[loop] =lemmingPos.x;
		y[loop] = lemmingPos.y; // Push the particle location down off the bottom of the screen
		xSpeed[loop] = xSpeedVal;
		ySpeed[loop] = ySpeedVal;
		particles.push_back(Quad::createQuad(180, 180, 20, 20 , program));
	}
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;
	
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

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
		glm::mat4 modelview;
		
		for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
		{
			program.use();
			modelview = glm::mat4(1.0f);
			program.setUniformMatrix4f("projection", projection);
			program.setUniform4f("color", 1, 1, 1, 1);
			//modelview = glm::translate(modelview, glm::vec3(x[particleLoop], y[particleLoop], 0));
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
