#include "Game.h"
#include "Map.h"

Game::Game() :
	screen(Vector2(400, 400), "날지 못하는 눈 먼 오리", { Color::black, Color::white }),
	animators(),
	mouseInput(),
	sound(),
	gameEnd(false)
{

	this->screen.SetConsole();
	this->animators["player"].StartAnimator();
	this->mouseInput.StartMouseInput();
}

Game::~Game()
{

	this->mouseInput.StopMouseInput();
	this->animators["player"].StopAnimator();
	this->sound.StopSoundAsset();
	this->screen.ResetConsole();
}


void Game::Start()
{
	this->animators["player"].LoadAnimations(std::vector<std::string>{"Assets/player.animation"});
	this->publicMemory["duckHp"] = 10;
	this->publicMemory["Stage"] = 0;
}


void Game::Play()
{
	if (!this->gameEnd)
		TitleScene(this);
	while (true)
	{
		if (!this->gameEnd)
			MainScene(this);
		if (!this->gameEnd)
			LoadingScene(this);
		if (!this->gameEnd)
			this->sound.StopSoundAsset();
		else
			break;
	}
}

void TitleScene(Game* game)
{

	Object titleObject("Assets/title/title.bmp", Vector2(200, 300));

	Object playButton("Assets/button/play_button.bmp", Vector2(200, 200), 0, Vector2(200, 80));

	Object quitButton("Assets/button/quit_button.bmp", Vector2(200, 100), 0, Vector2(200, 80));

	game->sound.PlaySoundAsset("Assets/sound/quack.wav", "LOOP");

	while (true)
	{
		titleObject.Draw(game->screen);
		playButton.Draw(game->screen);
		quitButton.Draw(game->screen);

		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
		{
			game->gameEnd = true;
			break;
		}

		if (game->mouseInput.GetLeft())
		{
			Vector2 mousePos = game->mouseInput.GetMousePos();
			if (playButton.Collision(mousePos, Vector2::one))
			{
				game->screen.DrawScreen();
				break;
			}
			if (quitButton.Collision(mousePos, Vector2::one))
			{
				game->gameEnd = true;
				break;
			}
		}

		game->screen.DrawScreen();
	}
	game->sound.PlaySoundAsset("Assets/sound/attack.wav", "ONCE");
}

