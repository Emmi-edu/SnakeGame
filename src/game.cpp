//
// Created by emmit on 23/4/2025.
//

#include "../header/game.h"
#include <iostream>
#include <cmath>
#include <bits/stdc++.h>

Game::Game() : window(VideoMode(MAP_WIDTH*TILE_SIZE, MAP_HEIGHT*TILE_SIZE), "Greedy Snake"),
               gameState(START),
               score(0), fastMode(false), slowMode(false), autoMode(false) {
    window.setFramerateLimit(60);

    pws=PowerUpSystem();

    // Load textures
    if (!wallTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/wall.png")) {
        std::cout << "Error loading image!" << std::endl;
    }
    if (!snakeHeadTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/snake_head.png")) {
        std::cout << "Error loading image!" << std::endl;
    }
    if (!snakeBodyTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/snake_body.png")) {
        std::cout << "Error loading image!" << std::endl;
    }
    if (!poisonedHeadTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/poisoned_head.png")) {
        std::cout << "Error loading image!" << std::endl;
    }
    if (!poisonedBodyTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/poisoned_body.png")) {
        std::cout << "Error loading image!" << std::endl;
    }
    if (!enemyHeadTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/enemy_head.png")) {
        std::cout << "Error loading image!" << std::endl;
    }
    if (!enemyBodyTexture.loadFromFile("C:/Users/emmit/CLionProjects/GreedySnake/assets/enemy_body.png")) {
        std::cout << "Error loading image!" << std::endl;
    }

    // Load font
    if (!font.loadFromFile(FONT_PATH)) {
        cerr << "Error loading font!" << endl;
    }
}

void Game::drawWall() {
    RectangleShape wall(Vector2f(TILE_SIZE, TILE_SIZE));
    //wall.setFillColor(Color(100,100,100));
    wall.setTexture(&wallTexture);

    // 上下边界
    for(int x=0; x<MAP_WIDTH; x++) {
        wall.setPosition(x*TILE_SIZE, 0);
        window.draw(wall);
        wall.setPosition(x*TILE_SIZE, (MAP_HEIGHT-1)*TILE_SIZE);
        window.draw(wall);
    }

    // 左右边界
    for(int y=1; y<MAP_HEIGHT-1; y++) {
        wall.setPosition(0, y*TILE_SIZE);
        window.draw(wall);
        wall.setPosition((MAP_WIDTH-1)*TILE_SIZE, y*TILE_SIZE);
        window.draw(wall);
    }
}

void Game::drawScore() {
    Text scoreText;
    scoreText.setFont(font);
    string showText=to_string(score);
    scoreText.setString("Score: " + showText);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(14, 12);

    Text levelText;
    levelText.setFont(font);
    showText=to_string(player->level);
    levelText.setString("Level: " + showText);
    levelText.setCharacterSize(24);
    levelText.setFillColor(Color::White);
    float textHeight;
    textHeight = scoreText.getGlobalBounds().height;
    levelText.setPosition(14, 24 + textHeight);

    Text speedMode;
    speedMode.setFont(font);
    if(autoMode) showText="Auto Mode";
    else if(slowMode) showText="Slow Mode";
    else if(fastMode) showText="Fast Mode";
    else showText="";
    speedMode.setString(showText);
    speedMode.setCharacterSize(36);
    speedMode.setFillColor(Color::White);
    float textWidth;
    textWidth = speedMode.getGlobalBounds().width/2;
    speedMode.setPosition(MAP_WIDTH*TILE_SIZE/2 - textWidth, 20);

    window.draw(scoreText);
    window.draw(levelText);
    window.draw(speedMode);
}

void Game::drawStartScreen() {
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Greedy Snake");
    titleText.setCharacterSize(48);
    titleText.setFillColor(Color::Green);
    titleText.setPosition(MAP_WIDTH*TILE_SIZE/2 - 150, MAP_HEIGHT*TILE_SIZE/2 - 100);

    Text hintText;
    hintText.setFont(font);
    hintText.setString("Press SPACE to start\nESC to exit");
    hintText.setCharacterSize(24);
    hintText.setFillColor(Color::White);
    hintText.setPosition(MAP_WIDTH*TILE_SIZE/2 - 100, MAP_HEIGHT*TILE_SIZE/2);

    RectangleShape overlay(Vector2f(MAP_WIDTH*TILE_SIZE, MAP_HEIGHT*TILE_SIZE));
    overlay.setFillColor(Color(0,0,0,180));

    window.draw(overlay);
    window.draw(titleText);
    window.draw(hintText);
}

