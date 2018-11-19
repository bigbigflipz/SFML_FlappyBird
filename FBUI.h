#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

#include "Component/FBInput.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window/Event.hpp"
#include <memory>

class ScoreUI
{
public:

    ScoreUI(sf::Vector2f pos,
            std::shared_ptr<sf::Texture> zero,
            std::shared_ptr<sf::Texture> one,
            std::shared_ptr<sf::Texture> two,
            std::shared_ptr<sf::Texture> three,
            std::shared_ptr<sf::Texture> four,
            std::shared_ptr<sf::Texture> five,
            std::shared_ptr<sf::Texture> six,
            std::shared_ptr<sf::Texture> seven,
            std::shared_ptr<sf::Texture> eight,
            std::shared_ptr<sf::Texture> nine);
            
    void Reset();
    void UpdateScore(int score);
    void UpdateHighScore(int score);
    void DrawScore(sf::RenderWindow& renderWindow);
    void DrawInstruction(sf::RenderWindow& renderWindow);
    void DrawGameOverScore(sf::RenderWindow& renderWindow);

private:

    std::vector<std::shared_ptr<sf::Texture>> numbers;
    sf::Sprite m_left;
    sf::Sprite m_right;

    sf::Sprite m_leftHS;
    sf::Sprite m_rightHS;

    sf::Vector2f m_initPos;

    sf::Font font;
    sf::Text m_score;
    sf::Text m_best;
    sf::Text m_instruction;

    bool m_doubleDigit;
    bool m_doubleDigitHS;
};
