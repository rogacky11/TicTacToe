#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <limits>

using namespace std;

// Funkcja do czyszczenia terminala
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void drawBoard(const vector<vector<char>>& board) {
    int size = board.size();
    string horizontalLine = " ";
    for (int i = 0; i < size; i++) {
        horizontalLine += "----";
    }

    cout << " " << horizontalLine << endl;
    for (int i = 0; i < size; i++) {
        cout << i + 1 << " " << "|";
        for (int j = 0; j < size; j++) {
            cout << " " << board[i][j] << " |";
        }
        cout << endl << " " << horizontalLine << endl;
    }
    for (int k = 0; k < size; k++) {
        cout << "   " << k + 1;
    }
    cout << endl;
}

bool isValidMove(const vector<vector<char>>& board, int row, int col) {
    int size = board.size();
    return row >= 0 && row < size&& col >= 0 && col < size&& board[row][col] == '-';
}

bool isBoardFull(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == '-')
                return false;
        }
    }
    return true;
}

bool hasWon(const vector<vector<char>>& board, char symbol, int winLength) {
    int size = board.size();

    // wiersze
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= size - winLength; j++) {
            bool hasWon = true;
            for (int k = 0; k < winLength; k++) {
                if (board[i][j + k] != symbol) {
                    hasWon = false;
                    break;
                }
            }
            if (hasWon)
                return true;
        }
    }

    // kolumny
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= size - winLength; j++) {
            bool hasWon = true;
            for (int k = 0; k < winLength; k++) {
                if (board[j + k][i] != symbol) {
                    hasWon = false;
                    break;
                }
            }
            if (hasWon)
                return true;
        }
    }
    // przekątne
    for (int i = 0; i <= size - winLength; i++) {
        for (int j = 0; j <= size - winLength; j++) {
            bool hasWon = true;
            for (int k = 0; k < winLength; k++) {
                if (board[i + k][j + k] != symbol) {
                    hasWon = false;
                    break;
                }
            }
            if (hasWon)
                return true;
        }
    }

    for (int i = 0; i <= size - winLength; i++) {
        for (int j = winLength - 1; j < size; j++) {
            bool hasWon = true;
            for (int k = 0; k < winLength; k++) {
                if (board[i + k][j - k] != symbol) {
                    hasWon = false;
                    break;
                }
            }
            if (hasWon)
                return true;
        }
    }

    return false;
}

int evaluate(const vector<vector<char>>& board, char playerSymbol, char computerSymbol, int winLength) {
    if (hasWon(board, computerSymbol, winLength))
        return winLength + 1;  
    else if (hasWon(board, playerSymbol, winLength))
        return -winLength - 1;  
    else
        return 0;
}

int minimax(vector<vector<char>>& board, int depth, int alpha, int beta, bool isMaximizingPlayer, char playerSymbol, char computerSymbol, int winLength, int maxDepth) {
    int score = evaluate(board, playerSymbol, computerSymbol, winLength);

    if (score == winLength + 1 || score == -winLength - 1 || depth == maxDepth)
        return score;

    if (isMaximizingPlayer) {
        int maxScore = numeric_limits<int>::min();

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == '-') {
                    board[i][j] = computerSymbol;
                    int currentScore = minimax(board, depth + 1, alpha, beta, false, playerSymbol, computerSymbol, winLength, maxDepth);
                    board[i][j] = '-';

                    maxScore = max(maxScore, currentScore);
                    alpha = max(alpha, maxScore);

                    if (beta <= alpha)
                        break;
                }
            }
        }

        return maxScore;
    }
    else {
        int minScore = numeric_limits<int>::max();

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == '-') {
                    board[i][j] = playerSymbol;
                    int currentScore = minimax(board, depth + 1, alpha, beta, true, playerSymbol, computerSymbol, winLength, maxDepth);
                    board[i][j] = '-';

                    minScore = min(minScore, currentScore);
                    beta = min(beta, minScore);

                    if (beta <= alpha)
                        break;
                }
            }
        }

        return minScore;
    }
}

void makeComputerMove(vector<vector<char>>& board, char playerSymbol, char computerSymbol, int winLength, int maxDepth) {
    bool isFirstMove = true;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board.size(); j++) {
            if (board[i][j] != '-') {
                isFirstMove = false;
                break;
            }
        }
        if (!isFirstMove)
            break;
    }

    if (isFirstMove) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, board.size() - 1);

        int randomRow = dis(gen);
        int randomCol = dis(gen);

        board[randomRow][randomCol] = computerSymbol;
    }
    else {
        int bestScore = numeric_limits<int>::min();
        int bestRow = -1;
        int bestCol = -1;

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == '-') {
                    board[i][j] = computerSymbol;
                    int currentScore = minimax(board, 0, numeric_limits<int>::min(), numeric_limits<int>::max(), false, playerSymbol, computerSymbol, winLength, maxDepth);
                    board[i][j] = '-';

                    if (currentScore == winLength + 2) {
                        board[i][j] = computerSymbol;
                        return;
                    }
                    else if (currentScore == -winLength - 1) {
                        if (bestScore < -winLength - 2) {
                            bestScore = currentScore;
                            bestRow = i;
                            bestCol = j;
                        }
                    }
                    else if (currentScore > bestScore) {
                        bestScore = currentScore;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }

        if (bestRow != -1 && bestCol != -1) {
            board[bestRow][bestCol] = computerSymbol;
        }
    }
}

int main() {
    int size;
    int winLength;
    int maxdepth;

    cout << "Podaj rozmiar planszy: ";
    cin >> size;

    cout << "Podaj ilosc znakow w rzedzie wymaganych do wygranej: ";
    cin >> winLength;

    cout << "Podaj glebokosc rekursji komputera: ";
    cin >> maxdepth;

    vector<vector<char>> board(size, vector<char>(size, '-'));

    char playerSymbol, computerSymbol; int maxDepth = 6;

    cout << "Wybierz swoj symbol (X lub O): ";
    cin >> playerSymbol;

    if (playerSymbol == 'X' || playerSymbol == 'x')
        computerSymbol = 'O';
    else
        computerSymbol = 'X';

    int currentPlayer = 1;
    int row, col;
    while (true) {
        clearScreen();

        drawBoard(board);

        if (currentPlayer == 1) {

            cout << "Twoj ruch, podaj rzad i kolumne (od 1 do " << size << "): ";
            cin >> row >> col;
            row--;
            col--;

            if (!isValidMove(board, row, col)) {
                cout << "Nieprawidlowy ruch. Sprobuj ponownie." << endl;
                continue;
            }


            board[row][col] = playerSymbol;

            if (hasWon(board, playerSymbol, winLength)) {
                clearScreen();
                drawBoard(board);
                cout << "Gracz wygral!" << endl;
                break;
            }
            if (isBoardFull(board)) {
                clearScreen();
                drawBoard(board);
                cout << "Remis!" << endl;
                break;
            }

            currentPlayer = 2;
        }
        else {
            cout << "Ruch komputera..." << endl;
            makeComputerMove(board, playerSymbol, computerSymbol, winLength, maxDepth);

            if (hasWon(board, computerSymbol, winLength)) {
                clearScreen();
                drawBoard(board);
                cout << "Komputer wygral!" << endl;
                break;
            }
            if (isBoardFull(board)) {
                clearScreen();
                drawBoard(board);
                cout << "Remis!" << endl;
                break;
            }

            currentPlayer = 1;
        }
    }

    return 0;
}