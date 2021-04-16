#include "States.h"
#include "StateManager.h"
#include "Engine.h"	

#define _CRT_SECURE_NO_WARNINGS

void State::Render()
{
	//SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume()
{
}

TitleState::TitleState() 
{
	m_pButtonTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "StartButton.png");
	m_pExitTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "ExitButton.png");	
	m_bkgTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "BackGround.jpg");
	m_TitleTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Title.png");
	m_pButton = { {0,0,500,300}, {400,400,200,100} };
	m_pTitle = { {0,0,627, 222}, {350,100,300,150} };
	m_exit = { {0,0,500, 300}, {400,600,200,100} };
	m_pBackground = { {0,0,1024,768}, {0,0,WIDTH,HEIGHT} };
}

void TitleState::Enter()
{
	cout << "entering TitleState" << endl;
}

void TitleState::Update()
{
	//cout << "updating titlestate" << endl;
	// Parse N key and change to new GameState
	SDL_GetMouseState(&mouseX, &mouseY);
	//cout << "(" << mouseX << ", " << mouseY << ")" << endl;
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		cout << "entering gamestate" << endl;
		STMA::ChangeState(new GameState());
	}
	if (mouseX > m_pButton.m_dst.x && mouseX < (m_pButton.m_dst.x + m_pButton.m_dst.w)
		&& mouseY > m_pButton.m_dst.y && mouseY < (m_pButton.m_dst.y + m_pButton.m_dst.h))
	{
		m_pButton.m_src.x = 500;
	}
	else 
		m_pButton.m_src.x = 0;
	if (mouseX > m_exit.m_dst.x && mouseX < (m_exit.m_dst.x + m_exit.m_dst.w)
		&& mouseY > m_exit.m_dst.y && mouseY < (m_exit.m_dst.y + m_exit.m_dst.h))
	{
		m_exit.m_src.x = 500;
	}
	else
		m_exit.m_src.x = 0;
}

void TitleState::Events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Engine::Instance().m_running = false;
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case '1':
				//Mix_PlayChannel(-1, m_laserSFX, 0); //-1 channel is first available
				//Mix_PlayChannel(-1, m_explosionSFX, 0);
				break;
			}

		case SDL_MOUSEBUTTONDOWN:
		{
			cout << "clicking" << endl;
			if (mouseX > m_pButton.m_dst.x && mouseX < (m_pButton.m_dst.x + m_pButton.m_dst.w)
				&& mouseY > m_pButton.m_dst.y && mouseY < (m_pButton.m_dst.y + m_pButton.m_dst.h))
			{
				STMA::ChangeState(new GameState());
			}
			if (mouseX > m_exit.m_dst.x && mouseX < (m_exit.m_dst.x + m_exit.m_dst.w)
				&& mouseY > m_exit.m_dst.y && mouseY < (m_exit.m_dst.y + m_exit.m_dst.h))
			{
				exit(2);
			}
			break;
		}
		}
	}
}

