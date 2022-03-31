#include "GameDriver.h"
#include <iostream>

int main()
{
    GameDriver* game = new GameDriver();

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
            }
        }

        game->drawScreen();
    }

    return 0;
}