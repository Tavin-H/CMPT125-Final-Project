#include <algorithm>
#include <iostream>
#include <locale>
#include <random>
#include <string>
using namespace std;

//--------------GAME SETTINGS---------------
bool use_colour = false;
//------------------------------------------

/*
           - CONVENTIONS -
Columns are 0-6 (increasing to the right)
Rows (height) are 0-5 (increasing going up)

X = Red, O = Blue
 */

string GameOverString = R"(

      _____          _____        ______  _______        ______                 _____     ____      ____      ______        _____
  ___|\    \     ___|\    \      |      \/       \   ___|\     \           ____|\    \   |    |    |    | ___|\     \   ___|\    \
 /    /\    \   /    /\    \    /          /\     \ |     \     \         /     /\    \  |    |    |    ||     \     \ |    |\    \
|    |  |____| |    |  |    |  /     /\   / /\     ||     ,_____/|       /     /  \    \ |    |    |    ||     ,_____/||    | |    |
|    |   _____ |    |__|    | /     /\ \_/ / /    /||     \--'\_|/      |     |    |    ||    |    |    ||     \--'\_|/|    |/____/
|    |  |     ||    .--.    ||     |  \|_|/ /    / ||     /___/|        |     |    |    ||    |    |    ||     /___/|  |    |\    \
|    |  |__,  ||    |  |    ||     |       |    |  ||     \____|\       |\     \  /    /||\    \  /    /||     \____|\ |    | |    |
|\ ___\___/  /||____|  |____||\____\       |____|  /|____ '     /|      | \_____\/____/ || \ ___\/___ / ||____ '     /||____| |____|
| |   /____ / ||    |  |    || |    |      |    | / |    /_____/ |       \ |    ||    | / \ |   ||   | / |    /_____/ ||    | |    |
 \|___|    | / |____|  |____| \|____|      |____|/  |____|     | /        \|____||____|/   \|___||___|/  |____|     | /|____| |____|
      |____|/                                            |_____|/                                             |_____|/              )";

enum Piece {
  RedNormal,
  BlueNormal,
  RedAnvil,
  BlueAnvil,
};
enum Player {
  Blue,
  Red,
};

char board[7][6];
char boardcopy[7][6];

void init_board() {
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 6; j++) {
      board[i][j] = ' ';
    }
  }
}

void copy_board(char copyfrom[7][6], char copyto[7][6]) {
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 6; j++) {
      copyto[i][j] = copyfrom[i][j];
    }
  }
}

// Will move along a direction, counting the steps taken until it reaches a tile
// without a players piece
int traverse_and_count(int startx, int starty, int stepx, int stepy,
                       Player player) {
  // cout << "traversing" << stepx << stepy << endl;
  const char *searching_for = (player == Red) ? "xX" : "oO";
  // Check first one
  int count = 0;
  int nextx = startx + stepx;
  int nexty = starty + stepy;
  if (nextx > 6 || nextx < 0) {
    return 0;
  }
  if (nexty > 5 || nexty < 0) {
    return 0;
  }
  char current_tile_val = board[startx + stepx][starty + stepy];
  while (current_tile_val == searching_for[0] ||
         current_tile_val == searching_for[1]) {
    count++;

    // advance
    int nextx = startx + stepx * (count + 1);
    int nexty = starty + stepy * (count + 1);
    if (nextx > 6 || nextx < 0) {
      break;
    }
    if (nexty > 5 || nexty < 0) {
      break;
    }
    current_tile_val = board[nextx][nexty];
  }
  return count;
}
bool _check_straights(Player player, int startx, int starty) {
  int right = traverse_and_count(startx, starty, 1, 0, player);
  int left = traverse_and_count(startx, starty, -1, 0, player);
  int up = traverse_and_count(startx, starty, 0, 1, player);
  int down = traverse_and_count(startx, starty, 0, -1, player);
  /*
cout << "Right: " << right << endl;
cout << "Left: " << left << endl;
cout << "Up: " << up << endl;
cout << "Down: " << down << endl;
  */
  if (right + left >= 3) {
    return true;
  } else if (up + down >= 3) {
    return true;
  } else {
    return false;
  }
}
bool _check_diagonals(Player player, int startx, int starty) {
  int right_up = traverse_and_count(startx, starty, 1, 1, player);
  int right_down = traverse_and_count(startx, starty, 1, -1, player);
  int left_up = traverse_and_count(startx, starty, -1, 1, player);
  int left_down = traverse_and_count(startx, starty, -1, -1, player);
  if (right_up + left_down >= 3) {
    return true;
  } else if (left_up + right_down >= 3) {
    return true;
  } else {
    return false;
  }
}
bool check_win(Player player, int startx, int starty) {
  /*
if (_check_diagonals(player, startx, starty))
cout << "Diagnonals";
if (_check_straights(player, startx, starty))
cout << "Straights";
          */
  return _check_diagonals(player, startx, starty) ||
         _check_straights(player, startx, starty);
}

