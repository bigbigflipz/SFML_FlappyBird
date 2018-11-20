#pragma once

#include <memory>

#include "SFML/Graphics.hpp"

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
    
    /* Reset the sate of UI to game start */
    void Reset();

    /* Update the Score Font to the correct number glyph
     * @param score the number to display
     */
    void UpdateScore(int score);

    /* Update the High Score Font to the correct number glyph
     * @param score the number to display
     */
    void UpdateHighScore(int score);

    /* Render the Score Font when game is running
     * @param renderWindow window to render to
     */
    void DrawScore(sf::RenderWindow& renderWindow);

    /* Render the Instruction Font
     * @param renderWindow window to render to
     */
    void DrawInstruction(sf::RenderWindow& renderWindow);

    /* Render the Score Font and High score font during game over
     * @param renderWindow window to render to
     */
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
