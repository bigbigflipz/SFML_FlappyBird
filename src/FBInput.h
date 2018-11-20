#pragma once

#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

class FBInput
{
public:

    static bool IsKeyUp(sf::Event event, sf::Keyboard::Key keycode)
    {
        if(event.type == sf::Event::KeyReleased && event.key.code == keycode)
            return true;
        else
            return false;
    }

    static bool IsKeyDown(sf::Event event, sf::Keyboard::Key keycode)
    {
        if(event.type == sf::Event::KeyPressed && event.key.code == keycode)
            return true;
        else
            return false;
    }

    static bool IsKeyPress(sf::Event event, sf::Keyboard::Key keycode)
    {
        if(sf::Keyboard::isKeyPressed(keycode))
            return true;
        else
            return false;
    }
};