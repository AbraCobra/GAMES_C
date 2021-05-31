#include "TXLib.h"

void MovingObject();
void DrawObject    (int x, int y, int vx, int vy, COLORREF outcolor, COLORREF fillcolor);
void PhysicsObject (int* x, int* y, int* vx, int* vy, int ax, int ay, int dt, int radius);
void ControlObject (int* vx, int* vy);

int main()
    {
    txCreateWindow (800, 600);

    MovingObject();

    return 0;
    }

void MovingObject()
    {
    int x1 =   0, y1 = 0, vx1 = 5, vy1 = 3, ax1 = 1, ay1 = 1;

    int x2 = 150, y2 = 0, vx2 = 5, vy2 = 3, ax2 = 1, ay2 = 1;

    int x3 = 350, y3 = 0, vx3 = 5, vy3 = 3, ax3 = 1, ay3 = 1;

    int dt = 1, radius = 15;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        DrawObject(x1, y1, vx1, vy1, TX_LIGHTGREEN, TX_GREEN);
        DrawObject (x2, y2, vx2, vy2, TX_RED, TX_BLUE);
        DrawObject (x3, y3, vx3, vy3, TX_LIGHTCYAN, TX_CYAN);

        PhysicsObject (&x1, &y1, &vx1, &vy1, ax1, ay1, dt, radius);
        PhysicsObject (&x2, &y2, &vx2, &vy2, ax2, ay2, dt, radius);
        PhysicsObject (&x3, &y3, &vx3, &vy3, ax3, ay3, dt, radius);

        ControlObject (&vx1, &vy1);

        txSleep(10);
        }
    }

void DrawObject (int x, int y, int vx, int vy, COLORREF outcolor, COLORREF fillcolor)
    {
    txSetColor (outcolor, 2);
    txSetFillColor (fillcolor);

    txCircle (x, y, 15);
    txLine   (x, y, x + vx*5, y + vy*5);
    }

void PhysicsObject (int* x, int* y, int* vx, int* vy, int ax, int ay, int dt, int radius)
    {
//    *vx = *vx + ax * dt;
//    *vy = *vy + ay * dt;

    *x = *x + *vx * dt;
    *y = *y + *vy * dt;

    if (*x >= 800 - radius)
        {
        *vx = - *vx;
        *x  = 800 - radius;
        }

    if (*y >= 600 - radius)
        {
        *vy = - *vy;
        *y  = 600 - radius;
        }

    if (*x <= 0 + radius)
        {
        *vx = - *vx;
        *x  = 0 + radius;
        }

    if (*y <= 0 + radius)
        {
        *vy = - *vy;
        *y  = 0 + radius;
        }
    }

void ControlObject (int* vx, int* vy)
    {
    if (txGetAsyncKeyState (VK_RIGHT)) *vx = *vx + 1;
    if (txGetAsyncKeyState (VK_LEFT))  *vx = *vx - 1;
    if (txGetAsyncKeyState (VK_UP))    *vy = *vy - 1;
    if (txGetAsyncKeyState (VK_DOWN))  *vy = *vy + 1;
    if (txGetAsyncKeyState (VK_SPACE)) *vx = *vy = 0;
    }
