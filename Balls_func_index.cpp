#include "TXLib.h"
#include <cmath>


struct Ball
   {
    double  x,  y,
           vx, vy;
    int radius;
    };

void MovingObject();
void DrawObject        (Ball  ball);
void PhysicsObject     (Ball* ball, double ax, double ay, int dt);
void ControlObject     (Ball* ball1);
void ControlCollision  (Ball* ball1, Ball* ball2, int dt, int* collisions);
double Distance        (double x1, double y1, double x2, double y2);
void OutputScore       (int  collisions);
void OutputCoordinates (Ball ball1, Ball ball2);


int main()
    {
    txCreateWindow (800, 600);

    MovingObject();

    return 0;
    }


void MovingObject()
    {
    HDC  background_Cartoon = txLoadImage ("bush.bmp");

    Ball ball1 = {10, 10, 5 + rand() % 10, 5 + rand() % 10, 55};

    Ball ball2 = {20, 20, 5 + rand() % 10, 5 + rand() % 10, 50};

    int dt = 1,  collisions  = 0;

    double ax = 0, ay = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBitBlt  (txDC(), 0, 0, 800, 600, background_Cartoon, 0, 0);

        DrawObject (ball1);
        DrawObject (ball2);


        PhysicsObject (&ball1, ax, ay, dt);
        PhysicsObject (&ball2, ax, ay, dt);

        ControlCollision (&ball1, &ball2, dt, &collisions);

        ControlObject (&ball1);

        OutputScore (collisions);

        OutputCoordinates (ball1, ball2);

        txSleep(20);
        }

    txDeleteDC (background_Cartoon);
    }

void DrawObject (Ball ball)
    {
    int a = 100, b = 200;

    for (int i = 1; i <= ball.radius; i++)
        {
        txSetColor     (RGB (a + i, b + i/2, b + i/2), 2);
        txSetFillColor (RGB (a + i, b + i/2, b + i/2));
        txCircle (ball.x, ball.y, ball.radius - i);
        }
    //txLine   (ball.x, ball.y, ball.x + (ball.vx)*7, ball.y + (ball.vy)*7);
    }

double Distance (double x1, double y1, double x2, double y2)
    {
    double distance = sqrt ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return distance;
    }

void PhysicsObject (Ball* ball,  double ax, double ay, int dt)
    {
    (*ball).vx = (*ball).vx + ax * dt;
    (*ball).vy = (*ball).vy + ay * dt;

    (*ball).x = (*ball).x + (*ball).vx * dt;
    (*ball).y = (*ball).y + (*ball).vy * dt;

    if ((*ball).x >= 800 - (*ball).radius)
        {
        (*ball).vx = - (*ball).vx;
        (*ball).x  = 800 - (*ball).radius;
        }

    if ((*ball).y >= 600 - (*ball).radius)
        {
        (*ball).vy = - (*ball).vy;
        (*ball).y  = 600 - (*ball).radius;
        }

    if ((*ball).x <= 0 + (*ball).radius)
        {
        (*ball).vx = - (*ball).vx;
        (*ball).x  = 0 + (*ball).radius;
        }

    if ((*ball).y <= 0 + (*ball).radius)
        {
        (*ball).vy = - (*ball).vy;
        (*ball).y  = 0 + (*ball).radius;
        }
    }

void ControlCollision (Ball* ball1, Ball* ball2, int dt, int* collisions)
    {
    double distance = Distance ((*ball1).x, (*ball1).y, (*ball2).x, (*ball2).y); if (distance == 0) distance = 0.01;

    double angle_sin = ((*ball1).x - (*ball2).x)/distance;
    double angle_cos = ((*ball1).y - (*ball2).y)/distance;

    if (distance < ((*ball1).radius + (*ball2).radius))
        {
        double Vn1 = (*ball2).vx*angle_sin + (*ball2).vy*angle_cos;

        double Vn2 = (*ball1).vx*angle_sin + (*ball1).vy*angle_cos;

        double dt  = (((*ball1).radius + (*ball2).radius) - distance)/(Vn1 - Vn2);

        double Vt1 = - (*ball2).vx*angle_cos + (*ball2).vy*angle_sin;

        double Vt2 = - (*ball1).vx*angle_cos + (*ball1).vy*angle_sin;

        double temp = Vn2;
        Vn2 = Vn1;
        Vn1 = temp;

        (*ball1).vx = Vn2*angle_sin - Vt2*angle_cos;

        (*ball1).vy = Vn2*angle_cos + Vt2*angle_sin;

        (*ball2).vx = Vn1*angle_sin - Vt1*angle_cos;

        (*ball2).vy = Vn1*angle_cos + Vt1*angle_sin;

        (*ball1).x +=  (*ball1).vx*dt;
        (*ball1).y +=  (*ball1).vy*dt;
        (*ball2).x +=  (*ball2).vx*dt;
        (*ball2).y +=  (*ball2).vy*dt;

        *collisions += 1;
        }
    }

void ControlObject (Ball* ball1)
    {
    if (txGetAsyncKeyState (VK_RIGHT)) (*ball1).vx = (*ball1).vx + 1;
    if (txGetAsyncKeyState (VK_LEFT))  (*ball1).vx = (*ball1).vx - 1;
    if (txGetAsyncKeyState (VK_UP))    (*ball1).vy = (*ball1).vy - 1;
    if (txGetAsyncKeyState (VK_DOWN))  (*ball1).vy = (*ball1).vy + 1;
    if (txGetAsyncKeyState (VK_SPACE)) (*ball1).vx = (*ball1).vy = 0;
    }

void OutputScore (int collisions)
    {
    txSetColor (TX_YELLOW);
    txSetFillColor (TX_YELLOW);

    txSelectFont ("Times", 50);
    char strx[20] = " ";
    sprintf (strx, "collisions =% d", collisions);

    int centerx = 20;
    int centery = 500;

    txTextOut (centerx, centery, strx);
    }

void OutputCoordinates (Ball ball1, Ball ball2)
    {
    char strx1 [20] = "";
    char stry1 [20] = "";
    char strx2 [20] = "";
    char stry2 [20] = "";
    sprintf (strx1, "coordinate X1 =% d", ROUND (ball1.x));
    sprintf (stry1, "coordinate Y1 =% d", ROUND (ball1.y));

    sprintf (strx2, "coordinate X2 =% d", ROUND (ball2.x));
    sprintf (stry2, "coordinate Y2 =% d", ROUND (ball2.y));


    txSelectFont ("Comic Sans MS", 30);

    txTextOut (20, 100, strx1);
    txTextOut (20, 150, stry1);

    txTextOut (20, 300, strx2);
    txTextOut (20, 350, stry2);
    }
