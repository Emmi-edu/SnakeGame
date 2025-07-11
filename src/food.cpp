//
// Created by emmit on 23/4/2025.
//

#include "../header/food.h"
#include <iostream>

Food::Food() :cnt(1){
    if (!foodTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/food.png")) {
        std::cout << "Error loading image!" << std::endl;
    }
}

Vector2i Food::spawnPos(int mapWidth, int mapHeight,
                  const vector<Vector2i>& snakes,
                  unordered_map<Vector2i, int, Vector2iHash>& poisonMap){
    Vector2i nextFood;
    bool valid;
    do {
        valid = true;
        nextFood.x = rand() % (mapWidth-4) +2;
        nextFood.y = rand() % (mapHeight-4)+2;

        if(foodMap[nextFood] || poisonMap[nextFood]>0){
            valid=false;
            continue;
        }

        // 检查是否与蛇身重叠
        for(auto& snake: snakes) {
            if (nextFood == snake) {
                valid = false;
                break;
            }
        }
    } while(!valid);
    return nextFood;
}

void Food::spawn(Vector2i newFood) {
    foodMap[newFood] = true;
    cnt++;
}

bool Food::checkCollision(const Vector2i& position) {
    if (foodMap[position]) {
        foodMap[position] = false;
        cnt--;
        return true;
    }
    return false;
}

void Food::draw(RenderWindow& window, int tileSize){
    RectangleShape foodTile(Vector2f(TILE_SIZE-2, TILE_SIZE-2));
    foodTile.setTexture(&foodTexture);
    for(auto& x: foodMap){
        if(x.second) {
            foodTile.setPosition(x.first.x * TILE_SIZE + 1, x.first.y * TILE_SIZE + 1);
            window.draw(foodTile);
        }
    }
}

int Food::getCount(){
    return cnt;
}

void Food::clear(){
    cnt=0;
    for(auto& x:foodMap){
        x.second=false;
    }
}