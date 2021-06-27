#include "TXLib.h"
#include <cmath>


struct Button
    {
    int key_left, key_right;
    int key_up,  key_down;
    };


struct Ball
   {
    double  x,  y,
           vx, vy;
    int radius;
    double mass;
    int a, b;
    int left, top, right, bottom;
    double friction;

    void BoardControl (double ax, double ay, int dt);
    void DrawObject ();
    void ControlObject (Button player);
    };

void MovingObject();
void ControlCollision  (Ball* ballpuck, Ball* ballRed, Ball* ballblue);
double Distance        (double x1, double y1, double x2, double y2);
double Dot_result      (double ux, double uy, double vx, double vy);
void OutputScore       (int scoreRed, int scoreBlue);



const int Widht = 1510;
const int Height = 790;

int main()
    {
    txCreateWindow (Widht, Height);

    MovingObject();

    return 0;
    }

void MovingObject()
    {
    HDC  Playing_field = txLoadImage ("AirfonMain.bmp");

    Ball ballpuck = { 10,   10,  8,  8, 45,  9, 100,  20,  80, 110, 1430, 690, 1.0};

    Ball ballRed =  {100,  400, 11,  6, 58, 70, 200,  90, 755, 110, 1430, 690, 0.9};

    Ball ballBlue = {1400, 400, 11,  6, 58, 70,  10, 200,  80, 110,  755, 690, 1.0};

    Button player1 = {'A', 'D', 'W', 'S'};
    Button player2 = {VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN};

    int dt = 1;
    int scoreRed = 0 , scoreBlue = 0;
    double ax = 1, ay = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBitBlt  (txDC(), 0, 0, 1510, 790, Playing_field, 0, 0);

        COLORREF Color =  txGetPixel(ballpuck.x, ballpuck.y, Playing_field);

        if (Color == RGB (0, 0, 255))  scoreRed  += 1;

        if (Color == RGB (255, 0, 0))  scoreBlue += 1;

        ballpuck.DrawObject ();
        ballRed.DrawObject ();
        ballBlue.DrawObject ();

        ballpuck.BoardControl  (ax, ay, dt);
        ballRed.BoardControl   (ax, ay, dt);
        ballBlue.BoardControl  (ax, ay, dt);
        ControlCollision (&ballpuck, &ballRed, &ballBlue);

        ballBlue.ControlObject (player1);
        ballRed.ControlObject  (player2);

        OutputScore  (scoreRed, scoreBlue);

        txSleep(20);
        }

    txDeleteDC (Playing_field);
    }

void Ball::DrawObject ()
    {
    for (int i = 1; i <= radius; i++)
        {
        txSetColor     (RGB (a + i, b + i/2, b + i/2), 2);
        txSetFillColor (RGB (a + i, b + i/2, b + i/2));
        txCircle (x, y, radius - i);
        }
    //txSetColor (RGB (255, 10, 130), 4);
    //txLine     (x, y, x + vx*7, y + vy*7);
    }

double Distance (double x1, double y1, double x2, double y2)
    {
    double distance = sqrt ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return distance;
    }

double Dot_result (double ux, double uy, double vx, double vy)
    {
    double result = (ux * vx + uy * vy);
    return result;
    }

void Ball::BoardControl (double ax, double ay, int dt)
    {
    vx = (vx + ax * dt)*friction;
    vy = (vy + ay * dt)*friction;

    x = x + vx * dt;
    y = y + vy * dt;

    if (x >= right - radius)
        {
        vx = - vx;
        x  = right - radius;
        }

    if (y >= bottom - radius)
        {
        vy = - vy;
        y  = bottom - radius;
        }

    if (x <= left + radius)
        {
        vx = - vx;
        x  = left + radius;
        }

    if (y <= top + radius)
        {
        vy = - vy;
        y  = top + radius;
        }
    }

