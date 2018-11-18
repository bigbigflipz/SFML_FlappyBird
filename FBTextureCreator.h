#pragma once

#include <unordered_map>
#include "SFML/Graphics.hpp"

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
            auto id = path + std::to_string(repeatTexture);
            mapTexture[id] = std::make_shared<sf::Texture>(texture);
            return mapTexture[id];
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mapTexture;

};

