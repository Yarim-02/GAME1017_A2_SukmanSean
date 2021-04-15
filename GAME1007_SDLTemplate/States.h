#pragma once
#include "Engine.h"
#ifndef _STATES_
#define _STATES_

class State
{
public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Events() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume();
	virtual ~State() = default;
protected:
	State() {};
};

class TitleState : public State
{
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Events();
	virtual void Render();
	virtual void Exit();
private:
	int mouseX, mouseY;

	SDL_Texture* m_pButtonTexture;
	SDL_Texture* m_bkgTexture;
	SDL_Texture* m_TitleTexture;
	Sprite m_pBackground;
	Sprite m_pButton;
	Sprite m_pTitle;
};

class EndState : public State
{
public:
	EndState();
	virtual void Enter();
	virtual void Update();
	virtual void Events();
	virtual void Render();
	virtual void Exit();
private:
	int mouseX, mouseY;

	SDL_Texture* m_pButtonTexture;
	SDL_Texture* m_bkgTexture;
	Sprite m_pBackground;
	Sprite m_pButton;
};

class PauseState : public State
{
public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Events();
	virtual void Render();
	virtual void Exit();
private:
	int mouseX, mouseY;

	SDL_Texture* m_pButtonTexture;	
	Sprite m_pButton;
	
};


class GameState : public State
{
public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Events();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
private:
	int enemySpawnPos;
	int frameTimer;
	//State s;

	SDL_Texture* m_pTexture;
	SDL_Texture* m_eTexture;
	SDL_Texture* m_bkgTexture;
	SDL_Texture* m_pFloorTexture;
	SDL_Texture* m_pBushTexture;
	SDL_Texture* m_pPlantTexture;
	SDL_Texture* m_pBenchTexture;
	SDL_Point m_velocity;
	Sprite m_player;
	Sprite m_bkg1, m_bkg2;
	Sprite m_floor1, m_floor2;
	Sprite m_bush, m_plant;
	int m_speed = 5; // In-class initialization. Not normal.
	int spawnObstacle;
	bool m_crouching;
	//Bullet vector
	vector<Sprite*> m_chains; //Not a bullet object butrather a pointer to a bullet
	vector<Sprite*> m_bench;
	vector<Sprite*> m_pole;

	//Sound effect objects
	map<string, Mix_Chunk*> m_sounds;
	/*Mix_Chunk* m_laserSFX;
	Mix_Chunk* m_explosionSFX; */
	//Music track objects
	Mix_Music* m_menuMusic;
	Mix_Music* m_gameMusic;
};
#endif // !_STATES_

