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

class Player
{
public:
	Player(sf::Sprite playerSprite) : m_playerSprite(playerSprite)
	{}
private:

	sf::Sprite m_playerSprite;
};
