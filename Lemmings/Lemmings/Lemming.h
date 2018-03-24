#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "Texture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	Lemming();
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet,VariableTexture *mask);
	void setAnimations();
	void update(int deltaTime);
	void render();
	
	
	void changeState(int x);
	void setMapMask(VariableTexture *mapMask);
	
	glm::vec2 getPosition();
	
private:
	int collisionFloor(int maxFall);
	bool collision();

	void move(float x, float y);

	

public:
	enum LemmingStatus {
		ALIVE_STATUS,	/* Siguen vivos				*/
		EXITED_STATUS,	/* Han finalizado el nivel	*/
		DEAD_STATUS		/* Han muerto				*/
	};
	
private:
	enum LemmingState {
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE,
		FALLING_LEFT_STATE, FALLING_RIGHT_STATE, 
		DIGGER_STATE, DIGGER_TRIGGERED
	};

	enum LemmingAnims {		// 19 ANIMATIONS
		WALKING_RIGHT, WALKING_LEFT,
		FALLING_RIGHT, FALLING_LEFT,
		BLOCKER,
		DIGGER,
		BASHER_RIGHT, BASHER_LEFT,
		CLIMBER_RIGHT, CLIMBER_LEFT,
		CLIMBER_TOP_RIGHT, CLIMBER_TOP_LEFT,
		BUILDER_RIGHT, BUILDER_LEFT,
		BUILDER_STOP_RIGHT, BUILDER_STOP_LEFT,
		EXIT,
		DIE_EXPLOSION,
		DIE_FALL
	};

	LemmingState oldState, state, nextState;
	//Texture spritesheet1;
	Sprite *sprite;
	VariableTexture *mask;

};


#endif // _LEMMING_INCLUDE


