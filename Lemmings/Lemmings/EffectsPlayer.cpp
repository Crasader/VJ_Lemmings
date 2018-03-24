#include "EffectsPlayer.h"
#include <iostream>
#include <exception>



EffectsPlayer::EffectsPlayer()
{
}


EffectsPlayer::~EffectsPlayer()
{
}

void EffectsPlayer::playSound()
{
	try {
		PlaySound(TEXT("Music/lemmigs.wav"), NULL, SND_ASYNC);
	}
	catch(int e){
		std::cout << "Can't load audio file," << std::endl;
	}
	
}
