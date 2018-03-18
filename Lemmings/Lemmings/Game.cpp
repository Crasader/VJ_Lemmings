#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	menuObj.init();
	
}

bool Game::update(int deltaTime)
{
	menuObj.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	menuObj.render();

}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		closeGame();
	else {
		menuObj.keyPressed(key);
	}
	keys[key] = true;

	
}
void Game::closeGame() {
	bPlay = false;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	
	
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
	menuObj.specialKey(key);
	
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	menuObj.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		menuObj.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		menuObj.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





