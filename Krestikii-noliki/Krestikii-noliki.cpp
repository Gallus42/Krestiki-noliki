#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include<Windows.h>
#include<conio.h>
#include<vector>
#include<io.h>
#include<string>

using namespace std;


enum Color
{
    Black, Blue, Green, Cyan, Red, Magenta, Brown,
    LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
void setColor(Color text, Color background)
{
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, esc = 27 };
void setCursor(int x, int y)
{
    COORD myCoords = { x,y };
    SetConsoleCursorPosition(hStdOut, myCoords);
}
void showMENU(vector<string>menu, int Punkt) {
    int X = 10, Y = 10;
    for (size_t i = 0; i < menu.size(); i++)
    {
        setCursor(X, Y + i);
        if (i == Punkt)
        {
            setColor(Red, Black);
        }
        else
        {
            setColor(White, Black);
        }
        cout << menu[i];
        setColor(White, Black);
    }
}
int MufMENU(vector<string>menu)
{
    int number = 0;
    int key;

    do
    {
        showMENU(menu, number);
        key = _getch();
        switch (key)
        {
        case Up:
            if (number > 0)
            {
                number--;
            }

            break;
        case Down:
            if (number < menu.size())
            {
                number++;
            }
            break;
        case Enter:
            system("cls");
            return number;
        }
    } while (key != esc);
}

// Функция для отрисовки игрового поля
void drawBoard(char board[9], int Punkt)
{
    int X = 10, Y = 10;
    // База
    for (size_t i = 0; i < 9; i++)
    {
        const int g = i % 3;
        if (i != 0 && g == 0)
        {
            Y += 2;
            X = 10;
            setCursor(X, Y);
            cout << "---+---+---";
            Y += 1;
            setCursor(X, Y);
        }

        setCursor(X, Y);
        if (i == Punkt)
        {
            setColor(Red, Black);
        }
        else
        {
            setColor(White, Black);
        }
        cout << i + 1;
        setColor(White, Black);
        X += 5;
        if (g != 2)
            cout << " | ";

    }
    // Наполнение
    X = 10; Y = 11;
    for (size_t i = 0; i < 9; i++)
    {
        const int g = i % 3;
        if (i != 0 && g == 0)
        {
            Y += 3;
            X = 10;
            setCursor(X, Y);
        }

        setCursor(X, Y);
        cout << board[i];
        X += 5;
        if (g != 2)
            cout << " | ";
    }


}

void showMessage(vector<string>message)
{

}


int SelectPos(char board[9])
{
    int number = 0;
    int key;
    while (true)
    {
        drawBoard(board, number);
        key = _getch();
        switch (key)
        {
        case Up:
            if (number > 3)
            {
                number -= 3;
            }

            break;
        case Down:
            if (number < 6)
            {
                number += 3;
            }
            break;
        case Left:
            if (number > 0)
            {
                number--;
            }
            break;
        case Right:
            if (number < 8)
            {
                number++;
            }
            break;
        case Enter:
            system("cls");
            return number;
        }
    }
}

// Функция для проверки выигрышной комбинации
bool checkWin(char board[9], char symbol) {
    // Проверка по горизонтали
    for (int i = 0; i <= 6; i += 3) {
        if (board[i] == symbol && board[i + 1] == symbol && board[i + 2] == symbol)
            return true;
    }

    // Проверка по вертикали
    for (int i = 0; i < 3; i++) {
        if (board[i] == symbol && board[i + 3] == symbol && board[i + 6] == symbol)
            return true;
    }

    // Проверка по диагонали
    if ((board[0] == symbol && board[4] == symbol && board[8] == symbol) ||
        (board[2] == symbol && board[4] == symbol && board[6] == symbol))
        return true;

    return false;
}

// Функция для хода компьютера
int computerMove(char board[9], char playerSymbol, char computerSymbol, int computerDifficulty)
{
    int possibleMoves[9];
    int numMoves = 0;
    int winCombinations[8][3]
    {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 4, 8},
        {2, 4, 6},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
    };


