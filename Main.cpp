#include "GameDriver.h"

int main()
{
    GameDriver* game = new GameDriver();

    while (game->window.isOpen()) {
        sf::Event event;
        while (game->window.pollEvent(event)) {
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) &&
                (event.key.code == sf::Keyboard::Escape)))
            {
                game->window.close();
                break;
            }
        }
        game->drawScreen();
    }

    return 0;
}