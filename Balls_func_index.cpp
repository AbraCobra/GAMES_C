
#include "TXLib.h"

void MovingSphere();
void DrawSphere    (int x, int y, int vx, int vy, COLORREF outcolor, COLORREF spherecolor);
void PhysicsSphere (int* x, int* y, int* vx, int* vy, int ax, int ay, int dt, int radius);
void ControlSphere (int* vx, int* vy);

int main()
    {
    txCreateWindow (800, 600);

    MovingSphere();

    return 0;
    }

void MovingSphere()
    {
    int x1 =   0, y1 = 0, vx1 = 5, vy1 = 3, ax1 = 0, ay1 = 1;

    int x2 = 150, y2 = 0, vx2 = 5, vy2 = 3, ax2 = 0, ay2 = 1;

    int x3 = 350, y3 = 0, vx3 = 5, vy3 = 3, ax3 = 0, ay3 = 1;

    int dt = 1, radius = 15;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        DrawSphere (x1, y1, vx1, vy1, TX_LIGHTGREEN, TX_GREEN);
        DrawSphere (x2, y2, vx2, vy2, TX_RED, TX_BLUE);
        DrawSphere (x3, y3, vx3, vy3, TX_LIGHTCYAN, TX_CYAN);

        PhysicsSphere (&x1, &y1, &vx1, &vy1, ax1, ay1, dt, radius);
        PhysicsSphere (&x2, &y2, &vx2, &vy2, ax2, ay2, dt, radius);
        PhysicsSphere (&x3, &y3, &vx3, &vy3, ax3, ay3, dt, radius);

        ControlSphere (&vx1, &vy1);

        txSleep(10);
        }
    }

void DrawSphere (int x, int y, int vx, int vy, COLORREF outcolor, COLORREF spherecolor)
    {
    txSetColor (outcolor, 2);
    txSetFillColor (spherecolor);

    txCircle (x, y, 15);
    }

void PhysicsSphere (int* x, int* y, int* vx, int* vy, int ax, int ay, int dt, int radius)
    {
    *vx = *vx + ax * dt;
    *vy = *vy + ay * dt;

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

void ControlSphere (int* vx, int* vy)
    {
    if (txGetAsyncKeyState (VK_RIGHT)) ++*vx;
    if (txGetAsyncKeyState (VK_LEFT))  --*vx;
    if (txGetAsyncKeyState (VK_UP))    ++*vy;
    if (txGetAsyncKeyState (VK_DOWN))  --*vy;
    if (txGetAsyncKeyState (VK_SPACE)) *vx = *vy = 0;
    }
