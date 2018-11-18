#pragma once

#include "SFML/Window/Event.hpp"
#include "Component/FBInput.h"
#include "SFML/Graphics.hpp"

class Player
{
public:
    Player(std::shared_ptr<sf::Texture> playerTexture, sf::Vector2f pos) :
        m_initPos(pos),
        m_jump(-300.0f),
        m_speed(100.0f, 0),
        m_rot(0),
        m_gravity(0.3f),
        m_max(300.0f)
    {
        m_sprite.setTexture(*playerTexture,true);
        m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f,
                            m_sprite.getLocalBounds().height / 2.0f);
        m_sprite.setPosition(pos);
    }

    void Reset()
    {
        m_sprite.setPosition(m_initPos);
        m_sprite.setRotation(0);
    }

    void HandleInput(sf::Event event)
    {
        if (FBInput::IsKeyDown(event, sf::Keyboard::Space))
        {
            m_speed.y = m_jump;
        }
    }

    void Update(float dt)
    {
        m_sprite.move(m_speed * dt);

        if (m_speed.y < m_max)
            m_speed.y += m_gravity;
        else
            m_speed.y = m_max;

        // angle range from (-45 deg to 90 deg )
        // speed range from -MAX to MAX
        // 0.8 is to smooth out the rotation
        m_rot = -45 + (135 / (m_max * 2) * (m_speed.y + m_max))*0.8;

        m_sprite.setRotation(m_rot);
    };

    void Draw(sf::RenderWindow& renderWindow)
    {
        renderWindow.draw(m_sprite);
    };

    const auto& GetPosition() const
    {
        return m_sprite.getPosition();
    }


    const sf::FloatRect& GetGlobalBounds() const
    {
        auto bound = m_sprite.getGlobalBounds();
        return bound;
    }
private:
    sf::Sprite m_sprite;
    sf::Vector2f m_speed;
    sf::Vector2f m_initPos;
    float m_rot;
    float m_jump;
    float m_gravity;
    float m_max;
};



