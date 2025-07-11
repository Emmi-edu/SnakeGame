#include "../header/game.h"

int main() {
    srand(time(NULL));
    Game game;
    game.window.setFramerateLimit(60);

    while (game.isRunning()) {
        Event event;
        while (game.window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                game.window.close();
            }
            game.handleInput(event);
        }

        game.update();
        game.render();
    }

    return 0;
}