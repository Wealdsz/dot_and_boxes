#include "grid.h"
#include <iostream>

//Helper functions: you may use them as a template.


// just constants for maximum coordinate values
constexpr int _bound_low = 0;
constexpr int _bound_high = 2;

// min and max functions below since we'll need them later on

int min(int a, int b) {
  if (a < b)
    return a;

  return b;
}

int max(int a, int b) {
  if (a > b)
    return a;

  return b;
}

// checks if input is valid, duh
bool validateInput(int row, int col, char dir) {
  // if input is generally out of bounds
  if (!((row >= _bound_low && row <= _bound_high ) && (col >= _bound_low && col <= _bound_high )))
    return false;

  // we're also checking if there's already a line drawn at the specified input location
  switch (dir) {
      // in the 'left' case we check if it's even possible to go left
      case 'l':
        if (col == _bound_low || grid.horizontal(row, col -1))
          return false;
        break;
      // right: we can't draw a line if we're at the rightmost column
      case 'r':
        if (col == _bound_high  || grid.horizontal(row, col))
          return false;
        break;
      // can't go up if we're at the top
      case 'u':
        if (row == _bound_low || grid.vertical(row -1, col))
          return false;
        break;
      // can't go deeper than mariana trench
      case 'd':
        if (row == _bound_high  || grid.vertical(row, col))
          return false;
        break;
      // this case shouldn't happen anyway
      default:
        return false;
    }

  // apparently our input is clean, yeay
  return true;
}

// Check if no more tokens can be placed
// this was already written hehe
bool isGridFull() {
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 2; ++col) {
            if (grid.fields(row, col) == ' ') {
                return false;
            }
        }
    }
    return true;
}

//Place a vertical or horizontal line, depending on direction
void drawLine(int row, int col, char direction) {
  // we just set the corresponding slot to true, nothing interesting except that if we want to go up / left we're gonna adjust the index accordingly
  switch (direction) {
      case 'l':
        grid.horizontal(row, col - 1) = true;
        break;
      case 'r':
        grid.horizontal(row, col) = true;
        break;
      case 'u':
        grid.vertical(row - 1, col) = true;
        break;
      case 'd':
        grid.vertical(row, col) = true;
        break;
      default:
        return;
    }
}

//Does adding a line result in drawing an unclaimed box?
bool shouldPlaceToken(int row, int col) {

  // we dont want to place a token if there is one already
  // (if the field has a content different from the initial one)
  if (grid.fields(row, col) != ' ')
    return false;


  // this function accepts the coordinates from the top-left point of the box.
  // thus, we check all four sides
  return grid.horizontal(row, col) && grid.horizontal(row + 1, col) && grid.vertical(row, col) && grid.vertical(row, col + 1);
}

int main() {
  // bools are cool since we can easily flip them
  bool player = false; // we define A <=> false; B <=> true

  // we'll loop for as long as there still are lines left to draw
  while (!isGridFull()) {
    // first we print the grid
    grid.printGrid();

    // we get the input
    int row, col;
    char direction;
    std::cin >> row >> col >> direction;

    // check if input is valid, if not we read it again and again
    while (!validateInput(row, col, direction)) {
      std::cout << std::endl << "Invalid move!\n";
      std::cin >> row >> col >> direction;
    }

    // at this point our input seems to be valid, so we can happily draw the line
    drawLine(row, col, direction);

    // now this stuff is a bit of a monstrosity, but since it works we are happy
    bool switch_player { true };
    for (int r = max(row - 1, _bound_low); r < min(row + 1, _bound_high); ++r) {
      for (int c = max(col - 1, _bound_low); c < min(col + 1, _bound_high); ++c) {
        if (shouldPlaceToken(r, c)) {
          char player_symbol = !player ? 'A' : 'B'; // we choose A/B depending on the value of player. (false -> A, true -> B)
          grid.fields(r, c) = player_symbol;
          switch_player = false; // if a token has been placed we want to prevent player change for this round
        }
      }
    }

    // switch player A -> B / B -> A - now we benefit from having chosen a bool value
    if (switch_player)
      player = !player; //flip c:

  }

  // count all em' Bs and sum them up
  int sum { 0 };
  for (int r = 0; r < _bound_high; ++r) {
    for (int c = 0; c < _bound_high; ++c) {
      sum += grid.fields(r, c) == 'B' ? 1 : 0;
    }
  }

  // final grid needs to be printed out one last time
  grid.printGrid();


  // here we determine who won the game
  int draw_threshold = _bound_high * _bound_high / 2; // this is equivalent to half the available slots

  if (sum > draw_threshold) { // if there's more Bs than half the available slots B won, obv.
    std::cout << "B won!";
  } else if (sum == draw_threshold) { // should make sense - Bs = half num of fields => the other half is filled with As
    std::cout << "Draw!";
  } else {
    std::cout << "A won!";
  }

  return 0;
}