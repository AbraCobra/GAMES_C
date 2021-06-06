#include "TXLib.h"
#include <cmath>

void MovingObject();
void DrawObject    (int x, int y, int vx, int vy, int a, int b, int radius);
void PhysicsObject (int* x, int* y, int* vx, int* vy, int ax, int ay, int dt, int* radius);
void ControlObject (int* vx, int* vy);
void Fon();

int main()
    {
    txCreateWindow (800, 600);

    MovingObject();

    return 0;
    }

void MovingObject()
    {
    int x1 =   0, y1 = 0, vx1 = 2 + rand() % 10, vy1 = 2 + rand() % 10, ax1 = 1, ay1 = 0;

    int x2 = 150, y2 = 0, vx2 = 2 + rand() % 10, vy2 = 2 + rand() % 10, ax2 = 1, ay2 = 0;

    int x3 = 350, y3 = 0, vx3 = 2 + rand() % 10, vy3 = 2 + rand() % 10, ax3 = 1, ay3 = 0;

    int x4 =  10, y4 = 0, vx4 = 2 + rand() % 10, vy4 = 2 + rand() % 10, ax4 = 1, ay4 = 0;

    int x5 = 250, y5 = 0, vx5 = 2 + rand() % 10, vy5 = 2 + rand() % 10, ax5 = 1, ay5 = 0;

    int x6 = 650, y6 = 0, vx6 = 2 + rand() % 10, vy6 = 2 + rand() % 10, ax6 = 1, ay6 = 0;

    int dt = 1, radius = 35;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        Fon();
        txClear();

        DrawObject (x1, y1, vx1, vy1, 100, 200, radius);
        DrawObject (x2, y2, vx2, vy2, 120, 175, radius);
        DrawObject (x3, y3, vx3, vy3, 140, 150, radius);
        DrawObject (x4, y4, vx4, vy4, 160, 125, radius);
        DrawObject (x5, y5, vx5, vy5, 180, 100, radius);
        DrawObject (x6, y6, vx6, vy6, 220,  75, radius);

        PhysicsObject (&x1, &y1, &vx1, &vy1, ax1, ay1, dt, &radius);
        PhysicsObject (&x2, &y2, &vx2, &vy2, ax2, ay2, dt, &radius);
        PhysicsObject (&x3, &y3, &vx3, &vy3, ax3, ay3, dt, &radius);
        PhysicsObject (&x4, &y4, &vx4, &vy4, ax4, ay4, dt, &radius);
        PhysicsObject (&x5, &y5, &vx5, &vy5, ax5, ay5, dt, &radius);
        PhysicsObject (&x6, &y6, &vx6, &vy6, ax6, ay6, dt, &radius);

        ControlObject (&vx1, &vy1);

        txSleep(40);
        }
    }

void DrawObject (int x, int y, int vx, int vy, int a, int b, int radius)
    {
    for (int i = 1; i <= radius; i++)
        {
        txSetColor     (RGB (a + i, b + i/2, b + i/2), 2);
        txSetFillColor (RGB (a + i, b + i/2, b + i/2));
        txCircle (x, y, radius - i);
        }
    //txLine   (x, y, x + vx*7, y + vy*7);
    }
void Fon()
    {
    txSetFillColor (TX_BLACK);
    POINT fon [4] = {{0, 0}, {800, 0}, {800, 600}, {0, 600}};
    txPolygon (fon,4);
    }

void PhysicsObject (int* x, int* y, int* vx, int* vy, int ax, int ay, int dt, int* radius)
    {
   // *vx = *vx + ax * dt;
   // *vy = *vy + ay * dt;

    *x = *x + *vx * dt;
    *y = *y + *vy * dt;

    if (*x >= 800 - *radius)
        {
        *vx = - *vx;
        *x  = 800 - *radius;
        }

    if (*y >= 600 - *radius)
        {
        *vy = - *vy;
        *y  = 600 - *radius;
        }

    if (*x <= 0 + *radius)
        {
        *vx = - *vx;
        *x  = 0 + *radius;
        }

    if (*y <= 0 + *radius)
        {
        *vy = - *vy;
        *y  = 0 + *radius;
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
