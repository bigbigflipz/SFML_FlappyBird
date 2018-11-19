#pragma once

#include "FBBase.h"
#include "FBPipe.h"
#include "FBPlayer.h"
#include "FBTextureCreator.h"
#include "FBUI.h"
#include "FBInput.h"

#include <string>
#include <iostream>
#include <fstream>
#include "FBGame.h"

FBGame::FBGame() :
    m_textureMgr(nullptr),
    m_baseMgr(nullptr),
    m_pipeMgr(nullptr),
    m_player(nullptr),
    m_scoreUI(nullptr),
    m_gameStart(false),
    m_gameoverDelay(1)
{}

void FBGame::Init(sf::RenderWindow* window)
{
    std::string filename = "../resources/savefile/highscore.txt";
    std::ifstream m_fileStream(filename);

    if (!m_fileStream.is_open() && !m_fileStream.good())
    {
        std::cout << "failed to open " << filename << '\n';
    }
    else
    {
        m_fileStream >> m_highScore;
    }


    if (nullptr == m_textureMgr)
        m_textureMgr = std::unique_ptr<TextureCreator>(new TextureCreator());

    auto messageTexture = m_textureMgr->CreateTexture("../resources/images/sprites/message.png");
    auto bgTexture = m_textureMgr->CreateTexture("../resources/images/sprites/background-night.png");
    auto baseTexture = m_textureMgr->CreateTexture("../resources/images/sprites/base.png", true);
    auto pipeTexture = m_textureMgr->CreateTexture("../resources/images/sprites/pipe-green.png");
    auto birdTexture = m_textureMgr->CreateTexture("../resources/images/sprites/redbird-midflap.png");
    auto gameoverTexture = m_textureMgr->CreateTexture("../resources/images/sprites/gameover.png");
    auto overlayTexture = m_textureMgr->CreateTexture("../resources/images/sprites/base.png");
    auto zero = m_textureMgr->CreateTexture("../resources/images/sprites/0.png");
    auto one = m_textureMgr->CreateTexture("../resources/images/sprites/1.png");
    auto two = m_textureMgr->CreateTexture("../resources/images/sprites/2.png");
    auto three = m_textureMgr->CreateTexture("../resources/images/sprites/3.png");
    auto four = m_textureMgr->CreateTexture("../resources/images/sprites/4.png");
    auto five = m_textureMgr->CreateTexture("../resources/images/sprites/5.png");
    auto six = m_textureMgr->CreateTexture("../resources/images/sprites/6.png");
    auto seven = m_textureMgr->CreateTexture("../resources/images/sprites/7.png");
    auto eight = m_textureMgr->CreateTexture("../resources/images/sprites/8.png");
    auto nine = m_textureMgr->CreateTexture("../resources/images/sprites/9.png");

    int gameHeight = window->getSize().y - baseTexture->getSize().y;

    if (nullptr == m_baseMgr)
    {
        m_baseMgr = std::unique_ptr<Base>(new Base(baseTexture, sf::Vector2f(0, gameHeight)));
    }

    if (nullptr == m_pipeMgr)
    {
        m_pipeMgr = std::unique_ptr<PipeManager>(
            new PipeManager(pipeTexture, sf::Vector2f(3 * window->getSize().x / 4, gameHeight / 2)));
    }

    if (nullptr == m_player)
    {
        m_player = std::unique_ptr<Player>(
            new Player(birdTexture, sf::Vector2f(window->getSize().x / 2, gameHeight / 2)));
    }

    if (nullptr == m_scoreUI)
    {
        m_scoreUI = std::unique_ptr<ScoreUI>(new ScoreUI(sf::Vector2f(window->getSize().x / 2, 10),
            zero,
            one,
            two,
            three,
            four,
            five,
            six,
            seven,
            eight,
            nine));
        m_scoreUI->UpdateScore(0);
    }

    //main screen title
    m_mainScreenSprite.setTexture(*messageTexture, true);
    m_mainScreenSprite.setPosition(window->getView().getCenter().x - messageTexture->getSize().x / 2, 10);
    m_bgSprite.setTexture(*bgTexture, true);

    //gameover background
    m_gameOverLay.setTexture(*overlayTexture, true);
    m_gameOverLay.setTextureRect(
        sf::IntRect(0, 0, m_gameOverLay.getTextureRect().width / 2, m_gameOverLay.getTextureRect().height * 2.5f));
    m_gameOverLay.setOrigin(m_gameOverLay.getTextureRect().width / 2, m_gameOverLay.getTextureRect().height / 2);

    //game over title
    m_overSprite.setTexture(*gameoverTexture, true);
    m_overSprite.setOrigin(gameoverTexture->getSize().x / 2, gameoverTexture->getSize().y / 2);
}

