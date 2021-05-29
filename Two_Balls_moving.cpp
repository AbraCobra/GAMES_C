#include "TXLib.h"

void MoveBalls();

int main()
    {
    txCreateWindow (800, 600);

    MoveBalls();

    return 0;
    }

void MoveBalls()
    {
    int  x = 0,  y = 0, vx = 5, vy = 3;
    int ax = 0, ay = 1;

    int  x1 = 100, y1 = 0, vx1 = 5, vy1 = 3;
    int ax1 = 0,  ay1 = 1;
    int  dt = 1;

    int radius = 15;



    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txSetColor (RGB (x, y, 128), 2);
        txSetFillColor (RGB (x/2, y/2, 128));

        txCircle ( x,  y, radius);

        txSetColor (RGB(x, y, 128), 2);
        txSetFillColor (RGB (x/2, y/2, 128));

        txCircle (x1, y1, radius);

        x   +=  vx * dt;
        y   +=  vy * dt;
        vx  +=  ax * dt;
        vy  +=  ay * dt;
        vx1 += ax1 * dt;
        vy1 += ay1 * dt;
        x1  += vx1 * dt;
        y1  += vy1 * dt;

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

//================================================

        if (x1 >= 800 - radius)
            {
            vx1 = -vx1;
             x1 = 800 - radius;
            }

        if (y1 >= 600 - radius)
            {
            vy1 = -vy1;
             y1 = 600 - radius;
            }

         if (x1 <= 0 + radius)
            {
            vx1 = -vx1;
             x1 = 0 + radius;
            }

        if (y1 <= 0 + radius)
            {
            vy1 = -vy1;
             y1 = 0 + radius;
            }

        if (txGetAsyncKeyState (VK_RIGHT)) vx++;
        if (txGetAsyncKeyState (VK_LEFT))  vx--;
        if (txGetAsyncKeyState (VK_UP))    vy++;
        if (txGetAsyncKeyState (VK_DOWN))  vy--;

        if (txGetAsyncKeyState (VK_SPACE)) vx=vy=0;

         txSleep(10);
         }

    }


