#include <iostream>
#include <stdio.h>
#include <random>
#include "TXLib.h"
#include <ctime>

struct Blocks
    {
    int x, y;
    int width, height;
    int GapX, GapY;
    int block_hit;
    };

struct Ball
    {
    double x, y;
    double vx, vy;
    double ax, ay;
    int radius;
    int missed_balls;

    void Draw_Ball();
    void Physics_Ball(double dt, int* game_state, int* score);
    };

struct Paddle
    {
    double x, y;
    double vx, vy, ax;

    int Paddle_width;
    int Paddle_height;
    double friction;
    void Draw_Paddle();
    void Physics_Paddle (double dt);
    };

#define SIZE_ROWS 6
#define SIZE_COLS 9
#define ORIGIN_X 10
#define ORIGIN_Y 70
#define LEFT_EDGE_WINDOW 0
#define RIGHT_EDGE_WINDOW 780
#define TOP_WINDOW 0
#define BOTTON_WINDOW 700
#define GAME_STATE_INIT 0
#define GAME_START_LEVEL 1
#define GAME_STATE_RUN   2
#define GAME_STATE_SHUTDOWN 3
#define GAME_STATE_EXIT 4

void Fill_array  (int arr[][SIZE_COLS], int rows);
void Print_array (int arr[][SIZE_COLS], int rows);
void Draw_blocks (int arr[][SIZE_COLS], int rows, Blocks* Set);
void Control_CollisionsBP (Ball* ball, Paddle* paddle);
void Control_CollisionsBB (int arr[][SIZE_COLS], int rows, Ball* ball,
                            Blocks* Set, int* game_state, int* score, int* number_level);
void Paddle_mouse (Paddle* paddle);
void Cycling (int game_state, int score, int number_level);
void OutputScore (char item [], int score, int tablo_x, int tablo_y, COLORREF color_score);
void Start_Picture();
void Finish_Picture();

Blocks Set = {10, 50, 80, 20, 85, 25, 0};
Ball ball  = {400, 400, 10, 10, 0, 0, 10, 0};
Paddle paddle = {0, 600, 0.5, 0, 0, 180, 20, 0.9};
double dt  = 1.0;
int number_level = 1;
int game_state = 0;
int score = 0;
int arr[SIZE_ROWS][SIZE_COLS]{};

int main()
    {
    txCreateWindow (RIGHT_EDGE_WINDOW, BOTTON_WINDOW);

    Cycling (game_state, score, number_level);

    return 0;
    }

void Cycling (int game_state, int score, int number_level)
    {
    HDC  Playing_field_View = txLoadImage ("fon.bmp");
    double dt = 1.0;
    bool quit = false;
    while (!quit)
        {
        switch (game_state)
            {
            case GAME_STATE_INIT:
                game_state = GAME_START_LEVEL;
                break;

            case GAME_START_LEVEL:
                Fill_array  (arr, SIZE_ROWS);
                game_state = GAME_STATE_RUN;
                break;

            case GAME_STATE_RUN:
                txBitBlt  (txDC(), 0, 0, RIGHT_EDGE_WINDOW, BOTTON_WINDOW, Playing_field_View, 0, 0);

                paddle.Physics_Paddle (dt);
                Draw_blocks (arr, SIZE_ROWS, &Set);
                ball.Physics_Ball (dt, &game_state, &score);

                Control_CollisionsBP (&ball, &paddle);
                Control_CollisionsBB (arr, SIZE_ROWS, &ball, &Set, &game_state, &score, &number_level);

                paddle.Draw_Paddle();
                ball.Draw_Ball();

                OutputScore ("—чЄт", score, 30, 650, TX_RED);
                OutputScore ("”ровень", number_level, 630, 650, TX_RED);

                if (txGetAsyncKeyState (VK_ESCAPE)) game_state = GAME_STATE_SHUTDOWN;

                break;

            case GAME_STATE_SHUTDOWN:
                score = 0;
                quit = true;
                Finish_Picture();
                if (txGetAsyncKeyState (VK_SPACE)) game_state = GAME_STATE_RUN;
                break;
            }

        txSleep (30);
        }

    txDeleteDC (Playing_field_View);
    }

void Fill_array (int arr[][SIZE_COLS], int rows)
    {
    for (int i = 0; i < SIZE_ROWS; i ++)
        {
        for (int j = 0; j < SIZE_COLS; j ++)
            {
            arr[i][j] = 1;
            }
        }
    }

void Print_array (int arr[][SIZE_COLS], int rows)
    {
    for (int i = 0; i < SIZE_ROWS; i ++)
        {
        for (int j = 0; j < SIZE_COLS; j ++)
            {
            std::cout << arr[i][j] << "\t";
            }
        std::cout<<std::endl;
        }
    }

void Draw_blocks (int arr[][SIZE_COLS], int rows, Blocks* Set)
    {
    for (int i = 0; i < SIZE_ROWS; i ++)
        {
        (*Set).x = ORIGIN_X;
        for (int j = 0; j < SIZE_COLS; j ++)
            {
            if (arr[i][j]!= 0)
                {
                txSetColor (TX_BLUE, 2);
                txSetFillColor (TX_CYAN);
                txRectangle((*Set).x, (*Set).y, (*Set).x + (*Set).width, (*Set).y + (*Set).height);
                }
                (*Set).x += (*Set).GapX;
            }
            (*Set).y += (*Set).GapY;
        }
    (*Set).y = ORIGIN_Y;
    }

