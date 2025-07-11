//
// Created by emmit on 23/4/2025.
//

#pragma once

#include "constants.h"
#include <unordered_map>
#include <vector>

class Poison {
public:
    Poison();

    void spawn(int mapWidth, int mapHeight, int snakeSize, int score, int spreadCount);
    void update();
    void draw(RenderWindow& window, int tileSize);
    unordered_map<Vector2i, int, Vector2iHash>& getPoisonMap();

    void clear();

    int maxDuration;
    int maxSpread;

private:
    unordered_map<Vector2i, int, Vector2iHash> poisonMap;
    vector<Texture> poisonTextures;
};