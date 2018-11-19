#pragma once

#include <memory>

#include "SFML/Graphics.hpp"

class Player
{
public:

    Player(std::shared_ptr<sf::Texture> playerTexture, sf::Vector2f pos);

    void Reset();

    void HandleInput(sf::Event event);

    void Update(float dt);

    void Draw(sf::RenderWindow& renderWindow) const;

    const sf::Vector2f& GetPosition() const;


    const sf::FloatRect& GetGlobalBounds() const;

private:

    sf::Sprite m_sprite;
    sf::Vector2f m_speed;
    sf::Vector2f m_initPos;
    float m_rot;
    float m_jump;
    float m_gravity;
    float m_max;
};
