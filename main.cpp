#include "FBGame.h"

const int WIDTH = 288;
const int HEIGHT = 512;

int main(int argc, char** arg)
{
    sf::RenderWindow renderWindow(sf::VideoMode(WIDTH, HEIGHT), "SFML Demo");
    sf::View view(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT));
    sf::Event event;

    FBGame FlappyBird;
    FlappyBird.Init(&renderWindow);

    sf::Clock frameTime;

    while (renderWindow.isOpen())
    {
        sf::Time dt = frameTime.restart();

        // Check for all the events that occured since the last frame.
        while (renderWindow.pollEvent(event)) 
        {
            //Handle events here
            if (event.type == sf::Event::EventType::Closed)
            {
                FlappyBird.DeInit();
                renderWindow.close();
            }

            FlappyBird.HandleInput(event);
        }

        FlappyBird.GameUpdate(&view, dt.asSeconds());

        renderWindow.clear(sf::Color::Cyan);

        FlappyBird.GameDraw(&renderWindow);

        renderWindow.setView(view);
        renderWindow.display();
    }

}