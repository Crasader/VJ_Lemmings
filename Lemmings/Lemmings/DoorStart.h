#pragma once

#include "Sprite.h"
#include "VariableTexture.h"
#include "Texture.h"

class DoorStart {

public:
	DoorStart(int color);
	~DoorStart();
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet);
	void update(int deltaTime);
	void render();
	glm::vec2 getPosition();

	enum StartColors {
		BROWN,
		GRAY
	};

private:
	Sprite* sprite;

	enum StartAnims {
		CLOSED,
		OPENING,
		OPEN
	};

	float ySpriteSheet;


	void setAnimations();
};

