#pragma once

#include <cstdint>
#include <deque>

#include "FBInput.h"
#include "FBPlayer.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window/Event.hpp"

#include "FBPipe.h"

	PipeTrap::PipeTrap(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos) :
            m_center(pos),
            m_active(false),
            m_scored(false)
    {
        m_top = std::unique_ptr<sf::Sprite>(new sf::Sprite(*pipeTexture));
        m_top->setOrigin(m_top->getOrigin().x + m_top->getGlobalBounds().width, m_top->getOrigin().y);
        m_top->rotate(180);
        m_top->setPosition(pos.x, pos.y - 60);

        m_btm = std::unique_ptr<sf::Sprite>(new sf::Sprite(*pipeTexture));
        m_btm->setPosition(pos.x, pos.y + 60);

        m_scoretarget.left = pos.x + m_btm->getGlobalBounds().width;
        m_scoretarget.top = 0;
        m_scoretarget.width = 10;
        m_scoretarget.height = 1000;
    }

    void PipeTrap::Init(sf::Vector2f pos)
    {
        m_top->setPosition(pos.x, pos.y - 50);
        m_btm->setPosition(pos.x, pos.y + 50);
        m_center = pos;

        m_scoretarget.left = pos.x + m_btm->getGlobalBounds().width;
        m_scoretarget.top = 0;
        m_scoretarget.width = 10;
        m_scoretarget.height = 1000;

        m_scored = false;
    }

    void PipeTrap::Draw(sf::RenderWindow& renderWindow)
    {
        if(m_active)
        {
            renderWindow.draw(*m_top);
            renderWindow.draw(*m_btm);
        }
    }

    void PipeTrap::Update(sf::View& view)
    {
        if(m_active)
        {
            float rightedge = m_top->getPosition().x + m_top->getTextureRect().width;

            if(rightedge < view.getCenter().x - view.getSize().x / 2)
            {
                Deactivate();
            }

            //m_btm->move(0, -0.01f);
        }
    }

    void PipeTrap::Activate()
    {
        m_active = true;
    }

    void PipeTrap::Deactivate()
    {
        m_active = false;
    }

    bool PipeTrap::CheckCollision(const Player& player)
    {
        auto bound = player.GetGlobalBounds();

        if(m_top->getGlobalBounds().intersects(bound))
        {
            return true;
        }

        if(m_btm->getGlobalBounds().intersects(bound))
        {
            return true;
        }

        return false;
    }

    bool PipeTrap::HitScore(const Player& player)
    {
        auto bound = player.GetGlobalBounds();
        if(!m_scored && m_scoretarget.intersects(bound))
        {
            m_scored = true;
            return true;
        }
        return false;
    }

    const sf::Vector2f& PipeTrap::GetPosition() const
    {
        return m_center;
    }

    const bool PipeTrap::IsActive() const
    {
        return m_active;
    }


	PipeManager::PipeManager(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos) :
            m_pipeTexture(pipeTexture),
            m_xStart(400),
            m_updown(-1),
            m_middle(pos.y),
            m_gapMultiplier(4)

    {
        sf::Vector2f spawnPos(m_xStart, pos.y);

        for(int count = 0; count < 10; ++count)
        {
            auto randomNumber = rand() % 100 *
                                m_updown; //generate a random number then confine it to a value of 0 - 255.
            m_updown *= -1;               //flip it for next spawn
            spawnPos.y = m_middle + randomNumber;
            m_pipePool.emplace_back(std::shared_ptr<PipeTrap>(new PipeTrap(pipeTexture, spawnPos)));
            spawnPos.x += m_pipeTexture->getSize().x * m_gapMultiplier;
        }

        m_nextPos = spawnPos;
    }

    void PipeManager::Reset()
    {
        sf::Vector2f spawnPos(m_xStart, m_middle);
        m_gapMultiplier = 4;

        for(auto pipe : m_pipePool)
        {
            auto randomNumber = rand() % 100 *
                                m_updown; //generate a random number then confine it to a value of 0 - 255.
            m_updown *= -1;               //flip it for next spawn
            spawnPos.y = m_middle + randomNumber;
            pipe->Init(spawnPos);
            spawnPos.x += m_pipeTexture->getSize().x * m_gapMultiplier;
        }

        m_nextPos = spawnPos;
    }

    void PipeManager::Update(sf::View& view)
    {
        float viewRightEdge = view.getCenter().x + view.getSize().x;
        float viewLeftEdge = view.getCenter().x - view.getSize().x;

        auto iter = m_pipePool.begin();

        //check for pipe to deactivate
        while(iter != m_pipePool.end())
        {
            if((*iter)->GetPosition().x > viewRightEdge)
            {
                //all the rest of the pipe is too far away from the view
                iter = m_pipePool.end();
                break;
            }

            if((*iter)->GetPosition().x < viewLeftEdge)
            {

                //remove the left most, reinit it, push it to back
                auto pipe = m_pipePool.front();
                m_pipePool.pop_front();

                pipe->Deactivate();

                sf::Vector2f spawnPos = m_nextPos;

                //generate a random number then confine it to a value of 0 - 255.
                auto randomNumber = rand() % 100 * m_updown;

                spawnPos.y = m_middle + randomNumber;
                m_updown *= -1; //flip it for next spawn
                pipe->Init(spawnPos);
                spawnPos.x += m_pipeTexture->getSize().x * m_gapMultiplier;
                m_nextPos = spawnPos;

                m_pipePool.emplace_back(pipe);
                iter = m_pipePool.begin();

                if(m_gapMultiplier > 2)
                    m_gapMultiplier -= 0.1f;
            }
            else
            {
                break;
            }
        }

        while(iter != m_pipePool.end())
        {
            auto pipe = *iter;
            auto posX = pipe->GetPosition().x;

            if(posX > viewRightEdge)
            {
                //all the pipe after is too far away from the view
                break;
            }
            else if(posX < viewRightEdge && posX > viewLeftEdge)
            {
                //near the view
                if(!pipe->IsActive())
                {
                    pipe->Activate();
                }
                pipe->Update(view);
            }

            ++iter;
        }
    }

    bool PipeManager::CheckCollision(const Player& player) const
    {
        float playerRightEdge = player.GetGlobalBounds().width + player.GetPosition().x;

        for(auto pipe : m_pipePool)
        {
            //if (playerRightEdge < pipe->GetPosition().x)
            //{
            //	return false;
            //}

            if(pipe->CheckCollision(player))
            {
                return true;
            }
        }

        return false;
    }

    bool PipeManager::CheckScored(const Player& player) const
    {
        float playerRightEdge = player.GetGlobalBounds().width + player.GetPosition().x;

        for(auto pipe : m_pipePool)
        {
            /*if (playerRightEdge < pipe->GetPosition().x)
			{
				return false;
			}*/

            if(pipe->HitScore(player))
            {
                return true;
            }
        }

        return false;
    }

    void PipeManager::Draw(sf::RenderWindow& renderWindow) const
    {
        for(auto pipe : m_pipePool)
        {
            pipe->Draw(renderWindow);
        }
    }

