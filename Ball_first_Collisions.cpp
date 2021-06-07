
#include "TXLib.h"
#include <cmath>

void MoveBall();
void Draw (float x, float y,  int a, int b, int radius);
void Fon();

int const Width = 300;
int const Height = 300;

int main()
    {
    txCreateWindow (Width, Height);

    MoveBall();

    return 0;
    }

void MoveBall()
    {
    float x1 = Width/4;
    float y1 = Height/2;
    float x2 = Width/6;
    float y2 = Height/2;
    float radius1 = 30;
    float radius2 = 30;
    float vx1 = 1 + rand() % 10;
    float vy1 = 1 + rand() % 10;
    float vx2 = 1 + rand() % 10;
    float vy2 = 1 + rand() % 10;
    int a = 100, b = 225;
    //int count_collisions = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        Fon();
        txClear();
        //printf ("    %d\n", count_collisions);

        Draw (x1, y1, a, b, radius1);
        Draw (x2, y2, a, b, radius2);

        x1 += vx1;
        y1 += vy1;

        if (x1 >= Width- radius1)
            {
            vx1 = -vx1;
             x1 = Width - radius1;
            }

        if (y1 >= Height - radius1)
            {
            vy1 = -vy1;
             y1 = Height - radius1;
            }

        if (x1 <= 0 + radius1)
            {
            vx1 = -vx1;
             x1 = 0 + radius1;
            }

        if (y1 <= 0 + radius1)
            {
            vy1 = -vy1;
             y1 = 0 + radius1;
            }

        x2 += vx2;
        y2 += vy2;

        if (x2 >= Width - radius2)
            {
            vx2 = -vx2;
             x2 = Width - radius2;
            }

        if (y2 >= Height - radius2)
            {
            vy2 = -vy2;
             y2 = Height - radius2;
            }

        if (x2 <= 0 + radius2)
            {
            vx2 = -vx2;
             x2 = 0 + radius2;
            }

        if (y2<= 0 + radius2)
            {
            vy2 = -vy2;
             y2 = 0 + radius2;
            }

      	float distance = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)); if (distance == 0) distance = 0.01;
      	float angle_sin = (x1 - x2)/distance;
      	float angle_cos = (y1 - y2)/distance;

      	if (distance < radius1 + radius2)
            {
      		float Vn1 = vx2*angle_sin + vy2*angle_cos;

      		float Vn2 = vx1*angle_sin + vy1*angle_cos;

      		float dt = ((radius2 + radius1) - distance)/(Vn1 - Vn2);

      		float Vt1 = -vx2*angle_cos + vy2*angle_sin;

      		float Vt2 = -vx1*angle_cos + vy1*angle_sin;

      		float temp = Vn2;
      		Vn2 = Vn1;
      		Vn1 = temp;

      		vx1 = Vn2*angle_sin - Vt2*angle_cos;

      		vy1 = Vn2*angle_cos + Vt2*angle_sin;

      		vx2 = Vn1*angle_sin - Vt1*angle_cos;

      		vy2 = Vn1*angle_cos + Vt1*angle_sin;

      		x1 += vx1*dt;
      		y1 += vy1*dt;
      		x2 += vx2*dt;
      		y2 += vy2*dt;

      		//count_collisions += 1;
            }

         txSleep(10);
        }

    }

void Draw (float x, float y, int a, int b, int radius)
    {
    for (int i = 1; i <= 30; i++)
            {
            txSetColor     (RGB (a + i*2, b + i, b + i));
            txSetFillColor (RGB (a + i*2, b + i, b + i));
            txCircle (x, y, radius - i);
            }
    }

void Fon()
    {
    txSetFillColor (TX_BLACK);
    POINT fon [4] = {{0, 0}, {Width, 0}, {Width, Height}, {0, Height}};
    txPolygon (fon,4);
    }

