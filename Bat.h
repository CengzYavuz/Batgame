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
	sf::Vector2f startingPosition;

	int elapsedTime = 0;
	int shoutRange;

	bool shout = false;

public:
	Bat(sf::RenderWindow* newWindow, std::vector<std::vector<int>>& newMap, int newBatSize,int newShoutRange=20)
		: window(newWindow), map(newMap), batSize(newBatSize) {
		shoutRange = newShoutRange;
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

	bool isShoutActive() {

		return shout;
	}

	int getPositionX() {

		return position.x;

	}
	int getPositionY() {

		return position.y;

	}
	sf::Vector2f getStartingPosition() {

		return startingPosition;

	}


	void batShout( int timesRepeat = 5) {


		
		if (!shout) {

			shout = true;
			
			startingPosition = position;


			for (int i = startingPosition.y -1; i < startingPosition.y + batSize +1; i++) {

				for (int j = startingPosition.x -1; j < startingPosition.x + batSize +1; j++) {

					if (i >= 0 && i < map.size() && j >= 0 && j < map[0].size()) {

						if (map[i][j] == 2) {

							if (map[i + 1][j] == 0 && i > 0 && i < map.size()) {
								map[i + 1][j] = 3;
							}
							if (map[i - 1][j] == 0 && i >= 1 && i <= map.size()) {
								map[i - 1][j] = 3;
							}
							if (map[i][j + 1] == 0 && j> 0 && j < map[0].size()) {
								map[i][j + 1] = 3;
							}
							if (map[i][j - 1] == 0 && j >= 1 && j <= map[0].size()) {
								map[i][j - 1] = 3;
							}

						}

					}
				}
			}


		}
		else if (shout) {


			



			for (int k = 0; k < timesRepeat; k++) {


				for (int i = startingPosition.y - shoutRange; i < startingPosition.y + batSize + shoutRange; i++) {

					for (int j = startingPosition.x - shoutRange; j < startingPosition.x + batSize + shoutRange; j++) {

						if (i >= 0 && i < map.size() && j >= 0 && j < map[0].size()) {

							if (map[i][j] == 3) {

								map[i][j] = 4;

							}


						}


					}

				}

				for (int i = startingPosition.y - shoutRange; i < startingPosition.y + batSize + shoutRange; i++) {

					for (int j = startingPosition.x - shoutRange; j < startingPosition.x + batSize + shoutRange; j++) {

						if (i >= 0 && i < map.size() && j >= 0 && j < map[0].size()) {


							if (map[i][j] == 4) {

								if (map[i + 1][j] == 0 && i > 0 && i < map.size()) {
									map[i + 1][j] = 3;
								}
								if (map[i - 1][j] == 0 && i >= 1 && i <= map.size()) {
									map[i - 1][j] = 3;
								}
								if (map[i][j + 1] == 0 && j > 0 && j < map[0].size()) {
									map[i][j + 1] = 3;
								}
								if (map[i][j - 1] == 0 && j >= 1 && j <= map[0].size()) {
									map[i][j - 1] = 3;
								}


							}

						}


					}

				}

				}



			










			elapsedTime++;
			if (elapsedTime == shoutRange) {
				shout = false;
				elapsedTime = 0;
				clearMap();
			}


		}
		

	}

private:


	void clearMap() {

		for (int i = startingPosition.y - shoutRange; i < startingPosition.y + batSize + shoutRange; i++) {

			for (int j = startingPosition.x - shoutRange; j < startingPosition.x + batSize + shoutRange; j++) {

				if (i >= 0 && i < map.size() && j >= 0 && j < map[0].size()) {


					if (map[i][j] == 4 || map[i][j] == 3) {

						map[i][j] = 0;

					}
				}
			}
		}



	}
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
				if (map[i][j] == 1) {
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