void MainScene(Game* game)
{
	const int tileDist = 32;
	const int mapSize = 21;
	char tileMap[mapSize][mapSize + 1] =
	{
		"111111112222211111111",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"200000000000000000002",
		"200000000000000000002",
		"200000000000000000002",
		"200000000000000000002",
		"200000000000000000002",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"111111112222211111111",
	};

	std::map<Vector2, Object> tiles;

	Sprite hpSprites[10] =
	{
		Sprite("Assets/hp/0.bmp"),
		Sprite("Assets/hp/1.bmp"),
		Sprite("Assets/hp/2.bmp"),
		Sprite("Assets/hp/3.bmp"),
		Sprite("Assets/hp/4.bmp"),
		Sprite("Assets/hp/5.bmp"),
		Sprite("Assets/hp/6.bmp"),
		Sprite("Assets/hp/7.bmp"),
		Sprite("Assets/hp/8.bmp"),
		Sprite("Assets/hp/9.bmp"),
	};

	for (int i = 0; i < 10; i++)
	{
		hpSprites[i].InitSprite();
	}

	Sprite tileSprites[3] =
	{
		Sprite("Assets/tile/tile.bmp"),
		Sprite("Assets/tile/wall.bmp"),
		Sprite("Assets/tile/road.bmp")
	};
	tileSprites[0].InitSprite();
	tileSprites[1].InitSprite();
	tileSprites[2].InitSprite();

	Map mapMaker;
	mapMaker.MakeMap();
	std::vector<Vector2> map = mapMaker.GetMaps();

	Object duck;
	duck.position = Vector2(game->screen.GetScreenSize() / 2);
	duck.rotation = 0;
	duck.size = Vector2(50, 50);

	std::vector<Enemy> enemys;
	std::vector<std::pair<Vector2, Object>> deadEnemys;
	Sprite deadEnemy("Assets/baquee/BaqueeDead.bmp");
	deadEnemy.InitSprite();


	int duckSpeed = 10;
	int duckDamage = 1;

	double duckHitTime = 0;
	double duckHitCoolTime = 1;

	std::vector<Object> duckChims;
	double chimTime = 0;
	const double chimCoolTime = 0.5;
	Sprite duckChim("Assets/projectile/projectile1.bmp");
	duckChim.InitSprite();

	Vector2 duckDestination(0, 0);
	Vector2 duckMapPos(0, 0);

	bool duckIdle = true;
	bool duckRot = true; // 오리가 취할 애니메이션의 방향 true은 Left false를 Right
	bool duckWalkRot = true;
	bool duckMove = true;

	game->animators["player"].SetAnimation("Left_Idle");

	Vector2 cameraPos(0, 0);
	Vector2 cameraTilePos(cameraPos / tileDist);
	int camSpeed = 8;

	Vector2 screenTileSize[2] =
	{
		Vector2(-(floor(game->screen.GetScreenSize().x / tileDist) + 2) / 2, (floor(game->screen.GetScreenSize().x / tileDist) + 2) / 2),
		Vector2(-(floor(game->screen.GetScreenSize().y / tileDist) + 2) / 2, (floor(game->screen.GetScreenSize().y / tileDist) + 2) / 2)
	};

	Object portal("Assets/tile/portal.bmp", Vector2::zero, 0, Vector2(64, 64));

	clock_t beginClock = clock();
	clock_t endClock = clock();


	while (true)
	{
		endClock = clock();
		double timeDistance = double(endClock - beginClock) / CLOCKS_PER_SEC;
		beginClock = endClock;

		chimTime += timeDistance;
		duckHitTime += timeDistance;

		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
		{
			game->gameEnd = true;
			break;
		}

		//오리 그리기, 조종

		if (GetAsyncKeyState((unsigned short)'Z') & 0x8000)
		{
			for (int i = 0; i < deadEnemys.size(); i++)
			{
				if (deadEnemys[i].second.Collision(duck.position, duck.size))
				{
					game->publicMemory["duckHp"]++;
					deadEnemys.erase(deadEnemys.begin() + i);
				}
			}
		}

		if (game->mouseInput.GetRight())
		{
			duckDestination = game->mouseInput.GetMousePos() - game->screen.GetScreenSize() / 2 + cameraPos;
		}


		bool duckHit = false;
		int hitDamage = 0;

		if (duckHitTime > duckHitCoolTime)
		{
			for (int i = 0; i < enemys.size(); i++)
			{
				if (enemys[i].object.Collision(duck.position, duck.size))
				{
					duckHit = true;
					hitDamage = enemys[i].damage;
				}
			}
		}

		if (duckHit)
		{
			game->publicMemory["duckHp"] -= hitDamage;
			game->sound.PlaySoundAsset("Assets/sound/attack.wav", "ONCE");
			duckHitTime = 0;
			if (game->publicMemory["duckHp"] <= 0)
			{
				game->gameEnd = true;
				return;
			}
		}
		else
		{
			// 왼쪽을 눌렀다면 총알을 생성하여 위치를 오리로 방향을 클릭한 지점으로 맞춤.
			if (game->mouseInput.GetLeft())
			{
				if (chimTime > chimCoolTime)
				{
					game->sound.PlaySoundAsset("Assets/sound/tue1.wav", "ONCE");
					Vector2 temp = game->mouseInput.GetMousePos();
					if ((game->mouseInput.GetMousePos() - game->screen.GetScreenSize() / 2 + cameraPos).x > duck.position.x)
					{
						duckRot = false;
					}
					else
					{
						duckRot = true;
					}
					Object chim;
					chim.position = duck.position;
					chim.rotation = 180 * atan2(-(temp.y - duck.position.y), temp.x - duck.position.x) / M_PI;
					chim.size = Vector2(16, 16);
					duckChims.push_back(chim);
					chimTime = 0;
				}
			}
		}

		Vector2 camDist(duckDestination - cameraPos);

		// 침이 있다면 향하는 방향으로 침 이동
		for (int i = 0; i < duckChims.size(); i++)
		{
			duckChims[i].position.x += cos(duckChims[i].rotation * M_PI / 180) * 20;
			duckChims[i].position.y += -sin(duckChims[i].rotation * M_PI / 180) * 20;

			if (duckChims[i].position.GetDistance(duck.position) > 40 * tileDist * mapSize)
			{
				duckChims.erase(duckChims.begin() + i);
				i--;
			}
		}

		// 오리 애니메이션
		if (duckIdle)
		{
			if (duckRot)
			{
				game->animators["player"].SetAnimation("Left_Idle");
			}
			else
			{
				game->animators["player"].SetAnimation("Right_Idle");
			}
		}
		else
		{
			if (duckWalkRot)
			{
				game->animators["player"].SetAnimation("Left_Walk");
				duckRot = true;
			}
			else
			{
				game->animators["player"].SetAnimation("Right_Walk");
				duckRot = false;
			}
		}
		duck.SetSprite(game->animators["player"].GetAnimation());

		cameraTilePos = cameraPos/ tileDist;


		bool duckCollisionX = false;
		bool duckCollisionY = false;

		bool mapMove = false;
		Vector2 mapMoveRot = Vector2::zero;
		Vector2 mapMoveTile = Vector2::zero;

		for (int i = 0; i < enemys.size(); i++)
		{
			enemys[i].SetEnemyDestination(cameraTilePos);

		}
		Vector2 enemyDestination(duck.position + camDist / cameraPos.GetDistance(duckDestination) * camSpeed);

		tiles.clear();
		// 맵 그리기 & 충돌체크
		for (int y = -mapSize / 2; y < mapSize / 2 + 1; y++)
		{
			for (int x = -mapSize / 2; x < mapSize / 2 + 1; x++)
			{
				Vector2 tilePos(floor(x) - cameraTilePos.x, floor(y) - cameraTilePos.y);

				tiles[tilePos].position = tilePos * tileDist + game->screen.GetScreenSize() / 2;
				tiles[tilePos].rotation = 0;

				Vector2 tileMapPos(x + (int)(mapSize / 2), y + (int)(mapSize / 2));

				int tileType = 0;
				Vector2 rot = Vector2::zero;

				switch (tileMap[(int)tileMapPos.y][(int)tileMapPos.x])
				{
				case '0': // tile
					tileType = 0;
					tiles[tilePos].SetSprite(&tileSprites[0]);
					tiles[tilePos].size = Vector2::zero;
					break;
				case '1': // wall
					tileType = 1;
					tiles[tilePos].SetSprite(&tileSprites[1]);
					tiles[tilePos].size = Vector2(32, 32);
					break;
				case '2': // road

					if (tileMapPos.y == 0)
					{
						rot = Vector2(0, -1);
					}
					else if (tileMapPos.y == mapSize - 1)
					{
						rot = Vector2(0, 1);
					}
					if (tileMapPos.x == 0)
					{
						rot = Vector2(-1, 0);
					}
					else if (tileMapPos.x == mapSize - 1)
					{
						rot = Vector2(1, 0);
					}
					if (std::find(map.begin(), map.end(), (duckMapPos + rot)) != map.end())
					{
						tileType = 2;
						tiles[tilePos].SetSprite(&tileSprites[2]);
						tiles[tilePos].size = Vector2(32, 32);
					}
					else
					{
						tileType = 1;
						tiles[tilePos].SetSprite(&tileSprites[1]);
						tiles[tilePos].size = Vector2(32, 32);
					}

					break;
				}

				switch (tileType)
				{
				case 0:
					
					break;

				case 1:
					for (int i = 0; i < duckChims.size(); i++)
					{
						if (tiles[tilePos].Collision(duckChims[i].position, duckChims[i].size))
						{
							duckChims.erase(duckChims.begin() + i);
							i--;
						}
					}

					// 오리 콜리션 
					if (tiles[tilePos].Collision(duck.position + Vector2(camDist.x / cameraPos.GetDistance(duckDestination) * camSpeed, 0), duck.size))
					{
						duckCollisionX = true;
					}
					if (tiles[tilePos].Collision(duck.position + Vector2(0, camDist.y / cameraPos.GetDistance(duckDestination) * camSpeed), duck.size))
					{
						duckCollisionY = true;
					}

					break;
				case 2:
					if (tiles[tilePos].Collision(duck.position, duck.size))
					{
						mapMove = true;
						mapMoveRot = rot;
						mapMoveTile = Vector2(cameraTilePos.x, cameraTilePos.y);
					}
					break;
				}

				tiles[tilePos].Draw(game->screen);
			}
		}

		if (mapMove)
		{

			duckMapPos = duckMapPos + mapMoveRot;
			if (mapMoveRot.x == -1)
			{
				cameraPos = Vector2((mapSize / 2 - 2) * tileDist, (mapMoveTile * tileDist).y);
			}
			else if (mapMoveRot.x == 1)
			{
				cameraPos = Vector2((-mapSize / 2 + 2) * tileDist, (mapMoveTile * tileDist).y);
			}
			else if (mapMoveRot.y == -1)
			{
				cameraPos = Vector2((mapMoveTile * tileDist).x, (mapSize / 2 - 2) * tileDist);
			}
			else if (mapMoveRot.y == 1)
			{
				cameraPos = Vector2((mapMoveTile * tileDist).x, (-mapSize / 2 + 2) * tileDist);
			}
			for (int i = 0; i < enemys.size(); i++)
			{
				enemys[i].animator.StopAnimator();
			}
			enemys.clear();
			duckChims.clear();
			deadEnemys.clear();
			duckDestination = cameraPos;
		}
		
		if (mapMove && !(duckMapPos == Vector2::zero)) // 몬스터 소환
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dist(-(mapSize / 2 - 1), mapSize / 2 - 1);
			std::uniform_int_distribution<int> count(1, 3);

			int enemyCount = count(gen);
			for (int i = 0; i < enemyCount; i++)
			{
				Vector2 enemyPos(dist(gen), dist(gen)); // 타일에서 위치.
				Vector2 tilePos(enemyPos - cameraTilePos);

				Enemy enemy;
				enemys.push_back(enemy);
				enemys[enemys.size() - 1].tilePosition = enemyPos;
				enemys[enemys.size() - 1].object.position = (enemys[enemys.size() - 1].tilePosition - cameraTilePos) * tileDist + game->screen.GetScreenSize() / 2;
				enemys[enemys.size() - 1].SetEnemyDestination(tilePos * tileDist + game->screen.GetScreenSize() / 2);
			}
		}

		// 오리 콜리션 무브
		if (duckCollisionX && duckCollisionY)
		{
			duckMove = false;
		}
		else
		{
			duckMove = true;
		}

		// 몬스터 이동
		for (int i = 0; i < enemys.size(); i++)
		{
			Vector2 enemyDist(enemys[i].destination - enemys[i].tilePosition);
			if ((abs(enemyDist.x) > 2 / enemys[i].speed || abs(enemyDist.y) > 2 / enemys[i].speed) && !mapMove)
			{
				if (!enemys[i].collisionX)
				{
					enemys[i].tilePosition.x = enemys[i].tilePosition.x + enemyDist.x / enemys[i].tilePosition.GetDistance(enemys[i].destination) / enemys[i].speed;
				}
				if (!enemys[i].collisionY)
				{
					enemys[i].tilePosition.y = enemys[i].tilePosition.y + enemyDist.y / enemys[i].tilePosition.GetDistance(enemys[i].destination) / enemys[i].speed;
				}
			}
		}

		// 오리 이동
		if ((abs(camDist.x) > camSpeed / 2 || abs(camDist.y) > camSpeed / 2) && duckMove && !mapMove)
		{
			bool b = false;
			if (!duckCollisionX)
			{
				if (cameraPos.x > duckDestination.x)
				{
					duckWalkRot = true;
				}
				else
				{
					duckWalkRot = false;
				}
				cameraPos.x = cameraPos.x + camDist.x / cameraPos.GetDistance(duckDestination) * camSpeed;
			}
			else
			{
				b = true;
			}
			if (!duckCollisionY)
			{
				cameraPos.y = cameraPos.y + camDist.y / cameraPos.GetDistance(duckDestination) * camSpeed;
			}
			else
			{
				b = true;
			}
			if (b)
			{
				duckDestination = cameraPos;
			}
			duckIdle = false;
		}
		else
		{
			duckIdle = true;
		}



		// 침 공격
		for (int i = 0; i < enemys.size(); i++)
		{
			for (int j = 0; j < duckChims.size(); j++)
			{
				if (enemys[i].object.Collision(duckChims[j].position, duckChims[j].size))
				{
					enemys[i].hp -= duckDamage;
					duckChims.erase(duckChims.begin() + j);
					if (enemys[i].hp <= 0)
					{
						enemys[i].animator.StopAnimator();
						Object dead;
						dead.position = enemys[i].object.position;
						dead.rotation = 0;
						dead.size = Vector2(32, 16);
						deadEnemys.push_back(std::make_pair(enemys[i].tilePosition, dead));
						enemys.erase(enemys.begin() + i);
					}
				}
			}
		}



		if (!mapMove)
		{
			// 포탈 생성
			if (duckMapPos == map[map.size() - 1])
			{
				Vector2 portalPos(-cameraTilePos.x, -cameraTilePos.y);
				portal.position = portalPos * tileDist + game->screen.GetScreenSize() / 2;


				portal.Draw(game->screen);
			}
		}


		// 죽은 적 그리기
		for (int i = 0; i < deadEnemys.size(); i++)
		{
			deadEnemys[i].second.SetSprite(&deadEnemy);
			deadEnemys[i].second.position = (deadEnemys[i].first - cameraTilePos) * tileDist + game->screen.GetScreenSize() / 2;
			deadEnemys[i].second.Draw(game->screen);
		}


		// 총알 그리기
		for (int i = 0; i < duckChims.size(); i++)
		{
			duckChims[i].SetSprite(&duckChim);
			duckChims[i].Draw(game->screen);
		}

		// 오리 그리기
		duck.Draw(game->screen);

		if (!mapMove)
		{
			// 적 그리기
			for (int i = 0; i < enemys.size(); i++)
			{
				enemys[i].object.position = (enemys[i].tilePosition - cameraTilePos) * tileDist + game->screen.GetScreenSize() / 2;

				enemys[i].DrawEnemy(game->screen);
			}
		}
		else
		{
			for (int i = 0; i < enemys.size(); i++)
			{
				enemys[i].animator.StartAnimator();
			}
		}

		//미니맵
		const int mapPixel = 6;
		int mapLength = mapMaker.GetLength() + 3;

		for (int i = 0; i < mapLength * mapPixel; i++)
		{
			for (int j = 0; j < mapLength * mapPixel; j++)
			{
				game->screen.SetPixelInScreen(Vector2(i, j + game->screen.GetScreenSize().y - mapLength * mapPixel), 14);
			}
		}

		for (int mX = 0; mX < mapLength; mX++)
		{
			for (int mY = 0; mY < mapLength + 1; mY++)
			{
				for (int x = 0; x < mapPixel; x++)
				{
					for (int y = 0; y < mapPixel; y++)
					{
						if (x == 0 || x == mapPixel - 1 || y == 0 || y == mapPixel - 1)
						{
							Vector2 pixelPos(mapPixel * mX + 1 + x, mapPixel * mY + 1 + y + game->screen.GetScreenSize().y - mapLength * (mapPixel + 1));
							Vector2 mapPos(mX - mapLength / 2, mY - mapLength / 2);
							if (std::find(map.begin(), map.end(), mapPos) != map.end())
							{
								if (mapPos == duckMapPos)
								{
									game->screen.SetPixelInScreen(pixelPos, 128);
								}
								else
								{
									game->screen.SetPixelInScreen(pixelPos, 255);
								}
							}
						}
					}
				}
			}
		}

		//체력
		
		std::vector<std::pair<Vector2, COLORREF>> vec;
		std::string hpStr = std::to_string(game->publicMemory["duckHp"]);
		for (int i = 0; i < hpStr.size(); i++)
		{
			int num = hpStr[i] - '0';
			vec = hpSprites[num].GetSprite(Vector2(20 * i + game->screen.GetScreenSize().x - 20 * hpStr.size() - 20, 360), 0);
			for (int i = 0; i < vec.size(); i++)
			{
				game->screen.SetPixelInScreen(vec[i].first, vec[i].second);
			}
		}


		game->screen.DrawScreen();

		if (portal.Collision(duck.position, duck.size))
		{
			game->publicMemory["Stage"]++;
			game->screen.DrawScreen();
			return;
		}

	}
}


