//
// Created by emmit on 2/5/2025.
//

#pragma once
#include "constants.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>

using namespace sf;
using namespace std;

class PowerUpSystem {
public:
    PowerUpSystem();

    struct PowerUp{
        string name;
        Texture texture;
        String description;
    };

    vector<PowerUp> powerUps;
    vector<PowerUp> powerDowns;

    // UI Elements
    RectangleShape overlay;
    RectangleShape group1;
    RectangleShape group2;
    RectangleShape group3;
    RectangleShape upcard1;
    RectangleShape upcard2;
    RectangleShape upcard3;
    RectangleShape downcard1;
    RectangleShape downcard2;
    RectangleShape downcard3;
    RectangleShape reroll;

    Text reminder;
    Text upText1;
    Text upText2;
    Text upText3;
    Text downText1;
    Text downText2;
    Text downText3;

private:
    // Helper methods
    void initializePowerUps();
    void initializePowerDowns();
    void setupUI();
};