void FBGame::Reset()
{
    m_pipeMgr->Reset();
    m_player->Reset();
    m_scoreUI->Reset();
    m_baseMgr->Reset();
    m_score = 0;

    m_gameoverDelay = 1.0f;
    m_gameOver = false;
    m_gameStart = false;
}

void FBGame::HandleInput(sf::Event event)
{
    if (FBInput::IsKeyDown(event, sf::Keyboard::Space))
    {
        if (m_gameOver)
        {
            if (m_gameoverDelay < 0.f)
                Reset();
        }
        else //start game
            m_gameStart = true;
    }

    if (m_gameStart && !m_gameOver)
        m_player->HandleInput(event);
}

void FBGame::GameUpdate(sf::View* view, float dt)
{
    if (m_gameOver)
    {
        m_gameoverDelay -= dt;

        //update highscore
        m_highScore = m_highScore < m_score ? m_score : m_highScore;
        
        //center the UIs on the camera
        m_scoreUI->UpdateHighScore(m_highScore);
        m_gameOverLay.setPosition(view->getCenter().x, view->getCenter().y);
        m_overSprite.setPosition(view->getCenter().x, view->getCenter().y - view->getSize().y / 3);
    }
    else if (m_gameStart)
    {
        m_player->Update(dt);
        m_baseMgr->Update(*view);
        m_pipeMgr->Update(*view);


        //check collision
        if (m_baseMgr->CheckCollision(*m_player))
        {
            m_gameOver = true;
        }

        if (m_pipeMgr->CheckCollision(*m_player))
        {
            m_gameOver = true;
        }

        if (m_pipeMgr->CheckScored(*m_player))
        {
            m_scoreUI->UpdateScore(++m_score);
        }
    }

    //center the view port on the bird
    auto viewpos = view->getCenter();
    viewpos.x = m_player->GetPosition().x;
    view->setCenter(viewpos);

    //make the background follow the camera
    m_bgSprite.setPosition(viewpos.x - view->getSize().x / 2, viewpos.y - view->getSize().y / 2);
}

void FBGame::GameDraw(sf::RenderWindow* window)
{
    window->draw(m_bgSprite);

    m_pipeMgr->Draw(*window);
    m_baseMgr->Draw(*window);
    m_player->Draw(*window);

    //draw game over assets
    if (m_gameOver && m_gameoverDelay < 0.5f)
    {
        window->draw(m_gameOverLay);
        window->draw(m_overSprite);
        m_scoreUI->DrawGameOverScore(*window);

        //alert the player to press space
        if (m_gameoverDelay < 0.1f)
            m_scoreUI->DrawInstruction(*window);
    }
    else //draw game asset
    {
        if (m_gameStart)
            m_scoreUI->DrawScore(*window);
        else
        {
            m_scoreUI->DrawInstruction(*window);
            window->draw(m_mainScreenSprite);
        }
    }
}

void FBGame::DeInit()
{
    //save the high score
    std::ofstream m_fileStream("../resources/savefile/highscore.txt");

    if (!m_fileStream.is_open() && !m_fileStream.good())
    {
        std::cout << "failed to open " << "../resources/savefile/highscore.txt" << '\n';
    }
    else
    {
        m_fileStream << m_highScore;
    }

    m_fileStream.close();
}

