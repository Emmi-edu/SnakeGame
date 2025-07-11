//
// Created by emmit on 2/5/2025.
//

#include "../header/powerupsystem.h"
#include <iostream>
#include <fstream>

PowerUpSystem::PowerUpSystem() {
    initializePowerUps();
    initializePowerDowns();
    setupUI();
}

void PowerUpSystem::initializePowerUps() {
    // Initialize all available power-ups
    vector<string> powerUpNames = {"air_purifier",
                                   "apple_pie",
                                   "cheat_code",
                                   "deluxe_harvest",
                                   "fast_bolt",
                                   "greenhouse",
                                   "il_milione",
                                   "infiltration",
                                   "marvel_of_travel",
                                   "omnivore",
                                   "rush_e",
                                   "zen_zone"
                                  };

    for (auto& name : powerUpNames) {
        PowerUp pu;
        pu.name = name;

        if(!pu.texture.loadFromFile("../assets/powerups/" + name + ".png")) {
            std::cout<<"Error Loading Image!"<<endl;
        }

        ifstream inputFile("../assets/powerups/description/" + name +".txt");
        if(!inputFile){
            std::cout<<"Error Loading Document!"<<endl;
        }
        std::string content;
        std::string line;
        // Read the file line by line and store it in the content variable
        while (std::getline(inputFile, line)) {
            content += line + "\n"; // Append each line to the content variable with a newline
        }
        inputFile.close();
        pu.description=content;

        powerUps.push_back(pu);
    }
}

void PowerUpSystem::initializePowerDowns() {
    // Initialize all available power-downs
    vector<string> powerDownNames = {"autotomy",
                                     "catalytic_reaction",
                                     "doppelganger",
                                     "el_nino",
                                     "energy_loss",
                                     "foggy",
                                     "hot_zone",
                                     "imminent_stardom",
                                     "inked",
                                     "late_bird",
                                     "pepper_spray",
                                     "sloth_lazy"
                                     };

    for (auto& name : powerDownNames) {
        PowerUp pd;
        pd.name = name;

        if(!pd.texture.loadFromFile("../assets/powerdowns/" + name + ".png")) {
            std::cout<<"Error Loading Image!"<<endl;
        }

        ifstream inputFile("../assets/powerdowns/description/" + name +".txt");
        if(!inputFile){
            std::cout<<"Error Loading Document!"<<endl;
        }
        std::string content;
        std::string line;
        // Read the file line by line and store it in the content variable
        while (std::getline(inputFile, line)) {
            content += line + "\n"; // Append each line to the content variable with a newline
        }
        inputFile.close();
        pd.description=content;

        powerDowns.push_back(pd);
    }
}

void PowerUpSystem::setupUI() {
    overlay.setSize(Vector2f(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE));
    overlay.setFillColor(Color(0, 0, 0, 180));

    float cardWidth = MAP_WIDTH / 8.0f * TILE_SIZE;
    float cardHeight = MAP_HEIGHT / 3.0f * TILE_SIZE;

    group1.setSize(Vector2f(cardWidth*2 + TILE_SIZE*2, cardHeight*2 + TILE_SIZE*4));
    group2.setSize(Vector2f(cardWidth*2 + TILE_SIZE*2, cardHeight*2 + TILE_SIZE*4));
    group3.setSize(Vector2f(cardWidth*2 + TILE_SIZE*2, cardHeight*2 + TILE_SIZE*4));

    group1.setFillColor(Color::Black);
    group1.setOutlineColor(Color::White);
    group1.setOutlineThickness(10);
    group2.setFillColor(Color::Black);
    group2.setOutlineColor(Color::White);
    group2.setOutlineThickness(10);
    group3.setFillColor(Color::Black);
    group3.setOutlineColor(Color::White);
    group3.setOutlineThickness(10);

    upcard1.setSize(Vector2f(cardWidth, cardHeight));
    upcard2.setSize(Vector2f(cardWidth, cardHeight));
    upcard3.setSize(Vector2f(cardWidth, cardHeight));

    downcard1.setSize(Vector2f(cardWidth, cardHeight));
    downcard2.setSize(Vector2f(cardWidth, cardHeight));
    downcard3.setSize(Vector2f(cardWidth, cardHeight));

    reroll.setSize(Vector2f(cardWidth/2,cardWidth/2));

    reminder.setString("Choose an upgrade");
    reminder.setCharacterSize(36);

    upText1.setCharacterSize(18);
    upText1.setFillColor(Color(0,200,0));
    upText2.setCharacterSize(18);
    upText2.setFillColor(Color(0,200,0));
    upText3.setCharacterSize(18);
    upText3.setFillColor(Color(0,200,0));

    downText1.setCharacterSize(18);
    downText1.setFillColor(Color::Red);
    downText2.setCharacterSize(18);
    downText2.setFillColor(Color::Red);
    downText3.setCharacterSize(18);
    downText3.setFillColor(Color::Red);

    group1.setPosition(TILE_SIZE, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*4);
    group2.setPosition(MAP_WIDTH*TILE_SIZE/2 - cardWidth - TILE_SIZE, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*4);
    group3.setPosition(MAP_WIDTH*TILE_SIZE - TILE_SIZE - (cardWidth*2 + TILE_SIZE*2), (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*4);

    upcard1.setPosition(TILE_SIZE*2, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*3);
    upcard2.setPosition(MAP_WIDTH*TILE_SIZE/2 - cardWidth, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*3);
    upcard3.setPosition(MAP_WIDTH*TILE_SIZE - (cardWidth*2 + TILE_SIZE*2), (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*3);

    downcard1.setPosition(TILE_SIZE*2, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 + cardHeight - TILE_SIZE);
    downcard2.setPosition(MAP_WIDTH*TILE_SIZE/2 - cardWidth, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 + cardHeight - TILE_SIZE);
    downcard3.setPosition(MAP_WIDTH*TILE_SIZE - (cardWidth*2 + TILE_SIZE*2), (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 + cardHeight - TILE_SIZE);

    reroll.setPosition(MAP_WIDTH * TILE_SIZE /2 - cardWidth/4, MAP_HEIGHT * TILE_SIZE /2 + cardHeight + TILE_SIZE);

    upText1.setPosition(TILE_SIZE*2 + cardWidth, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*3 + cardHeight/2);
    upText2.setPosition(MAP_WIDTH*TILE_SIZE/2, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*3 + cardHeight/2);
    upText3.setPosition(MAP_WIDTH*TILE_SIZE - cardWidth - TILE_SIZE*2, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 - TILE_SIZE*3 + cardHeight/2);

    downText1.setPosition(TILE_SIZE*2 + cardWidth, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 + cardHeight*1.5);
    downText2.setPosition(MAP_WIDTH*TILE_SIZE/2, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 + cardHeight*1.5);
    downText3.setPosition(MAP_WIDTH*TILE_SIZE - cardWidth - TILE_SIZE*2, (MAP_HEIGHT * TILE_SIZE / 2 - cardHeight / 2)/2 + cardHeight*1.5);
}

