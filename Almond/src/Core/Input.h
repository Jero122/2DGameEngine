#pragma once
#include <set>
#include <SDL/SDL.h>

class Input
{
public:
	static Input* GetInstance()
	{
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new Input();
	}

	void Listen();
	bool GetKeyDown(SDL_Scancode key);
	bool GetKeyUp(SDL_Scancode key);

	bool GetKey(SDL_Scancode key);
	void Reset();

	Sint32 getWheelY()
	{
		return wheelY;
	}
	
private:
	Input();
	void KeyUp();
	void KeyDown();
	
	const Uint8* m_KeyDownStates;

	std::set<SDL_Scancode> keysPressed;
	std::set<SDL_Scancode> KeysReleased;

	Sint32 wheelY;

	
	static Input* s_Instance;
};
