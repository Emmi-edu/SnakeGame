//
// Created by emmit on 23/4/2025.
//

#pragma once

#include "constants.h"
#include "snake.h"
#include "poison.h"
#include "food.h"
#include "powerupsystem.h"
#include <vector>
#include <memory>

class Game {
public:
    Game();

    void init();
    void handleInput(Event& event);
    void update();
    void render();

    bool isRunning();

    void snakeCollapse(shared_ptr<Snake>& snake);
    bool playerAi(Direction dir);
    void autoSearch();
    void spawnEnemy();
    void newEnemyDir(shared_ptr<Snake>& enemy);
    bool enemyAi(Vector2i newHead);
    Direction newEnemyAi(shared_ptr<Snake>& enemy);
    void drawWall();
    void drawScore();
    void drawStartScreen();
    void drawGameOver();
    void drawFow(int mode);

    RenderWindow window;
    GameState gameState;
    Clock gameClock;

    shared_ptr<Snake> player;
    vector<shared_ptr<Snake>> enemies;
    unordered_map<Vector2i,bool,Vector2iHash> enemyExist;
    Poison poison;
    Food food;

    int score;

    unordered_map<Vector2i,int,Vector2iHash> fow; //1:unrevealed 2:fog 3:revealed
    void updateFow();
    int sight=0;

    PowerUpSystem pws;
    int requiredScore;
    int levelUpDone;
    void showLevelUpScreen(bool rerolled);
    bool powerUpCondition(string name);
    void powerUpEffect(string name);
    bool powerDownCondition(string name);
    void powerDownEffect(string name);

    int multiplier;
    int speedBoost;
    int spawnBoost;
    int enemySpawnBoost;
    int enemyMultiplier;
    int poisonSpawnBoost;

    Clock spawnClock;
    int spawnTime;
    Clock poisonClock;
    int poisonTime;
    Clock enemyClock;
    int enemyTime;

    Font font;

    // Textures
    Texture wallTexture;
    Texture snakeHeadTexture;
    Texture snakeBodyTexture;
    Texture poisonedHeadTexture;
    Texture poisonedBodyTexture;
    Texture enemyHeadTexture;
    Texture enemyBodyTexture;
    vector<Texture> poisonTexture;

    int updateFrequency;
    bool fastMode;
    bool slowMode;
    bool autoMode;
};