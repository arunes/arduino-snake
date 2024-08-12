struct Position {
  int X;
  int Y;
};

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

Position getNewPosition(int x, int y, Direction direction) {
  switch (direction) {
    case RIGHT: y++; break;
    case LEFT: y--; break;
    case UP: x--; break;
    case DOWN: x++; break;
  }

  if (x < 0) {
    x = 7;
  } else if (x > 7) {
    x = 0;
  }

  if (y < 0) {
    y = 7;
  } else if (y > 7) {
    y = 0;
  }

  return Position{ x, y };
}

void showCountdown(LedControl lc) {
  byte three[] = { B01111100, B01111110, B00000110, B00111110, B00111110, B00000110, B01111110, B01111100 };
  byte two[] = { B00111100, B01111110, B01000110, B00001100, B00011000, B00110000, B01111110, B01111110 };
  byte one[] = { B00011000, B00111000, B01111000, B00011000, B00011000, B00011000, B01111110, B01111110 };

  lc.clearDisplay(0);
  for (int r = 0; r < 8; r++) {
    lc.setRow(0, r, three[r]);
  }
  delay(1000);
  lc.clearDisplay(0);
  for (int r = 0; r < 8; r++) {
    lc.setRow(0, r, two[r]);
  }
  delay(1000);
  lc.clearDisplay(0);
  for (int r = 0; r < 8; r++) {
    lc.setRow(0, r, one[r]);
  }
  delay(1000);
  lc.clearDisplay(0);
}