#include "TXLib.h"
#include <cmath>

struct Playing_field
    {
    int left_boarder,
        up_boarder,
        right_boarder,
        down_boarder;
    };

struct Vector
    {
    double x, y;
    };

struct Button
    {
    int key_left, key_right;
    int key_up,  key_down;
    };

struct Ball
    {
    Vector pos;
    Vector v;
    Vector a;

    int radius;
    double mass;
    int rColor, gbColor;
    double friction;

    void Physics (Playing_field rect, double dt);
    void DrawObject ();
    void ControlObject (Button player);
    };

void MovingObject();
void ControlCollision  (Ball* ball1, Ball* ball2);
double Distance        (Ball ball1, Ball ball2);
double Dot_result      (double ux, double uy, double vx, double vy);
void OutputScore       (int score, int tablo_x, int tablo_y, COLORREF color_score);


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
    HDC  Playing_field_View = txLoadImage ("AirfonMain.bmp");

    Playing_field rect_Red_side   = {755, 110, 1430, 690};
    Playing_field rect_Blue_side  = { 80, 110,  755, 690};
    Playing_field rect_Puck_field = { 80, 110, 1430, 690};

    Ball ballpuck = {{  10,  10}, { 8, 8}, {0, 0}, 45,  9, 100,  20, 1.0};

    Ball ballRed =  {{1200, 400}, {11, 6}, {0, 0}, 58, 70, 200,  90, 0.9};

    Ball ballBlue = {{1400, 400}, {11, 6}, {0, 0}, 58, 70,  10, 200, 1.0};

    Button player1 = {'A', 'D', 'W', 'S'};
    Button player2 = {VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN};

    double dt = 1;
    int scoreRed = 0, scoreBlue = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBitBlt  (txDC(), 0, 0, 1510, 790, Playing_field_View, 0, 0);

        COLORREF Color1 =  txGetPixel (ballpuck.pos.x, ballpuck.pos.y, Playing_field_View);

        if (Color1 == RGB (0, 0, 255))  scoreRed  += 1;

        if (Color1 == RGB (255, 0, 0))  scoreBlue += 1;

        ballpuck.DrawObject ();

        ballRed.DrawObject ();

        ballBlue.DrawObject ();

        ballpuck.Physics  (rect_Puck_field, dt);

        ballRed.Physics   (rect_Red_side, dt);

        ballBlue.Physics  (rect_Blue_side, dt);

        ControlCollision (&ballpuck, &ballRed);

        ControlCollision (&ballpuck, &ballBlue);

        ballBlue.ControlObject (player1);

        ballRed.ControlObject  (player2);

        OutputScore  (scoreRed, 1210, 740, TX_RED);
        OutputScore  (scoreBlue, 180, 740, TX_BLUE);

        txSleep(20);
        }

    txDeleteDC (Playing_field_View);
    }

void Ball::DrawObject ()
    {
    for (int i = 1; i <= radius; i++)
        {
        txSetColor     (RGB (rColor + i, gbColor + i/2, gbColor + i/2), 2);
        txSetFillColor (RGB (rColor + i, gbColor + i/2, gbColor + i/2));
        txCircle (pos.x, pos.y, radius - i);
        }
    //txSetColor (RGB (255, 10, 130), 4);
    //txLine     (x, y, x + vx*7, y + vy*7);
    }

double Distance (Ball ball1, Ball ball2)
    {
    double distance = sqrt ((ball1.pos.x - ball2.pos.x) * (ball1.pos.x - ball2.pos.x) +
                            (ball1.pos.y - ball2.pos.y) * (ball1.pos.y - ball2.pos.y));
    return distance;
    }

double Dot_result (double ux, double uy, double vx, double vy)
    {
    double result = (ux * vx + uy * vy);
    return result;
    }

