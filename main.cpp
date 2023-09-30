#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <random>
#include <thread>

enum Direction { LEFT, RIGHT, UP, DOWN };

bool gameOver;
const int width = 20;
const int height = 20;
int posX, posY, foodX, foodY, score;
int tailX[100], tailY[100];
int snekLength;
Direction dir;
int ch;
int left, right, top, bottom, topr, topl, bottoml, bottomr;
WINDOW *win;

int randomizer(int max) {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distribution(1, max - 1);
  return distribution(generator);
}

void setup() {
  gameOver = false;
  posX = 1;
  posY = 1;
  dir = RIGHT;
  foodX = randomizer(width);
  foodY = randomizer(height);
  snekLength = 0;
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

  win = newwin(height, width, 4, 0);
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

  mvwprintw(win, foodY, foodX, "6");
  mvwprintw(win, posY, posX, "8");

  for (int i = 0; i < snekLength; ++i) {
    mvwprintw(win, tailY[i], tailX[i], "o");
  }

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
  // tail movement logic
  int prevx = tailX[0];
  int prevy = tailY[0];
  int prevx2, prevy2;
  tailX[0] = posX;
  tailY[0] = posY;
  for (int i = 1; i < snekLength; ++i) {
    prevx2 = tailX[i];
    prevy2 = tailY[i];
    tailX[i] = prevx;
    tailY[i] = prevy;
    prevx = prevx2;
    prevy = prevy2;
  }

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

  // check if we hit ourself
  for (int i = 0; i < snekLength; ++i) {
    if (tailX[i] == posX && tailY[i] == posY) {
      gameOver = true;
    }
  }

  // eating logic
  if (posX == foodX && posY == foodY) {
    score++; // increase the score
    snekLength++;

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

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  curs_set(1);
  endwin(); // cleanup for ncurses
  return 0;
}
