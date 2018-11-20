#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

#include "FBPlayer.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window/Event.hpp"

class Base
{
public:

    Base(std::shared_ptr<sf::Texture> baseTexture, sf::Vector2f pos);

    void Reset();
    void Update(const sf::View& view);

    void Draw(sf::RenderWindow& renderWindow);

    bool CheckCollision(const Player& player);

private:

    std::unique_ptr<sf::Sprite> m_primary;
    std::unique_ptr<sf::Sprite> m_secondary;

    sf::Vector2f m_initPosL;
    sf::Vector2f m_initPosR;
};