void Ball::Physics (Playing_field rect, double dt)
    {
    v.x = (v.x + a.x * dt) * friction;
    v.y = (v.y + a.y * dt) * friction;

    pos.x = pos.x + v.x * dt;
    pos.y = pos.y + v.y * dt;

    if (pos.x >= rect.right_boarder - radius)
        {
        v.x = - v.x;
        pos.x  = rect.right_boarder - radius;
        }

    if (pos.y >= rect.down_boarder - radius)
        {
        v.y = - v.y;
        pos.y  = rect.down_boarder - radius;
        }

    if (pos.x <= rect.left_boarder + radius)
        {
        v.x = - v.x;
        pos.x  = rect.left_boarder + radius;
        }

    if (pos.y <= rect.up_boarder + radius)
        {
        v.y = - v.y;
        pos.y  = rect.up_boarder + radius;
        }
    }

void ControlCollision (Ball* ball1, Ball* ball2)
    {
//    double distanceRED  = Distance ((*ballpuck).x, (*ballpuck).y, (*ballRed).x, (*ballRed).y);
//    double distanceBLUE = Distance ((*ballpuck).x, (*ballpuck).y, (*ballBlue).x, (*ballBlue).y);

    double distance  = Distance (*ball1, *ball2);
    //double distanceBLUE = Distance (*ball1, *ballBlue);

// step 1: Test collision
    if (distance <= ((*ball1).radius + (*ball2).radius))
        {
// compute the normal vector
        double n12x = (*ball1).pos.x - (*ball2).pos.x;
        double n12y = (*ball1).pos.y - (*ball2).pos.y;

// normalize normal vector
        n12x /= distance;
        n12y /= distance;

// compute the tangential vector perpendicular to normal
        double t12x =  - n12y;
        double t12y =    n12x;

// tangential is also normalized since it's just a rotated normal vector

// step 2: compute all the initial velocities
// notation ball: (1, 2) initial: i, final: f, n: normal direction, t: tangential direction
        double v1it = Dot_result ((*ball1).v.x, (*ball1).v.y, t12x, t12y);

        double v1in = Dot_result ((*ball1).v.x, (*ball1).v.y, n12x, n12y);

        double v2it = Dot_result ((*ball2).v.x,  (*ball2).v.y, t12x, t12y);

        double v2in = Dot_result ((*ball2).v.x,  (*ball2).v.y, n12x, n12y);

// step 3: compute final velocities after collision
        double m1 = (*ball1).mass;
        double m2 = (*ball2).mass;

        double cof_E = 0.9;
        double v1fn = (m2*v2in*(cof_E + 1) + v1in*(m1 - cof_E*m2)) / (m1 + m2);
        double v2fn = (m1*v1in*(cof_E + 1) - v2in*(m1 - cof_E*m2)) / (m1 + m2);

        double v1ft = v1it;
        double v2ft = v2it;

        double xf1 = v1fn*n12x + v1ft*t12x;
        double yf1 = v1fn*n12y + v1ft*t12y;

        double xf2 = v2fn*n12x + v2ft*t12x;
        double yf2 = v2fn*n12y + v2ft*t12y;

        (*ball1).v.x = xf1;
        (*ball1).v.y = yf1;

        (*ball2).v.x = xf2;
        (*ball2).v.y = yf2;

        (*ball1).pos.x += (*ball1).v.x;
        (*ball1).pos.y += (*ball1).v.y;

        (*ball2).pos.x += (*ball2).v.x;
        (*ball2).pos.y += (*ball2).v.y;
        }
    }

void Ball::ControlObject (Button player)
    {
    if (txGetAsyncKeyState (player.key_right)) v.x = v.x + 5;
    if (txGetAsyncKeyState (player.key_left))  v.x = v.x - 5;
    if (txGetAsyncKeyState (player.key_up))    v.y = v.y - 5;
    if (txGetAsyncKeyState (player.key_down))  v.y = v.y + 5;

    if (txGetAsyncKeyState (VK_SPACE))         v.x = v.y = 0;
    }

void OutputScore (int score, int tablo_x, int tablo_y, COLORREF color_score)
    {
    txSelectFont ("Times", 30, 12);
    char str [20] = "";
    sprintf (str, "GOAL = %d", score);
    txSetColor (color_score);
    txSetFillColor (color_score);
    txTextOut (tablo_x, tablo_y, str);
    }

