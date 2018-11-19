#pragma once

#include <memory>

#include "SFML/Graphics.hpp"

class Player
{
public:

    Player(std::shared_ptr<sf::Texture> playerTexture, sf::Vector2f pos);

    /* Reset the state of player to game start */
    void Reset();

    /* Handle Input Commands that are relevant to the player
     * @param event The window event
     */
    void HandleInput(sf::Event event);

    /* Update Player
     * @param dt delta time of the frame
     */
    void Update(float dt);
    
    /* Render the player sprite
     * @param renderWindow window object
     */
    void Draw(sf::RenderWindow& renderWindow) const;
    
    /* Get player sprite's anchor position
     * @return position of the player
     */
    const sf::Vector2f& GetPosition() const;

    /* Get player sprite's bounding rect
     * @return bounding rect of player
     */
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
