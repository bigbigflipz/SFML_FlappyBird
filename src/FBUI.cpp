#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

#include "FBInput.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window/Event.hpp"
#include "FBUI.h"


ScoreUI::ScoreUI(sf::Vector2f pos,
        std::shared_ptr<sf::Texture> zero,
        std::shared_ptr<sf::Texture> one,
        std::shared_ptr<sf::Texture> two,
        std::shared_ptr<sf::Texture> three,
        std::shared_ptr<sf::Texture> four,
        std::shared_ptr<sf::Texture> five,
        std::shared_ptr<sf::Texture> six,
        std::shared_ptr<sf::Texture> seven,
        std::shared_ptr<sf::Texture> eight,
        std::shared_ptr<sf::Texture> nine) :
        m_initPos(pos),
        m_doubleDigit(false),
        m_doubleDigitHS(false)
{
    numbers.emplace_back(zero);
    numbers.emplace_back(one);
    numbers.emplace_back(two);
    numbers.emplace_back(three);
    numbers.emplace_back(four);
    numbers.emplace_back(five);
    numbers.emplace_back(six);
    numbers.emplace_back(seven);
    numbers.emplace_back(eight);
    numbers.emplace_back(nine);


    font.loadFromFile("../resources/fonts/FlappyBirdy.ttf");

    // select the font
    m_score.setFont(font); // font is a sf::Font
    m_score.setString("Score");
    m_score.setFillColor(sf::Color::Black);
    m_score.setStyle(sf::Text::Bold);
    m_score.setCharacterSize(60);

    m_best.setFont(font); // font is a sf::Font
    m_best.setString("Best");
    m_best.setFillColor(sf::Color::Red);
    m_best.setStyle(sf::Text::Bold);
    m_best.setCharacterSize(60);

    m_instruction.setFont(font); // font is a sf::Font
    m_instruction.setString("Press Space");
    m_instruction.setFillColor(sf::Color::Red);
    m_instruction.setStyle(sf::Text::Bold);
    m_instruction.setCharacterSize(70);
    m_instruction.setOrigin(m_instruction.getGlobalBounds().width / 2, m_instruction.getGlobalBounds().height);
}

void ScoreUI::Reset()
{
    m_doubleDigit = false;
    UpdateScore(0);
}


void ScoreUI::UpdateScore(int score)
{
    if(score < 10)
    {
        m_left.setTexture(*numbers[score], true);
    }
    else
    {
        m_doubleDigit = true;

        //shape the font
        int right = score % 10;
        int left = score / 10 % 10;

        m_left.setTexture(*numbers[left], true);
        m_right.setTexture(*numbers[right], true);
    }
}

void ScoreUI::UpdateHighScore(int score)
{
    if(score < 10)
    {
        m_leftHS.setTexture(*numbers[score], true);
    }
    else
    {
        m_doubleDigitHS = true;


        int right = score % 10;
        int left = score / 10 % 10;

        m_leftHS.setTexture(*numbers[left], true);
        m_rightHS.setTexture(*numbers[right], true);
    }
}

void ScoreUI::DrawScore(sf::RenderWindow& renderWindow)
{
    auto x = renderWindow.getView().getCenter().x;
    auto leftSizeX = m_left.getTexture()->getSize().x;
    if(!m_doubleDigit)
    {
        //center the font
        m_left.setPosition(x - leftSizeX / 2, m_initPos.y);
        renderWindow.draw(m_left);
    }
    else
    {
        //shape the font
        m_left.setPosition(x - leftSizeX, m_initPos.y);
        m_right.setPosition(x, m_initPos.y);
        renderWindow.draw(m_left);
        renderWindow.draw(m_right);
    }
}

void ScoreUI::DrawInstruction(sf::RenderWindow& renderWindow)
{
    m_instruction.setPosition(renderWindow.getView().getCenter().x, renderWindow.getView().getCenter().y + 200);
    renderWindow.draw(m_instruction);
}

void ScoreUI::DrawGameOverScore(sf::RenderWindow& renderWindow)
{
    auto x = renderWindow.getView().getCenter().x;
    auto y = renderWindow.getView().getCenter().y - 120;

    auto leftSizeX = m_left.getTexture()->getSize().x;

    m_score.setPosition(x - m_score.getGlobalBounds().width / 2, y);
    renderWindow.draw(m_score);

    y += 80;

    if(!m_doubleDigit)
    {

        //center the font
        m_left.setPosition(x - leftSizeX / 2, y);
        renderWindow.draw(m_left);
    }
    else
    {
        //shape the font
        m_left.setPosition(x - leftSizeX, y);
        m_right.setPosition(x, y);
        renderWindow.draw(m_left);
        renderWindow.draw(m_right);
    }


    y += 40;

    m_best.setPosition(x - m_best.getGlobalBounds().width / 2, y);
    renderWindow.draw(m_best);

    y += 80;

    if(!m_doubleDigitHS)
    {
        m_leftHS.setPosition(x - leftSizeX / 2, y);
        renderWindow.draw(m_leftHS);
    }
    else
    {
        m_leftHS.setPosition(x - leftSizeX, y);
        m_rightHS.setPosition(x, y);
        renderWindow.draw(m_leftHS);
        renderWindow.draw(m_rightHS);
    }
}
