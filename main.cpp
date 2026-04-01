#include <iostream>
using namespace std;

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
  // Need to make 1x1 into 3x3
  for (int row = 0; row < 6; row++) {
    // Loops through each row

    for (int z = 0; z < 3; z++) {
      // Loops through the 3 rows of each smaller row

      for (int column = 0; column < 7; column++) {
        // Loops through each column

        if (board[column][row] == 'x') {
          if (z == 0 || z == 2) {
            cout << "\033[31m" << "x x x " << "\033[0m";
          } else {
            cout << "\033[31m" << "x o x " << "\033[0m";
          }
        } else {
          cout << ". . . ";
        }
        if (board[column][row] == 'X') {
          if (z == 0 || z == 2) {
            cout << "\033[34m" << "x x x " << "\033[0m";
          } else {
            cout << "\033[34m" << "x o x " << "\033[0m";
          }
        } else {
          cout << ". . . ";
        }
      }
      cout << endl;
    }
  }
}

int main() {
  init_board();
  board[0][0] = 'x';
  board[2][0] = 'x';
  board[4][0] = 'X';
  board[6][2] = 'x';
  print_board();
  cout << "working!";
  return 0;
}
