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
	void exitGameEffect();
	void buttonEffect();
private:
	
	void init();
	
};

