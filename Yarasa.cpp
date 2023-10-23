#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Bat.h"
#include<windows.h> 
#include "Cell.h"

const int WIDTH = 180;
const int HEIGHT = 90;
const int TILE_SIZE = 10;
const int RANDOM_FILL_PERCENT = 45;
const int SMOOTHING_STEPS = 12;
const int BAT_SIZE = 3;


// Map:
// 1 == Obstacle
// 0 == Road
// 2 == Bat
// 3 == SoundWave expansion indicator
// 4 == SoundWave 
// 5 == Enemy
// 6 == Food
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



    sf::RenderWindow window(sf::VideoMode(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "Bat vs Wuhanese");
    int count = 0;

    RandomFillMap();

    for (int i = 0; i < SMOOTHING_STEPS; i++) {
        SmoothMap();
    }

    Bat bat(&window, map, BAT_SIZE, 25);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !bat.isShoutActive()) {

            bat.batShout();
        }



        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                tile.setFillColor(sf::Color::Black);


                if (map[x][y] == 2) {
                    tile.setFillColor(sf::Color::Red);
                }
                else if (x > 1 && x < map.size() - 1 && y > 1 && y < map[0].size() - 1) {

                    int distance = hypot((bat.getStartingPosition().y - x), (bat.getStartingPosition().x - y)) * 8;
                    if (distance > 255) {
                        distance = 255;
                    }

                    if (map[x][y] == 1 && (map[x + 1][y] == 4 || map[x - 1][y] == 4 || map[x][y + 1] == 4 || map[x][y - 1] == 4)) {


                        tile.setFillColor(sf::Color(155, 155, 155, 255 - distance));
                    }
                    if (map[x][y] == 1 && (map[x + 1][y] == 2 || map[x - 1][y] == 2 || map[x][y + 1] == 2 || map[x][y - 1] == 2)) {

                        tile.setFillColor(sf::Color(155, 155, 155, 255));

                    }


                }

                window.draw(tile);
            }
        }



        window.display();
        if (bat.isShoutActive()) {
            bat.batShout();
        }
        Sleep(16.67);
    }
    return 0;
};