void Game::drawGameOver() {
    RectangleShape overlay(Vector2f(MAP_WIDTH*TILE_SIZE, MAP_HEIGHT*TILE_SIZE));
    overlay.setFillColor(Color(0,0,0,180));
    Text gameOverText;
    gameOverText.setFont(font);
    string scoreText=to_string(score);
    string levelText=to_string(player->level);
    gameOverText.setString("Game Over!\nFinal Score: " + scoreText + "\nLevel: " + levelText);
    gameOverText.setCharacterSize(36);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(MAP_WIDTH*TILE_SIZE/2 - 120, MAP_HEIGHT*TILE_SIZE/2 - 50);

    window.draw(overlay);
    window.draw(gameOverText);
}

void Game::drawFow(int mode) {
    RectangleShape rec;
    rec.setSize(Vector2f(TILE_SIZE,TILE_SIZE));
    if(mode==1)
        rec.setFillColor(Color::Black);
    else if(mode==2)
        rec.setFillColor(Color(30,30,30));
    else
        rec.setFillColor((Color(60,60,60)));
    for(auto x:fow){
        if(x.second==mode){
            rec.setPosition(x.first.x*TILE_SIZE,x.first.y*TILE_SIZE);
            window.draw(rec);
        }
    }
}

void Game::init() {
    gameClock.restart();
    spawnClock.restart();
    poisonClock.restart();
    enemyClock.restart();

    score = 0;

    requiredScore=10;
    levelUpDone=0;
    multiplier=0;
    speedBoost=0;
    spawnBoost=0;
    enemyMultiplier=0;
    enemySpawnBoost=0;
    poisonSpawnBoost=0;

    fow.clear();
    for(int i=0;i<MAP_WIDTH;i++){
        for(int j=0;j<MAP_HEIGHT;j++){
            fow[{i,j}]=1;
        }
    }
    sight=1;

    spawnTime = rand() % 11 + 5;
    poisonTime = rand() % 16 + 10;
    enemyTime = rand() % 31 + 15;

    player = make_shared<Snake>(true);
    player->setTextures(snakeHeadTexture, snakeBodyTexture,
                        poisonedHeadTexture, poisonedBodyTexture);

    enemies.clear();
    for(auto &x: enemyExist) x.second=false;

    food.clear();
    Vector2i firstPos=food.spawnPos(MAP_WIDTH/2,MAP_HEIGHT/2, player->body, poison.getPoisonMap());
    food.spawn(firstPos);
    for(int i=firstPos.x-2;i<=firstPos.x+2;i++){
        for(int j=firstPos.y-2;j<=firstPos.y+2;j++)
            fow[{i,j}]=3;
    }
    
    poison.clear();

    gameState = PLAYING;
    updateFrequency = 100;
    fastMode = false;
    slowMode = false;
    autoMode = false;

    updateFow();
}

void Game::handleInput(sf::Event &event) {
    if(event.type == Event::KeyPressed) {
        switch(event.key.code) {

            case Keyboard::Escape:
                window.close();
                break;

            case Keyboard::Space:
                if(gameState == START || gameState == GAME_OVER) {
                    gameState = PLAYING;
                    init();
                }
                break;

            // 控制方向（仅在游戏中生效）
            case Keyboard::W:
                if(gameState == PLAYING && player->dir != DOWN)
                    player->dir = UP;
                autoMode = false;
                break;
            case Keyboard::S:
                if(gameState == PLAYING && player->dir != UP)
                    player->dir = DOWN;
                autoMode = false;
                break;
            case Keyboard::A:
                if(gameState == PLAYING && player->dir != RIGHT)
                    player->dir = LEFT;
                autoMode = false;
                break;
            case Keyboard::D:
                if(gameState == PLAYING && player->dir != LEFT)
                    player->dir = RIGHT;
                autoMode = false;
                break;

            case Keyboard::F:
                if(!fastMode) {
                    fastMode = true;
                    slowMode = false;
                }
                else fastMode = false;
                break;
            case Keyboard::C:
                if(!slowMode) {
                    slowMode = true;
                    fastMode = false;
                }
                else slowMode = false;
                break;

            case Keyboard::R:
                autoMode=true;
                break;

            default:
                autoMode = false;
        }
    }
}

Vector2i nextPos(Vector2i newHead,Direction dir){
    switch (dir) {
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
    }
    return newHead;
}

