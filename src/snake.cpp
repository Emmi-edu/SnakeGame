//
// Created by emmit on 23/4/2025.
//

#include "../header/snake.h"

Snake::Snake(bool player) : isPlayer(player), isPoisoned(false), level(0) {
    if (isPlayer) {
        body = { {5, 5}, {4,5}, {3,5} };
        dir = RIGHT;
    }
}

void Snake::setTextures(const Texture& head, const Texture& body,
                        const Texture& pHead, const Texture& pBody) {
    headTexture = head;
    bodyTexture = body;
    poisonedHeadTexture = pHead;
    poisonedBodyTexture = pBody;
}

void Snake::move(Vector2i newHead) {
    body.insert(body.begin(), newHead);
    body.pop_back();
}

void Snake::grow(Vector2i newHead) {
    body.insert(body.begin(), newHead);
}

void Snake::shrink() {
    if (body.size() > 1) {
        body.pop_back();
    }
}

bool Snake::checkPoison(const unordered_map<Vector2i, int, Vector2iHash>& poison) {
    isPoisoned = false;
    for (auto& seg : body) {
        if (poison.count(seg) && poison.at(seg) > 0) {
            isPoisoned = true;
            break;
        }
    }
    return isPoisoned;
}

void Snake::draw(RenderWindow& window, int tileSize) {
    RectangleShape tile(Vector2f(tileSize - 2, tileSize - 2));

    for (size_t i = 0; i < body.size(); i++) {
        tile.setPosition(body[i].x * tileSize + 1, body[i].y * tileSize + 1);

        if (i == 0) {
            tile.setTexture(isPoisoned ? &poisonedHeadTexture : &headTexture);
        } else {
            tile.setTexture(isPoisoned ? &poisonedBodyTexture : &bodyTexture);
        }

        window.draw(tile);
    }
}