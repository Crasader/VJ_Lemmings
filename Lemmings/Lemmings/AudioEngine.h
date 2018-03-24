#pragma once
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
	void playMusic();
};