void TitleState::Render()
{
	//cout << "rendering TitleState" << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_bkgTexture, &m_pBackground.m_src, &m_pBackground.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pButtonTexture, &m_pButton.m_src, &m_pButton.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pExitTexture, &m_exit.m_src, &m_exit.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_TitleTexture, &m_pTitle.m_src, &m_pTitle.m_dst);
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void TitleState::Exit()
{
}

// Game State

GameState::GameState()
{
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
	{
		m_pTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "133621.png");
		m_eTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "EnemyShip.png");
		m_bkgTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "BackGround.jpg");
		m_pFloorTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Floor.png");
		m_pBushTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "bush.png");
		m_pPlantTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "plant.png");
		m_pBenchTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "bench.png");
		m_pChainTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "chain.png");
		m_pPoleTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "pole.png");
	}

	if (Mix_Init(MIX_INIT_MP3) != 0)
	{
		//Configure mixer
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
		Mix_AllocateChannels(16);
		//Load sounds
		m_sounds.emplace("laser", new Mix_Chunk);
		m_sounds.emplace("jump", new Mix_Chunk);
		m_sounds["laser"] = Mix_LoadWAV("Music/LaserSFX.wav");
		m_sounds["jump"] = Mix_LoadWAV("Music/Jump.wav");
		m_menuMusic = Mix_LoadMUS("Music/GreenPath.mp3");
		m_gameMusic = Mix_LoadMUS("Music/GreenPath.mp3");
	}

	if (TTF_Init() == 0)
	{
		m_font = TTF_OpenFont("LTYPE.TTF", 22);
	}

	m_bkg1 = { {0,0,2268,653}, {0,0,WIDTH,HEIGHT} };
	m_bkg2 = { {0,0,2268,653}, {1024,0,WIDTH,HEIGHT} };
	m_floor1 = { {0,0, 306, 52}, {0,670,WIDTH, 228} };
	m_floor2 = { {0,0, 306, 52}, {1024,670,WIDTH, 228} };
	m_bush = { {0,0, 361, 217}, {0,620,360, 300} };
	m_plant = { {0,0, 226, 222}, {0,550,360, 300} };
	m_player = { {3,1191,159,190}, {0,540,110,130} }; //first {} is src rectangle, and second {} destination rectangle
	m_player.alive = true;
	Mix_PlayMusic(m_menuMusic, -1); //0-n for # of loops, or -1 for infinite looping
	Mix_VolumeMusic(16); //0-128
	srand(unsigned(time(0)));
}

