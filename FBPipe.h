#pragma once

#include <cstdint>
#include <deque>

#include "FBPlayer.h"
#include "SFML/Graphics.hpp"

class PipeTrap
{
public:

    PipeTrap(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos);

    void Init(sf::Vector2f pos);

    void Draw(sf::RenderWindow& renderWindow);

    void Update(sf::View& view);

    void Activate();

    void Deactivate();

    bool CheckCollision(const Player& player);

    bool HitScore(const Player& player);

    const auto GetPosition() const;

    const auto IsActive() const;

private:

    std::unique_ptr<sf::Sprite> m_top;
    std::unique_ptr<sf::Sprite> m_btm;

    sf::Vector2f m_center;
    sf::FloatRect m_scoretarget;

    bool m_scored;
    bool m_active;
};

class PipeManager
{
public:

    PipeManager(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos);

    void Reset();

    void Update(sf::View& view);

    bool CheckCollision(const Player& player) const;

    bool CheckScored(const Player& player) const;

    void Draw(sf::RenderWindow& renderWindow) const;

private:
    
    float m_gapMultiplier;//controls the gap btwn pipe
    float m_xStart;//where will the first pipe appear
    float m_middle;
    int m_updown; // -1 for up, 1 for down
    sf::Vector2f m_nextPos;

    std::deque<std::shared_ptr<PipeTrap>> m_pipePool;
    
    std::shared_ptr<sf::Texture> m_pipeTexture;
    std::shared_ptr<sf::Texture> m_firstAvailable;
};
