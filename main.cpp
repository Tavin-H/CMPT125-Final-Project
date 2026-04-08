#include <algorithm>
#include <iostream>
#include <locale>
#include <string>
using namespace std;

//--------------GAME SETTINGS---------------
bool use_colour = true;
//------------------------------------------

/*
  _    _ _____            _   _ _______ ____  _   _
 | |  | |_   _|     /\   | \ | |__   __/ __ \| \ | |
 | |__| | | |      /  \  |  \| |  | | | |  | |  \| |
 |  __  | | |     / /\ \ | . ` |  | | | |  | | . ` |
 | |  | |_| |_   / ____ \| |\  |  | | | |__| | |\  |
 |_|  |_|_____| /_/    \_\_| \_|  |_|  \____/|_| \_|


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

char col1[6] = {};
char col2[6] = {};
char col3[6] = {};
char col4[6] = {};
char col5[6] = {};
char col6[6] = {};
char col7[6] = {};

char **board = new char *[7]{col1, col2, col3, col4, col5, col6, col7};

void init_board() {
  for (int i = 0; i < 6; i++) {
    col1[i] = ' ';
    col2[i] = ' ';
    col3[i] = ' ';
    col4[i] = ' ';
    col5[i] = ' ';
    col6[i] = ' ';
    col7[i] = ' ';
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
  cout << "Right: " << right << endl;
  cout << "Left: " << left << endl;
  cout << "Up: " << up << endl;
  cout << "Down: " << down << endl;
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
  cout << "\n\n\n\n\n\n\n\n\n";
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
  // bool redAnvilPlayed = false;
  // bool blueAnvilPlayed = false;
  bool running = false;
  // bool botmode; // determines whether player is facing the bot or not
  string input;
  int turnsTaken = 0;
  init_board();

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
  } while (!validInput);

  while (running) {
    print_board();
    cout << "\n"
         << (turnsTaken % 2 == 0 ? "Red" : "Blue") << "'s turn! " << endl;
    cin >> input;
    int position = stoi(input) - 1;
    if (drop_piece(position, turnsTaken % 2 == 0 ? RedNormal : BlueNormal)) {
      cout << GameOverString << endl;
      running = false;
    }
    turnsTaken++;
    if (input == "quit") {
      running = false;
    }
  }
  cout << "working! \n";
  return 0;
}
