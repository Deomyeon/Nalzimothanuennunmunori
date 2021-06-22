#pragma once

#include "Screen.h"
#include "Sprite.h"
#include "Object.h"
#include "MouseInput.h"
#include "Animator.h"
#include "Sound.h"
#include "Enemy.h"


class Game
{
public:
	Screen screen;
	std::map<std::string, Animator> animators;
	MouseInput mouseInput;
	Sound sound;

	bool gameEnd;

	std::map<std::string, int> publicMemory;


	Game();
	~Game();

	void Start();

	void Play();
};


void TitleScene(Game* game);

void MainScene(Game* game);

void LoadingScene(Game* game);