#include "AudioEngine.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <irrKlang.h>
#include <vector>
using namespace irrklang;



ISoundEngine* engineMusic;
ISoundEngine* engineEffects;
ISound* music;
ISound* effect;

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
	engineMusic->stopAllSounds();
	music = engineMusic->play2D(url, true, false, true);
	music->setVolume(0.1);
}	
void AudioEngine::buttonEffect() {
	effect = engineEffects->play2D("Music/button.wav", false, false, true);
	effect->setVolume(0.2);
}

void AudioEngine::diggEffect()
{

	effect = engineEffects->play2D("Music/dig.mp3",false,false,true);
	effect->setVolume(0.15);
}

void AudioEngine::yippee() {
	effect = engineEffects->play2D("YIPPEE.wav", false, false, true);
	effect->setVolume(0.2);
}

void AudioEngine::stopEffect()
{
	engineEffects->stopAllSounds();
}

void AudioEngine::exitGameEffect() {
	effect = engineEffects->play2D("Music/bye.mp3", false,false, true);
	effect->setVolume(0.2);
}
void AudioEngine::init() {
	engineMusic = createIrrKlangDevice();
	engineEffects = createIrrKlangDevice();
}