bool Game::playerAi(Direction dir) {
    Vector2i newHead=player->body[0];
    newHead=nextPos(newHead,dir);

    if (newHead.x <= 0 || newHead.x >= MAP_WIDTH - 1 || newHead.y <= 0 || newHead.y >= MAP_HEIGHT - 1) return false;
    if (enemyExist[newHead]) return false;
    if (poison.getPoisonMap()[newHead] > 0) return false;
    for (auto seg: player->body) {
        if (seg == newHead) return false;
    }
    return true;
}

void Game::autoSearch(){
    Vector2i foodPos={-1,-1};
    Vector2i playerPos=player->body[0];
    int chaseDis=min(MAP_HEIGHT/2, MAP_WIDTH/2);
    float foodDis;
    float smallDis=INT_MAX;
    for(auto x:food.getMap()){
        foodDis=pow(abs(playerPos.x-x.first.x),2)+pow(abs(playerPos.y-x.first.y),2);
        foodDis=pow(foodDis,0.5);
        if(x.second && (foodPos.x==-1 || foodDis<=chaseDis && foodDis<smallDis) && poison.getPoisonMap()[x.first]<=0){
            foodPos=x.first;
            smallDis=foodDis;
        }
    }

    if(playerPos.x<foodPos.x && player->dir!=LEFT && playerAi(RIGHT)) player->dir=RIGHT;
    else if(playerPos.x>foodPos.x && player->dir!=RIGHT && playerAi(LEFT)) player->dir=LEFT;
    else if(playerPos.y<foodPos.y && player->dir!=UP && playerAi(DOWN)) player->dir=DOWN;
    else if(player->dir!=DOWN && playerAi(UP)) player->dir=UP;

    else if(player->dir!=RIGHT && playerAi(LEFT)) player->dir=LEFT;
    else if(player->dir!=LEFT && playerAi(RIGHT)) player->dir=RIGHT;
    else if(player->dir!=DOWN && playerAi(UP)) player->dir=UP;
    else if(player->dir!=UP && playerAi(DOWN)) player->dir=DOWN;
}

void Game::updateFow(){
    /*for(auto&x:fow){
        if(x.second==3) x.second=2;
    }*/
    /*for(auto x:food.getMap()){
        if(x.second) //omg it is the problem all along
            fow[x.first]=2;
    }*/
    for(auto seg:player->body){
        for(int i=seg.x-sight;i<=seg.x+sight;i++){
            for(int j=seg.y-sight;j<=seg.y+sight;j++)
                if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                    fow[{i, j}] = 3; // Mark valid tiles as visible
                }
        }
    }
}

void Game::snakeCollapse(shared_ptr<Snake>& snake){
    int loop=snake->body.size();
    for(int i=0;i<loop;i++) {
        if (snake->body.back().x > 0 && snake->body.back().x < MAP_WIDTH - 1
            && snake->body.back().y > 0 && snake->body.back().y < MAP_HEIGHT - 1) {
            food.spawn(snake->body.back());
        }
        if (!snake->isPlayer) enemyExist[snake->body.back()] = false;
        snake->body.pop_back();
    }
}