void GameState::Enter()
{
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_C))
	{
		cout << "entering titlestate" << endl;
		STMA::ChangeState(new TitleState());
	}
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		cout << "entering titlestate" << endl;
		STMA::PushState(new PauseState());
	}

	frameTimer++;
	m_player.m_dst.x += m_velocity.x;
	m_player.m_dst.y += m_velocity.y;

	if (m_player.alive == true)
	{
		
		if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_player.m_dst.x > 0)
			m_player.m_dst.x -= m_speed;
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && m_player.m_dst.x < WIDTH - m_player.m_dst.w)
			m_player.m_dst.x += m_speed;
		if (Engine::Instance().KeyDown(SDL_SCANCODE_S))
			m_crouching = true;
		else
			m_crouching = false;
	}

	if (frameTimer % 60 == 0)
	{
		secondTimer++;
	}
	sprintf_s(test, "%d", secondTimer);
	//tmp = std::to_strin
	//t/est = frameTimer;
	std::cout << frameTimer << endl;
	if (m_player.m_dst.y < 540)
		m_velocity.y += 1;
	else if(m_player.m_dst.y >= 540)
		m_velocity.y = 0;




	if (m_player.m_dst.y >= 540)
	{
		if (m_crouching == false)
		{
			m_player.m_src.y = 1191;
			m_player.m_src.w = 159;
			m_player.m_dst.w = 110;
			m_player.m_src.h = 190;
			m_player.m_dst.h = 130;
			if (frameTimer % 40 >= 0 && frameTimer % 40 <= 4)
				m_player.m_src.x = 3;
			else if (frameTimer % 40 >= 5 && frameTimer % 40 <= 9)
				m_player.m_src.x = 165;
			else if (frameTimer % 40 >= 10 && frameTimer % 40 <= 14)
				m_player.m_src.x = 327;
			else if (frameTimer % 40 >= 15 && frameTimer % 40 <= 19)
				m_player.m_src.x = 489;
			else if (frameTimer % 40 >= 20 && frameTimer % 40 <= 24)
				m_player.m_src.x = 651;
			else if (frameTimer % 40 >= 25 && frameTimer % 40 <= 29)
				m_player.m_src.x = 813;
			else if (frameTimer % 40 >= 30 && frameTimer % 40 <= 34)
				m_player.m_src.x = 975;
			else if (frameTimer % 40 >= 35 && frameTimer % 40 <= 40)
				m_player.m_src.x = 1137;
		}
		else
		{
			m_player.m_dst.y = 630;
			m_player.m_src.y = 2628;
			m_player.m_src.x = 350;
			m_player.m_src.w = 200;
			m_player.m_src.h = 124;
			m_player.m_dst.w = 180;
			m_player.m_dst.h = 60;
		}
	}
	else
	{
		m_player.m_src.x = 3;
		m_player.m_src.y = 1627;
	}

	if (frameTimer >= 200)
	{
		spawnObstacle = rand() % 3;
		if (spawnObstacle == 0)
		{
			m_bench.push_back(new Sprite{ {0,0,189,107}, {1024, 580, 100, 100} });
			m_bench.shrink_to_fit();
		}
		else if (spawnObstacle == 1)
		{
			m_chains.push_back(new Sprite{ {0,0,69,604}, {1024, 0, 60, 550} });
			m_chains.shrink_to_fit();
		}
		else if (spawnObstacle == 2)
		{
			m_pole.push_back(new Sprite{ {0,0,76,449}, {1024, 350, 60, 350} });
			m_pole.shrink_to_fit();
		}
		frameTimer = 0;
	}

	for (unsigned i = 0; i < m_bench.size(); i++)
	{
		m_bench[i]->m_dst.x -= 3;
		if (m_bench[i]->m_dst.x <= -200)
		{
			delete m_bench[i];
			m_bench[i] = nullptr;
			m_bench.erase(m_bench.begin() + i);
			m_bench.shrink_to_fit();
		}
		
	}
	for (unsigned i = 0; i < m_bench.size(); i++)
	{
		if (SDL_HasIntersection(&m_player.m_dst, &m_bench[i]->m_dst))
		{
			STMA::ChangeState(new EndState());
		}
	}

	for (unsigned i = 0; i < m_chains.size(); i++)
	{
		m_chains[i]->m_dst.x -= 3;
		if (m_chains[i]->m_dst.x <= -200)
		{
			delete m_chains[i];
			m_chains[i] = nullptr;
			m_chains.erase(m_chains.begin() + i);
			m_chains.shrink_to_fit();
		}
		
	}

	for (unsigned i = 0; i < m_chains.size(); i++)
	{
		if (SDL_HasIntersection(&m_player.m_dst, &m_chains[i]->m_dst))
		{
			STMA::ChangeState(new EndState());
		}
	}

	for (unsigned i = 0; i < m_pole.size(); i++)
	{
		m_pole[i]->m_dst.x -= 3;
		if (m_pole[i]->m_dst.x <= -200)
		{
			delete m_pole[i];
			m_pole[i] = nullptr;
			m_pole.erase(m_pole.begin() + i);
			m_pole.shrink_to_fit();
		}
		
	}

	for (unsigned i = 0; i < m_pole.size(); i++)
	{
		if (SDL_HasIntersection(&m_player.m_dst, &m_pole[i]->m_dst))
		{
			STMA::ChangeState(new EndState());
		}
	}

	m_bkg1.m_dst.x -= 1;
	m_bkg2.m_dst.x -= 1;
	if (m_bkg1.m_dst.x <= 0 - m_bkg1.m_dst.w)
	{
		m_bkg1.m_dst.x = 1022;
	}
	if (m_bkg2.m_dst.x <= 0 - m_bkg2.m_dst.w)
	{
		m_bkg2.m_dst.x = 1022;
	}

	m_floor1.m_dst.x -= 3;
	m_floor2.m_dst.x -= 3;
	if (m_floor1.m_dst.x <= 0 - m_floor1.m_dst.w)
	{
		m_floor1.m_dst.x = 1022;
	}
	if (m_floor2.m_dst.x <= 0 - m_floor2.m_dst.w)
	{
		m_floor2.m_dst.x = 1022;
	}

	m_bush.m_dst.x -= 7;
	if (m_bush.m_dst.x <= -2000 - m_bush.m_dst.w)
	{
		m_bush.m_dst.x = 1022;
	}
	m_plant.m_dst.x -= 7;
	if (m_plant.m_dst.x <= -500 - m_plant.m_dst.w)
	{
		m_plant.m_dst.x = 1022;
	}
}

