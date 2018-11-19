#pragma once

#include <cstdint>
#include <deque>

#include "FBPlayer.h"
#include "SFML/Graphics.hpp"

class PipeTrap
{
public:

    PipeTrap(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos);

    /* Initialize the Pipe Object */
    void Init(sf::Vector2f pos);

    /* Render the upper and lower pipes sprite
     * @param renderWindow window object
     */
    void Draw(sf::RenderWindow& renderWindow);

    /* Update pipe
     * @param dt delta time of the frame
     */
    void Update(sf::View& view);

    /* Toggle the pipe to become active */
    void Activate();

    /* Toggle the pipe to become inactive */
    void Deactivate();

    /* Check collision between input and pipe
     * @Player player object
     * @return true if collided, else false
     */
    bool CheckCollision(const Player& player);

    /* Check collision between input and scorehitbox
    * @Player player object
    * @return true if collided, else false
    */
    bool HitScore(const Player& player);

    /* Get PipeTrap position, center of the two pipe sprite
     * @return the position of the pipe
     */
    const sf::Vector2f& GetPosition() const;

    /* Check if pipe is active
     * @return true is active, else false
     */
    const bool IsActive() const;

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