void ControlCollision (Ball* ballpuck, Ball* ballRed, Ball* ballBlue)
    {
    double distanceRED  = Distance ((*ballpuck).x, (*ballpuck).y, (*ballRed).x, (*ballRed).y);
    double distanceBLUE = Distance ((*ballpuck).x, (*ballpuck).y, (*ballBlue).x, (*ballBlue).y);

// step 1: Test collision
    if (distanceRED <= ((*ballpuck).radius + (*ballRed).radius))
        {
// compute the normal vector
        double n12x = (*ballRed).x - (*ballpuck).x;
        double n12y = (*ballRed).y - (*ballpuck).y;

// normalize normal vector
        n12x /= distanceRED;
        n12y /= distanceRED;

// compute the tangential vector perpendicular to normal
        double t12x =  - n12y;
        double t12y =    n12x;

// tangential is also normalized since it's just a rotated normal vector

// step 2: compute all the initial velocities
// notation ball: (1, 2) initial: i, final: f, n: normal direction, t: tangential direction
        double v1it = Dot_result ((*ballpuck).vx, (*ballpuck).vy, t12x, t12y);

        double v1in = Dot_result ((*ballpuck).vx, (*ballpuck).vy, n12x, n12y);

        double v2it = Dot_result ((*ballRed).vx, (*ballRed).vy, t12x, t12y);

        double v2in = Dot_result ((*ballRed).vx, (*ballRed).vy, n12x, n12y);

// step 3: compute final velocities after collision
        double m1 = (*ballpuck).mass;
        double m2 = (*ballRed).mass;

        double cof_E = 1.0;
        double v1fn = (m2*v2in*(cof_E+1) + v1in*(m1 - cof_E*m2)) / (m1 + m2);
        double v2fn = (m1*v1in*(cof_E+1) - v2in*(m1 - cof_E*m2)) / (m1 + m2);

        double v1ft = v1it;
        double v2ft = v2it;

        double xf1 = v1fn*n12x + v1ft*t12x;
        double yf1 = v1fn*n12y + v1ft*t12y;

        double xf2 = v2fn*n12x + v2ft*t12x;
        double yf2 = v2fn*n12y + v2ft*t12y;

        (*ballpuck).vx = xf1;
        (*ballpuck).vy = yf1;

        (*ballRed).vx = xf2;
        (*ballRed).vy = yf2;

        (*ballpuck).x += (*ballpuck).vx;
        (*ballpuck).y += (*ballpuck).vy;

        (*ballRed).x += (*ballRed).vx;
        (*ballRed).y += (*ballRed).vy;
        }

//==================================================================================//
    if (distanceBLUE <= ((*ballpuck).radius + (*ballBlue).radius))
        {
        double n12x = (*ballBlue).x - (*ballpuck).x;
        double n12y = (*ballBlue).y - (*ballpuck).y;

        n12x /= distanceBLUE;
        n12y /= distanceBLUE;

        double t12x =  - n12y;
        double t12y =    n12x;

        double v1it = Dot_result ((*ballpuck).vx, (*ballpuck).vy, t12x, t12y);

        double v1in = Dot_result ((*ballpuck).vx, (*ballpuck).vy, n12x, n12y);

        double v2it = Dot_result ((*ballBlue).vx, (*ballBlue).vy, t12x, t12y);

        double v2in = Dot_result ((*ballBlue).vx, (*ballBlue).vy, n12x, n12y);

        double m1 = (*ballpuck).mass;
        double m2 = (*ballBlue).mass;

        double cof_E = 1.0;
        double v1fn = (m2*v2in*(cof_E + 1) + v1in*(m1 - cof_E*m2)) / (m1 + m2);
        double v2fn = (m1*v1in*(cof_E + 1) - v2in*(m1 - cof_E*m2)) / (m1 + m2);

        double v1ft = v1it;
        double v2ft = v2it;

        double xf1 = v1fn*n12x + v1ft*t12x;
        double yf1 = v1fn*n12y + v1ft*t12y;

        double xf2 = v2fn*n12x + v2ft*t12x;
        double yf2 = v2fn*n12y + v2ft*t12y;

        (*ballpuck).vx = xf1;
        (*ballpuck).vy = yf1;

        (*ballBlue).vx = xf2;
        (*ballBlue).vy = yf2;

        (*ballpuck).x += (*ballpuck).vx;
        (*ballpuck).y += (*ballpuck).vy;

        (*ballBlue).x += (*ballBlue).vx;
        (*ballBlue).y += (*ballBlue).vy;

        }
    }

void Ball::ControlObject (Button player)
    {
    if (txGetAsyncKeyState (player.key_right)) vx = vx + 5;
    if (txGetAsyncKeyState (player.key_left))  vx = vx - 5;
    if (txGetAsyncKeyState (player.key_up))    vy = vy - 5;
    if (txGetAsyncKeyState (player.key_down))  vy = vy + 5;

    if (txGetAsyncKeyState (VK_SPACE))         vx = vy = 0;
    }

void OutputScore (int scoreRed, int scoreBlue)
    {
    txSelectFont ("Times", 30, 12);

    char strR [20] = "";
    char strB [20] = "";

    sprintf (strR, "GOAL =% d", scoreRed);
    sprintf (strB, "GOAL =% d", scoreBlue);

    txSetColor (TX_RED);
    txSetFillColor (TX_RED);
    txTextOut (1210, 740, strR);

    txSetColor (TX_BLUE);
    txSetFillColor (TX_BLUE);
    txTextOut ( 180, 740, strB);
    }

