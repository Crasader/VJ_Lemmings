#pragma once
#include "Sprite.h"
#include "VariableTexture.h"
#include "Texture.h"

class DoorEnd {

public:
	DoorEnd(int color);
	~DoorEnd();
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet);
	void update(int deltaTime);
	void render();
	glm::vec2 getPosition();

	enum EndColors {
		BLACK,
		BROWN,
		PINK,
		GRAY
	};


private:
	Sprite * sprite;

	float ySpriteSheet;
};