    // Поиск свободных клеток
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ')
            possibleMoves[numMoves++] = i;
    }

    if (0 == numMoves)
        return 0;
    
    auto randomPos = [numMoves, possibleMoves]()
    {
        // Генерация случайного хода
        int randomIndex = rand() % numMoves;
        return possibleMoves[randomIndex];
    };

    auto possibleMoveByWeightArray = [winCombinations, numMoves, possibleMoves](int weights[8])
    {
        int targetWeight = 2;
        while (targetWeight > 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (weights[i] == targetWeight)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        for (int z = 0; z < numMoves; z++)
                        {
                            if (winCombinations[i][j] == possibleMoves[z])
                            {
                                return possibleMoves[z];
                                break;
                            }
                        }

                    }

                }
            }

            --targetWeight;
        }

        return 0;
    };

    auto secondDifficultWeights = [possibleMoveByWeightArray, winCombinations, board, computerSymbol, playerSymbol, randomPos]()
    {
        // Ищем винкон
        int weights[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        int combinationIndex = 0;
        for (int k = 0; k < 9; k++)
        {
            for (int i = 0; i < 8; i++)
            {
                int weightCounter = 0;
                for (int j = 0; j < 3; j++)
                {
                    if (winCombinations[i][j] == k)
                    {
                        if (board[k] == computerSymbol)
                        {
                            ++weightCounter;
                        }
                        else if (board[k] == playerSymbol)
                        {
                            weightCounter = -1;
                            break;
                        }
                    }

                }

                weights[i] += weightCounter;
            }
        }

        return weights;
    };

    if(computerDifficulty == 1)
    {
        return randomPos();
    }
    else if (computerDifficulty == 2)
    {
        int ret = possibleMoveByWeightArray(secondDifficultWeights());
        if (ret == 0)
            return randomPos();
        else
            return ret;
    }
    else if (computerDifficulty == 3)
    {
        auto winWeights = secondDifficultWeights();
        // Ищем лузкон оппонента
        int loseWeights[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        int combinationIndex = 0;
        for (int k = 0; k < 9; k++)
        {
            for (int i = 0; i < 8; i++)
            {
                int weightCounter = 0;
                for (int j = 0; j < 3; j++)
                {
                    if (winCombinations[i][j] == k)
                    {
                        if (board[k] == playerSymbol)
                        {
                            ++weightCounter;
                        }
                    }

                }

                loseWeights[i] += weightCounter;
            }
        }

        // Если мы вот-вот выиграет - берём
        for (int i = 0; i < 8; ++i)
            if (winWeights[i] == 2)
                return possibleMoveByWeightArray(winWeights);

        // Если оппонент вот-вот выиграет - мешаем
        for (int i = 0; i < 8; ++i)
            if (loseWeights[i] == 2)
                return possibleMoveByWeightArray(loseWeights);
  
        
        // первый ход на этом уровне всегда одиннаковый, добавим случайности
        bool isFirstMove = true;
        
        for (int i = 0; i < 8; ++i)
        {
            if (winWeights[i] > 0)
            {
                isFirstMove = false;
                break;
            }
        }

        if (isFirstMove)
            return randomPos();

        int ret = possibleMoveByWeightArray(winWeights); // Иначе просто пытаемся выиграть
        if (ret != 0) 
            return ret;

        return possibleMoveByWeightArray(loseWeights); // Если и просто выиграть не получается то мешаем
    }

    delete[] possibleMoves;
}

void gameCycle(int computerDifficulty)
{
    char board[9];
    for (int i = 0; i < 9; ++i)
    {
        board[i] = ' ';
    }

    bool computerWon = false;
    bool playerWon = false;
    bool draw = false;

    // Определение символов для игрока и компьютера
    char playerSymbol, computerSymbol;
    while (true)
    {
        cout << "Выберите символ (X или O): ";
        cin >> playerSymbol;
        if (playerSymbol != 'X' && playerSymbol != 'O')
            cout << "Недопустимый символ";
        else break;
    }
    computerSymbol = (playerSymbol == 'X') ? 'O' : 'X';

    // Определение, кто ходит первым
    char currentPlayer = playerSymbol;

    // Игровой цикл
    while (true)
    {
        if (currentPlayer == playerSymbol) {
            // Ход игрока
            int playerMove = SelectPos(board);
            

            if (board[playerMove] == ' ') {
                board[playerMove] = playerSymbol;

                if (checkWin(board, playerSymbol)) {
                    playerWon = true;
                    break;
                }

                currentPlayer = computerSymbol;
            }
            else {
                cout << "Недопустимый ход. Попробуйте еще раз." << endl;
            }
        }
        else {
            // Ход компьютера
            cout << "Ход компьютера." << endl;
            int computerMoveIndex = computerMove(board, playerSymbol, computerSymbol, computerDifficulty);
            board[computerMoveIndex] = computerSymbol;

            if (checkWin(board, computerSymbol))
            {
                computerWon = true;
                break;
            }

            currentPlayer = playerSymbol;
        }

        bool isDraw = false;
        // Проверка на ничью
        for (int i = 0; i < 9; ++i)
        {
            if (board[i] == ' ')
            {
                isDraw = false;
                break;
            }
            else
            {
                isDraw = true;
            }
        }

        if (isDraw)
        {
            draw = true;
            break;
        }
    }

    if (playerWon)
        cout << "Вы выиграли! Поздравляем!" << endl;

    if (computerWon)
        cout << "Компьютер выиграл. Вы проиграли." << endl;

    if (draw)
        cout << "Ничья!" << endl;

    cout << "Игра окончена." << endl;
    drawBoard(board, 0);
}

int main()
{
    setlocale(LC_ALL, "Russian");
  
    srand(time(NULL));
    int punctMenu;
    vector<string>firstmenu = { "Новая игра" , "Выход" };
    vector<string>secondmenu = { "Лёгкий", "Средний", "Сложно" };
  
    while (true)
    {
        punctMenu = MufMENU(firstmenu);
        if (punctMenu == 0)
        {
            gameCycle(MufMENU(secondmenu) + 1);
            auto waitForInput = _getch();
            system("cls");
        }
        else
        {
            return 0;
        }
    }

    return 0;
}


