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
            m_WheelY = e.wheel.y;
        }
    }
    m_MouseButtons = SDL_GetRelativeMouseState(&m_MouseX, &m_MouseY);
}


void Input::Reset()
{
    keysPressed.clear();
    KeysReleased.clear();
    m_WheelY = 0;
    m_PreviousMouseButtons = m_MouseButtons;
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


bool Input::GetMouseButton(MouseButton button)
{
    uint32_t mask = 0;
	switch (button)
	{
    case MouseButton::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MouseButton::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MouseButton::middle:
        mask = SDL_BUTTON_MMASK;
        break;
	}

    return m_MouseButtons & mask;
}

bool Input::GetMouseButtonDown(MouseButton button)
{
    uint32_t mask = 0;
    switch (button)
    {
    case MouseButton::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MouseButton::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MouseButton::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    }

    return !(m_PreviousMouseButtons & mask) && (m_MouseButtons & mask);
}


bool Input::GetMouseButtonUp(MouseButton button)
{
    uint32_t mask = 0;
    switch (button)
    {
    case MouseButton::left:
        mask = SDL_BUTTON_LMASK;
        break;
    case MouseButton::right:
        mask = SDL_BUTTON_RMASK;
        break;
    case MouseButton::middle:
        mask = SDL_BUTTON_MMASK;
        break;
    }

    return (m_PreviousMouseButtons & mask) && !(m_MouseButtons & mask);
}
