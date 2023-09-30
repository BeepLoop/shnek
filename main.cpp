#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <ncurses.h>
#include <random>
#include <unistd.h>

enum Direction { LEFT, RIGHT, UP, DOWN };

bool gameOver;
const int width = 20;
const int height = 20;
int posX, posY, foodX, foodY, score;
Direction dir;
int ch;

int left, right, top, bottom, topr, topl, bottoml, bottomr;

WINDOW *win;

int randomizer(int max) {
  std::random_device rd;
  // Use Mersenne Twister as the random number engine
  std::mt19937 generator(rd());

  // Define a distribution (e.g., uniform distribution between 1 and 100)
  std::uniform_int_distribution<int> distribution(1, max - 1);

  // Generate a random number
  return distribution(generator);
}

void setup() {
  gameOver = false;
  posX = 1;
  posY = 1;
  dir = RIGHT;
  foodX = randomizer(width);
  foodY = randomizer(height);
  score = 0;

  left = right = (int)'|';
  top = bottom = (int)'-';
  topr = topl = bottoml = bottomr = (int)'+';

  // ncurses setup
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);

  win = newwin(height, width, 2, 0);
}

void draw() {
  refresh();

  clear();
  printw("press ctrl + c to exit\n");
  printw("score: %d", score);

  // clear the board
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      mvwprintw(win, i, j, " ");
    }
  }

  mvwprintw(win, foodY, foodX, "o");
  mvwprintw(win, posY, posX, "@");

  wborder(win, left, right, top, bottom, topl, topr, bottoml, bottomr);

  wrefresh(win);
}

void input() {
  if (ch == 'w' || ch == 'W') {
    dir = UP;
  } else if (ch == 'a' || ch == 'A') {
    dir = LEFT;
  } else if (ch == 's' || ch == 'S') {
    dir = DOWN;
  } else if (ch == 'd' || ch == 'D') {
    dir = RIGHT;
  }
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

  // eating logic
  if (posX == foodX && posY == foodY) {
    score++; // increase the score

    // move food to random position
    // regenerate pos if its in the border
    do {
      foodX = randomizer(width);
    } while (foodX == 0 || foodX == width - 1);

    do {
      foodY = randomizer(height);
    } while (foodY == 0 || foodY == height - 1);
  }
}

int main() {
  setup();

  while (!gameOver) {
    ch = getch();

    input();
    logic();
    draw();

    sleep(1); // sleep for 1sec
  }

  curs_set(1);
  endwin(); // cleanup for ncurses
  return 0;
}
