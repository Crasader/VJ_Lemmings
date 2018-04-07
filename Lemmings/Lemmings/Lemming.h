#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "Texture.h"
#include "Text.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming {

public:
	Lemming();
	~Lemming();
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet, VariableTexture *map, VariableTexture *mask);
	
	void update(int deltaTime);
	
	void render();
	
	
	void changeState(int x);
	void setMapMask(VariableTexture *mapMap, VariableTexture *mapMask);
	void doubleSpeed();
	void resetSpeed();
	void pause();
	void goKill();
	void goExit();
	
	glm::vec2 getPosition();


	/* Lemming status. Dead and Exited aren't rendered */
	enum LemmingStatus {
		ALIVE_STATUS,	/* Still alive				*/
		EXITED_STATUS,	/* Have been saved			*/
		DEAD_STATUS		/* Have died				*/
	};
	LemmingStatus getStatus();

	/* All lemming's posible states */
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
		BUILDER_STOP_STATE,

		FLOATER_RIGHT_STATE, FLOATER_LEFT_STATE,
		FLOATER_TRIGGERED,

		BOMBER_RIGHT_STATE, BOMBER_LEFT_STATE,
		BOMBER_TRIGGERED,

		EXITING_STATE,

		DYING_EXPLOSION_STATE, DYING_EXPLOSION_TRIGGERED,

		DYING_FALL_STATE,
		COUNTDOWN,
		DIE_EXPLIDING
	};
	LemmingState getState();
	LemmingState getNextState();

	
private:
	int collisionFloor(int maxFall);
	int collisionRight(int maxWall);
	int collisionLeft(int maxWall);

	bool collisionWalk();
	bool collisionFullWall();
	bool collisionHeadRight();
	bool collisionHeadLeft();
	void setAnimations();

	void move(int x, int y);

	void dig();
	void bashLeft(int q);
	void bashRight(int q);
	void build(glm::vec2 offset);
	void block();

	/* change Animation and states */
	void goDieExplosion();
	void goDieFall();
	void goBlocker();
	void goDigger(LemmingState oldNewState);
	void goFloaterRight();
	void goFloaterLeft();
	void goBasherRight();
	void goBasherLeft();
	void goBuilderRight();
	void goBuilderLeft();
	void goStopBuilderRight();
	void goStopBuilderLeft();
	void goClimberRight();
	void goClimberLeft();
	void goWalkLeft();
	void goWalkRight();
	void goFallLeft();
	void goFallRight();

	void resetActionTime();
	
private:
	
	/* Lemming animations, currently 23 */
	enum LemmingAnims {	
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
		DIE_FALL,
		OPENING_UMBRELLA_RIGHT, FLOATER_RIGHT,
		OPENING_UMBRELLA_LEFT, FLOATER_LEFT
	};

	LemmingState 
		oldState,	/* either walking_right or walking_left. only indicates direction	*/
		state,		/* actual state														*/
		nextState;	/* next state. only triggers 										*/
	Sprite *sprite;	
	VariableTexture *map;	/* needed for builders, they put the stairs directly in the map */
	VariableTexture *mask;	/* bitmask for collisions and stuff */
	LemmingStatus status;	/* current status */
	int actionTime;			/* time doing action */
	int counter;			/* builder counter */
	int elapsedTime;
	int timeToDisplay;
	Text countdown;

};


#endif // _LEMMING_INCLUDE


