#include <Core/Input.h>

#include "imgui/imgui_impl_sdl.h"

Input* Input::s_Instance = nullptr;

Input::Input()
{
	m_KeyDownStates = SDL_GetKeyboardState(nullptr);
}

void Input::Listen()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
        switch (e.type)
	    {
	        case SDL_QUIT: break;
	        case SDL_KEYDOWN:
		        if (e.key.repeat == 0)
		        {
                    keysPressed.insert(e.key.keysym.scancode);
		        }
                KeyDown();
        		break;
	        case SDL_KEYUP: 
                KeyUp();
                KeysReleased.insert(e.key.keysym.scancode);
        		break;
        }
    }
}


void Input::Reset()
{
    keysPressed.clear();
    KeysReleased.clear();
}

bool Input::GetKeyDown(SDL_Scancode key)
{
    return (keysPressed.find(key) != keysPressed.end());
}

bool Input::GetKeyUp(SDL_Scancode key)
{
    return (KeysReleased.find(key) != KeysReleased.end());
}


bool Input::GetKey(SDL_Scancode key)
{
    return (m_KeyDownStates[key] == 1);
}



void Input::KeyUp()
{
    m_KeyDownStates = SDL_GetKeyboardState(nullptr);

}

void Input::KeyDown()
{
    m_KeyDownStates = SDL_GetKeyboardState(nullptr);
}
