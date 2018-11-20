#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

#include <iostream>

#include "FBPlayer.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window/Event.hpp"
#include "FBBase.h"


Base::Base(std::shared_ptr<sf::Texture> baseTexture, sf::Vector2f pos)
    {
        m_primary = std::unique_ptr<sf::Sprite>(new sf::Sprite(*baseTexture));
        m_initPosL = pos;
        m_primary->setPosition(m_initPosL);
        m_primary->setTextureRect(
            sf::IntRect(0, 0, m_primary->getTextureRect().width * 2, m_primary->getTextureRect().height));

        m_secondary = std::unique_ptr<sf::Sprite>(new sf::Sprite(*baseTexture));
        m_initPosR = sf::Vector2f(pos.x + m_primary->getTextureRect().width, pos.y);
        m_secondary->setPosition(m_initPosR);
        m_secondary->setTextureRect(
            sf::IntRect(0, 0, m_secondary->getTextureRect().width * 2, m_secondary->getTextureRect().height));
    }

    void Base::Reset()
    {
        m_primary->setPosition(m_initPosL);
        m_secondary->setPosition(m_initPosR);
    }
    void Base::Update(const sf::View& view)
    {
        float y1 = m_primary->getPosition().x + m_primary->getTextureRect().width;
        float y2 = m_secondary->getPosition().x + m_secondary->getTextureRect().width;

        if(y1 < view.getCenter().x - view.getSize().x / 2)
        {
            m_primary->setPosition(sf::Vector2f(m_secondary->getPosition().x + m_secondary->getTextureRect().width,
                                                m_primary->getPosition().y));
        }
        else if(y2 < view.getCenter().x - view.getSize().x / 2)
        {
            m_secondary->setPosition(sf::Vector2f(m_primary->getPosition().x + m_primary->getTextureRect().width,
                                                  m_secondary->getPosition().y));
        }
    }

    void Base::Draw(sf::RenderWindow& renderWindow)
    {
        renderWindow.draw(*m_primary);
        renderWindow.draw(*m_secondary);
    };

    bool Base::CheckCollision(const Player& player)
    {
        auto bound = player.GetGlobalBounds();
        if(m_primary->getGlobalBounds().intersects(bound))
        {

            return true;
        }

        if(m_secondary->getGlobalBounds().intersects(bound))
        {

            return true;
        }

        return false;
    }

