#include <LedControl.h>
#include "Utils.h"

// joystick pins
const int JoystickSWPin = 2;  // digital pin connected to switch output
const int JoystickXPin = 0;   // analog pin connected to X output
const int JoystickYPin = 1;   // analog pin connected to Y output

// let board pins, pin 12: DataIn, pin 11: LOAD(CS), pin 10: CLK
LedControl lc = LedControl(12, 10, 11, 1);

// board setup
Direction direction = UP;
Position headPosition = { 0, 0 };
int length = 1;
bool micePresent = false;
int board[8][8] = { 0 };

void setup() {
  // The led board is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  // Set the brightness values
  lc.setIntensity(0, 5);  // 0 to 15

  // configure johstick
  pinMode(JoystickSWPin, INPUT);
  digitalWrite(JoystickSWPin, HIGH);
  Serial.begin(9600);

  // start a new game
  startNewGame();
}

void loop() {
  setDirection();
  gameLoop();
}

// Starts a new game with countdown
void startNewGame() {
  showCountdown(lc);

  // game defaults
  direction = RIGHT;
  headPosition.X = 4;
  headPosition.Y = 3;
  length = 2;

  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      board[r][c] = 0;
      
      if (r == headPosition.X && c == headPosition.Y) {
        // place snake's head
        board[r][c] = 1;
      }
    }
  }

  placeMice();
}

void died() {
  startNewGame();
}

void won() {
  startNewGame();
}

void gameLoop() {
  bool skipDelay = false;
  if (!micePresent) {
    placeMice();
  }

  // change head's position
  auto [x, y] = getNewPosition(headPosition.X, headPosition.Y, direction);

  // touched the tail
  if (board[x][y] > 1) {
    died();
    return;
  }

  // found a mice
  if (board[x][y] == -1) {
    length++;
    micePresent = false;
    skipDelay = true;
  }

  board[x][y] = 1;
  headPosition.X = x;
  headPosition.Y = y;

  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      int value = board[r][c];
      int isHead = r == headPosition.X && c == headPosition.Y;

      if (!isHead && board[r][c] > 0) {  // draw tail
        board[r][c] = value >= length ? 0 : value + 1;
      }

      lc.setLed(0, r, c, board[r][c] != 0);
    }
  }

  if (!skipDelay) {
    delay(500);
  }
}

void placeMice() {
  int placeAt = random(1, 64 - length);
  int current = 1;
  int lastX = 0;
  int lastY = 0;

  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      if (board[r][c] == 0) {
        if (current == placeAt) {
          board[r][c] = -1;
          micePresent = true;
          return;
        }

        current++;
        lastX = r;
        lastY = c;
      }
    }
  }

  if (board[lastX][lastY] != 0) {
    won();
    return;
  }

  // put in the last available space
  board[lastX][lastY] = -1;
  micePresent = true;
}

void setDirection() {
  int x = analogRead(JoystickXPin);
  int y = analogRead(JoystickYPin);

  if (x > 562 && direction != LEFT) {
    direction = RIGHT;
  } else if (x < 462 && direction != RIGHT) {
    direction = LEFT;
  } else if (y > 562 && direction != UP) {
    direction = DOWN;
  } else if (y < 462 && direction != DOWN) {
    direction = UP;
  }
}