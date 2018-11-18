#pragma once

#include <cstdint>
#include <unordered_map>
#include <functional>

#include "SFML/Window/Event.hpp"
#include "Component/FBInput.h"
#include "SFML/Graphics.hpp"
#include <deque>

typedef void(*callback_function)(void);


enum PlayerStateType
{
    enALIVE,
    enDEAD,
};

class GameObject
{
public:
    typedef std::unique_ptr<sf::Sprite> uSpritePtr;

    GameObject() {}


protected:
    uSpritePtr m_sprite;
};


class Player : public GameObject
{
public:
    Player(std::shared_ptr<sf::Texture> playerTexture, sf::Vector2f pos) :
        m_initPos(pos),
        m_jump(-350.0f),
        m_speed(100.0f, 0),
        m_rot(0),
        m_gravity(0.4f),
        m_max(350.0f)
    {
        m_sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*playerTexture));
        m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2.0f,
                            m_sprite->getLocalBounds().height / 2.0f);
        m_sprite->setPosition(pos);
    }

    void Reset()
    {
        m_sprite->setPosition(m_initPos);
        m_sprite->setRotation(0);
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
        m_sprite->move(m_speed * dt);

        if (m_speed.y < m_max)
            m_speed.y += m_gravity;
        else
            m_speed.y = m_max;

        // angle range from (-45 deg to 90 deg )
        // speed range from -MAX to MAX
        // 0.8 is to smooth out the rotation
        m_rot = -45 + (135 / (m_max * 2) * (m_speed.y + m_max))*0.8;

        m_sprite->setRotation(m_rot);
    };

    void Draw(sf::RenderWindow& renderWindow)
    {
        renderWindow.draw(*m_sprite);
    };

    const auto& GetPosition() const
    {
        return m_sprite->getPosition();
    }


    const sf::FloatRect& GetGlobalBounds() const
    {
        auto bound = m_sprite->getGlobalBounds();
        return bound;
    }
