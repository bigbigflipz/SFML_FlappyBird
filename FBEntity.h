#pragma once

#include <cstdint>
#include <unordered_map>
#include <functional>

#include "SFML/Window/Event.hpp"
#include "Component/FBInput.h"
#include "SFML/Graphics.hpp"

typedef void(*callback_function)(void);


enum PlayerStateType
{
	enALIVE,
	enDEAD,
};

class SpriteCreator
{
public:

	SpriteCreator(){};

	SpriteCreator(const SpriteCreator& rhs) = delete;
	SpriteCreator& operator=(const SpriteCreator& rhs) = delete;

	SpriteCreator(SpriteCreator&& rhs) = delete;
	SpriteCreator& operator=(SpriteCreator& rhs) = delete;

	std::unique_ptr<sf::Sprite> CreateSpriteObject(std::string path)
	{
		auto texture = CreateTexture(path);
		sf::Sprite sprite(*texture);
		return std::make_unique<sf::Sprite>(sprite);
	}


private:
	std::shared_ptr<sf::Texture> CreateTexture(std::string path)
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
			mapTexture[path] = std::make_shared<sf::Texture>(texture);
			return mapTexture[path];
		}

	}
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mapTexture;
};

class GameObject
{
public:
	typedef std::unique_ptr<sf::Sprite> uSpritePtr;

	GameObject(uSpritePtr sprite) : m_sprite(std::move(sprite))
	{

	}
protected:
	uSpritePtr m_sprite;
};


class Player : public GameObject
{
public:
	Player(uSpritePtr playerSprite, sf::Vector2f pos) : 
		GameObject(std::move(playerSprite)),
		m_jump(-350.0f),
		m_speed(100.0f,0),
		m_rot(0),
		m_gravity(0.3f),
		m_max(350.0f)

	{
		m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2.0f, m_sprite->getLocalBounds().height / 2.0f);
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
		sf::Vector2f position = m_sprite->getPosition();

		position += m_speed * dt;
		m_sprite->setPosition(position);

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
