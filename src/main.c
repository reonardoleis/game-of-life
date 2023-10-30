#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define SIZE 100
#define RENDER_SIZE 100
#define SLEEP 0.016
#define TICKS 250
#define CHANCE 50
#define MAX_STEPS 125

void initializeBoard(int board[SIZE][SIZE]);
int randint(int min, int max);
void renderBoard(int board[SIZE][SIZE]);
void stepBoard(int board[SIZE][SIZE]);
void copyBoard(int src[SIZE][SIZE], int dst[SIZE][SIZE]);
void cclear();

int randint(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

void initializeBoard(int board[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            if (randint(0, 100) <= CHANCE)
            {
                board[i][j] = 1;
            }
            else
            {
                board[i][j] = 0;
            }
        }
}

void renderBoard(int board[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < RENDER_SIZE - 1; i++)
        for (j = 0; j < RENDER_SIZE - 1; j++)
        {
            if (board[i][j] == 1)
            {
                mvaddch(j + 1, i + 1, ' ' | COLOR_PAIR(2));
            }
        }
}

void copyBoard(int src[SIZE][SIZE], int dst[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            dst[i][j] = src[i][j];
        }
}

void stepBoard(int board[SIZE][SIZE])
{
    int new_board[SIZE][SIZE];
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            int lx = i - 1 < 0 ? 0 : i - 1;
            int ux = i + 1 >= SIZE ? SIZE - 1 : i + 1;
            int ly = j - 1 < 0 ? 0 : j - 1;
            int uy = j + 1 >= SIZE ? SIZE - 1 : j + 1;
            int a, b;

            int count = 0;
            for (a = lx; a <= ux; a++)
            {
                for (b = ly; b <= uy; b++)
                {
                    if (a == i && b == j)
                        continue;
                    if (board[b][a] == 1)
                        count++;
                }
            }

            if (board[i][j] == 1 && (count == 2 || count == 3))
            {
                new_board[i][j] = 1;
            }
            else if (board[i][j] == 0 && count == 3)
            {
                new_board[i][j] = 1;
            }
            else
            {
                new_board[i][j] = 0;
            }
        }

    copyBoard(new_board, board);
}

void cclear()
{
    int i, j;
    for (i = 0; i < RENDER_SIZE; i++)
        for (j = 0; j < RENDER_SIZE; j++)
        {

            mvaddch(j, i, ' ' | COLOR_PAIR(1));
        }
}

int main()
{
    initscr();
    start_color();
    srand(time(NULL));
    int board[SIZE][SIZE];
    initializeBoard(board);
    int steps = 0;
    int ticks = 0;
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    while (true)
    {
        ticks++;
        cclear();
        renderBoard(board);
        if (ticks >= TICKS)
        {
            stepBoard(board);
            refresh();
            ticks = 0;

            steps++;
            if (steps >= MAX_STEPS)
            {
                steps = 0;
                initializeBoard(board);
            }
        }
        sleep(SLEEP);
    }

    return 0;
}