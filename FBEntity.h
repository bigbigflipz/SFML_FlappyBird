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


class PipeTrap
{
public:
	PipeTrap(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos):
		m_center(pos),
		m_active(false)
	{
		m_top = std::unique_ptr<sf::Sprite>(new sf::Sprite(*pipeTexture));
		m_top->setOrigin(m_top->getOrigin().x + m_top->getGlobalBounds().width, m_top->getOrigin().y);
		m_top->rotate(180);
		m_top->setPosition(pos.x, pos.y - 50);

		m_btm = std::unique_ptr<sf::Sprite>(new sf::Sprite(*pipeTexture));
		m_btm->setPosition(pos.x, pos.y + 50);
	}

	void Init(sf::Vector2f pos)
	{
		m_top->setPosition(pos.x, pos.y - 50);
		m_btm->setPosition(pos.x, pos.y + 50);
		m_center = pos;
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
			float y = m_top->getPosition().x + m_top->getTextureRect().width;

			if (y < view.getCenter().x - view.getSize().x / 2)
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

	bool m_active;
};

class PipeManager
{
public:
	PipeManager(std::shared_ptr<sf::Texture> pipeTexture, sf::Vector2f pos) :
		m_pipeTexture(pipeTexture),
		m_xStart(600)

	{
		sf::Vector2f spawnPos(m_xStart, pos.y);

		for (int count = 0; count < 20; ++count)
		{
			m_pipePool.emplace_back(std::shared_ptr<PipeTrap>(new PipeTrap(pipeTexture, spawnPos)));
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
				pipe->Init(m_nextPos);
				m_nextPos.x += m_pipeTexture->getSize().x * 3;
				
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

	void Draw(sf::RenderWindow& renderWindow)
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
	std::shared_ptr<sf::Texture> m_pipeTexture;
	std::deque<std::shared_ptr<PipeTrap>> m_pipePool;
	std::shared_ptr<sf::Texture> m_firstAvailable;
};

class ScrollingBackground
{
public:

	ScrollingBackground(std::shared_ptr<sf::Texture> baseTexture, sf::Vector2f pos)
	{
		m_primary = std::unique_ptr<sf::Sprite>(new sf::Sprite(*baseTexture));
		m_primary->setPosition(pos);
		m_primary->setTextureRect(sf::IntRect(0, 0, m_primary->getTextureRect().width * 2, m_primary->getTextureRect().height));
		
		m_secondary = std::unique_ptr<sf::Sprite>(new sf::Sprite(*baseTexture));
		m_secondary->setPosition(pos.x + m_primary->getTextureRect().width, pos.y);
		m_secondary->setTextureRect(sf::IntRect(0, 0, m_secondary->getTextureRect().width * 2, m_secondary->getTextureRect().height));
		m_secondary->setColor(sf::Color::Red);
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

private:
	std::unique_ptr<sf::Sprite> m_primary;
	std::unique_ptr<sf::Sprite> m_secondary;
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

class GameObject
{
public:
	typedef std::unique_ptr<sf::Sprite> uSpritePtr;

	GameObject(){}

	
protected:
	uSpritePtr m_sprite;
};


class Player : public GameObject
{
public:
	Player(std::shared_ptr<sf::Texture> playerTexture, sf::Vector2f pos) : 
		m_jump(-350.0f),
		m_speed(100.0f,0),
		m_rot(0),
		m_gravity(0.3f),
		m_max(350.0f)
	{
		m_sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*playerTexture));
		m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2.0f, 
							m_sprite->getLocalBounds().height / 2.0f);
		m_sprite->setPosition(pos);
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
		/*sf::Vector2f position = m_sprite->getPosition();

		position += m_speed * dt;
		m_sprite->setPosition(position);*/

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

private:

	sf::Vector2f m_speed;
	float m_rot;
	float m_jump;
	float m_gravity;
	float m_max;

};
