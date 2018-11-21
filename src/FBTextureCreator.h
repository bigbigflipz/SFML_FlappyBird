#pragma once

#include "SFML/Graphics.hpp"
#include <unordered_map>

class TextureCreator
{
public:

    TextureCreator();

    TextureCreator(const TextureCreator& rhs) = delete;
    TextureCreator& operator=(const TextureCreator& rhs) = delete;

    TextureCreator(TextureCreator&& rhs) = delete;
    TextureCreator& operator=(TextureCreator&& rhs) = delete;

    std::shared_ptr<sf::Texture> CreateTexture(std::string path, bool repeatTexture = false);

private:

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_mapTexture;
};
