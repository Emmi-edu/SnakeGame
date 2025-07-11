//
// Created by emmit on 23/4/2025.
//

#pragma once

#include "constants.h"
#include <unordered_map>
#include <vector>
#include <memory>

class Food {
public:
    Food();

    Vector2i spawnPos(int mapWidth, int mapHeight,
               const vector<Vector2i>& snakes,
               unordered_map<Vector2i, int, Vector2iHash>& poisonMap);
    void spawn(Vector2i newFood);
    bool checkCollision(const Vector2i& position);
    void draw(RenderWindow& window, int tileSize);
    int getCount();
    unordered_map<Vector2i, bool,Vector2iHash> getMap() { return foodMap; }

    void clear();

private:
    unordered_map<Vector2i, bool, Vector2iHash> foodMap;
    int cnt;
    Texture foodTexture;
};