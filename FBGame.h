#pragma once

#include "FBBase.h"
#include "FBPipe.h"
#include "FBPlayer.h"
#include "FBTextureCreator.h"
#include "FBUI.h"

#include <string>
#include <fstream>

class FBGame
{
public:

    FBGame();

    void Init(sf::RenderWindow* window);

    void Reset();

    void HandleInput(sf::Event event);

    void GameUpdate(sf::View* view, float dt);

    void GameDraw(sf::RenderWindow* window);

    void DeInit();

private:

    //static const std::string IMAGE_PATH("../resources/images/sprites/");

    std::unique_ptr<TextureCreator> m_textureMgr;
    std::unique_ptr<Base> m_baseMgr;
    std::unique_ptr<PipeManager> m_pipeMgr;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<ScoreUI> m_scoreUI;

    sf::Sprite m_bgSprite;
    sf::Sprite m_mainScreenSprite;
    sf::Sprite m_overSprite;
    sf::Sprite m_gameOverLay;
    
    std::fstream m_fileStream;

    int m_score;
    int m_highScore;
    bool m_gameStart;
    bool m_gameOver;

    float m_gameoverDelay;
    //std::shared_ptr<sf::RenderWindow> m_window;
    //std::shared_ptr<sf::View> m_view;
};