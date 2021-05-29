#include "TXLib.h"

void MoveBall();

int main()
    {
    txCreateWindow (800, 600);

    MoveBall();

    return 0;
    }

void MoveBall()
    {
    int x = 0, y = 0, vx = 5, vy = 3;

    int dt = 1;

    int radius = 15;

    txSetColor (TX_LIGHTGREEN, 2);
    txSetFillColor (RGB (204, 255, 51));

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txCircle(x, y, radius);

        x += vx * dt;
        y += vy * dt;

        if (x >= 800 - radius)
            {
            vx = -vx;
             x = 800 - radius;
            }

        if (y >= 600 - radius)
            {
            vy = -vy;
             y = 600 - radius;
            }

         if (x <= 0 + radius)
            {
            vx = -vx;
             x = 0 + radius;
            }

        if (y <= 0 + radius)
            {
            vy = -vy;
             y = 0 + radius;
            }

        if (txGetAsyncKeyState (VK_RIGHT)) vx++;
        if (txGetAsyncKeyState (VK_LEFT))  vx--;
        if (txGetAsyncKeyState (VK_UP))    vy++;
        if (txGetAsyncKeyState (VK_DOWN))  vy--;

        if (txGetAsyncKeyState (VK_SPACE)) vx=vy=0;

        txSleep(10);
         }
    }
