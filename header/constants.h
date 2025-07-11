//
// Created by emmit on 23/4/2025.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

// Game constants
const int TILE_SIZE = 15;
const int MAP_WIDTH = 120;
const int MAP_HEIGHT = 60;

// System font paths
#if defined(_WIN32)
const string FONT_PATH = "C:/Windows/Fonts/ARLRDBD.ttf";
#elif defined(__APPLE__)
const string FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
#else
const string FONT_PATH = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";
#endif

// Enums
enum GameState { START, PLAYING, GAME_OVER };
enum Direction { UP, DOWN, LEFT, RIGHT };

// Utility struct for Vector2i hashing
struct Vector2iHash {
    size_t operator()(const Vector2i& vec) const {
        return hash<int>()(vec.x) ^ hash<int>()(vec.y);
    }
};