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
	int collisionRight(int maxWall);
	int collisionLeft(int maxWall);

	bool collision();


	void move(float x, float y);

	void dig();
	void bashLeft(int q);
	void bashRight(int q);

	

public:
	enum LemmingStatus {
		ALIVE_STATUS,	/* Siguen vivos				*/
		EXITED_STATUS,	/* Han finalizado el nivel	*/
		DEAD_STATUS		/* Han muerto				*/
	};
	
private:
	enum LemmingState {
		WALKING_RIGHT_STATE, WALKING_LEFT_STATE,

		FALLING_RIGHT_STATE, FALLING_LEFT_STATE,

		BLOCKER_STATE, BLOCKER_TRIGGERED,

		DIGGER_STATE, DIGGER_TRIGGERED,

		BASHER_RIGHT_STATE, BASHER_LEFT_STATE,
		BASHER_TRIGGERED,

		CLIMBER_RIGHT_STATE, CLIMBER_LEFT_STATE,
		CLIMBER_TRIGGERED,
		CLIMBER_TOP_STATE,

		BUILDER_RIGHT_STATE, BUILDER_LEFT_STATE,
		BUILDER_TRIGGERED,

		EXITING_STATE,

		DYING_EXPLOSION_STATE, DYING_EXPLOSION_TRIGGERED,

		DYING_FALL_STATE
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
		EXITING,
		DIE_EXPLOSION,
		DIE_FALL
	};

	LemmingState oldState, state, nextState;
	//Texture spritesheet1;
	Sprite *sprite;
	VariableTexture *mask;

	int basher_time, dig_time;

};


#endif // _LEMMING_INCLUDE


