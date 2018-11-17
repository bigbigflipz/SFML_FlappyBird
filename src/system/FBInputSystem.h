#pragma once

#include "Component/FBInput.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void Update();
private:

};

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::Update()
{
	if (FBInput::IsKeyDown(event, sf::Keyboard::Space))
	{
		speed = jump;
		if (!gameStart)
		{
			gameStart = true;
		}

	}
}
