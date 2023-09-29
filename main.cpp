#include <cstdlib>
#include <iostream>
#include <unistd.h>

bool gameOver;
const int width = 20;
const int height = 20;
int posX, posY, foodX, foodY, score;

void setup() {
  gameOver = false;
  posX = width / 2;
  posY = height / 2;
  foodX = rand() % width;
  foodY = rand() % height;
  score = 0;
}

void draw() {
  // draw board
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      if (j == 0 || j == width - 1 || i == 0 || i == height - 1) {
        std::cout << "#";
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

  while (!gameOver) {
    setup();
    draw();

    sleep(1);        // sleep for 1sec
    system("clear"); // clear the screen
  }

  return 0;
}
