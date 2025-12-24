#include <stdio.h>
#include <conio.h>
#include <windows.h>  
#include <stdlib.h>   
#include <stdbool.h>  


#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"


int height = 20, width = 50;
int x, y;
int fruitX, fruitY;
int score;
int gameover;
int currentMap = 2;
int tailX[100], tailY[100]; 
int nTail;                


enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum direction dir;


void enableAnsi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void RespawnFruit()
{
    int valid = 0;

    while (!valid)
    {
        fruitX = rand() % width;
        fruitY = rand() % height;
        
        valid = 1; 

        if (currentMap == 2)
        {
            if ((fruitY == 11 || fruitY == 10 || fruitY == 9) && (fruitX < 15 || fruitX > 34))
                valid = 0;
            
            if ((fruitX == 24 || fruitX == 25 || fruitX == 26) && (fruitY < 5 || fruitY > 14))
                valid = 0;
        }

        for (int i = 0; i < nTail; i++)
        {
            if (tailX[i] == fruitX && tailY[i] == fruitY)
                valid = 0;
        }

        if (fruitX == x && fruitY == y)
            valid = 0;
    }
}

void setup()
{
    gameover = 0;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    RespawnFruit();
    score = 0;
    
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void draw()
{
    gotoxy(0, 0);

    int i , j;
    for (i = 0; i < width + 2; i++)
        printf(BLUE "#" RESET);
    printf("\n");

    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(j == 0) 
                printf(BLUE "#" RESET);  //left divar

            else if(j == width - 1)
                printf(BLUE "#" RESET);  //right divar

            else if(i == y && j == x)
                printf("O");

            else if(currentMap == 2 && (i == 11 || i == 10 || i == 9) && (j < 15 || j > 34))
                printf(CYAN "#" RESET);

            else if(currentMap == 2 && (j == 24 || j == 25 || j == 26) && (i < 5 || i > 14))
                printf(CYAN "#" RESET);

            else if(i == fruitY && j == fruitX)
                printf(RED "O" RESET);

            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        printf("o"); 
                        print = true;
                    }
                }
                if (!print)
                    printf(" ");
            }
        }
        printf("\n");
    }

    for (i = 0; i < width + 2; i++)
        printf(BLUE "#" RESET);
    printf("\n");
}

void input()
{
    if(_kbhit())
    {
       char key = _getch();

       switch (key)
       {
            case 'd':
                if(dir != LEFT)
                    dir = RIGHT;
                break;

            case 'D':
                if(dir != LEFT)
                    dir = RIGHT;
                break;
            
            case 'a':
                if(dir != RIGHT)
                    dir = LEFT;
                break;

            case 'A':
                if(dir != RIGHT)
                    dir = LEFT;
                break;

            case 'w':
                if(dir != DOWN)
                    dir = UP;
                break;

            case 'W':
                if(dir != DOWN)
                    dir = UP;
                break;

            case 's':
                if(dir != UP)
                    dir = DOWN;
                break;

            case 'S':
                if(dir != UP)
                    dir = DOWN;
                break;
            
            case 'x':
                gameover = 1;
                break;
       } 
    }
}

void logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
        case RIGHT:
            x++;
            break;
        case LEFT:
            x--;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
    }

    if(x >= width || x < 0)
        gameover = 1;
    
    if(y >= height || y < 0)
        gameover = 1;

    if(currentMap == 2 && (y == 11 || y == 10 || y == 9) && (x < 15 || x > 34))
        gameover = 1;

    if(currentMap == 2 && (x == 24 || x == 25 || x == 26) && (y < 5 || y > 14))
        gameover = 1;
    
    if(x == fruitX && y == fruitY)
    {
        score += 10;
        nTail++;
        RespawnFruit();
    }

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
            gameover = 1;
    }

}

int main()
{
    enableAnsi();
    setup();
    
    while (!gameover)
    {
        draw();
        input();
        logic();
        Sleep(70);
    }
    return 0;
}