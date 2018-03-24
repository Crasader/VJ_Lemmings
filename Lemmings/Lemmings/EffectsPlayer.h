#pragma once
#include <Windows.h>
#include <MMSystem.h>
class EffectsPlayer
{
public:
	static EffectsPlayer &instance()
	{
		static EffectsPlayer G;

		return G;
	}
	EffectsPlayer();
	~EffectsPlayer();
	void playSound();
};