void Game::showLevelUpScreen(bool rerolled) {
    int powerUp1;
    int powerUp2;
    int powerUp3;
    int powerDown1;
    int powerDown2;
    int powerDown3;

    do{
        powerUp1 = rand() % pws.powerUps.size();
    } while (powerUpCondition(pws.powerUps[powerUp1].name));
    do {
        powerUp2 = rand() % pws.powerUps.size();
    } while (powerUp1 == powerUp2 || powerUpCondition(pws.powerUps[powerUp2].name));
    do {
        powerUp3 = rand() % pws.powerUps.size();
    } while (powerUp1 == powerUp3 || powerUp2 == powerUp3 || powerUpCondition(pws.powerUps[powerUp3].name));

    do{
        powerDown1 = rand() % pws.powerDowns.size();
    } while (powerDownCondition(pws.powerDowns[powerDown1].name));
    do {
        powerDown2 = rand() % pws.powerDowns.size();
    } while (powerDown1 == powerDown2 || powerDownCondition(pws.powerDowns[powerDown2].name));
    do {
        powerDown3 = rand() % pws.powerDowns.size();
    } while (powerDown1 == powerDown3 || powerDown2 == powerDown3 || powerDownCondition(pws.powerDowns[powerDown3].name));

    pws.upcard1.setTexture(&pws.powerUps[powerUp1].texture);
    pws.upcard2.setTexture(&pws.powerUps[powerUp2].texture);
    pws.upcard3.setTexture(&pws.powerUps[powerUp3].texture);
    pws.downcard1.setTexture(&pws.powerDowns[powerDown1].texture);
    pws.downcard2.setTexture(&pws.powerDowns[powerDown2].texture);
    pws.downcard3.setTexture(&pws.powerDowns[powerDown3].texture);
    Texture rerollTexture;
    rerollTexture.loadFromFile("../assets/reroll.png");
    pws.reroll.setTexture(&rerollTexture);

    string showText;
    pws.reminder.setFont(font);
    pws.upText1.setFont(font);
    pws.upText1.setString(pws.powerUps[powerUp1].description);
    pws.upText2.setFont(font);
    pws.upText2.setString(pws.powerUps[powerUp2].description);
    pws.upText3.setFont(font);
    pws.upText3.setString(pws.powerUps[powerUp3].description);
    pws.downText1.setFont(font);
    pws.downText1.setString(pws.powerDowns[powerDown1].description);
    pws.downText2.setFont(font);
    pws.downText2.setString(pws.powerDowns[powerDown2].description);
    pws.downText3.setFont(font);
    pws.downText3.setString(pws.powerDowns[powerDown3].description);

    float textWidth;
    textWidth = pws.reminder.getGlobalBounds().width/2;
    pws.reminder.setPosition(MAP_WIDTH*TILE_SIZE/2 - textWidth, MAP_HEIGHT/5);

    window.draw(pws.overlay);
    window.draw(pws.group1);
    window.draw(pws.group2);
    window.draw(pws.group3);
    window.draw(pws.upcard1);
    window.draw(pws.upcard2);
    window.draw(pws.upcard3);
    window.draw(pws.downcard1);
    window.draw(pws.downcard2);
    window.draw(pws.downcard3);
    if(!rerolled) window.draw(pws.reroll);
    window.draw(pws.reminder);
    window.draw(pws.upText1);
    window.draw(pws.upText2);
    window.draw(pws.upText3);
    window.draw(pws.downText1);
    window.draw(pws.downText2);
    window.draw(pws.downText3);
    window.display();

    bool picked=false;
    int selectedPowerUp=-1;
    int selectedPowerDown=-1;
    bool reroll=false;
    while (window.isOpen() && !picked) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left ||
                event.type == Event::KeyPressed &&
                (event.key.code==Keyboard::Num1 ||
                event.key.code==Keyboard::Num2 ||
                event.key.code==Keyboard::Num3)
                ) {

                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                if (pws.group1.getGlobalBounds().contains(mousePos) || event.key.code==Keyboard::Num1) {
                    selectedPowerUp = powerUp1;
                    selectedPowerDown = powerDown1;
                    picked = true;
                }
                else if (pws.group2.getGlobalBounds().contains(mousePos) || event.key.code==Keyboard::Num2) {
                    selectedPowerUp = powerUp2;
                    selectedPowerDown = powerDown2;
                    picked = true;
                }
                else if (pws.group3.getGlobalBounds().contains(mousePos) || event.key.code==Keyboard::Num3) {
                    selectedPowerUp = powerUp3;
                    selectedPowerDown = powerDown3;
                    picked = true;
                }
                else if ((pws.reroll.getGlobalBounds().contains(mousePos)) && !rerolled){
                    picked = true;
                    reroll = true;
                }
            }
        }
    }

    window.clear();

    if(reroll){
        render();
        showLevelUpScreen(true);
        return;
    }
    powerDownEffect(pws.powerDowns[selectedPowerDown].name);
    powerUpEffect(pws.powerUps[selectedPowerUp].name);
    levelUpDone++;
    requiredScore = requiredScore + multiplier*10 + levelUpDone*30 + 20;
}

bool Game::powerUpCondition(string name) {
    if(name=="air_purifier"){
        for(auto x:poison.getPoisonMap()){
            if(x.second>0){
                return false;
            }
        }
        return true;
    }

    if(name=="cheat_code"){
        if(enemies.empty()) return true;
    }

    if(name=="deluxe_harvest"){
        if(enemies.empty()) return true;
    }

    if(name=="fast_bolt" || name=="rush_e"){
        if(100-speedBoost<=30)
            return true;
    }

    if(name=="greenhouse"){
        if(spawnBoost>=10){
            return true;
        }
    }

    if(name=="infiltration"){
        if(enemies.empty()) return true;
    }

    return false;
}