void Ball::Draw_Ball()
    {
    txSetColor (TX_WHITE, 2);
    txSetFillColor (TX_YELLOW);
    txCircle (x, y, radius);
    }

void Ball::Physics_Ball (double dt, int* game_state, int* score)
    {
    vx = vx + ax * dt;
    vy = vy + ay * dt;

    x = x + vx * dt;
    y = y + vy * dt;

    if (x >= RIGHT_EDGE_WINDOW - radius)
        {
        vx = - vx;
         x  = RIGHT_EDGE_WINDOW - radius;
        }

    if (y >= BOTTON_WINDOW - radius)
        {
        missed_balls += 1;
        *score -= 10;
        y  = BOTTON_WINDOW/2 - radius;
        }

    if (x <= LEFT_EDGE_WINDOW + radius)
        {
        vx = - vx;
        x  = LEFT_EDGE_WINDOW + radius;
        }

    if (y <= TOP_WINDOW + radius)
        {
        vy = - vy;
        y  = TOP_WINDOW + radius;
        }
    if (missed_balls == 5) (*game_state) = GAME_STATE_SHUTDOWN;

    }

void Paddle::Draw_Paddle()
    {
    txSetColor (TX_WHITE);
    txSetFillColor (TX_WHITE);
    txRectangle (txMouseX(), y, txMouseX() + Paddle_width, y + Paddle_height);
    }

void Paddle::Physics_Paddle (double dt)
    {

//    vx = vx + (ax * dt) * friction;
//    x = x + vx * dt ;
    x = txMouseX();


    //if (txGetAsyncKeyState (VK_RIGHT)) vx++;

    if (x + Paddle_width >= RIGHT_EDGE_WINDOW)
        {
        vx = - vx;
        x  = RIGHT_EDGE_WINDOW - Paddle_width;
        }

    //if (txGetAsyncKeyState (VK_LEFT))  vx--;

    if (x <= LEFT_EDGE_WINDOW)
        {
        vx = - vx;
        x  = LEFT_EDGE_WINDOW;
        }

    if (txGetAsyncKeyState (VK_SPACE))  vx = 0;

    if (vx >= 15) vx = 10;
    }

void Control_CollisionsBP (Ball* ball, Paddle* paddle)
    {
    if (((*ball).x + (*ball).radius >= (*paddle).x)
        && ((*ball).x + (*ball).radius <= ((*paddle).x + (*paddle).Paddle_width))
        && ((*ball).y + (*ball).radius >= (*paddle).y)
        && ((*ball).y + (*ball).radius <= ((*paddle).y + (*paddle).Paddle_height)))
        {

        (*ball).vy =- (*ball).vy;
        (*ball).vy= -1.0f * abs((*ball).vy);
        (*ball).y  += (*ball).vy;

        if (txGetAsyncKeyState (VK_RIGHT)) (*ball).vx -= (rand()%10);

        if (txGetAsyncKeyState (VK_LEFT))  (*ball).vx += (rand()%10);
        else
         (*ball).vx += (-1 + rand()%10);

         }
    }

void Control_CollisionsBB (int arr[][SIZE_COLS], int rows, Ball* ball, Blocks* Set, int* game_state, int* score, int* number_level)
    {
     for (int i = 0; i < SIZE_ROWS; i ++)
        {
        (*Set).x = ORIGIN_X;
        for (int j = 0; j < SIZE_COLS; j ++)
            {
            if (arr[i][j]!=0)
                {
                if (((*ball).x + (*ball).radius > (*Set).x)
                    && ((*ball).x + (*ball).radius < ((*Set).x + (*Set).width))
                    && ((*ball).y + (*ball).radius > (*Set).y)
                    && ((*ball).y + (*ball).radius < ((*Set).y + (*Set).height)))
                    {
                    (*Set).block_hit += 1;
                    *score += 15;
                    arr[i][j] = 0;

                    (*ball).vy = -(*ball).vy;
                    (*ball).vx += (rand()%3);
                    }
                }
            (*Set).x += (*Set).GapX;
            }
        (*Set).y += (*Set).GapY;

        }
    (*Set).y = ORIGIN_Y;
    if ((*Set).block_hit == SIZE_ROWS*SIZE_COLS)
        {
        *game_state = GAME_STATE_INIT;
        *score = 0;
        *number_level ++;
        }
   }
void OutputScore (char item [], int score, int tablo_x, int tablo_y, COLORREF color_score)
    {
    txSelectFont ("Times", 26, 12);
    char str [30] = "";
    sprintf (str, "%s: %d", item, score);
    txSetColor (color_score);
    txSetFillColor (color_score);
    txTextOut (tablo_x, tablo_y, str);
    }

void Finish_Picture()
    {
    HDC  Playing_field_View1 = txLoadImage ("fon_over.bmp");
    txBitBlt  (txDC(), 0, 0, RIGHT_EDGE_WINDOW, BOTTON_WINDOW, Playing_field_View1, 0, 0);
    txDeleteDC (Playing_field_View1);
    }
