#include <iostream>
#include <cmath>
#include <graphics.h>

using namespace std;

float dt = 0.01;
float c1 = 30.0;
float c2 = 20.0;
float c2_quen = 45.0;
float m1 = 7.0;
float m2 = 5.0;
float m2_quen = 5.0;
float p = 1.0;
float p_quen = 3.0;
float t = 0.0;
float l1 = 3.0;
float l2 = 3.0;
float tmax = 200.0;
float scale = 5.0;
int coil = 7;
int coil2 = 5;

struct Block
{
    float x;
    float y;
    float vx;
    float vy;
};

Block b_d, b_u, b;

void physics(float** b_mat, float m2, float c2, float p)
{
    b_u.y = 0.0;
    b_d.y = 0.0;
    b.y = 0.05;
    b_u.vy = 0.0;
    b_d.vy = 0.0;
    b.vy = 0.0;
    for (int i1 = 0; i1<tmax/dt; i1++)
    {
        b_d.vy += (-c1*(b_d.y) + c2*(b_u.y-b_d.y) + 0.5*sin(p*t))/m1*dt;
        b_u.vy += -c2*(b_u.y - b_d.y)/m2*dt;
        b.vy += -c1*b.y/m2*dt;
        b_d.y += b_d.vy*dt;
        b_u.y += b_u.vy*dt;
        b.y += b.vy*dt;
        b_mat[0][i1] = b_d.y;
        b_mat[1][i1] = b_u.y;
        b_mat[2][i1] = b.y;
        t += dt;
    }
}

void grath(float** b_mat, float** b_mat_quen, int xmax, int ymax, int grath_number)
{
    int i1;
    switch(grath_number)
    {
        case 1:
        tmax = 20.0;
        for(int i = 0; i<xmax; i++)
        {
            i1 = round(i*tmax/dt/xmax);
            setcolor(GREEN);// Green - Lower body with quen cond
            circle(i,scale*ymax*b_mat_quen[0][i1]+ymax/2,1);
            setcolor(WHITE);// White - Lower body without quen cond
            circle(i,scale*ymax*b_mat[0][i1]+ymax/2,1);
        }
        break;

        case 2:
        tmax = 20.0;
        for(int i = 0; i<xmax; i++)
        {
            i1 = round(i*tmax/dt/xmax);
            setcolor(GREEN);
            circle(i,scale*ymax*b_mat[2][i1]+ymax/2,1);
        }
        break;

        case 3:
        for (int i = 0; i<tmax/dt;i=i+5)
        {
            setfillstyle(1,2);
            pieslice(xmax/2, 200*b_mat[0][i]+ymax/2 , 0, 360,  20);
            setcolor(2);
            moveto(xmax/2, 200*b_mat[0][i]+ymax/2);
            float x1, y1, x2, y2;
            for (int j = 0; j <= coil; j++)
            {
                y1 = 200*(b_mat[0][i]) +ymax/2 - abs(200*(b_mat[1][i]) +ymax/3 - (200*(b_mat[0][i]) +ymax/2))/coil*j;
                x1 = xmax/2 + ((j%2==0)?-1:1)*10 ;
                lineto(x1, y1);
            }
            moveto(xmax/2,ymax-170);
            for (int j = 0; j < coil2; j++)
            {
                y2 = ymax-170 - abs(200*(b_mat[0][i]) +ymax/2 - (ymax-170))/coil2*j;
                x2 = xmax/2 + ((j%2==0)?-1:1)*10;
                lineto(x2, y2);
            }
            setfillstyle(1,4);
            bar(xmax/2-30, 200*b_mat[1][i]-15+ymax/3, xmax/2+30, 200*b_mat[1][i]+15+ymax/3);
            setfillstyle(4,15);
            bar(xmax/2-30, ymax-170 , xmax/2+30, ymax-150);
            delay(30);
            cleardevice();
        }
        break;

        case 4:
        for (int i = 0; i<tmax/dt;i=i+5)
        {
            setfillstyle(1,2);
            pieslice(xmax/2, 200*b_mat[2][i]+ymax/2 , 0, 360,  20);
            setcolor(2);
            float x1, y1, x2, y2;
            moveto(xmax/2,ymax-170);
            for (int j = 0; j < coil2; j++)
            {
                y2 = ymax-170 - abs(200*(b_mat[2][i]) +ymax/2 - (ymax-170))/coil2*j;
                x2 = xmax/2 + ((j%2==0)?-1:1)*10;
                lineto(x2, y2);
            }
            setfillstyle(4,15);
            bar(xmax/2-30, ymax-170 , xmax/2+30, ymax-150);
            delay(30);
            cleardevice();
        }
        break;

        default:
        tmax = 20.0;
        for(int i = 0; i<xmax; i++)
        {
            i1 = round(i*tmax/dt/xmax);
            setcolor(GREEN);// Green - Lower body
            circle(i,scale*ymax*b_mat[0][i1]+ymax/2,1);
            setcolor(WHITE);// White - Upper body
            circle(i,scale*ymax*b_mat[1][i1]+ymax/2,1);
        }
        break;
    }
}

int main()
{
    int gdriver = DETECT, gmode, errorcode;
    initgraph(&gdriver,&gmode,"");
    setfillstyle(SOLID_FILL,GREEN);
    setcolor(GREEN);

    float** b_mat = new float*[3];//0 - b_d.y; 1 - b_u.y; 2 - b.y
    for(int i = 0; i < 3; i++)
        b_mat[i] = new float[20000];

    float** b_mat_quen = new float*[3];//0 - b_d.y; 1 - b_u.y; 2 - b.y
    for(int i = 0; i < 3; i++)
        b_mat_quen[i] = new float[20000];

    physics(b_mat, m2, c2, p);
    physics(b_mat_quen, m2_quen, c2_quen, p_quen);

    int xmax = getmaxx();
    int ymax = getmaxy();
    //cout << xmax <<" " << ymax;

    int grath_number = 0;
    //0 - Coordinates of the upper and lower body;
    //1 - Coordinates of the lower body under the condition of quenching and without the condition of quenching;
    //2 - Coordinates of a single block;
    //3 - Animation 2 blocks;
    //4 - Animation 1 block;
    grath(b_mat, b_mat_quen, xmax, ymax, grath_number);


    delete b_mat;
    b_mat = NULL;
    delete b_mat_quen;
    b_mat_quen = NULL;

    getch();
    closegraph();
    return 0;
}
