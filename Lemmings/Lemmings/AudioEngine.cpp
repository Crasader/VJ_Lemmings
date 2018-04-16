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
	music->setVolume(0.1f);
}	

void AudioEngine::stopMusic() {
	engineMusic->stopAllSounds();
}

void AudioEngine::buttonEffect() {
	effect = engineEffects->play2D("Music/button.wav", false, false, true);
	effect->setVolume(0.5f);
}

void AudioEngine::diggEffect()
{

	effect = engineEffects->play2D("Music/dig.mp3",false,false,true);
	effect->setVolume(0.15f);
}

void AudioEngine::yippee() {
	effect = engineEffects->play2D("Music/YIPPEE.wav", false, false, true);
	effect->setVolume(0.2f);
}

void AudioEngine::stopEffect()
{
	engineEffects->stopAllSounds();
}

void AudioEngine::exitGameEffect() {
	effect = engineEffects->play2D("Music/bye.mp3", false,false, true);
	effect->setVolume(0.2f);
}

void AudioEngine::init() {
	engineMusic = createIrrKlangDevice();
	engineEffects = createIrrKlangDevice();
}

void AudioEngine::explosionEffect() {
	effect = engineEffects->play2D("Music/Explosion.mp3", false, false, true);
	effect->setVolume(0.15f);
}

void AudioEngine::splatEffect() {
	effect = engineEffects->play2D("Music/FALLSPLAT.wav", false, false, true);
	effect->setVolume(0.15f);
}

void AudioEngine::umbrellaEffect() {
	effect = engineEffects->play2D("Music/PLOP.wav", false, false, true);
	effect->setVolume(0.15f);
}

void AudioEngine::loseEffect() {
	effect = engineEffects->play2D("Music/fail.mp3", false, false, true);
	effect->setVolume(0.15f);
}

void AudioEngine::openDoor() {
	effect = engineEffects->play2D("Music/DOOR.wav", false, false, true);
	effect->setVolume(0.15f);
}

void AudioEngine::enterDoorEffect() {
	effect = engineEffects->play2D("Music/OING.wav", false, false, true);
	effect->setVolume(0.15f);
}

void AudioEngine::portalEffect() {
	effect = engineEffects->play2D("Music/portal.mp3", false, false, true);
	
}
void AudioEngine::orderLemmingEffect() {
	effect = engineEffects->play2D("Music/CHANGEOP.wav", false, false, true);
	effect->setVolume(0.5f);
 }