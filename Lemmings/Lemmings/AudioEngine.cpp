#include "AudioEngine.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <conio.h>





AudioEngine::AudioEngine()
{
}


AudioEngine::~AudioEngine()
{
}

void AudioEngine::playMusic()
{
	
	PlaySound(TEXT("Music/lemmings.wav"), NULL, SND_ASYNC);

}
