#pragma once

#include "FBInput.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window/Event.hpp"
#include <memory>
#include "FBPlayer.h"

    Player::Player(std::shared_ptr<sf::Texture> playerTexture, sf::Vector2f pos) :
            m_initPos(pos),
            m_jump(-250.0f),
            m_speed(100.0f, 0),
            m_rot(0),
            m_gravity(20.f),
            m_max(250.0f)
    {
        m_sprite.setTexture(*playerTexture, true);
        m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);
        m_sprite.setPosition(pos);
    }

    void Player::Reset()
    {
        m_sprite.setPosition(m_initPos);
        m_sprite.setRotation(0);
    }

    void Player::HandleInput(sf::Event event)
    {
        if(FBInput::IsKeyDown(event, sf::Keyboard::Space))
        {
            m_speed.y = m_jump;
        }
    }

    void Player::Update(float dt)
    {
        m_sprite.move(m_speed * dt);

        if(m_speed.y < m_max)
            m_speed.y += m_gravity;
        else
            m_speed.y = m_max;

        // angle range from (-45 deg to 90 deg )
        // speed range from -MAX to MAX
        // 0.8 is to smooth out the rotation
        m_rot = -45 + (135 / (m_max * 2) * (m_speed.y + m_max)) * 0.8;

        m_sprite.setRotation(m_rot);
    };

    void Player::Draw(sf::RenderWindow& renderWindow) const
    {
        renderWindow.draw(m_sprite);
    };

    const sf::Vector2f& Player::GetPosition() const
    {
        return m_sprite.getPosition();
    }


    const sf::FloatRect& Player::GetGlobalBounds() const
    {
        auto bound = m_sprite.getGlobalBounds();
        return bound;
    }
