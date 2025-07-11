//
// Created by emmit on 23/4/2025.
//

#include "../header/poison.h"
#include <iostream>

Poison::Poison() {
    poisonTextures.resize(15);
    for(int i=0;i<15;i++){
        string number=to_string(i);
        string url="C:/Users/emmit/CLionProjects/GreedySnake/assets/poison/poison"+number+".png";
        if (!poisonTextures[i].loadFromFile(url)){
            std::cout<<"Error loading image!"<<endl;
        }
    }
}

void Poison::spawn(int mapWidth, int mapHeight, int snakeSize, int score, int spreadCount){
    int maxRadius = min(min(MAP_HEIGHT/3,MAP_WIDTH/3),snakeSize);
    int poisonRadius = rand() % (maxRadius/2) +1;
    int poisonCenterX = rand() % (MAP_WIDTH-2) +1;
    int poisonCenterY = rand() % (MAP_HEIGHT-2) +1;
    int poisonDuration = rand() % 50 + score/10 + poisonRadius*5;
    for(int i=-poisonRadius;i<=poisonRadius;i++){
        for(int j=-poisonRadius;j<=poisonRadius;j++){
            Vector2i newPoison;
            newPoison.x = poisonCenterX + i;
            newPoison.y = poisonCenterY + j;
            if(newPoison.x>0 && newPoison.x<MAP_WIDTH-1 && newPoison.y>0 && newPoison.y<MAP_HEIGHT-1
               && poisonMap[newPoison]<=0) {
                poisonMap[newPoison] = poisonDuration;
                spreadCount++;
            }
        }
    }
    if(spreadCount<maxRadius*maxRadius)
        spawn(mapWidth,mapHeight,snakeSize,score,spreadCount);
}

void Poison::update(){
    for(auto& x: poisonMap){
        x.second--;
    }
}

void Poison::draw(RenderWindow& window, int tileSize){
    RectangleShape poisonTile(Vector2f(TILE_SIZE-2, TILE_SIZE-2));
    for(auto x: poisonMap){
        if(x.second>0) {
            poisonTile.setPosition(x.first.x * TILE_SIZE + 1, x.first.y * TILE_SIZE + 1);
            poisonTile.setTexture(&poisonTextures[x.second%15]);
            //poisonTile.setFillColor(Color(128, 64, 255, 64 * sin(x.second * M_PI / 9) +128));//* (foodPos[x.first]?2:1) + 128));
            window.draw(poisonTile);
        }
    }
}

unordered_map<Vector2i, int, Vector2iHash>& Poison::getPoisonMap(){
    return poisonMap;
}

void Poison::clear(){
    for(auto& x:poisonMap){
        x.second=0;
    }
}