void Game::powerUpEffect(string name) {
    if(name=="air_purifier"){
        poison.clear();
        requiredScore-=10;
        poisonClock.restart();
    }

    if(name=="apple_pie"){
        int total = rand() % player->body.size()/2 + 5;
        Direction lastDir;
        Vector2i tail=player->body.back();
        Vector2i next=player->body[player->body.size()-2];
        if(tail.y==next.y-1) lastDir=UP;
        else if(tail.y==next.y+1) lastDir=DOWN;
        else if(tail.x==next.x-1) lastDir=LEFT;
        else lastDir=RIGHT;
        switch(lastDir){
            case UP:
                for(int i=1;i<=total;i++){
                    if(tail.y-i <= 0 || enemyExist[{tail.x,tail.y-i}])
                        break;
                    player->body.push_back({tail.x,tail.y-i});
                }
                break;
            case DOWN:
                for(int i=1;i<=total;i++){
                    if(tail.y+i >= MAP_HEIGHT-1 || enemyExist[{tail.x,tail.y+i}])
                        break;
                    player->body.push_back({tail.x,tail.y+i});
                }
                break;
            case LEFT:
                for(int i=1;i<=total;i++){
                    if(tail.x-i <= 0 || enemyExist[{tail.x-i,tail.y}])
                        break;
                    player->body.push_back({tail.x-i,tail.y});
                }
                break;
            case RIGHT:
                for(int i=1;i<=total;i++){
                    if(tail.x+i >= MAP_WIDTH-1 || enemyExist[{tail.x+i,tail.y}])
                        break;
                    player->body.push_back({tail.x+i,tail.y});
                }
                break;
        }
        score+=total*5;
        requiredScore-=10;
    }

    if(name=="cheat_code"){
        int total = rand() % enemies.size();
        if(total==0) total=1;
        for(int i=0;i<total;i++) {
            int snake = rand() % enemies.size();
            for (auto &x: enemies[snake]->body) {
                food.spawn(x);
                enemyExist[x] = false;
            }
            enemies.erase(enemies.begin() + snake);
        }
        requiredScore-=10;
    }

    if(name=="deluxe_harvest"){
        vector<Vector2i> allSnake=player->body;
        for(int i=0;enemies.empty() || i<enemies.size();i++){
            for(auto j:enemies[i]->body) allSnake.push_back(j);
        }
        int tem=score/20;
        int maxFood = rand() % (tem+1);
        if(maxFood<=6) maxFood+=6;
        for(int i=0;i<=maxFood;i++){
            food.spawn(food.spawnPos(MAP_WIDTH,MAP_HEIGHT,allSnake,poison.getPoisonMap()));
        }
        spawnClock.restart();
        spawnTime = rand() % 11;
        if(spawnTime < 5) spawnTime+=5;
        requiredScore-=10;
    }

    if(name=="fast_bolt"){
        speedBoost+=15;
    }

    if(name=="greenhouse"){
        spawnBoost++;
    }

    if(name=="il_milione"){
        for(auto seg:player->body){
            for(int i=seg.x-sight*4;i<=seg.x+sight*4;i++){
                for(int j=seg.y-sight*4;j<=seg.y+sight*4;j++)
                    if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                        fow[{i, j}] = 3; // Mark valid tiles as visible
                    }
            }
        }
        requiredScore-=10;
    }

    if(name=="infiltration"){
        for(auto& snake:enemies){
            for(auto& seg:snake->body){
                for(int x=seg.x-sight;x<=seg.x+sight;x++){
                    for(int y=seg.y-sight;y<=seg.y+sight;y++){
                        fow[{x,y}] = 3;
                    }
                }
            }
        }
        requiredScore-=10;
    }

    if(name=="marvel_of_travel"){
        sight++;
    }

    if(name=="omnivore"){
        multiplier++;
    }

    if(name=="rush_e"){
        speedBoost+=30;
    }

    if(name=="zen_zone"){
        enemySpawnBoost--;
    }
}

bool Game::powerDownCondition(std::string name) {
    if(name=="autotomy"){
        if(player->body.size()<=3) return true;
    }

    if(name=="catalytic_reaction"){
        if(poisonSpawnBoost>=10) return true;
    }

    if(name=="hot_zone"){
        if(enemySpawnBoost>=10) return true;
    }

    if(name=="inked"){
        if(sight==1) return true;
    }

    if(name=="late_bird"){
        if(multiplier==1) return true;
    }

    return false;
}

