#pragma once


#include <Windows.h>
#include <string>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


DWORD WINAPI SoundAsset(LPVOID param);

class Sound
{
public:

	std::string path;
	std::string mode;

	bool stop;

	Sound();
	~Sound();

	inline void PlaySoundAsset(std::string path, std::string mode)
	{
		this->path = path;
		this->mode = mode;

		HANDLE hThread = CreateThread(NULL, 0, SoundAsset, this, NULL, 0);
	}

	inline void StopSoundAsset()
	{
		this->stop = true;
	}
};

