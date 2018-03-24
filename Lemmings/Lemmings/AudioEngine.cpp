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
	music->drop();
	engineEffects->drop();
	engineMusic->drop();
	
}

void AudioEngine::playMusic(char *url)
{
	if (!music){
		music = engineMusic->play2D(url, true, false, true);
		music->setVolume(0.2);
	}
}	
void AudioEngine::buttonEffect() {
	engineEffects->play2D("Music/button.wav", false);
}

void AudioEngine::exitGameEffect() {
	engineEffects->play2D("Music/bye.mp3", false);
}
void AudioEngine::init() {
	engineMusic = createIrrKlangDevice();
	engineEffects = createIrrKlangDevice();
}