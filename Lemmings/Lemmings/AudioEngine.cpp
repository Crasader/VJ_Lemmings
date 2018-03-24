#include "AudioEngine.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;



ISoundEngine* engineMusic;
ISoundEngine* engineEffects;
ISound* music;

AudioEngine::AudioEngine()
{
	init();
}


AudioEngine::~AudioEngine()
{
	
}

void AudioEngine::playMusic()
{
	if (!music){
		music = engineMusic->play2D("Music/lemmings.wav", true, false, true);
		music->setVolume(0.5);
	}
	
	
}	
void AudioEngine::buttonEffect() {
	engineEffects->play2D("Music/button.wav", false);
}
void AudioEngine::init() {
	engineMusic = createIrrKlangDevice();
	engineEffects = createIrrKlangDevice();
}