void print_board() {
  cout << "\n\n\n\n\n\n";
  cout << "Red is x" << endl;
  cout << "Blue is o" << endl;
  // Need to make 1x1 into 3x3
  for (int row = 5; row > -1; row--) {
    // Loops through each row

    for (int z = 0; z < 3; z++) {
      // Loops through the 3 rows of each smaller row

      for (int column = 0; column < 7; column++) {
        // Loops through each column

        if (use_colour) {
          if (board[column][row] == 'x') {
            if (z == 0) {
              cout << "\033[31m" << "╔ ═ ╗ " << "\033[0m";
            } else if (z == 1) {
              cout << "\033[31m" << "║ x ║ " << "\033[0m";
            } else {
              cout << "\033[31m" << "╚ ═ ╝ " << "\033[0m";
            }
          } else if (board[column][row] == 'o') {
            if (z == 0) {
              cout << "\033[34m" << "╔ ═ ╗ " << "\033[0m";
            } else if (z == 1) {
              cout << "\033[34m" << "║ o ║ " << "\033[0m";
            } else {
              cout << "\033[34m" << "╚ ═ ╝ " << "\033[0m";
            }

          } else if (board[column][row] == 'X') {
            if (z == 0) {
              cout << "\033[31m" << "╔ x ╗ " << "\033[0m";
            } else if (z == 1) {
              cout << "\033[31m" << "╚ ╦ ╝ " << "\033[0m";
            } else {
              cout << "\033[31m" << "╚ ╩ ╝ " << "\033[0m";
            }

          } else if (board[column][row] == 'O') {
            if (z == 0) {
              cout << "\033[34m" << "╔ o ╗ " << "\033[0m";
            } else if (z == 1) {
              cout << "\033[34m" << "╚ ╦ ╝ " << "\033[0m";
            } else {
              cout << "\033[34m" << "╚ ╩ ╝ " << "\033[0m";
            }

          } else {
            cout << ". . . ";
          }

        } else {
          // Not coloured output
          if (board[column][row] == 'x') {
            if (z == 0 || z == 2) {
              cout << "x x x ";
            } else {
              cout << "x x x ";
            }
          } else if (board[column][row] == 'o') {
            if (z == 0 || z == 2) {
              cout << "o o o ";
            } else {
              cout << "o o o ";
            }
          } else if (board[column][row] == 'X') {
            if (z == 0) {
              cout << "╔ x ╗ ";
            } else if (z == 1) {
              cout << "╚ ╦ ╝ ";
            } else {
              cout << "╚ ╩ ╝ ";
            }
          } else if (board[column][row] == 'O') {
            if (z == 0) {
              cout << "╔ o ╗ ";
            } else if (z == 1) {
              cout << "╚ ╦ ╝ ";
            } else {
              cout << "╚ ╩ ╝ ";
            }
          } else {
            cout << ". . . ";
          }
        }
      }
      cout << endl;
    }
  }
  for (int i = 0; i < 7; i++) {
    cout << "╟ " << i + 1 << " ╢ ";
  }
}