void LoadingScene(Game* game)
{
	double radius = 60;
	double circleRot = 0; // euler
	double duckRot = 0; // euler

	Sprite duck("Assets/player/IdleLeft1.bmp");
	duck.InitSprite();

	Sprite number[10] = { 
		Sprite("Assets/loading/0.bmp"),
		Sprite("Assets/loading/1.bmp"),
		Sprite("Assets/loading/2.bmp"),
		Sprite("Assets/loading/3.bmp"),
		Sprite("Assets/loading/4.bmp"),
		Sprite("Assets/loading/5.bmp"),
		Sprite("Assets/loading/6.bmp"),
		Sprite("Assets/loading/7.bmp"),
		Sprite("Assets/loading/8.bmp"),
		Sprite("Assets/loading/9.bmp")
	};

	for (int i = 0; i < 10; i++)
	{
		number[i].InitSprite();
	}

	Object Stage("Assets/loading/Stage.bmp", Vector2(100, 350), 0, Vector2::zero);

	while (true)
	{


		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
		{
			game->gameEnd = true;
			break;
		}

		std::vector<std::pair<Vector2, COLORREF>> vec = duck.GetSprite(Vector2(cos(circleRot * M_PI / 180), sin(circleRot * M_PI / 180)) * radius + game->screen.GetScreenSize() / 2, duckRot);
		for (int i = 0; i < vec.size(); i++)
		{
			game->screen.SetPixelInScreen(vec[i].first, vec[i].second);
		}

		std::string stageNumber = std::to_string(game->publicMemory["Stage"]);


		circleRot = (int)(circleRot + 10);
		duckRot = (int)(duckRot + 20) % 360;

		Stage.Draw(game->screen);

		for (int i = 0; i < stageNumber.size(); i++)
		{
			int num = stageNumber[i] - '0';
			vec = number[num].GetSprite(Vector2(200 + 20 + 40 * i, 350), 0);
			for (int i = 0; i < vec.size(); i++)
			{
				game->screen.SetPixelInScreen(vec[i].first, vec[i].second);
			}
		}

		game->screen.DrawScreen();

		if (circleRot == 720)
		{
			game->screen.DrawScreen();
			game->sound.PlaySoundAsset("Assets/sound/tue.wav", "ONCE");
			return;
		}
	}
}