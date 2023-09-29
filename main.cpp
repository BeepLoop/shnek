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
  dir = RIGHT;
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
  // movement direction logic
  switch (dir) {
  case LEFT: {
    posX -= 1;
    if (posX == 0) {
      gameOver = true;
    }
    break;
  }
  case RIGHT: {
    posX += 1;
    if (posX == width) {
      gameOver = true;
    }
    break;
  }
  case UP: {
    posY -= 1;
    if (posY == 0) {
      gameOver = true;
    }
    break;
  }
  case DOWN: {
    posY += 1;
    if (posY == height) {
      gameOver = true;
    }
    break;
  }
  }
}

int main() {

  setup();

  while (!gameOver) {
    system("clear"); // clear the screen

    draw();
    input();
    logic();

    sleep(1); // sleep for 1sec
  }

  return 0;
}
