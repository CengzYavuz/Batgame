#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Bat.h"

const int WIDTH = 180;
const int HEIGHT = 90;
const int TILE_SIZE = 10;
const int RANDOM_FILL_PERCENT = 45;
const int SMOOTHING_STEPS = 12;
const int BAT_SIZE = 3;

std::vector<std::vector<int>> map(WIDTH, std::vector<int>(HEIGHT, 0));

void RandomFillMap() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                map[x][y] = 1;
            }
            else {
                map[x][y] = (std::rand() % 100 < RANDOM_FILL_PERCENT) ? 1 : 0;
            }
        }
    }
}

int countNeighborWallCount(int i, int j) {
    int neighborWallCount = 0;
    for (int neighborX = i - 1; neighborX <= i + 1; neighborX++) {
        for (int neighborY = j - 1; neighborY <= j + 1; neighborY++) {
            if (neighborX >= 0 && neighborX < WIDTH && neighborY >= 0 && neighborY < HEIGHT) {
                neighborWallCount += map[neighborX][neighborY];
            }
            else {
                neighborWallCount++;
            }
        }
    }
    return neighborWallCount;
}

void SmoothMap() {
    std::vector<std::vector<int>> newMap(WIDTH, std::vector<int>(HEIGHT, 0));

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            int neighborWallCount = countNeighborWallCount(i, j);

            if (neighborWallCount > 4) {
                newMap[i][j] = 1;
            }
            else if (neighborWallCount < 4) {
                newMap[i][j] = 0;
            }
        }
    }

    // Copy the new map back to the original map
    map = newMap;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "Map Generation with SFML");

    RandomFillMap();

    for (int i = 0; i < SMOOTHING_STEPS; i++) {
        SmoothMap();
    }

    Bat bat(&window, map, BAT_SIZE);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            bat.batMovesUp();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            bat.batMovesLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            bat.batMovesDown();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            bat.batMovesRight();
        }

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                if (map[x][y] == 1) {
                    tile.setFillColor(sf::Color::Black);
                }
                else if (map[x][y] == 2) {
                    tile.setFillColor(sf::Color::Red);
                }
                else {
                    tile.setFillColor(sf::Color::White);
                }
                window.draw(tile);
            }
        }

        window.display();
    }
    return 0;
}