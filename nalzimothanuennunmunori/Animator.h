#pragma once


#include <vector>
#include <string>
#include <map>
#include <time.h>
#include <fstream>
#include <sstream>
#include "Sprite.h"


std::vector<std::string> DivideString(std::string str);
DWORD WINAPI CountTime(LPVOID param);


struct Animation // ��������Ʈ�� ��� ������� �ִϸ��̼� ��ü
{
public:
	std::string name; // �ִϸ��̼��� �̸�
	std::vector<Sprite> sprites; // ��������Ʈ�� ���� ����
	std::vector<double> durations; // ���ӽð��� ���� ����

	Animation() : name(""), sprites(std::vector<Sprite>()), durations(std::vector<double>())
	{
		
	}
};

class Animator
{
private:
public:
	std::map<std::string, Animation> animations; // �ִϸ��̼ǵ��� ����
	double time; // �ִϸ��̼ǿ� �����Ǵ� �ð�
	clock_t clck; //Ŭ���̿�
	int index; // �ִϸ��̼��� �ε���

	bool stop;

	std::string animationName; // ���� ������� �ִϸ��̼��� �̸�

	Animator();
	~Animator();


	inline void StartAnimator()
	{
		HANDLE hThread = CreateThread(NULL, 0, CountTime, this, NULL, 0);
	}

	inline void StopAnimator()
	{
		this->stop = true;
	}

	inline bool SetAnimation(std::string name)
	{
		if (this->animations.find(name) != this->animations.end())
		{
			this->animationName = name;
			return true;
		}
		return false;
	}

	inline Sprite* GetAnimation()
	{
		while (this->time >= this->animations[this->animationName].durations[this->index])
		{
			this->time -= this->animations[this->animationName].durations[this->index];
			this->index = (this->index + 1) % this->animations[this->animationName].durations.size();
		}
		if (this->animations[this->animationName].sprites.size() <= index)
			return NULL;
		return &this->animations[this->animationName].sprites[index];
	}

	inline void LoadAnimations(std::vector<std::string> paths)
	{
		std::ifstream file;
		for (int p = 0; p < paths.size(); p++)
		{
			file.open(paths[p], std::ios::in | std::ios::binary);
			if (file.is_open())
			{
				while (!file.eof())
				{
					std::string s;
					std::getline(file, s);
					std::vector<std::string> vec = DivideString(s);

					std::string name = vec[0];
					int count = std::stoi(vec[1]);
					for (int i = 0; i < count; i++)
					{
						std::getline(file, s);
						vec = DivideString(s);
						std::string path = vec[0];
						double duration = std::stod(vec[1]);
						Sprite sprite(path);
						sprite.InitSprite();
						this->animations[name].name = name;
						this->animations[name].sprites.emplace_back(sprite);
						this->animations[name].durations.emplace_back(duration);
					}
				}
				file.close();
			}
		}
	}

};
