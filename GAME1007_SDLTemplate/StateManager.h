#pragma once
#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_

#include "States.h"
#include <vector>


class StateManager
{
public:
	static std::vector<State*> s_states;
public:
	static void Update();
	static void Render();
	static void Events();
	static void PushState(State* pState);
	static void PopState();
	static void ChangeState(State* pState);
	static void Quit();
	static std::vector<State*>& GetStates();
private:
	StateManager();
};

typedef StateManager STMA;

#endif // !
