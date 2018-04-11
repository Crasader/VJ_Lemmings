#pragma once
#include <string>
class AudioEngine
{
public:
	static AudioEngine &instance()
	{
		static AudioEngine G;

		return G;
	}
	AudioEngine();
	~AudioEngine();
	void playMusic(char * url);
	void stopMusic();
	void exitGameEffect();
	void buttonEffect();
	void diggEffect();
	void yippee();
	void stopEffect();
	void explosionEffect();
	void splatEffect();
	void umbrellaEffect();
	void loseEffect();
	void openDoor();
	void enterDoorEffect();
	void portalEffect();
	void orderLemmingEffect();
private:
	
	void init();

	
	
};

