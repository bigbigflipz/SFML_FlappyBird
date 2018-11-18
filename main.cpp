#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Font.hpp"
#include <random>

#include "FBEntity.h"



const int WIDTH = 288;
const int HEIGHT = 512;


bool gameStart = false;
float speed{ 0 };



static const std::string IMAGE_PATH("../resources/images/sprites/");

int main(int argc, char** arg)
{
	sf::RenderWindow renderWindow(sf::VideoMode(WIDTH, HEIGHT), "SFML Demo");
	sf::View view(sf::Vector2f(WIDTH/2, HEIGHT/2), sf::Vector2f(WIDTH, HEIGHT));

	sf::Event event;

	// A Clock starts counting as soon as it's created
	sf::Color color(sf::Color::Red);
	

	TextureCreator textureCreator;

	auto bgTexture = textureCreator.CreateTexture(IMAGE_PATH + "background-night.png");
	auto baseTexture = textureCreator.CreateTexture(IMAGE_PATH + "base.png", true);
	auto pipeTexture = textureCreator.CreateTexture(IMAGE_PATH + "pipe-green.png");
	auto birdTexture = textureCreator.CreateTexture(IMAGE_PATH + "redbird-midflap.png");

	float gameHeight = HEIGHT - baseTexture->getSize().y;
	ScrollingBackground scrolling(baseTexture, sf::Vector2f(0, gameHeight));

	
	PipeManager pipeMgr(pipeTexture, sf::Vector2f(3 * WIDTH / 4, gameHeight / 2));
	//PipeTrap pipeTrap(pipeTexture, sf::Vector2f( 3 * WIDTH / 4, gameHeight / 2));
	Player flappybird(birdTexture, sf::Vector2f(WIDTH/2, gameHeight /2));


	sf::Clock clock;
	sf::Clock frameTime;
	// C++ 11 way of generating a random between 0 - 255
	// mt19937 is an implementation of the Mersenne Twister pseudo random number generator
	// random_device() returns a random number to use as a seed for the mt algorithm... slow however so that's why we dont just use it for all randoms if you were wondering
	// mt results arent in a human friendly format, so we use uniform_int_distribution to "shape" the results to our range and type
	// uniform_int_distribution is a fairly "light" object.  random_device and mt19937 aren't.
	std::uniform_int_distribution<int> randomColorRange(0, 255);
	std::random_device rd;
	std::mt19937 randomNumbers(rd());

	// Pre-C++ 11 but more common way (*with issues, see:
	//https://www.reddit.com/r/programming/comments/1rnudl/quite_interesting_why_cs_rand_is_considered/
	// Mostly doesn't apply to game devs if not using rand for say... security.

	
	srand(time(NULL));          //seed random number generator with the current time
	auto randomNumber = rand() % 255;   //generate a random number then confine it to a value of 0 - 255.
	




	sf::Font font;
	if (!font.loadFromFile("../resources/fonts/arial.ttf"))
	{
		// error...
		std::cout << "NO FONT LA!";
	}

	sf::Text txtPos;
	// select the font
	txtPos.setFont(font); // font is a sf::Font
	txtPos.setPosition(0, 0);
	// set the character size
	txtPos.setCharacterSize(24); // in pixels, not points!
	// set the color
	txtPos.setFillColor(sf::Color::White);
	// set the text style
	txtPos.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::Text txtFPS;
	// select the font
	txtFPS.setFont(font); // font is a sf::Font
	txtFPS.setPosition(0, 40);
	// set the character size
	txtFPS.setCharacterSize(24); // in pixels, not points!
	// set the color
	txtFPS.setFillColor(sf::Color::White);
	// set the text style
	txtFPS.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::Text txtSpeed;
	// select the font
	txtSpeed.setFont(font); // font is a sf::Font
	txtSpeed.setPosition(0, 80);
	// set the character size
	txtSpeed.setCharacterSize(24); // in pixels, not points!
	// set the color
	txtSpeed.setFillColor(sf::Color::White);
	// set the text style
	txtSpeed.setStyle(sf::Text::Bold | sf::Text::Underlined);

	while (renderWindow.isOpen()) 
	{

		sf::Time dt = frameTime.restart();
			   		 	  
		// Check for all the events that occured since the last frame.
		while (renderWindow.pollEvent(event)) {
			//Handle events here
			if (event.type == sf::Event::EventType::Closed)
				renderWindow.close();


			flappybird.HandleInput(event);

			if (FBInput::IsKeyDown(event, sf::Keyboard::Space))
			{
				gameStart = true;
			}
	
		}
			   
		if (gameStart)
		{
			flappybird.Update(dt.asSeconds());
			scrolling.Update(view);
			pipeMgr.Update(view);
			/*if (spritePipe->getGlobalBounds().intersects(spritePipe2->getGlobalBounds()))
			{
				std::cout << "HIT! PIPE & PIPE2\n";
			}*/
		}



		auto viewpos = view.getCenter();
		viewpos.x = flappybird.GetPosition().x;
		view.setCenter(viewpos);
		//spriteBG->setPosition(viewpos);

		txtFPS.setPosition(view.getCenter());
		txtFPS.setString("FPS:" + std::to_string(flappybird.GetPosition().x));

		/*
		txtFPS.setString("FPS:" + std::to_string(fRot));
		txtSpeed.setString("Speed: " +  std::to_string(speed));
		txtPos.setString("Pos: " + std::to_string(position.x) + "," + std::to_string(position.y));*/


		renderWindow.clear(color);
	//	renderWindow.draw(*spriteBG);
		renderWindow.draw(txtFPS);
		renderWindow.draw(txtSpeed);
		renderWindow.draw(txtPos);

		pipeMgr.Draw(renderWindow);
		scrolling.Draw(renderWindow);
		flappybird.Draw(renderWindow);
		
		renderWindow.setView(view);
		renderWindow.display();
	}
}