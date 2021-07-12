#pragma once
#include <string>
using std::string;

 
class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	static void Play2DSound(const char* aPath);
	static void Play3DSound(const  char* aPath);
private:
	static void* mSoundEngine;
};

 