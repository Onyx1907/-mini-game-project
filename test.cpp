// test game without using file handling
#include <stdio.h>
#include <conio.h>
#include <windows.h>  
#include <stdlib.h>   
#include <stdbool.h>  
#include <string.h>

void enableAnsi();
void gotoxy(int x, int y);
void setup();
void ShowMapSelection();
void showmenu();
void ShowGameoverScean();
void RespawnFruit();
void draw();
void input();
void logic();
void getPasswordMasked(char *password);
void drawLoginFrame();
void clearLine(int y);
void LoginScreen();
void ShowScoreboardScreen();
void saveBestScore();

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define PURPLE "\x1b[35m"
#define CYAN    "\x1b[36m"
#define GOLD    "\x1b[38;5;220m"
#define SILVER "\x1b[38;5;248m"
#define BRONZE  "\x1b[38;5;130m"
#define RESET   "\x1b[0m"

int height = 20, width = 50;
int x, y;
int fruitX, fruitY;
int isSuperApple = 0;
int score;
int gameover;
int currentMap = 1;
int tailX[100], tailY[100]; 
int nTail = 0;     
int gameoverDrawn = 0;      
int subMenuDrawn = 0;    
int mapSelectDrawn = 0;
int scoreboardDrawn = 0;

enum direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum direction dir;

typedef enum{
    LOGIN,
    MAP_SELECT,
    SUB_MENU,
    GAME,
    SCOREBOARD,
    GAMEOVER
} GameState;
GameState state = LOGIN;

struct USER{
    char username[50];
    char password[50];
    int bestScoreMap1 = 0;
    int bestScoreMap2 = 0; 
};
struct USER *currentUser = NULL;
struct USER users[50];
int userCnt = 0;

int main()
{
    enableAnsi();
    setup();
    
    while (1)
    {
        if(state == LOGIN)
        {
            LoginScreen();
            state = MAP_SELECT;
        }
        else if(state == MAP_SELECT)
        {
            ShowMapSelection();
        }
        else if(state == SUB_MENU)
        {
            showmenu();
        }
        else if(state == GAME)
        {
            draw();
            input();
            logic();
            if(currentMap == 1)
                Sleep(70);
            else
                Sleep(90);
            if(gameover)
                state = GAMEOVER;
        }
        else if(state == SCOREBOARD)
        {
            ShowScoreboardScreen();
        }
        else if(state == GAMEOVER)
            ShowGameoverScean();
    }
    return 0;
}

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

void ShowMapSelection()
{
    if(mapSelectDrawn == 0)
    {
        system("cls");
        printf("\n\n");
        printf(CYAN "         =====S N A K E  G A M E=====\n\n" RESET);
        printf(YELLOW "            ===== SELECT MAP =====\n\n\n" RESET);
        printf("         1. Map One (Classic)\n\n");
        printf("         2. Map Two (Obstacles)\n\n");
        printf("         3. " RED "Logout\n\n" RESET);
        printf("         Select: ");
        
        mapSelectDrawn = 1;
    }

    if(_kbhit())
    {
        char ch = _getch();
        if(ch == '1')
        {
            currentMap = 1;       
            state = SUB_MENU;     
            subMenuDrawn = 0;     
            mapSelectDrawn = 0;   
        }
        else if(ch == '2')
        {
            currentMap = 2;      
            state = SUB_MENU;    
            subMenuDrawn = 0;
            mapSelectDrawn = 0;
        }
        else if(ch == '3')
        {
            state = LOGIN;
            subMenuDrawn = 0;
            mapSelectDrawn = 0;
        }
    }
}

void showmenu()
{
    if(subMenuDrawn == 0)
    {
        system("cls");
    
        printf("\n\n");
        printf(CYAN "         =====S N A K E  G A M E=====\n\n" RESET);
        if(currentMap == 1)
            printf(GREEN "                 ==M A P  1==\n\n\n" RESET);
        else if(currentMap == 2)
            printf(GREEN "                 ==M A P  2==\n\n\n" RESET);
        printf("         1. Start Game\n\n");
        printf("         2. " YELLOW "Score Board\n\n" RESET);
        printf("         3. " RED "Back\n\n" RESET);
        printf("         Select: ");

        subMenuDrawn = 1;
    }

    if(_kbhit())
    {
        char ch = _getch();
        if(ch == '1')
        {
            setup();
            state = GAME;
            subMenuDrawn = 0;
        }
        else if(ch == '2')
        {
            state = SCOREBOARD;
            scoreboardDrawn = 0;
            subMenuDrawn = 0;
        }
        else if(ch == '3')
        {
            state = MAP_SELECT;
            mapSelectDrawn = 0;
            subMenuDrawn = 0;
        }
    }
}

