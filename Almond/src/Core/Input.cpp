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
        //ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT)
        {
	        
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.repeat == 0)
            {
                keysPressed.insert(e.key.keysym.scancode);
            }
            KeyDown();
        }
        else if (e.type == SDL_KEYUP)
        {
            KeyUp();
            KeysReleased.insert(e.key.keysym.scancode);
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
            wheelY = e.wheel.y;
        }

    }
}


void Input::Reset()
{
    keysPressed.clear();
    KeysReleased.clear();
    wheelY = 0;
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