void Game::powerDownEffect(string name){
    if(name=="autotomy"){
        int total = player->body.size()/2;
        for(int i=0;i<total;i++){
            player->body.pop_back();
        }
        requiredScore-=10;
    }

    if(name=="catalytic_reaction"){
        poisonSpawnBoost++;
    }

    if(name=="doppelganger"){
        enemyMultiplier++;
    }

    if(name=="el_nino"){
        spawnBoost--;
    }

    if(name=="energy_loss"){
        speedBoost-=15;
    }

    if(name=="foggy"){
        int total=0;
        for(int x=0;x<MAP_WIDTH;x++){
            for(int y=0;y<MAP_HEIGHT;y++){
                if(fow[{x,y}]==1) total++;
            }
        }
        int maxFow = rand() % (total/2);
        if(maxFow<total/(sight+2)) maxFow=total/(sight+2);
        for(int i=0;i<maxFow;i++){
            int nextx=rand()%(MAP_WIDTH-2)+1;
            int nexty=rand()%(MAP_HEIGHT-2)+1;
            for(int x=nextx-sight;x<=nextx+sight;x++) {
                for(int y=nexty-sight;y<=nexty+sight;y++) {
                    if (fow[{x, y}] == 1) continue;
                    fow[{x, y}] = 1;
                    i++;
                }
            }
        }
        requiredScore-=10;
    }

    if(name=="hot_zone"){
        enemySpawnBoost++;
    }

    if(name=="imminent_stardom"){
        int tem=score/40;
        int maxSnake = rand() % (tem+1) + enemyMultiplier;
        if(maxSnake<=6) maxSnake+=6;
        for(int i=0;i<maxSnake;i++){
            spawnEnemy();
        }
        enemyClock.restart();
        requiredScore-=10;
    }

    if(name=="inked"){
        sight--;
    }

    if(name=="late_bird"){
        multiplier--;
    }

    if(name=="pepper_spray"){
        int tem=player->body.size();
        poison.spawn(MAP_WIDTH,MAP_HEIGHT,player->body.size(),score,-tem*tem);
        requiredScore-=10;
    }

    if(name=="sloth_lazy"){
        speedBoost-=30;
    }
}

void Game::spawnEnemy() {
    int enemyDir = rand() % 4;
    int newEnemyX = rand() % (MAP_WIDTH-1) +1;
    int newEnemyY = rand() % (MAP_HEIGHT-1) +1;
    int newEnemyLength = rand() % (player->body.size()/3+1);
    if(newEnemyLength < 3) newEnemyLength=3;
    shared_ptr<Snake> newEnemy;
    newEnemy = make_shared<Snake>(false);
    newEnemy->setTextures(enemyHeadTexture,
                          enemyBodyTexture,
                          poisonedHeadTexture,
                          poisonedBodyTexture);
    switch(enemyDir) {
        case 0:
            for(int i=0;i<=newEnemyLength;i++){
                newEnemy->body.push_back({newEnemyX, MAP_HEIGHT-1 + i});
            }
            newEnemy->dir = UP;
            break;
        case 1:
            for(int i=0;i<=newEnemyLength;i++){
                newEnemy->body.push_back({newEnemyX, -i});
            }
            newEnemy->dir = DOWN;
            break;
        case 2:
            for(int i=0;i<=newEnemyLength;i++){
                newEnemy->body.push_back({MAP_WIDTH-1 + i, newEnemyY});
            }
            newEnemy->dir = LEFT;
            break;
        case 3:
            for(int i=0;i<=newEnemyLength;i++){
                newEnemy->body.push_back({-i, newEnemyY});
            }
            newEnemy->dir = RIGHT;
            break;
        default:
            break;
    }
    enemies.push_back(newEnemy);
    enemyClock.restart();
    enemyTime = rand() % 16;
    if(enemyTime < 5) enemyTime+=5;
}

