#include "AudioManager.h"
#include <irrKlang.h>
 
void* AudioManager::mSoundEngine;
AudioManager::AudioManager()
{
	this->mSoundEngine = irrklang::createIrrKlangDevice();
}

AudioManager::~AudioManager()
{
}

void AudioManager::Play2DSound(const char* aPath)
{	 
	irrklang::ISoundEngine * sound = static_cast<irrklang::ISoundEngine*>(mSoundEngine);
	
	sound->play2D(aPath,true);
	
}

void AudioManager::Play3DSound(const char*aPath)
{
	irrklang::ISoundEngine * sound = static_cast<irrklang::ISoundEngine*>(mSoundEngine);
	//mSoundEngine->play3D(aPath,);
}
