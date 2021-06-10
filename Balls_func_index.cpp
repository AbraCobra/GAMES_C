#include "TXLib.h"
#include <cmath>

void MovingObject();
void DrawObject       (float x, float y, float vx, float vy, int a, int b, int radius);
void PhysicsObject    (float* x, float* y, float* vx, float* vy, float ax, float ay, int dt, int radius);
void ControlObject    (float* vx, float* vy);
void ControlCollision (float* x1, float* y1, float* x2, float* y2,
                       float* vx1, float* vy1, float* vx2, float* vy2, int radius, int dt, int* collisions);
float Distance        (float x1, float y1, float x2, float y2);
void OutputScore      (int* collisions);


int main()
    {
    txCreateWindow (800, 600);

    MovingObject();

    return 0;
    }

void MovingObject()
    {
    HDC  background_Cartoon = txLoadImage ("bush.bmp");

    float x1 =   0, y1 = 0, vx1 = 5 + rand() % 10, vy1 = 5 + rand() % 10, ax1 = 1, ay1 = 0;

    float x2 = 150, y2 = 0, vx2 = 5 + rand() % 10, vy2 = 5 + rand() % 10, ax2 = 1, ay2 = 0;

    int dt = 1, radius = 55, collisions  = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBitBlt  (txDC(), 0, 0, 800, 600, background_Cartoon, 0, 0);


        DrawObject (x1, y1, vx1, vy1, 100, 200, radius);
        DrawObject (x2, y2, vx2, vy2, 200, 175, radius);


        PhysicsObject (&x1, &y1, &vx1, &vy1, ax1, ay1, dt, radius);
        PhysicsObject (&x2, &y2, &vx2, &vy2, ax2, ay2, dt, radius);

        ControlCollision (&x1, &y1, &x2, &y2, &vx1, &vy1, &vx2, &vy2, radius, dt, &collisions);

        ControlObject (&vx1, &vy1);

        OutputScore (&collisions);

        txSleep(20);
        }

    txDeleteDC (background_Cartoon);
    }

void DrawObject (float x, float y, float vx, float vy, int a, int b, int radius)
    {
    for (int i = 1; i <= radius; i++)
        {
        txSetColor     (RGB (a + i, b + i/2, b + i/2), 2);
        txSetFillColor (RGB (a + i, b + i/2, b + i/2));
        txCircle (x, y, radius - i);
        }
    //txLine   (x, y, x + vx*7, y + vy*7);
    }

float Distance (float x1, float y1, float x2, float y2)
    {
    float distance = sqrt ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return distance;
    }

void PhysicsObject (float* x, float* y, float* vx, float* vy, float ax, float ay, int dt, int radius)
    {
   // *vx = *vx + ax * dt;
   // *vy = *vy + ay * dt;

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

void ControlCollision (float* x1, float* y1, float* x2, float* y2,
                       float* vx1, float* vy1, float* vx2, float* vy2,
                       int radius, int dt, int* collisions)
    {
    float distance = Distance (*x1, *y1, *x2, *y2); if (distance == 0) distance = 0.01;
    float angle_sin = (*x1 - *x2)/distance;
    float angle_cos = (*y1 - *y2)/distance;

      	if (distance < 2*radius)
            {
      		float Vn1 = *vx2*angle_sin + *vy2*angle_cos;

      		float Vn2 = *vx1*angle_sin + *vy1*angle_cos;

      		float dt = (2*radius - distance)/(Vn1 - Vn2);

      		float Vt1 = -*vx2*angle_cos + *vy2*angle_sin;

      		float Vt2 = -*vx1*angle_cos + *vy1*angle_sin;

      		float temp = Vn2;
      		Vn2 = Vn1;
      		Vn1 = temp;

      		*vx1 = Vn2*angle_sin - Vt2*angle_cos;

      		*vy1 = Vn2*angle_cos + Vt2*angle_sin;

      		*vx2 = Vn1*angle_sin - Vt1*angle_cos;

      		*vy2 = Vn1*angle_cos + Vt1*angle_sin;

      		*x1 += *vx1*dt;
      		*y1 += *vy1*dt;
      		*x2 += *vx2*dt;
      		*y2 += *vy2*dt;

      		*collisions += 1;
            }
    }

void ControlObject (float* vx, float* vy)
    {
    if (txGetAsyncKeyState (VK_RIGHT)) *vx = *vx + 1;
    if (txGetAsyncKeyState (VK_LEFT))  *vx = *vx - 1;
    if (txGetAsyncKeyState (VK_UP))    *vy = *vy - 1;
    if (txGetAsyncKeyState (VK_DOWN))  *vy = *vy + 1;
    if (txGetAsyncKeyState (VK_SPACE)) *vx = *vy = 0;
    }

void OutputScore(int* collisions)
    {
    txSetColor (TX_YELLOW);
    txSetFillColor (TX_YELLOW);

    txSelectFont ("Times", 80);
    char strx[20] = " ";
    sprintf (strx, "collisions =% d", *collisions);

    int centerx = 220;
    int centery = 500;

    txTextOut (centerx, centery, strx);
    }