void GameState::Events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Engine::Instance().m_running = false;
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case '1':
				//Mix_PlayChannel(-1, m_laserSFX, 0); //-1 channel is first available
				//Mix_PlayChannel(-1, m_explosionSFX, 0);
				break;
			case '2':
				
				//Mix_PlayMusic(m_menuMusic, -1);
				break;
			case 's':
				m_player.m_dst.y = 540;
				break;
			case 'p':
				if (Mix_PausedMusic() == 1)
					Mix_ResumeMusic();
				else
					Mix_PauseMusic();
				break;
			case ' ':
				//Spawn bullet
				if (m_player.alive == true)
				{
					if (m_player.m_dst.y > 535)
					{
						m_player.m_dst.y = 530;
						m_velocity.y = -30;
						Mix_PlayChannel(-1, m_sounds["jump"], 0);
					}
					/*m_bullets.push_back(new Bullet(1, { (m_player.m_dst.x + m_player.m_dst.w), (m_player.m_dst.y + (m_player.m_dst.h / 2)) }));
					m_bullets.shrink_to_fit();
					cout << "New bullet vector capacity: " << m_bullets.capacity() << endl;
					Mix_PlayChannel(-1, m_sounds["laser"], 0);*/
				}
				break;
			}
			break;
		}
	}
}

void GameState::Render()
{
	cout << "rendering GameState" << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_bkgTexture, &m_bkg1.m_src, &m_bkg1.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_bkgTexture, &m_bkg2.m_src, &m_bkg2.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pFloorTexture, &m_floor1.m_src, &m_floor1.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pFloorTexture, &m_floor2.m_src, &m_floor2.m_dst);
	if (m_player.alive == true)
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pTexture, &m_player.m_src, &m_player.m_dst, 0.0, NULL, SDL_FLIP_HORIZONTAL);
	for (unsigned i = 0; i < m_pole.size(); i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pPoleTexture, &m_pole[i]->m_src, &m_pole[i]->m_dst);
	for (unsigned i = 0; i < m_chains.size(); i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pChainTexture, &m_chains[i]->m_src, &m_chains[i]->m_dst);
	for (unsigned i = 0; i < m_bench.size(); i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBenchTexture, &m_bench[i]->m_src, &m_bench[i]->m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBushTexture, &m_bush.m_src, &m_bush.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pPlantTexture, &m_plant.m_src, &m_plant.m_dst);
	m_pFontSurf = TTF_RenderText_Solid(m_font, test, { 255,255,255,225 });
	SDL_DestroyTexture(m_pTextTexture);
	m_pTextTexture = SDL_CreateTextureFromSurface(Engine::Instance().GetRenderer(), m_pFontSurf);
	m_scoreRect = { 950, 40, m_pFontSurf->w, m_pFontSurf->h };
	SDL_FreeSurface(m_pFontSurf);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pTextTexture, NULL, &m_scoreRect, m_angle, 0, SDL_FLIP_NONE);
	SDL_RenderPresent(Engine::Instance().GetRenderer()); // Flip buffers - send data to window.
}

void GameState::Exit()
{
	for (unsigned i = 0; i < m_bench.size(); i++)
	{
		delete m_bench[i]; //Flag for reallocation
		m_bench[i] = nullptr;
	}
	m_bench.clear();
	m_bench.shrink_to_fit();
	for (unsigned i = 0; i < m_pole.size(); i++)
	{
		delete m_pole[i]; //Flag for reallocation
		m_pole[i] = nullptr;
	}
	m_pole.clear();
	m_pole.shrink_to_fit();
	for (unsigned i = 0; 9 < m_chains.size(); i++)
	{
		delete m_chains[i];
		m_chains[i] = nullptr;
	}
	m_chains.clear();
	m_chains.shrink_to_fit();
	TTF_CloseFont(m_font);
	SDL_DestroyTexture(m_pTexture);
	Mix_FreeChunk(m_sounds["laser"]);
	Mix_FreeChunk(m_sounds["kaboom"]);
	Mix_FreeMusic(m_menuMusic);
	Mix_FreeMusic(m_gameMusic);
}