void Game::newEnemyDir(shared_ptr<Snake>& enemy){
    int enemyDir = rand() % 10;
    switch (enemyDir) {
        case 1:
            if (enemy->dir == UP) enemy->dir = LEFT;
            if (enemy->dir == LEFT) enemy->dir = DOWN;
            if (enemy->dir == DOWN) enemy->dir = RIGHT;
            if (enemy->dir == RIGHT) enemy->dir = UP;
            break;
        case 2:
            if (enemy->dir == UP) enemy->dir = RIGHT;
            if (enemy->dir == LEFT) enemy->dir = UP;
            if (enemy->dir == DOWN) enemy->dir = LEFT;
            if (enemy->dir == RIGHT) enemy->dir = DOWN;
            break;
        default:
            break;
    }

    // 碰撞拐弯
    if (enemy->body[0].x <= 1) enemy->dir=RIGHT;
    else if (enemy->body[0].x >= MAP_WIDTH - 2) enemy->dir=LEFT;
    else if (enemy->body[0].y <= 1) enemy->dir=DOWN;
    else if (enemy->body[0].y >= MAP_HEIGHT - 2) enemy->dir=UP;
}

bool Game::enemyAi(Vector2i newHead) {
    if (newHead.x <= 1 || newHead.x >= MAP_WIDTH - 2 || newHead.y <= 1 || newHead.y >= MAP_HEIGHT - 2) return false;
    if (enemyExist[newHead]) return true;
    if (food.getMap()[newHead]) return false;
    if (poison.getPoisonMap()[newHead] > 0 && score >= 500) return true;
    if (score >= 1000) {
        for (auto seg: player->body) {
            if (seg == newHead) return true;
        }
    }
    return false;
}

Direction Game::newEnemyAi(shared_ptr<Snake>& enemy) {
    unordered_set<Vector2i,Vector2iHash> visited;
    queue<Vector2i> next;
    int maxDistance = min(MAP_HEIGHT/2, MAP_WIDTH/2);
    maxDistance*=maxDistance;
    Vector2i startPos=enemy->body[0];
    next.push(startPos);
    Vector2i a[4]={{1,0},{0,1},{-1,0},{0,-1}};
    bool find=true;

    while(!next.empty()){
        Vector2i curTile=next.front();
        next.pop();
        visited.insert(curTile);
        Vector2i nextTile;
        if(pow(curTile.x-startPos.x,2)+pow(curTile.y-startPos.y,2)>=maxDistance)
            continue;
        
        for(int i=0;i<4;i++){
            nextTile={curTile.x+a[i].x,curTile.y+a[i].y};
            if(visited.count(nextTile))
                continue;
            if((nextTile.x <= 0 || nextTile.x >= MAP_WIDTH - 1 || nextTile.y <= 0 || nextTile.y >= MAP_HEIGHT - 1))
                continue;
            if(score >= 1000) {
                bool out=false;
                for (auto seg: player->body) {
                    if (seg == nextTile){
                        out=true;
                        break;
                    }
                }
                if(out) continue;
            }
            if(poison.getPoisonMap()[nextTile])
                continue;
            if(enemyExist[nextTile])
                continue;
            if(food.getMap()[nextTile])
                find=false;
            next.push(nextTile);
        }
    }
    if(!find)
        return enemy->dir;
}

