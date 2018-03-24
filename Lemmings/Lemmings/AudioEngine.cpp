#include "AudioEngine.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;





AudioEngine::AudioEngine()
{
}


AudioEngine::~AudioEngine()
{
}

void AudioEngine::playMusic()
{
	ISoundEngine* engine = createIrrKlangDevice();
	engine->play2D("Music/lemmings.wav", true);


}
