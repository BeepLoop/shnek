#include <cstdlib>
#include <iostream>
#include <unistd.h>

enum Direction { LEFT, RIGHT, UP, DOWN };

bool gameOver;
const int width = 20;
const int height = 20;
int posX, posY, foodX, foodY, score;
Direction dir;
char snake, food, wall;

void setup() {
  gameOver = false;
  snake = '@';
  food = 'o';
  wall = '#';
  posX = width / 2;
  posY = height / 2;
  dir = LEFT;
  foodX = rand() % width - 1;
  foodY = rand() % height - 1;
  score = 0;
}

void draw() {
  // draw board
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      if (j == 0 || j == width - 1 || i == 0 || i == height - 1) {
        std::cout << wall;
      } else if (j == posX && i == posY) {
        std::cout << snake;
      } else if (j == foodY && i == foodY) {
        std::cout << food;
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}

void input() {
  // code for input
}

void logic() {
  // code for logic
}

int main() {

  setup();

  while (!gameOver) {
    system("clear"); // clear the screen
                     //
    draw();
    input();
    logic();

    sleep(1); // sleep for 1sec
  }

  return 0;
}
