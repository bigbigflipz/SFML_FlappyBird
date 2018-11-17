#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Font.hpp"
#include <random>

#include "FBEntity.h"



const int WIDTH = 288;
const int HEIGHT = 512;
const float MAX = 350.0f;

bool gameStart = true;
float speed{ 0 };
float gravity{ 0.5f };
float jump{ -MAX };

static const std::string IMAGE_PATH("../resources/images/sprites/");

int main(int argc, char** arg)
{
	sf::RenderWindow renderWindow(sf::VideoMode(288, 512), "SFML Demo");

	sf::Event event;

	// A Clock starts counting as soon as it's created
	sf::Color color(sf::Color::Red);
	

	SpriteCreator spCreator;		

	auto spriteBG = spCreator.CreateSpriteObject(IMAGE_PATH + "background-night.png");
	auto spritePipe = spCreator.CreateSpriteObject(IMAGE_PATH + "pipe-green.png");
	auto spritePipe2 = spCreator.CreateSpriteObject(IMAGE_PATH + "pipe-green.png");
	spritePipe2->setPosition(3* WIDTH / 4, 3 * HEIGHT / 4);
	
	auto sprite = spCreator.CreateSpriteObject(IMAGE_PATH + "redbird-midflap.png");
	sprite->setOrigin(sprite->getLocalBounds().width / 2.0f, sprite->getLocalBounds().height / 2.0f);
	sprite->setPosition(WIDTH / 2, HEIGHT / 2);

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

	/*
	srand(time(NULL));          //seed random number generator with the current time
	auto randomNumber = rand() % 255;   //generate a random number then confine it to a value of 0 - 255.
	*/




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

	while (renderWindow.isOpen()) {

		sf::Time dt = frameTime.restart();
		sf::Vector2f position = sprite->getPosition();



		// Check for all the events that occured since the last frame.
		while (renderWindow.pollEvent(event)) {
			//Handle events here
			if (event.type == sf::Event::EventType::Closed)
				renderWindow.close();

			// Now demonstrate input via polling
			if (FBInput::IsKeyDown(event, sf::Keyboard::R))
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
					color.r = 0;
				else
					color.r = randomColorRange(randomNumbers);

			else if (FBInput::IsKeyUp(event, sf::Keyboard::G))
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
					color.g = 0;
				else
					color.g = randomColorRange(randomNumbers);
			else if (FBInput::IsKeyPress(event, sf::Keyboard::B))
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
					color.b = 0;
				else
					color.b = randomColorRange(randomNumbers);

			if (FBInput::IsKeyPress(event, sf::Keyboard::A))
			{
				sprite->setPosition(position.x - 1, position.y);
			}
			else if (FBInput::IsKeyPress(event, sf::Keyboard::D))
			{
				sprite->setPosition(position.x + 1, position.y);
			}
			
			/*Control PIPE */
			sf::Vector2f positionPipe = spritePipe->getPosition();
			if (FBInput::IsKeyPress(event, sf::Keyboard::Left))
			{
				spritePipe->setPosition(positionPipe.x - 1, positionPipe.y);
			}
			else if (FBInput::IsKeyPress(event, sf::Keyboard::Right))
			{
				spritePipe->setPosition(positionPipe.x + 1, positionPipe.y);
			}
			else if (FBInput::IsKeyPress(event, sf::Keyboard::Up))
			{
				spritePipe->setPosition(positionPipe.x, positionPipe.y - 1);
			}
			else if (FBInput::IsKeyPress(event, sf::Keyboard::Down))
			{
				spritePipe->setPosition(positionPipe.x, positionPipe.y + 1);
			}


			if (FBInput::IsKeyDown(event, sf::Keyboard::Space))
			{
				speed = jump;
				if (!gameStart)
				{
					gameStart = true;
				}

			}
		}

		float fRot = 0;

		if (gameStart)
		{
			position.y += speed * dt.asSeconds();
			sprite->setPosition(position);
			if (speed < MAX)
				speed += gravity;
			else
				speed = MAX;


			// angle range from (-45 deg to 90 deg )
			// speed range from -MAX to MAX
			// 0.8 is to smooth out the rotation
			fRot = -45 + (135 / (MAX*2) * (speed+MAX))*0.8;

			sprite->setRotation(fRot);

			if (spritePipe->getGlobalBounds().intersects(spritePipe2->getGlobalBounds()))
			{
				std::cout << "HIT! PIPE & PIPE2\n";
			}
			if (spritePipe->getGlobalBounds().intersects(sprite->getGlobalBounds()))
			{
				std::cout << "HIT! PIPE & BIRD\n";
			}
			

		}
		txtFPS.setString("FPS:" + std::to_string(fRot));
		txtSpeed.setString("Speed: " +  std::to_string(speed));
		txtPos.setString("Pos: " + std::to_string(position.x) + "," + std::to_string(position.y));

		renderWindow.clear(color);
		renderWindow.draw(*spriteBG);
		renderWindow.draw(*spritePipe);
		renderWindow.draw(*spritePipe2);
		renderWindow.draw(*sprite);
		renderWindow.draw(txtFPS);
		renderWindow.draw(txtSpeed);
		renderWindow.draw(txtPos);
		renderWindow.display();
	}
}