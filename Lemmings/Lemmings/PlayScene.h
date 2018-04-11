#ifndef _PLAYSCENE_INCLUDE
#define _PLAYSCENE_INCLUDE

#include "EntityManager.h"
#include "InterfazUsuario.h"
#include "Cursor.h"
#include "TextProcessor.h"
#include "MaskedTexturedQuad.h"

#include "EndScene.h"

class InterfazUsuario;

class PlayScene : public Scene {
public:
	PlayScene(string levelPath);
	~PlayScene();
	void init();
	void update(int deltaTime);
	Scene* changeState();
	void render();

private:
	bool effectForLemming(int mouseX, int mouseY, EntityManager::Effect effect);
	void setGUI();
	void getLevelData();
	void createMap();
	void initShaders();

private:
	enum PlayState {
		ON,
		EXIT_CHOSEN,
		END
	};
	PlayState state;

	VariableTexture colorTexture, maskTexture;
	MaskedTexturedQuad *map;
	InterfazUsuario *gui;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	
	string path;
	float currentTime,maxTime, timeLeft;
	glm::mat4 projection;
	

	float cameraX, cameraY;
	EntityManager *manager;
	int buttonPressed;
	int count;
	bool armageddon;
	

	int numDiggers, numBlockers, numBashers, numFloaters, numBombers, numClimbers, numBuilders;
	
	
};

#endif

