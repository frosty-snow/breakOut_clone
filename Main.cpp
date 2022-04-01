#include "GameDriver.h"
#include <iostream>

int main()
{
    GameDriver* game = new GameDriver();
    sf::Clock clock;

    while (game->window.isOpen()) {
        sf::Event event;
        while (game->window.pollEvent(event)) {
            // Escape key pressed - exit game
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) &&
                (event.key.code == sf::Keyboard::Escape)))
            {
                game->window.close();
                break;
            }
            // Space key pressed - start game
            if (((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)))
            {
                game->init_Game();
                game->state.screen = playing;
                clock.restart();
            }
        }
        float deltaTime = clock.restart().asSeconds();

        if (game->state.screen == playing)
        {
            game->handleInput(deltaTime);
            game->simulatePhysics(deltaTime);
        }

        game->renderScreen();
    }


    // so would need master thread that would manage game time
    // first thread would simulate physics
    // second thread would render the screen
    // third thread that handles user input?

    return EXIT_SUCCESS;
}