private:

    sf::Vector2f m_speed;
    sf::Vector2f m_initPos;
    float m_rot;
    float m_jump;
    float m_gravity;
    float m_max;

};

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
            std::shared_ptr<sf::Texture> nine):
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

     

        if (!font.loadFromFile("../resources/fonts/FlappyBirdy.ttf"))
        {
            // error...
            std::cout << "NO FONT LA!";
        }

        // select the font
        m_score.setFont(font); // font is a sf::Font
        m_score.setString("Score");
        m_score.setFillColor(sf::Color::Red);
        m_score.setCharacterSize(60);

        m_best.setFont(font); // font is a sf::Font
        m_best.setString("Best");
        m_best.setFillColor(sf::Color::Red);
        m_best.setCharacterSize(60);


    }

    void Reset()
    {
        m_doubleDigit = false;
        UpdateScore(0);
    }


    void UpdateScore(int score)
    {
        if (score < 10)
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

    void UpdateHighScore(int score)
    {
        if (score < 10)
        {
            m_leftHS.setTexture(*numbers[score], true);
        }
        else
        {
            m_doubleDigitHS = true;


            int right = score % 10;
            int left = score / 10 % 10;

            m_leftHS.setTexture(*numbers[left], true);
            m_leftHS.setColor(sf::Color::Black);
            m_rightHS.setTexture(*numbers[right], true);
            m_rightHS.setColor(sf::Color::Black);
        }

    }

    void DrawScore(sf::RenderWindow& renderWindow)
    {
        auto x = renderWindow.getView().getCenter().x;
        auto leftSizeX = m_left.getTexture()->getSize().x;
        if (!m_doubleDigit)
        {
            //center the font
            m_left.setPosition(x - leftSizeX / 2 , m_initPos.y);
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

    void DrawGameOverScore(sf::RenderWindow& renderWindow)
    {
        auto x = renderWindow.getView().getCenter().x;
        auto y = renderWindow.getView().getCenter().y - 120;

        auto leftSizeX = m_left.getTexture()->getSize().x;
        
        m_score.setPosition(x-m_score.getGlobalBounds().width/2, y);
        renderWindow.draw(m_score);

        y += 80;

        if (!m_doubleDigit)
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

        if (!m_doubleDigitHS)
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

    bool m_doubleDigit;
    bool m_doubleDigitHS;
};

class PipeTrap
{
public:
    PipeTrap(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos):
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

    void Init(sf::Vector2f pos)
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

    void Draw(sf::RenderWindow& renderWindow)
    {
        if (m_active)
        {
            renderWindow.draw(*m_top);
            renderWindow.draw(*m_btm);
        }
    }

    void Update(sf::View& view)
    {
        if (m_active)
        {
            float rightedge = m_top->getPosition().x + m_top->getTextureRect().width;

            if (rightedge < view.getCenter().x - view.getSize().x / 2)
            {
                Deactivate();
            }

            //m_btm->move(0, -0.01f);
        }
    }

    void Activate()
    {
        m_active = true;
    }

    void Deactivate()
    {
        m_active = false;
    }

    bool CheckCollision(const Player& player)
    {
        auto bound = player.GetGlobalBounds();

        if (m_top->getGlobalBounds().intersects(bound))
        {
            //std::cout << "HIT! TOP\n";
            return true;
        }
            
        if (m_btm->getGlobalBounds().intersects(bound))
        {
            //std::cout << "HIT! BTM\n";
            return true;
        }

        return false;
    }

    bool HitScore(const Player& player)
    {
        auto bound = player.GetGlobalBounds();
        if(!m_scored && m_scoretarget.intersects(bound))
        {
            m_scored = true;
            return true;
        }		
        return false;
    }

    const auto GetPosition() const
    {
        return m_center;
    }

    const auto IsActive() const
    {
        return m_active;
    }
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
    PipeManager(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos) :
        m_pipeTexture(pipeTexture),
        m_xStart(600),
        m_updown(-1),
        m_middle(pos.y)

    {
        sf::Vector2f spawnPos(m_xStart, pos.y);
            
        for (int count = 0; count < 20; ++count)
        {
            auto randomNumber = rand() % 100 * m_updown;   //generate a random number then confine it to a value of 0 - 255.
            std::cout << randomNumber << "\n";
            m_updown *= -1;//flip it for next spawn
            spawnPos.y = m_middle + randomNumber;
            m_pipePool.emplace_back(std::shared_ptr<PipeTrap>(new PipeTrap(pipeTexture, spawnPos)));
            spawnPos.x += m_pipeTexture->getSize().x * 4;
        }

        m_nextPos = spawnPos;
    }

    void Reset()
    {
        sf::Vector2f spawnPos(m_xStart, m_middle);

        for(auto pipe: m_pipePool)
        {
            auto randomNumber = rand() % 100 * m_updown;   //generate a random number then confine it to a value of 0 - 255.
            std::cout << randomNumber << "\n";
            m_updown *= -1;//flip it for next spawn
            spawnPos.y = m_middle + randomNumber;
            pipe->Init(spawnPos);
            spawnPos.x += m_pipeTexture->getSize().x * 3;
        }

        m_nextPos = spawnPos;
    }

    void Update(sf::View& view)
    {
        float viewRightEdge = view.getCenter().x + view.getSize().x;
        float viewLeftEdge = view.getCenter().x - view.getSize().x;

        auto iter = m_pipePool.begin();

        //check for pipe to deactivate
        while (iter != m_pipePool.end())
        {
            if ((*iter)->GetPosition().x > viewRightEdge)
            {
                //all the rest of the pipe is too far away from the view
                iter = m_pipePool.end();
                break;
            }

            if ((*iter)->GetPosition().x < viewLeftEdge)
            {
    
                //remove the left most, reinit it, push it to back
                auto pipe = m_pipePool.front();
                m_pipePool.pop_front();
                std::cout << "removed " << pipe->GetPosition().x << " " << pipe->GetPosition().y << "\n";

                pipe->Deactivate();

                sf::Vector2f spawnPos = m_nextPos;
                auto randomNumber = rand() % 100 * m_updown;   //generate a random number then confine it to a value of 0 - 255.
                std::cout << randomNumber << "\n";
                spawnPos.y = m_middle + randomNumber;
                m_updown *= -1;//flip it for next spawn
                pipe->Init(spawnPos);
                spawnPos.x += m_pipeTexture->getSize().x * 3;
                m_nextPos = spawnPos;
                
                std::cout << "emplaced " << pipe->GetPosition().x << " " << pipe->GetPosition().y << "\n";

                m_pipePool.emplace_back(pipe);
                iter = m_pipePool.begin();
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

            if (posX > viewRightEdge)
            {
                //all the pipe after is too far away from the view
                break;
            }
            else if (posX < viewRightEdge && posX >  viewLeftEdge)
            {
                //near the view
                if (!pipe->IsActive())
                {
                    pipe->Activate();
                }
                pipe->Update(view);
                
            }

            ++iter;			
        }	

    }

    bool CheckCollision(const Player& player) const
    {
        float playerRightEdge = player.GetGlobalBounds().width + player.GetPosition().x;

        for (auto pipe : m_pipePool)
        {
            //if (playerRightEdge < pipe->GetPosition().x)
            //{
            //	return false;
            //}

            if (pipe->CheckCollision(player))
            {
                return true;
            }				
        }

        return false;
    }

    bool CheckScored(const Player& player) const
    {
        float playerRightEdge = player.GetGlobalBounds().width + player.GetPosition().x;

        for (auto pipe : m_pipePool)
        {
            /*if (playerRightEdge < pipe->GetPosition().x)
            {
                return false;
            }*/

            if (pipe->HitScore(player))
            {
                return true;
            }
        }
        
        return false;
    }

    void Draw(sf::RenderWindow& renderWindow) const
    {
        for (auto pipe : m_pipePool)
        {
            pipe->Draw(renderWindow);
        }
    }

private:
    //where will the first pipe appear
    float m_xStart;
    sf::Vector2f m_nextPos;
    float m_middle;
    int m_updown; // -1 for up, 1 for down
    std::shared_ptr<sf::Texture> m_pipeTexture;
    std::deque<std::shared_ptr<PipeTrap>> m_pipePool;
    std::shared_ptr<sf::Texture> m_firstAvailable;
    


};

class Base
{
public:

    Base(std::shared_ptr<sf::Texture> baseTexture, sf::Vector2f pos)
    {
        m_primary = std::unique_ptr<sf::Sprite>(new sf::Sprite(*baseTexture));
        m_initPosL = pos;
        m_primary->setPosition(m_initPosL);
        m_primary->setTextureRect(sf::IntRect(0, 0, m_primary->getTextureRect().width * 2, m_primary->getTextureRect().height));
        
        m_secondary = std::unique_ptr<sf::Sprite>(new sf::Sprite(*baseTexture));
        m_initPosR = sf::Vector2f(pos.x + m_primary->getTextureRect().width, pos.y);
        m_secondary->setPosition(m_initPosR);
        m_secondary->setTextureRect(sf::IntRect(0, 0, m_secondary->getTextureRect().width * 2, m_secondary->getTextureRect().height));
        m_secondary->setColor(sf::Color::Red);
    }

    void Reset()
    {
        m_primary->setPosition(m_initPosL);
        m_secondary->setPosition(m_initPosR);
    }
    void Update(const sf::View& view)
    {
        float y1 = m_primary->getPosition().x + m_primary->getTextureRect().width;
        float y2 = m_secondary->getPosition().x + m_secondary->getTextureRect().width;

        if (y1 < view.getCenter().x - view.getSize().x / 2)
        {
            m_primary->setPosition(sf::Vector2f(m_secondary->getPosition().x + m_secondary->getTextureRect().width, m_primary->getPosition().y));
        }
        else if (y2 < view.getCenter().x - view.getSize().x / 2)
        {
            m_secondary->setPosition(sf::Vector2f(m_primary->getPosition().x + m_primary->getTextureRect().width, m_secondary->getPosition().y));
        }
    }

    void Draw(sf::RenderWindow& renderWindow)
    {
        renderWindow.draw(*m_primary);
        renderWindow.draw(*m_secondary);
    };
    
    bool CheckCollision(const Player& player)
    {
        auto bound = player.GetGlobalBounds();
        if (m_primary->getGlobalBounds().intersects(bound))
        {
            std::cout << "HIT! Floor\n";
            return true;
        }
        
        if (m_secondary->getGlobalBounds().intersects(bound))
        {
            std::cout << "HIT! Floor\n";
            return true;
        }

        return false;
    }

private:
    std::unique_ptr<sf::Sprite> m_primary;
    std::unique_ptr<sf::Sprite> m_secondary;

    sf::Vector2f m_initPosL;
    sf::Vector2f m_initPosR;
};

class TextureCreator
{
public:
    TextureCreator() {};

    TextureCreator(const TextureCreator& rhs) = delete;
    TextureCreator& operator=(const TextureCreator& rhs) = delete;

    TextureCreator(TextureCreator&& rhs) = delete;
    TextureCreator& operator=(TextureCreator& rhs) = delete;

    std::shared_ptr<sf::Texture> CreateTexture(std::string path, bool repeatTexture = false)
    {
        auto itr = mapTexture.find(path);

        //if the texture already exist
        if (itr != mapTexture.end())
        {
            return itr->second;
        }
        else
        {
            sf::Texture texture;
            texture.loadFromFile(path);
            texture.setRepeated(repeatTexture);
            mapTexture[path] = std::make_shared<sf::Texture>(texture);
            return mapTexture[path];
        }
    }

private:

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mapTexture;

};

//class SpriteCreator
//{
//public:
//
//	SpriteCreator(){};
//
//	SpriteCreator(const SpriteCreator& rhs) = delete;
//	SpriteCreator& operator=(const SpriteCreator& rhs) = delete;
//
//	SpriteCreator(SpriteCreator&& rhs) = delete;
//	SpriteCreator& operator=(SpriteCreator& rhs) = delete;
//
//	std::unique_ptr<sf::Sprite> CreateSpriteObject(std::string path, bool repeatTexture = false)
//	{
//		auto texture = CreateTexture(path);
//		texture->setRepeated(repeatTexture);
//		sf::Sprite sprite(*texture);
//		return std::make_unique<sf::Sprite>(sprite);
//	}
//
//
//private:
//	std::shared_ptr<sf::Texture> CreateTexture(std::string path)
//	{
//		auto itr = mapTexture.find(path);
//
//		//if the texture already exist
//		if (itr != mapTexture.end())
//		{
//			return itr->second;
//		}
//		else
//		{
//			sf::Texture texture;
//			texture.loadFromFile(path);
//			mapTexture[path] = std::make_shared<sf::Texture>(texture);
//			return mapTexture[path];
//		}
//
//	}
//	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mapTexture;
//};