bool drop_piece(int column, Piece piece) {
  bool won = false;
  char *column_array = board[column];

  switch (piece) {
  case RedNormal:
    for (int i = 0; i < 6; i++) {
      if (column_array[i] == ' ') {
        column_array[i] = 'x';
        if (check_win(Red, column, i)) {
          won = true;
        }
        break;
      }
    }
    break;
  case BlueNormal:
    for (int i = 0; i < 6; i++) {
      if (column_array[i] == ' ') {
        column_array[i] = 'o';
        if (check_win(Blue, column, i)) {
          won = true;
        }
        break;
      }
    }
    break;
  case RedAnvil:
    for (int i = 0; i < 6; i++) {
      column_array[i] = ' ';
    }
    column_array[0] = 'X';
    if (check_win(Red, column, 0)) {
      won = true;
    }
    break;
  case BlueAnvil:
    for (int i = 0; i < 6; i++) {
      column_array[i] = ' ';
    }
    column_array[0] = 'O';
    if (check_win(Blue, column, 0)) {
      won = true;
    }
    break;
  }

  return won;
}
int main() {
  bool redAnvilPlayed = false;
  bool blueAnvilPlayed = false;
  bool running = false;
  bool botmode = false;
  string player1name;
  string player2name;
  string input;
  bool goingfirst = true;
  int turnsTaken = 0;
  init_board();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr06(0, 6);
  std::uniform_int_distribution<> distr010(0, 9);
  std::uniform_int_distribution<> distr01(0, 1);

  //// Manual testing stuff ////
  /*
board[0][0] = 'X';
board[1][1] = 'X';
board[2][2] = 'X';
board[3][3] = 'X';
  */
  // drop_piece(0, RedNormal);
  //  drop_piece(0, BlueNormal);
  //  print_board();
  //// end of manual testing area ////

  bool validInput = false;
  do {
    cout << "\nWelcome to Connect Four \n";
    cout << "Select who you're going up against: \n   (1): Another Player\n   "
            "(2): A Bot\n\n";
    cout << "Select by entering the corresponding number: ";
    cin >> input;
    cout << endl;
    if (input == "1" || input == "2" || input == "quit") {
      validInput = true;
      running = true;
    }
    if (input == "1") {
      cout << "What is Player 1's name?" << endl;
      cin >> player1name;
      cout << "What is Player 2's name?" << endl;
      cin >> player2name;
      input = "1";
    }
    if (input == "2") {
      botmode = true;
      string answer;
      cout << "would you like to go first (1), second (2), or random (3)?"
           << endl;
      cin >> answer;
      while (!(answer != "1" || answer != "2" || answer != "3")) {
        cout << "Please enter a valid number" << endl;
        cin >> answer;
      }
      if (answer == "1") {
        goingfirst = true;
      } else if (answer == "2") {
        goingfirst = false;
      } else {
        int first = distr01(gen);
        if (first == 0) {
          goingfirst = true;
        } else {
          goingfirst = false;
        }
      }
    }
    if (input == "quit") {
      running = false;
    }
  } while (!validInput);

  Piece current_piece;
  while (running) {
    print_board();
    if (botmode) {

      cout << "\n"
           << ((turnsTaken + !goingfirst) % 2 == 0 ? "Player" : "Computer")
           << "'s turn! " << endl;
    } else {

      cout << "\n"
           << (turnsTaken % 2 == 0 ? player1name : player2name) << "'s turn! "
           << endl;
    }
    bool valid_input = false;
    if (botmode && turnsTaken % 2 == goingfirst) {
      current_piece = BlueNormal;
      // int dice = rand() % 10;
      int dice = distr010(gen);
      if (!blueAnvilPlayed && dice == 7) {
        current_piece = BlueAnvil;
        blueAnvilPlayed = true;
      }
      valid_input = true;
    }
    while (!valid_input) {

      if (!(botmode && !goingfirst && turnsTaken == 0)) {

        cout << "Choose a piece: Regular (r) Anvil (a)" << endl;
        cin >> input;
        if (input == "quit") {
          valid_input = true;
          running = false;
        } else if (input == "r") {
          current_piece =
              turnsTaken % 2 == !goingfirst ? RedNormal : BlueNormal;
          valid_input = true;
        } else if (input == "a") {
          if (turnsTaken % 2 == !goingfirst) { // Red if true
            if (redAnvilPlayed) {
              cout << "You already dropped an Anvil piece! Placing Regular"
                   << endl;
              current_piece = RedNormal;
            } else {
              current_piece = RedAnvil;
              redAnvilPlayed = true;
            }
          } else {
            if (blueAnvilPlayed) {
              cout << "You already dropped an Anvil piece! Placing Regular"
                   << endl;
              current_piece = BlueNormal;
            } else {
              current_piece = BlueAnvil;
              blueAnvilPlayed = true;
            }
          }
          valid_input = true;
        } else {
          cout << endl << "Invalid input" << endl;
        }
      } else {
        cout << "COMPUTER GOING FIRST" << endl;
        valid_input = true;
      }
    }
    valid_input = false;
    if (botmode && turnsTaken % 2 == goingfirst) {
      for (int i = 0; i < 7; i++) {
        copy_board(board, boardcopy);
        if (drop_piece(i, RedNormal)) { // Prevent player from winning
          input = to_string(i + 1);
          valid_input = true;
        }
        copy_board(boardcopy, board);
      }
      for (int i = 0; i < 7; i++) {
        copy_board(board, boardcopy);
        if (drop_piece(i, current_piece)) { // Winning itself (higher priority)
          input = to_string(i + 1);
          valid_input = true;
        }
        copy_board(boardcopy, board);
      }
      while (!valid_input) {
        // int current = rand() % 7;

        int current = distr06(gen);
        if (board[current][5] == ' ') {
          input = to_string(current + 1);
          valid_input = true;
        }
      }
    }
    if (!running) {
      break;
    }
    while (!valid_input) {
      cout
          << "Which column would you like to place in? (enter a number between "
             "1-7)"
          << endl;
      cin >> input;
      if (input == "quit") {
        running = false;
        valid_input = true;
      } else if (input.empty() || !isdigit(input[0])) {
        cout << endl << "Please enter a number" << endl;

      } else if (stoi(input) < 8 && stoi(input) > 0) {
        if (board[stoi(input) - 1][5] == ' ') {
          valid_input = true;
        } else {
          cout << endl << "Column is full!" << endl;
        }
      } else {
        cout << endl << "Invalid Input" << endl;
      }
    }
    if (botmode) {
      if (current_piece == BlueNormal) {
        cout << endl
             << "The bot chose to place a normal piece in column " << input;
      } else if (current_piece == BlueAnvil) {
        cout << endl
             << "The bot chose to place an anvil piece in column " << input;
      }
    }
    if (!running) {
      break;
    }
    int position = stoi(input) - 1;
    if (drop_piece(position, current_piece)) {
      print_board();
      cout << GameOverString << endl;
      if (botmode) {

        cout << "Game over! "
             << (turnsTaken % 2 == goingfirst ? "Computer" : "Player")
             << " Won!" << endl;
      } else {

        cout << "Game over! "
             << (turnsTaken % 2 == 0 ? player1name : player2name) << " Won!"
             << endl;
      }
      running = false;
    }
    bool tie = true;
    for (int i = 0; i < 7; i++) {
      if (board[i][5] == ' ') {
        tie = false;
      }
    }
    if (tie) {
      cout << GameOverString << endl;
      cout << "It's a tie!";
      running = false;
    }
    turnsTaken++;
  }
  return 0;
}
