#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <random>
#include <thread>
#include <tuple>

enum Direction { LEFT, RIGHT, UP, DOWN };

bool gameOver;
const int width = 30;
const int height = 20;
std::tuple<int, int> snekPos;
std::tuple<int, int> foodPos;
int score;
int tailX[100], tailY[100];
int snekLength;
Direction dir;
int ch;
int left, right, top, bottom, topr, topl, bottoml, bottomr;
WINDOW *win;

bool offTheBoard(int &px, int &py) {
  if (px == 0 || px == width - 1 || py == 0 || py == height - 1) {
    return true;
  }
  return false;
}

void spawnFood() {
  std::random_device rd;
  std::mt19937 generator(rd());

  int px;
  int py;
  // keep generating location if foodPos is off the board
  do {
    std::uniform_int_distribution<int> widthDistr(1, width - 1);
    std::uniform_int_distribution<int> heightDistr(1, height - 1);
    px = widthDistr(generator);
    py = heightDistr(generator);
  } while (offTheBoard(px, py));

  std::get<0>(foodPos) = px;
  std::get<1>(foodPos) = py;
}

void setup() {
  gameOver = false;
  snekPos = {width / 2, height / 2};
  spawnFood();
  dir = RIGHT;
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
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      mvwprintw(win, i, j, " ");
    }
  }

  int foodx = std::get<0>(foodPos);
  int foody = std::get<1>(foodPos);
  int posx = std::get<0>(snekPos);
  int posy = std::get<1>(snekPos);
  mvwprintw(win, foody, foodx, "6");
  mvwprintw(win, posy, posx, "8");

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
  // check if wall is hit
  if (std::get<0>(snekPos) == 0 || std::get<0>(snekPos) == width ||
      std::get<1>(snekPos) == 0 || std::get<1>(snekPos) == height) {
    gameOver = true;
  }

  // tail movement logic
  int prevx = tailX[0];
  int prevy = tailY[0];
  int prevx2, prevy2;
  tailX[0] = std::get<0>(snekPos);
  tailY[0] = std::get<1>(snekPos);
  for (int i = 1; i < snekLength; ++i) {
    prevx2 = tailX[i];
    prevy2 = tailY[i];
    tailX[i] = prevx;
    tailY[i] = prevy;
    prevx = prevx2;
    prevy = prevy2;
  }

  // check if hit wall

  // movement direction logic
  switch (dir) {
  case LEFT: {
    /* posX -= 1; */
    std::get<0>(snekPos) -= 1;
    break;
  }
  case RIGHT: {
    std::get<0>(snekPos) += 1;
    break;
  }
  case UP: {
    std::get<1>(snekPos) -= 1;
    break;
  }
  case DOWN: {
    std::get<1>(snekPos) += 1;
    break;
  }
  }

  // check if we hit ourself
  for (int i = 0; i < snekLength; ++i) {
    if (tailX[i] == std::get<0>(snekPos) && tailY[i] == std::get<1>(snekPos)) {
      gameOver = true;
    }
  }

  // eating logic
  if (std::get<0>(snekPos) == std::get<0>(foodPos) &&
      std::get<1>(snekPos) == std::get<1>(foodPos)) {
    score++; // increase the score
    snekLength++;

    // move foodPos to random position
    // regenerate snekPos if its in the border
    spawnFood();
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
