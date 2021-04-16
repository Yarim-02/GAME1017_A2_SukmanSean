#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

class Sprite
{
public:
	SDL_Rect m_src;
	SDL_Rect m_dst;
	bool alive;
};

class Bullet
{
private:
	SDL_Rect m_rect;
	int allegance;
public:
	Bullet(int direction, SDL_Rect spawnLoc = { 112, 384 })
	{
		cout << "Creating bullet at " << &(*this) << endl;
		this->m_rect.x = spawnLoc.x; //"this->" is optional
		this->m_rect.y = spawnLoc.y;
		this->m_rect.w = 20;
		this->m_rect.h = 4;
		allegance = direction;
		//m_rect = {spawnLoc.x, spawnLoc.y, 4, 4}; //short way

	}
	~Bullet() //Destructor
	{
		cout << "De-allocating bullet at " << &(*this) << endl;
	}
	void SetLoc(SDL_Point loc)
	{
		m_rect.x = loc.x;
		m_rect.y = loc.y;
	}
	void Update()
	{
		this->m_rect.x += (10 * allegance);
	}
	void Render(SDL_Renderer* rend)
	{
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		SDL_RenderFillRect(rend, &m_rect);
	}
	SDL_Rect* GetRekt() { return &m_rect; }
};

class Enemy
{
public:
	SDL_Rect m_src;
	SDL_Rect m_dst;
	
	Enemy(SDL_Rect sourceRect, SDL_Rect destinationRect)
	{
		m_src = sourceRect;
		m_dst = destinationRect;
	}
	~Enemy()
	{
		cout << "De-allocating emeny at " << &(*this) << endl;
	}
	void SetLoc(SDL_Point loc)
	{
		m_dst.x = loc.x;
		m_dst.y = loc.y;
	}
	void Update()
	{
		this->m_dst.x -= 5;
	}
	SDL_Rect* GetRekt() { return &m_dst; }
};

class Engine
{
private:   // private properties
	
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	

private:  // private method prototypes
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	
	void Update();
	void Render();
	void Sleep();
public:
	bool m_running = false;

	int Run();

	static Engine& Instance();

	SDL_Renderer* GetRenderer() { return m_pRenderer; }

	bool KeyDown(SDL_Scancode c);
};

#endif

// Reminder: you can ONLY have declarations in headers, not logical code
