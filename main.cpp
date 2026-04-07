#include <iostream>
#include <locale>
#include <string>
using namespace std;

//--------------GAME SETTINGS---------------
bool use_colour = false;
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

enum Piece { RedNormal, BlueNormal, RedAnvil, BlueAnvil };

char *col1 = new char[6];
char *col2 = new char[6];
char *col3 = new char[6];
char *col4 = new char[6];
char *col5 = new char[6];
char *col6 = new char[6];
char *col7 = new char[6];

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

void drop_piece(int column, Piece piece) {
  char *column_array = board[column];

  switch (piece) {
  case RedNormal:
    for (int i = 0; i < 6; i++) {
      if (column_array[i] == ' ') {
        column_array[i] = 'x';
        return;
      }
    }
    break;
  case BlueNormal:
    for (int i = 0; i < 6; i++) {
      if (column_array[i] == ' ') {
        column_array[i] = 'o';
        return;
      }
    }
    break;
  case RedAnvil:
    for (int i = 0; i < 6; i++) {
      column_array[i] = ' ';
    }
    column_array[0] = 'X';
    break;
  case BlueAnvil:
    for (int i = 0; i < 6; i++) {
      column_array[i] = ' ';
    }
    column_array[0] = 'O';
    break;
  }

  return;
}

int main() {
  // bool redAnvilPlayed = false;
  // bool blueAnvilPlayed = false;
  bool running = false;
  //bool botmode; // determines whether player is facing the bot or not
  string input;
  int turnsTaken = 0;
  init_board();
  //// Manual testing stuff ////
  // board[0][0] = 'X';
  // drop_piece(0, RedNormal);
  // drop_piece(0, BlueNormal);
  // print_board();
  //// end of manual testing area ////
  bool validInput = false;
  do {
  cout << "\nWelcome to Tic-Tac-Toe! \n";
  cout << "Select who you're going up against: \n   (1): Another Player\n   (2): A Bot\n\n";
  cout << "Select by entering the corresponding number: ";
  cin >> input; cout << endl;
  if(input == "1" || input == "2" || input == "quit") {
    validInput = true;
    running = true;
  }
  } while(!validInput);

  while (running) {
    print_board();
    cout << "\n"
         << (turnsTaken % 2 == 0 ? "Red" : "Blue") << "'s turn! " << endl;
    cin >> input;
    int position = stoi(input) - 1;
    drop_piece(position, turnsTaken % 2 == 0 ? RedNormal : BlueNormal);
    turnsTaken++;
    if(input == "quit") {
      running = false;
    }
  }
  cout << "working! \n";
  return 0;
}