void GameState::Resume()
{
}

PauseState::PauseState()
{
	m_pButtonTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "ResumeButton.png");
	m_pButton = { {0,0,500,300}, {400,400,200,100} };
}

void PauseState::Enter()
{
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::PopState();
	}

	if (mouseX > m_pButton.m_dst.x && mouseX < (m_pButton.m_dst.x + m_pButton.m_dst.w)
		&& mouseY > m_pButton.m_dst.y && mouseY < (m_pButton.m_dst.y + m_pButton.m_dst.h))
	{
		m_pButton.m_src.x = 500;
	}
	else
		m_pButton.m_src.x = 0;

	SDL_GetMouseState(&mouseX, &mouseY);
}

void PauseState::Events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Engine::Instance().m_running = false;
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case '1':
				//Mix_PlayChannel(-1, m_laserSFX, 0); //-1 channel is first available
				//Mix_PlayChannel(-1, m_explosionSFX, 0);
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
		{
			cout << "clicking" << endl;
			if (mouseX > m_pButton.m_dst.x && mouseX < (m_pButton.m_dst.x + m_pButton.m_dst.w)
				&& mouseY > m_pButton.m_dst.y && mouseY < (m_pButton.m_dst.y + m_pButton.m_dst.h))
			{
				STMA::PopState();
			}
			break;
		}
		}

	}
}

void PauseState::Render()
{
	STMA::s_states.front()->Render();

	cout << "rendering pause state" << endl;
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 192);
	SDL_Rect rect = { 256, 128, 512, 512 };
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pButtonTexture, &m_pButton.m_src, &m_pButton.m_dst);
	SDL_RenderPresent(Engine::Instance().GetRenderer());
	State::Render();
}

void PauseState::Exit()
{
}

EndState::EndState()
{
	m_pButtonTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "MenuButton.png");
	m_bkgTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "BackGround.jpg");
	m_pButton = { {0,0,500,300}, {400,200,200,100} };
	m_pBackground = { {0,0,1024,768}, {0,0,WIDTH,HEIGHT} };
}

void EndState::Enter()
{
	cout << "entering TitleState" << endl;
}

void EndState::Update()
{
	//cout << "updating titlestate" << endl;
	// Parse N key and change to new GameState
	SDL_GetMouseState(&mouseX, &mouseY);
	//cout << "(" << mouseX << ", " << mouseY << ")" << endl;
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		cout << "entering gamestate" << endl;
		STMA::ChangeState(new GameState());
	}

	if (mouseX > m_pButton.m_dst.x && mouseX < (m_pButton.m_dst.x + m_pButton.m_dst.w)
		&& mouseY > m_pButton.m_dst.y && mouseY < (m_pButton.m_dst.y + m_pButton.m_dst.h))
	{
		m_pButton.m_src.x = 500;
	}
	else
		m_pButton.m_src.x = 0;
}

void EndState::Events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Engine::Instance().m_running = false;
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case '1':
				//Mix_PlayChannel(-1, m_laserSFX, 0); //-1 channel is first available
				//Mix_PlayChannel(-1, m_explosionSFX, 0);
				break;
			}

		case SDL_MOUSEBUTTONDOWN:
		{
			cout << "clicking" << endl;
			if (mouseX > m_pButton.m_dst.x && mouseX < (m_pButton.m_dst.x + m_pButton.m_dst.w)
				&& mouseY > m_pButton.m_dst.y && mouseY < (m_pButton.m_dst.y + m_pButton.m_dst.h))
			{
				STMA::ChangeState(new TitleState());
			}
			break;
		}
		}
	}
}

void EndState::Render()
{
	//cout << "rendering TitleState" << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_bkgTexture, &m_pBackground.m_src, &m_pBackground.m_dst);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pButtonTexture, &m_pButton.m_src, &m_pButton.m_dst);
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void EndState::Exit()
{
}
