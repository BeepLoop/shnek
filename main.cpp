#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <ncurses.h>
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

void setup() {
  gameOver = false;
  posX = width / 2;
  posY = height / 2;
  dir = RIGHT;
  foodX = rand() % width - 1;
  foodY = rand() % height - 1;
  score = 0;

  left = right = top = bottom = (int)'#';
  topr = topl = bottoml = bottomr = (int)'+';

  // ncurses setup
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr, TRUE);

  win = newwin(height, width, 0, 0);
}

void draw() {
  refresh();

  // draw board
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      if (j == posX && i == posY) {
        mvwprintw(win, i, j, "@");
      } else if (j == foodY && i == foodY) {
        mvwprintw(win, i, j, "o");
      } else {
        mvwprintw(win, i, j, " ");
      }
    }
  }

  wborder(win, left, right, top, bottom, topl, topr, bottoml, bottomr);

  wrefresh(win);
}

void input() {
  // code for input
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

  endwin(); // cleanup for ncurses
  return 0;
}
