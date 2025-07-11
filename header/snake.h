//
// Created by emmit on 23/4/2025.
//

#pragma once

#include "constants.h"
#include <vector>
#include <memory>
#include <unordered_map>

class Snake {
public:
    Snake(bool player = false);

    void setTextures(const Texture& head, const Texture& body,
                     const Texture& pHead, const Texture& pBody);
    void move(Vector2i newHead);
    void grow(Vector2i newHead);
    void shrink();
    bool checkPoison(const unordered_map<Vector2i, int, Vector2iHash>& poison);
    void draw(RenderWindow& window, int tileSize);

    vector<Vector2i> body;
    Direction dir;
    int level;
    bool isPlayer;

private:
    bool isPoisoned;
    Texture headTexture;
    Texture bodyTexture;
    Texture poisonedHeadTexture;
    Texture poisonedBodyTexture;
};