void Game::update(){
    //if(201-score/10>20) updateFrequency=201-score/10; else updateFrequency=20;
    updateFrequency=200-speedBoost;
    if(updateFrequency<=0) updateFrequency=5;
    if(fastMode) updateFrequency/=2;
    if(slowMode) updateFrequency*=1.5;
    if(gameState == PLAYING && gameClock.getElapsedTime().asMilliseconds() > updateFrequency) {
        gameClock.restart();

        //更新障碍蛇
        for(int i=0; i<enemies.size(); i++) {

            // 检测身体是否在毒气里
            if(enemies[i]->checkPoison(poison.getPoisonMap()) && enemies[i]->body.size()<=2){
                enemyExist[enemies[i]->body[0]]=false;
                enemyExist[enemies[i]->body[1]]=false;
                snakeCollapse(enemies[i]);
                enemies.erase(enemies.begin() + i);
                i--;
                spawnEnemy();
                break;
            }

            // 移动障碍蛇头
            Vector2i newEnemyHead;
            Direction oldDirection;
            int tryTimes=0;
            do {
                newEnemyHead = enemies[i]->body.front();
                if(tryTimes>0) enemies[i]->dir = oldDirection;
                oldDirection = enemies[i]->dir;
                newEnemyDir(enemies[i]);
                //enemies[i]->dir=newEnemyAi(enemies[i]);

                newEnemyHead=nextPos(newEnemyHead,enemies[i]->dir);

                tryTimes++;
            }while(enemyAi(newEnemyHead) && tryTimes<=100);

            // 检测与玩家蛇碰撞
            bool collapse = false;
            for (auto &seg: player->body) {
                if (newEnemyHead == seg) {
                    snakeCollapse(enemies[i]);
                    enemies.erase(enemies.begin() + i);
                    i--;
                    collapse = true;
                    break;
                }
            }
            if (collapse) continue;

            enemyExist[newEnemyHead]=true;

            // 检查是否吃到食物
            if (!food.checkCollision(newEnemyHead)) {
                enemyExist[enemies[i]->body.back()]=false;
                enemies[i]->move(newEnemyHead);
            }else{
                enemies[i]->grow(newEnemyHead);
            }

            // 毒气效果--减短障碍蛇身
            if(enemies[i]->checkPoison(poison.getPoisonMap())){
                enemyExist[enemies[i]->body.back()]=false;
                enemies[i]->shrink();
            }
        }

        // 玩家蛇移动
        Vector2i newHead=player->body[0];
        if(autoMode) autoSearch();
        newHead=nextPos(newHead,player->dir);

        // 碰撞检测
        if(newHead.x <=0 || newHead.x >= MAP_WIDTH-1 ||
           newHead.y <=0 || newHead.y >= MAP_HEIGHT-1) {
            snakeCollapse(player);
            gameState = GAME_OVER;
        }

        // 检测自身碰撞
        for(auto& seg : player->body) {
            if(newHead == seg) {
                snakeCollapse(player);
                gameState = GAME_OVER;
                break;
            }
        }

        // 检测与障碍蛇碰撞
        if(enemyExist[newHead]){
            snakeCollapse(player);
            gameState = GAME_OVER;
        }

        // 剩下头时还在毒气里
        if(player->checkPoison(poison.getPoisonMap()) && player->body.size()<=2){
            snakeCollapse(player);
            gameState = GAME_OVER;
        }

        if(gameState == PLAYING) {

            if (food.checkCollision(newHead)){
                player->grow(newHead);
                score+=10+multiplier;
            }
            else{
                player->move(newHead);
            }

            // 毒气效果--减短玩家蛇身
            if(player->checkPoison(poison.getPoisonMap())){
                player->shrink();
            }
        }

        vector<Vector2i> allSnake=player->body;
        for(int i=0;i<enemies.size();i++){
            for(auto j:enemies[i]->body) allSnake.push_back(j);
        }

        // 随机生成食物
        if(spawnClock.getElapsedTime().asSeconds() > spawnTime && score!=0){
            int maxFood = rand() % 6;
            for(int i=0;i<=maxFood;i++){
                food.spawn(food.spawnPos(MAP_WIDTH,MAP_HEIGHT,allSnake,poison.getPoisonMap()));
            }
            spawnClock.restart();
            spawnTime = (rand() % 16);
            spawnTime -= spawnBoost;
            if(spawnTime < 5) spawnTime+=5;
        }

        if(food.getCount()==0){
            food.spawn(food.spawnPos(MAP_WIDTH,MAP_HEIGHT,allSnake,poison.getPoisonMap()));
        }

        // 随机生成毒气
        if(poisonClock.getElapsedTime().asSeconds() > poisonTime && score!=0){
            poison.spawn(MAP_WIDTH,MAP_HEIGHT,player->body.size(),score,0);
            poisonClock.restart();
            poisonTime = rand() % 16;
            poisonTime-=poisonSpawnBoost;
            if(poisonTime < 5) poisonTime+=5;
        }

        // 随机生成障碍蛇
        if(enemyClock.getElapsedTime().asSeconds() > enemyTime && score!=0){
            for(int i=0;i<=enemyMultiplier;i++){
                spawnEnemy();
            }
        }

        // 毒气消散
        poison.update();
        updateFow();

        if(score>=requiredScore) player->level++;
        if(levelUpDone!=player->level) showLevelUpScreen(false);
    }
}

void Game::render(){
    window.clear(Color::Black);

    drawFow(3);

    for(auto x:enemies){
        x->draw(window,TILE_SIZE);
    }

    //drawFow(2);

    food.draw(window,TILE_SIZE);

    if(gameState != START) player->draw(window,TILE_SIZE);

    poison.draw(window,TILE_SIZE);

    drawFow(1);

    drawWall();

    if(gameState == START){
        drawStartScreen();
    }
    else if(gameState == GAME_OVER) {
        drawGameOver();
    }
    else{
        drawScore();
    }

    window.display();
}

bool Game::isRunning(){
    return window.isOpen();
}