void ShowGameoverScean()
{
    gameover = 0;
    nTail = 0;
    if(gameoverDrawn == 0)
    {
        saveBestScore();
        system("cls");

        printf("\n\n");
        printf(RED "         =====GAME OVER=====\n\n\n" RESET);
        printf(YELLOW "         Score : %d\n\n" RESET, score);
        printf("         Press 'R' to Retry or 'Q' to Quit\n");

        gameoverDrawn = 1;
    }

    if(_kbhit())
    {
        char ch = _getch();
        if(ch == 'r' || ch == 'R')
        {
            setup();
            state = GAME;
            gameoverDrawn = 0;
            subMenuDrawn = 0;
        }
        else if((ch == 'q' || ch == 'Q'))
        {
            setup();
            state = SUB_MENU;
            gameoverDrawn = 0;
            subMenuDrawn = 0;
        }
    }
}

void RespawnFruit()
{
    int valid = 0;

    while (!valid)
    {
        fruitX = rand() % (width) + 1;
        fruitY = rand() % height;
        
        isSuperApple = (rand() % 10) == 0;

        valid = 1; 

        if (currentMap == 2)
        {
            if ((fruitY == 11 || fruitY == 10 || fruitY == 9) && (fruitX < 16 || fruitX > 35))
                valid = 0;
            
            if ((fruitX == 27 || fruitX == 25 || fruitX == 26) && (fruitY < 5 || fruitY > 14))
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

void draw()
{
    gotoxy(0, 0);

    int i , j;
    for (i = 0; i < width + 2; i++)
        printf(BLUE "%c" RESET, 219);
    printf("\n");

    for(i = 0; i < height; i++)
    {
        for(j = 0; j <= width + 1; j++)
        {
            if(j == 0) 
                printf(BLUE "%c" RESET, 219);  //left divar

            else if(j == width + 1)
                printf(BLUE "%c" RESET, 219);  //right divar

            else if(i == y && j == x)
                printf("O");

            else if(currentMap == 2 && (i == 11 || i == 10 || i == 9) && (j < 16 || j > 35))
                printf(CYAN "%c" RESET, 219);

            else if(currentMap == 2 && (j == 25 || j == 26 || j == 27) && (i < 5 || i > 14))
                printf(CYAN "%c" RESET, 219);

            else if(i == fruitY && j == fruitX)
            {
                if(isSuperApple)
                    printf(YELLOW "%c" RESET, 149);
                else
                    printf(RED "%c" RESET, 149);
            }
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
        printf(BLUE "%c" RESET, 219);
    printf("\n\n");
    printf(YELLOW "Score : %d\n" RESET, score);
    printf("Length of your snake : %d", nTail + 1);
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

    if(x >= width + 1 || x <= 0)
        gameover = 1;
    
    if(y >= height || y < 0)
        gameover = 1;

    if(currentMap == 2 && (y == 11 || y == 10 || y == 9) && (x < 16 || x > 35))
        gameover = 1;

    if(currentMap == 2 && (x == 25 || x == 26 || x == 27) && (y < 5 || y > 14))
        gameover = 1;
    
    if(x == fruitX && y == fruitY)
    {
        if(isSuperApple)
        {
            score += 15;
            RespawnFruit();
        }
        else
        {
            score += 5;
            nTail++;
            RespawnFruit();
        }
    }

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
            gameover = 1;
    }

}

void getPasswordMasked(char *password) 
{
    int i = 0;
    char ch;
    while (1) {
        ch = _getch(); 
        
        if (ch == 13) { //enter
            password[i] = '\0';
            break;
        }
        else if (ch == 8) { //backspace
            if (i > 0) {
                i--;
                printf("\b \b"); 
            }
        }
        else if (i < 49) {
            password[i] = ch;
            i++;
            printf("*"); 
        }
    }
}

void drawLoginFrame() 
{
    system("cls");
    printf("\n\n");
    printf(CYAN "      =======================================\n");
    printf("      |              USER LOGIN             |\n");
    printf("      =======================================\n" RESET);
    printf("\n");
    printf("      Username: \n");
    printf("\n");
    printf("      Password: \n");
    printf("\n");
    printf(CYAN "      =======================================\n" RESET);
}

void clearLine(int y) 
{
    gotoxy(0, y);
    printf("                                                 "); 
    gotoxy(0, y);
}

void LoginScreen() 
{

    char inputUser[50];
    char inputPass[50];
    char inputPass2[50];
    int found = 0;
    int i = 0;
    struct USER *tempUser;

    drawLoginFrame();  

    gotoxy(16, 6);
    printf(YELLOW);

    scanf("%s", inputUser);
    printf(RESET);

    for (i = 0; i < userCnt; i++) 
    {
        tempUser = &users[i];
        if (strcmp(tempUser->username, inputUser) == 0) 
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        while (1) 
        {
            gotoxy(16, 8);
            printf(YELLOW);
            getPasswordMasked(inputPass);
            printf(RESET);

            if (strcmp(tempUser->password, inputPass) == 0) 
            {
                currentUser = tempUser;
                clearLine(10);
                gotoxy(6, 10);
                printf(GREEN "Login Successful! Welcome back, %s :)" RESET, currentUser->username);
                Sleep(1200);
                return;
            }
            else 
            {
                clearLine(10);
                gotoxy(6, 10);
                printf(RED "Wrong Password! Please Try again..." RESET);
                gotoxy(16, 8);
                printf("                         ");
            }
        }
    } 

    else 
    {
        clearLine(10);
        gotoxy(6, 10);
        printf(PURPLE "New user detected! Creating account..." RESET);
        Sleep(1000);

        gotoxy(16, 8);
        printf(YELLOW);
        getPasswordMasked(inputPass);
        printf(RESET);

        while (1)
        {
            clearLine(10);
            gotoxy(6, 10);
            printf(PURPLE "Enter your password again!" RESET);
            Sleep(1000);
            gotoxy(16, 8);
            printf("                                       ");
            gotoxy(16, 8);
            printf(YELLOW);
            getPasswordMasked(inputPass2);
            printf(RESET);

            if(strcmp(inputPass, inputPass2) == 0)
            {
                currentUser = &users[userCnt];
                strcpy(currentUser->username, inputUser);
                strcpy(currentUser->password, inputPass);
                currentUser->bestScoreMap1 = 0;
                currentUser->bestScoreMap2 = 0;

                userCnt++;

                clearLine(10);
                gotoxy(6, 10);
                printf(GREEN "Registration Successful! Welcome, %s." RESET, currentUser->username);
                Sleep(1500);
                return;
            }
            else
            {
                clearLine(10);
                gotoxy(6, 10);
                printf(RED "Wrong Password! Please Try again..." RESET);
                Sleep(1500);
            }
        }
    }
}

void ShowScoreboardScreen() 
{

    if (scoreboardDrawn == 0) 
    {
        system("cls"); 

        struct USER allUsers[100];

        printf("\n\n");
        printf(PURPLE "      =======================================\n");
        printf("      |        TOP PLAYERS (Map %d)          |\n", currentMap);
        printf("      =======================================\n" RESET, currentMap);
        printf("\n");
        
        printf(CYAN "      %-6s %-20s %-10s\n" RESET, "RANK", "USERNAME", "SCORE");
        printf("      ---------------------------------------\n");


        for (int i = 0; i < userCnt - 1; i++) 
        {
            for (int j = 0; j < userCnt - i - 1; j++) 
            {
                int scoreA, scoreB;
                
                if (currentMap == 1) 
                {
                    scoreA = users[j].bestScoreMap1;
                    scoreB = users[j + 1].bestScoreMap1;
                } 
                else 
                {
                    scoreA = users[j].bestScoreMap2;
                    scoreB = users[j + 1].bestScoreMap2;
                }

                if (scoreB > scoreA) {
                    struct USER temp = users[j];
                    users[j] = users[j + 1];
                    users[j + 1] = temp;
                }
            }
        }

        if (userCnt == 0) 
        {
            printf("\n      No records found yet!\n");
        }

        for (int i = 0; i < userCnt; i++) 
        {
            int score = (currentMap == 1) ? users[i].bestScoreMap1 : users[i].bestScoreMap2;
      
            char *color = RESET;
            if (i == 0) color = GOLD;      
            else if (i == 1) color = SILVER;    
            else if (i == 2) color = BRONZE;     
            printf("%s      %-6d %-20s %-10d" RESET "\n", color, i + 1, users[i].username, score);
        }
        
        printf("\n\n");
        printf("      Press any key to return to menu...");
        scoreboardDrawn = 1; 
    }

    if (_kbhit()) {
        char ch = _getch(); 
        
        scoreboardDrawn = 0; 
    
        state = SUB_MENU;
        subMenuDrawn = 0; 
    }
}

void saveBestScore()
{
    if(currentMap == 1)
    {
        if(score > currentUser->bestScoreMap1)
        {
            currentUser->bestScoreMap1 = score;
        }
    }
    else
    {
        if(score > currentUser->bestScoreMap2)
        {
            currentUser->bestScoreMap2 = score;
        }
    }
    for (int i = 0; i < userCnt; i++)
    {
        if(strcmp(currentUser->username, users[i].username) == 0)
        {
            if(currentMap == 1)
                users[i].bestScoreMap1 = currentUser->bestScoreMap1;
            else
                users[i].bestScoreMap2 = currentUser->bestScoreMap2;
            break;
        }
    }
    
}

