#include "Sound.h"

Sound::Sound() : stop(false)
{

}

Sound::~Sound()
{

}


DWORD WINAPI SoundAsset(LPVOID param)
{
	Sound* sound = (Sound*)param;
	if (sound->mode == "LOOP")
	{
		PlaySoundA(sound->path.c_str(), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		while (!sound->stop);
	}
	else
	{
		PlaySoundA(sound->path.c_str(), 0, SND_FILENAME | SND_SYNC);
	}
	return 0;
}