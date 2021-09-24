#pragma once
#include <set>
#include <SDL/SDL.h>

class Input
{
public:
	enum class MouseButton
	{
		left, middle, right
	};
	
	static Input* GetInstance()
	{
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new Input();
	}

	void Listen();
	bool GetKeyDown(SDL_Scancode key);
	bool GetKeyUp(SDL_Scancode key);
	
	bool GetMouseButton(MouseButton button);
	bool GetMouseButtonDown(MouseButton button);
	bool GetMouseButtonUp(MouseButton button);

	int GetMouseX() { return  m_MouseX; }
	int GetMouseY() { return  m_MouseY; }
	
	bool GetKey(SDL_Scancode key);
	void Reset();

	Sint32 getWheelY()
	{
		return m_WheelY;
	}
	
private:
	Input();
	void KeyUp();
	void KeyDown();
	
	const Uint8* m_KeyDownStates;

	std::set<SDL_Scancode> keysPressed;
	std::set<SDL_Scancode> KeysReleased;

	Uint32 m_MouseButtons;
	Uint32 m_PreviousMouseButtons;
	Sint32 m_WheelY;
	int m_MouseX;
	int m_MouseY;

	
	static Input* s_Instance;
};
