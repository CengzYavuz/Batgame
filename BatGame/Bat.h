#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

class Bat {
private:
	sf::RenderWindow* window;
	std::vector<std::vector<int>>& map;
	const int batSize;
	sf::Vector2f position;

public:
	Bat(sf::RenderWindow* newWindow, std::vector<std::vector<int>>& newMap, int newBatSize)
		: window(newWindow), map(newMap), batSize(newBatSize) {
		createBat();
	}

	void batMovesRight() {
		moveBat(0, 1);
	}

	void batMovesLeft() {
		moveBat(0, -1);
	}

	void batMovesUp() {
		moveBat(-1, 0);
	}

	void batMovesDown() {
		moveBat(1, 0);
	}

private:
	void createBat() {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		do {
			position.x = rand() % (map[0].size() - batSize);
			position.y = rand() % (map.size() - batSize);

		} while (!areRectangularCoordinatesOccupied(position, batSize, batSize));

		setBatLocationInsideMap();
	}

	bool areRectangularCoordinatesOccupied(sf::Vector2f position, int width, int height) {
		int numRows = map.size();
		if (numRows == 0) return false;

		int numCols = map[0].size();
		if (numCols == 0) return false;

		// Check if the requested position and size are within bounds
		if (position.x < 0 || position.y < 0 || position.x + width > numCols || position.y + height > numRows)
			return false;

		// Check each cell within the rectangle
		for (int i = position.y; i < position.y + height; i++) {
			for (int j = position.x; j < position.x + width; j++) {
				if (map[i][j] != 0) {
					return false; // Occupied cell found
				}
			}
		}

		return true; // No occupied cells found
	}

	void clearCurrentBatLocationFromMap() {
		for (int i = position.y; i < position.y + batSize; i++) {
			for (int j = position.x; j < position.x + batSize; j++) {
				map[i][j] = 0;
			}
		}
	}

	void setBatLocationInsideMap() {
		for (int i = position.y; i < position.y + batSize; i++) {
			for (int j = position.x; j < position.x + batSize; j++) {
				map[i][j] = 2;
			}
		}
	}

	void moveBat(int dx, int dy) {
		clearCurrentBatLocationFromMap();
		if (isValidMove(position.x + dx, position.y + dy)) {
			position.x += dx;
			position.y += dy;
		}
		setBatLocationInsideMap();
	}

	bool isValidMove(float newX, float newY) {
		return newX >= 0 && newY >= 0 &&
			newX + batSize <= map[0].size() &&
			newY + batSize <= map.size() &&
			areRectangularCoordinatesOccupied(sf::Vector2f(newX, newY), batSize, batSize);
	}
};