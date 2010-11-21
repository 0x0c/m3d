#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xlocale.h>
#include"m3d.h"
#include<math.h>

#define BORDER 2
#define WIN_W 800
#define WIN_H 600
#define TITLE "x3dcube_m3d"
#define RAD	(M_PI / 180.0)
#define min 0.01

#define OBJECT 0
#define MAP 1
#define ALL 2

////////////////////////////////////////
//X valiable
////////////////////////////////////////
Display *dpy;//X server.
Window w;//main window.
Window root;//root window.
Window quit,cls;//button window.
int screen;//screen.
unsigned long black,white;//pixel value of black and white.
GC gc;//graphic context.
Pixmap pmap;
XEvent e;//event struct.

////////////////////////////////////////
//controller valiable
////////////////////////////////////////
double	xangle,
		yangle,
		zangle;

double	sx,
		sy,
		ex,
		ey,
		tmpx,
		tmpy;

double	xeye = 0.0,
		yeye = 30.0,
		zeye = 80.0;

int		objectSW = MAP;
int		game;

////////////////////////////////////////
//info valiable
////////////////////////////////////////
double fps;

////////////////////////////////////////
//3D space valiable
////////////////////////////////////////
Object	fighter,map;
Object	xaxis,yaxis,zaxis;
Camera	camera;
Matrix	mat;
double	vertex[MAX_ELEMENTS];

////////////////////////////////////////
//オブジェクトの定義
////////////////////////////////////////
//戦闘機
Vector fighterVec[] = {
	0	,0	,-7	,1,//0
	2	,0	,0	,1,//1
	-2	,0	,0	,1,//2
	2	,-1	,0	,1,//3
	-2	,-1	,0	,1,//4
	6	,-1	,3	,1,//5
	6	,-1	,4	,1,//6
	-6	,-1	,3	,1,//7
	-6	,-1	,4	,1,//8
	1	,0	,4	,1,//9
	1	,-1	,4	,1,//10
	-1	,-1	,4	,1,//11
	-1	,0	,4	,1,//12
	0	,-1	,4	,1,//13
	2	,0	,6	,1,//14
	-2	,0	,6	,1,//15
	1	,0	,6	,1,//16
	1	,-1	,6	,1,//17
	1	,1	,6	,1,//18
	-1	,1	,6	,1,//19
	-1	,-1	,6	,1,//20
	-1	,0	,6	,1,//21
	0	,1	,0	,1,//22
	6	,0	,4	,1,//23
	6	,0	,3	,1,//24
	-6	,0	,4	,1,//25
	-6	,0	,3	,1,//26
	0	,0	,6	,1,//27
};
Wire fighterWire[] = {
	0,	1,	0,	2,	0,	3,	0,	4,	0,	22,//5
	1,	3,	1,	5,	1,	9,	1,	22,//4
	2,	4,	2,	7,	2,	12,	2,	22,//4
	3,	4,	3,	5,//2
	4,	7,	//1
	5,	6,	5,	24,//2
	6,	9,	6,	10,	6,	23,//3
	7,	8,	7,	26,//2
	8,	11,	8,	12,	8,	25,//3
	9,	10,	9,	12,	9,	14,	9,	16,	9,	18,	9,	22,//6
	10,	11,	10,	14,	10,	17,//3
	11,	12,	11,	15,//2
	12,	19,	12,	22,//2
	13,	16,	13,	20,	13,	21,//3
	14,	16,	14,	17,//2
	15,	19,	15,	20,	15,	21,//3
	16,	17,	16,	18,	16,	21,	16,	27,//4
	17,	27,//1
	18,	14,	18,	27,//2
	19,	27,//1
	20,	21,	20,	27,//2
	23,	24,//1
	25,	26,//1
};
//フィールド
Vector mapVec[] = {
	5.0		,-2.0	,10.0,	1.0,//0
	5.0		,-2.0	,9.0,	1.0,//1
	5.0		,-2.0	,8.0,	1.0,//2
	5.0		,-2.0	,7.0,	1.0,//3 
	5.0		,-2.0	,6.0,	1.0,//4
	5.0		,-2.0	,5.0,	1.0,//5
	5.0		,-2.0	,4.0,	1.0,//6
	5.0		,-2.0	,3.0,	1.0,//7
	5.0		,-2.0	,2.0,	1.0,//8
	5.0		,-2.0	,1.0,	1.0,//9 
	5.0		,-2.0	,0.0,	1.0,//10
	5.0		,-2.0	,-1.0,	1.0,//11
	5.0		,-2.0	,-2.0,	1.0,//12
	5.0		,-2.0	,-3.0,	1.0,//13
	5.0		,-2.0	,-4.0,	1.0,//14
	5.0		,-2.0	,-5.0,	1.0,//15
	5.0		,-2.0	,-6.0,	1.0,//16
	5.0		,-2.0	,-7.0,	1.0,//17
	5.0		,-2.0	,-8.0,	1.0,//18
	5.0		,-2.0	,-9.0,	1.0,//19
	5.0		,-2.0	,-10.0,	1.0,//20
								//21
	4.0		,-2.0	,10.0,	1.0,//0
	4.0		,-2.0	,9.0,	1.0,//1
	4.0		,-2.0	,8.0,	1.0,//2
	4.0		,-2.0	,7.0,	1.0,//3 
	4.0		,-2.0	,6.0,	1.0,//4
	4.0		,-2.0	,5.0,	1.0,//5
	4.0		,-2.0	,4.0,	1.0,//6
	4.0		,-2.0	,3.0,	1.0,//7
	4.0		,-2.0	,2.0,	1.0,//8
	4.0		,-2.0	,1.0,	1.0,//9 
	4.0		,-2.0	,0.0,	1.0,//10
	4.0		,-2.0	,-1.0,	1.0,//11
	4.0		,-2.0	,-2.0,	1.0,//12
	4.0		,-2.0	,-3.0,	1.0,//13
	4.0		,-2.0	,-4.0,	1.0,//14
	4.0		,-2.0	,-5.0,	1.0,//15
	4.0		,-2.0	,-6.0,	1.0,//16
	4.0		,-2.0	,-7.0,	1.0,//17
	4.0		,-2.0	,-8.0,	1.0,//18
	4.0		,-2.0	,-9.0,	1.0,//19
	4.0		,-2.0	,-10.0,	1.0,//20
								//42
	3.0		,-2.0	,10.0,	1.0,//0
	3.0		,-2.0	,9.0,	1.0,//1
	3.0		,-2.0	,8.0,	1.0,//2
	3.0		,-2.0	,7.0,	1.0,//3 
	3.0		,-2.0	,6.0,	1.0,//4
	3.0		,-2.0	,5.0,	1.0,//5
	3.0		,-2.0	,4.0,	1.0,//6
	3.0		,-2.0	,3.0,	1.0,//7
	3.0		,-2.0	,2.0,	1.0,//8
	3.0		,-2.0	,1.0,	1.0,//9 
	3.0		,-2.0	,0.0,	1.0,//10
	3.0		,-2.0	,-1.0,	1.0,//11
	3.0		,-2.0	,-2.0,	1.0,//12
	3.0		,-2.0	,-3.0,	1.0,//13
	3.0		,-2.0	,-4.0,	1.0,//14
	3.0		,-2.0	,-5.0,	1.0,//15
	3.0		,-2.0	,-6.0,	1.0,//16
	3.0		,-2.0	,-7.0,	1.0,//17
	3.0		,-2.0	,-8.0,	1.0,//18
	3.0		,-2.0	,-9.0,	1.0,//19
	3.0		,-2.0	,-10.0,	1.0,//20
								//63
	2.0		,-2.0	,10.0,	1.0,//0
	2.0		,-2.0	,9.0,	1.0,//1
	2.0		,-2.0	,8.0,	1.0,//2
	2.0		,-2.0	,7.0,	1.0,//3 
	2.0		,-2.0	,6.0,	1.0,//4
	2.0		,-2.0	,5.0,	1.0,//5
	2.0		,-2.0	,4.0,	1.0,//6
	2.0		,-2.0	,3.0,	1.0,//7
	2.0		,-2.0	,2.0,	1.0,//8
	2.0		,-2.0	,1.0,	1.0,//9 
	2.0		,-2.0	,0.0,	1.0,//10
	2.0		,-2.0	,-1.0,	1.0,//11
	2.0		,-2.0	,-2.0,	1.0,//12
	2.0		,-2.0	,-3.0,	1.0,//13
	2.0		,-2.0	,-4.0,	1.0,//14
	2.0		,-2.0	,-5.0,	1.0,//15
	2.0		,-2.0	,-6.0,	1.0,//16
	2.0		,-2.0	,-7.0,	1.0,//17
	2.0		,-2.0	,-8.0,	1.0,//18
	2.0		,-2.0	,-9.0,	1.0,//19
	2.0		,-2.0	,-10.0,	1.0,//20
								//84
	2.0		,-2.0	,10.0,	1.0,//0
	2.0		,-2.0	,9.0,	1.0,//1
	2.0		,-2.0	,8.0,	1.0,//2
	2.0		,-2.0	,7.0,	1.0,//3 
	2.0		,-2.0	,6.0,	1.0,//4
	2.0		,-2.0	,5.0,	1.0,//5
	2.0		,-2.0	,4.0,	1.0,//6
	2.0		,-2.0	,3.0,	1.0,//7
	2.0		,-2.0	,2.0,	1.0,//8
	2.0		,-2.0	,1.0,	1.0,//9 
	2.0		,-2.0	,0.0,	1.0,//10
	2.0		,-2.0	,-1.0,	1.0,//11
	2.0		,-2.0	,-2.0,	1.0,//12
	2.0		,-2.0	,-3.0,	1.0,//13
	2.0		,-2.0	,-4.0,	1.0,//14
	2.0		,-2.0	,-5.0,	1.0,//15
	2.0		,-2.0	,-6.0,	1.0,//16
	2.0		,-2.0	,-7.0,	1.0,//17
	2.0		,-2.0	,-8.0,	1.0,//18
	2.0		,-2.0	,-9.0,	1.0,//19
	2.0		,-2.0	,-10.0,	1.0,//20
								//105
	1.0		,-2.0	,10.0,	1.0,//0
	1.0		,-2.0	,9.0,	1.0,//1
	1.0		,-2.0	,8.0,	1.0,//2
	1.0		,-2.0	,7.0,	1.0,//3 
	1.0		,-2.0	,6.0,	1.0,//4
	1.0		,-2.0	,5.0,	1.0,//5
	1.0		,-2.0	,4.0,	1.0,//6
	1.0		,-2.0	,3.0,	1.0,//7
	1.0		,-2.0	,2.0,	1.0,//8
	1.0		,-2.0	,1.0,	1.0,//9 
	1.0		,-2.0	,0.0,	1.0,//10
	1.0		,-2.0	,-1.0,	1.0,//11
	1.0		,-2.0	,-2.0,	1.0,//12
	1.0		,-2.0	,-3.0,	1.0,//13
	1.0		,-2.0	,-4.0,	1.0,//14
	1.0		,-2.0	,-5.0,	1.0,//15
	1.0		,-2.0	,-6.0,	1.0,//16
	1.0		,-2.0	,-7.0,	1.0,//17
	1.0		,-2.0	,-8.0,	1.0,//18
	1.0		,-2.0	,-9.0,	1.0,//19
	1.0		,-2.0	,-10.0,	1.0,//20
								//126
	0.0		,-2.0	,10.0,	1.0,//0
	0.0		,-2.0	,9.0,	1.0,//1
	0.0		,-2.0	,8.0,	1.0,//2
	0.0		,-2.0	,7.0,	1.0,//3 
	0.0		,-2.0	,6.0,	1.0,//4
	0.0		,-2.0	,5.0,	1.0,//5
	0.0		,-2.0	,4.0,	1.0,//6
	0.0		,-2.0	,3.0,	1.0,//7
	0.0		,-2.0	,2.0,	1.0,//8
	0.0		,-2.0	,1.0,	1.0,//9 
	0.0		,-2.0	,0.0,	1.0,//10
	0.0		,-2.0	,-1.0,	1.0,//11
	0.0		,-2.0	,-2.0,	1.0,//12
	0.0		,-2.0	,-3.0,	1.0,//13
	0.0		,-2.0	,-4.0,	1.0,//14
	0.0		,-2.0	,-5.0,	1.0,//15
	0.0		,-2.0	,-6.0,	1.0,//16
	0.0		,-2.0	,-7.0,	1.0,//17
	0.0		,-2.0	,-8.0,	1.0,//18
	0.0		,-2.0	,-9.0,	1.0,//19
	0.0		,-2.0	,-10.0,	1.0,//20
								//147
	-1.0		,-2.0	,10.0,	1.0,//0
	-1.0		,-2.0	,9.0,	1.0,//1
	-1.0		,-2.0	,8.0,	1.0,//2
	-1.0		,-2.0	,7.0,	1.0,//3
	-1.0		,-2.0	,6.0,	1.0,//4
	-1.0		,-2.0	,5.0,	1.0,//5
	-1.0		,-2.0	,4.0,	1.0,//6
	-1.0		,-2.0	,3.0,	1.0,//7
	-1.0		,-2.0	,2.0,	1.0,//8
	-1.0		,-2.0	,1.0,	1.0,//9
	-1.0		,-2.0	,0.0,	1.0,//10
	-1.0		,-2.0	,-1.0,	1.0,//11
	-1.0		,-2.0	,-2.0,	1.0,//12
	-1.0		,-2.0	,-3.0,	1.0,//13
	-1.0		,-2.0	,-4.0,	1.0,//14
	-1.0		,-2.0	,-5.0,	1.0,//15
	-1.0		,-2.0	,-6.0,	1.0,//16
	-1.0		,-2.0	,-7.0,	1.0,//17
	-1.0		,-2.0	,-8.0,	1.0,//18
	-1.0		,-2.0	,-9.0,	1.0,//19
	-1.0		,-2.0	,-10.0,	1.0,//20
									//168
	-2.0		,-2.0	,10.0,	1.0,//0
	-2.0		,-2.0	,9.0,	1.0,//1
	-2.0		,-2.0	,8.0,	1.0,//2
	-2.0		,-2.0	,7.0,	1.0,//3
	-2.0		,-2.0	,6.0,	1.0,//4
	-2.0		,-2.0	,5.0,	1.0,//5
	-2.0		,-2.0	,4.0,	1.0,//6
	-2.0		,-2.0	,3.0,	1.0,//7
	-2.0		,-2.0	,2.0,	1.0,//8
	-2.0		,-2.0	,1.0,	1.0,//9
	-2.0		,-2.0	,0.0,	1.0,//10
	-2.0		,-2.0	,-1.0,	1.0,//11
	-2.0		,-2.0	,-2.0,	1.0,//12
	-2.0		,-2.0	,-3.0,	1.0,//13
	-2.0		,-2.0	,-4.0,	1.0,//14
	-2.0		,-2.0	,-5.0,	1.0,//15
	-2.0		,-2.0	,-6.0,	1.0,//16
	-2.0		,-2.0	,-7.0,	1.0,//17
	-2.0		,-2.0	,-8.0,	1.0,//18
	-2.0		,-2.0	,-9.0,	1.0,//19
	-2.0		,-2.0	,-10.0,	1.0,//20
									//189
	-3.0		,-2.0	,10.0,	1.0,//0
	-3.0		,-2.0	,9.0,	1.0,//1
	-3.0		,-2.0	,8.0,	1.0,//2
	-3.0		,-2.0	,7.0,	1.0,//3
	-3.0		,-2.0	,6.0,	1.0,//4
	-3.0		,-2.0	,5.0,	1.0,//5
	-3.0		,-2.0	,4.0,	1.0,//6
	-3.0		,-2.0	,3.0,	1.0,//7
	-3.0		,-2.0	,2.0,	1.0,//8
	-3.0		,-2.0	,1.0,	1.0,//9
	-3.0		,-2.0	,0.0,	1.0,//10
	-3.0		,-2.0	,-1.0,	1.0,//11
	-3.0		,-2.0	,-2.0,	1.0,//12
	-3.0		,-2.0	,-3.0,	1.0,//13
	-3.0		,-2.0	,-4.0,	1.0,//14
	-3.0		,-2.0	,-5.0,	1.0,//15
	-3.0		,-2.0	,-6.0,	1.0,//16
	-3.0		,-2.0	,-7.0,	1.0,//17
	-3.0		,-2.0	,-8.0,	1.0,//18
	-3.0		,-2.0	,-9.0,	1.0,//19
	-3.0		,-2.0	,-10.0,	1.0,//20
									//210
	-4.0		,-2.0	,10.0,	1.0,//0
	-4.0		,-2.0	,9.0,	1.0,//1
	-4.0		,-2.0	,8.0,	1.0,//2
	-4.0		,-2.0	,7.0,	1.0,//3
	-4.0		,-2.0	,6.0,	1.0,//4
	-4.0		,-2.0	,5.0,	1.0,//5
	-4.0		,-2.0	,4.0,	1.0,//6
	-4.0		,-2.0	,3.0,	1.0,//7
	-4.0		,-2.0	,2.0,	1.0,//8
	-4.0		,-2.0	,1.0,	1.0,//9
	-4.0		,-2.0	,0.0,	1.0,//10
	-4.0		,-2.0	,-1.0,	1.0,//11
	-4.0		,-2.0	,-2.0,	1.0,//12
	-4.0		,-2.0	,-3.0,	1.0,//13
	-4.0		,-2.0	,-4.0,	1.0,//14
	-4.0		,-2.0	,-5.0,	1.0,//15
	-4.0		,-2.0	,-6.0,	1.0,//16
	-4.0		,-2.0	,-7.0,	1.0,//17
	-4.0		,-2.0	,-8.0,	1.0,//18
	-4.0		,-2.0	,-9.0,	1.0,//19
	-4.0		,-2.0	,-10.0,	1.0,//20
									//231
	-5.0		,-2.0	,10.0,	1.0,//0
	-5.0		,-2.0	,9.0,	1.0,//1
	-5.0		,-2.0	,8.0,	1.0,//2
	-5.0		,-2.0	,7.0,	1.0,//3
	-5.0		,-2.0	,6.0,	1.0,//4
	-5.0		,-2.0	,5.0,	1.0,//5
	-5.0		,-2.0	,4.0,	1.0,//6
	-5.0		,-2.0	,3.0,	1.0,//7
	-5.0		,-2.0	,2.0,	1.0,//8
	-5.0		,-2.0	,1.0,	1.0,//9
	-5.0		,-2.0	,0.0,	1.0,//10
	-5.0		,-2.0	,-1.0,	1.0,//11
	-5.0		,-2.0	,-2.0,	1.0,//12
	-5.0		,-2.0	,-3.0,	1.0,//13
	-5.0		,-2.0	,-4.0,	1.0,//14
	-5.0		,-2.0	,-5.0,	1.0,//15
	-5.0		,-2.0	,-6.0,	1.0,//16
	-5.0		,-2.0	,-7.0,	1.0,//17
	-5.0		,-2.0	,-8.0,	1.0,//18
	-5.0		,-2.0	,-9.0,	1.0,//19
	-5.0		,-2.0	,-10.0,	1.0,//20
									//252
 };
 Wire mapWire[] = {
	 0,0,
 };

int initXWindowSystem(void)
{
	dpy = XOpenDisplay("");//connect to x server.
	screen = DefaultScreen(dpy);
	root = DefaultRootWindow(dpy);
	white = WhitePixel(dpy,screen);
	black = BlackPixel(dpy,screen);
	
	w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 100, 100, WIN_W, WIN_H, BORDER, black, black);
	quit = XCreateSimpleWindow(dpy, w, 10, 3, 35, 15, 1, white, black);
	cls = XCreateSimpleWindow(dpy, w, 10, 25, 40, 15, 1, white, black);
	gc = XCreateGC(dpy, w, 0, NULL);//set graphic context.
	pmap = XCreatePixmap(dpy, root,
						 WIN_W, WIN_H, DefaultDepth(dpy, screen));
	
	XStoreName(dpy, w, TITLE);
	XSelectInput(dpy, w, ButtonPressMask | ButtonReleaseMask | KeyPressMask);
	XSelectInput(dpy, quit, ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask);
	XSelectInput(dpy, cls, ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask);
	
	XMapWindow(dpy, w);
	XMapSubwindows(dpy, w);
	
	return 0;
}

void executeGeometryPipeline(void)
{
	mat = initWithIMatrix();
	
	//init camera
	camera.eye = initWithVector(xeye, yeye, zeye);
	camera = initWithCamera(initWithVector(camera.eye.x, camera.eye.y, camera.eye.z),initWithVector(0.0,0.0,0.0), initWithVector(0.0,1.0,0.0));
	setViewCoordinateSystem(&mat,camera.eye, camera.at, camera.up);
	
	//geometry pipeline
	setProjectionCoordinateSystem(&mat,RAD*30, (double)WIN_W/WIN_H, 100.0, 1000.0);
	setScreenCoordinateSystem(&mat,WIN_W, WIN_H);
	
	//init object
	fighter = initWithObject(fighterVec, 28, fighterWire, 59,"fighter", 7);
	fighter.xscale = 0.1;
	fighter.yscale = 0.1;
	fighter.zscale = 0.1;
	map = initWithObject(mapVec, 255, mapWire, 1,"map", 3);
	map.xscale = 100.0;
	map.yscale = 100.0;
	map.zscale = 100.0;
}

void setAreaClear(void)
{
	XSetForeground(dpy, gc, black);
	XFillRectangle(dpy, pmap, gc, 0, 0, WIN_W, WIN_H);
}

void getAreaCopy(void)
{
	XCopyArea(dpy, pmap, w, gc, 0, 0, WIN_W, WIN_H, 0, 0);
}

void renderObject(Object *obj,int objectType)
{
	Object obj2;
	Matrix pre;
	
	obj2 = *obj;
	obj2.xmov += obj->xmovq * min;
	obj2.ymov += obj->ymovq * min;
	obj2.zmov += obj->zmovq * min;
	obj->xmov = obj2.xmov;
	obj->ymov = obj2.ymov;
	obj->zmov = obj2.zmov;
	
	pre = initWithIMatrix();
	setMatrixOfScale(&pre, obj2.xscale, obj2.yscale, obj2.zscale);
	if(objectType == MAP)setMatrixOfRotateXYZ(&pre, xangle, yangle, zangle);
	setMatrixOfMove(&pre, obj2.xmov, obj2.ymov, obj2.zmov);
	pre = getMatrixMultiple(&pre, &mat);
	setObjectTransformed(&obj2, &pre);
	drawObjectWithWireFrame(&obj2, &camera,objectType, dpy, pmap, gc);
	
	if(xangle > 360)xangle = 0.0;
	else if(xangle < -360)xangle = 0.0;
	else xangle += tmpy;
	if(yangle > 360)yangle = 0.0;
	else if(yangle < -360)yangle = 0.0;
	else yangle -= tmpx;
	
	XFlush(dpy);
}

void drawAxis(void)
{
	////////////////////////////////////////
	//XYZ軸
	////////////////////////////////////////
	Vector vx[] = {
		100.0,	0.0,	0.0,	1.0,
		-100.0,	0.0,	0.0,	1.0
	};
	Vector vy[] = {
		0.0,	100.0,	0.0,	1.0,
		0.0,	-100.0,	0.0,	1.0
	};
	Vector vz[] = {
		0.0,	0.0,	100.0,	1.0,
		0.0,	0.0,	-100.0,	1.0
	};
	Wire waxis[] = {
		0,1
	};
	
	xaxis = initWithObject(vx, 2, waxis, 1,"X_axis",6);//x axis
	yaxis = initWithObject(vy, 2, waxis, 1,"Y_axis",6);//y axis
	zaxis = initWithObject(vz, 2, waxis, 1,"Z_axis",6);//z axis
	
	setObjectTransformed(&xaxis, &mat);
	setObjectTransformed(&yaxis, &mat);
	setObjectTransformed(&zaxis, &mat);
	
	XDrawString(dpy, pmap, gc, 700, 100, "X", 1);
	XDrawString(dpy, pmap, gc, xaxis.vector[0].x, xaxis.vector[0].y, "X", 1);
	drawObjectWithWireFrame(&xaxis, &camera,OBJECT, dpy, pmap, gc);
	XDrawString(dpy, pmap, gc, 700, 150, "Y", 1);
	XDrawString(dpy, pmap, gc, yaxis.vector[0].x, yaxis.vector[0].y, "Y", 1);
	drawObjectWithWireFrame(&yaxis, &camera,OBJECT,dpy, pmap, gc);
	XDrawString(dpy, pmap, gc, 700, 200, "Z", 1);
	XDrawString(dpy, pmap, gc, zaxis.vector[0].x, zaxis.vector[0].y, "Z", 1);
	drawObjectWithWireFrame(&zaxis, &camera,OBJECT,dpy, pmap, gc);
	XFlush(dpy);
	XSetLineAttributes(dpy, gc, 0,LineSolid, CapRound, JoinMiter);
}

void controlObject(void)
{
	XNextEvent(dpy, &e);
	switch (e.type)
	{
		case ButtonPress:
			if(e.xany.window == w)
			{
				sx = e.xbutton.x;
				sy = e.xbutton.y;
			}
			break;
			
		case ButtonRelease:
			if(e.xany.window == quit)
			{
				printf("Thank you for playing.\n");
				exit(0);
			}
			else if(e.xany.window == w)
			{
				ex = e.xbutton.x;
				ey = e.xbutton.y;
				
				tmpx = (ex - sx) * min;
				tmpy = (ey - sy) * min;
				
				xangle = tmpy;
				yangle = -tmpx;
			}
			else if(e.xany.window == cls)
			{
				game = 1;
				xangle = yangle = zangle = sx = sy = ex = ey = tmpx = tmpy = 0;
				map.xmovq = map.ymovq = map.zmovq = 0;
				fighter.xmovq = fighter.ymovq = fighter.zmovq = 0;
				map.xmov = map.ymov = map.zmov = 0;
				fighter.xmov = fighter.ymov = fighter.zmov = 0;
				tmpy = tmpx = 0;
				
				fighter.xscale = 0.5;
				fighter.yscale = 0.5;
				fighter.zscale = 0.5;
			}
			break;
			
		case KeyPress:
			if(e.xkey.keycode == 134)//up
			{
				if(xangle < -340.0)xangle = -0.0;
				else xangle -= 0.5;
			}
			else if(e.xkey.keycode == 133)//down
			{
				if(xangle > 340.0)xangle = 0.0;
				else xangle += 0.5;
			}
			else if(e.xkey.keycode == 15)//right
			{
				if(zangle < -340.0)zangle = -0.0;
				else zangle -= 0.5;
			}
			else if(e.xkey.keycode == 14)//left
			{
				if(zangle > 340.0)zangle = 0.0;
				else zangle += 0.5;
			}
			else if(e.xkey.keycode == 132)//z
			{
				if(yangle < -340.0)yangle = -0.0;
				else yangle -= 0.5;
			}
			else if(e.xkey.keycode == 131)//x
			{
				if(yangle > 340.0)yangle = 0.0;
				else yangle += 0.5;
			}
			else if(e.xkey.keycode == 46)//mov +x
			{
				if(objectSW == MAP)
				{
					map.xmovq += 10;
				}
				else if(objectSW == OBJECT)
				{
					fighter.xmovq += 10;
				}
			}
			else if(e.xkey.keycode == 45)//mov -x
			{
				if(objectSW == MAP)
				{
					map.xmovq -= 10;
				}
				else if(objectSW == OBJECT)
				{
					fighter.xmovq -= 10;
				}
			}
			else if(e.xkey.keycode == 42)//mov +y
			{
				if(objectSW == MAP)
				{
					map.ymovq += 10;
				}
				else if(objectSW == OBJECT)
				{
					fighter.ymovq += 10;
				}
			}
			else if(e.xkey.keycode == 48)//mov -y
			{
				if(objectSW == MAP)
				{
					map.ymovq -= 10;
				}
				else if(objectSW == OBJECT)
				{
					fighter.ymovq -= 10;
				}
			}
			else if(e.xkey.keycode == 40)//mov +z
			{
				if(objectSW == MAP)
				{
					map.zmovq += 10;
				}
				else if(objectSW == OBJECT)
				{
					fighter.zmovq += 10;
				}
			}
			else if(e.xkey.keycode == 39)//mov -z
			{
				if(objectSW == MAP)
				{
					map.zmovq -= 10;
				}
				else if(objectSW == OBJECT)
				{
					fighter.zmovq -= 10;
				}
			}
			
			else if(e.xkey.keycode == 32)//zoom +
			{
				if(objectSW == MAP)
				{
					if(!(map.xscale > 100))
					{
						map.xscale += 1;
						map.yscale += 1;
						map.zscale += 1;	
					}
				}
				else if(objectSW == OBJECT)
				{
					if(!(fighter.xscale > 100))
					{
						fighter.xscale += 1;
						fighter.yscale += 1;
						fighter.zscale += 1;
					}
				}
				else if(objectSW == ALL)
				{
					if(!(fighter.xscale > 100))
					{
						map.xscale += 1;
						map.yscale += 1;
						map.zscale += 1;
						fighter.xscale += 1;
						fighter.yscale += 1;
						fighter.zscale += 1;
					}
				}
			}
			else if(e.xkey.keycode == 35)//zoom -
			{
				if(objectSW == MAP)
				{
					if(!(map.xscale < 2))
					{
						map.xscale -= 1;
						map.yscale -= 1;
						map.zscale -= 1;	
					}
				}
				else if(objectSW == OBJECT)
				{
					if(!(fighter.xscale < 2))
					{
						fighter.xscale -= 1;
						fighter.yscale -= 1;
						fighter.zscale -= 1;	
					}
				}
				else if(objectSW == ALL)
				{
					if(!(fighter.xscale < 2))
					{
						map.xscale -= 1;
						map.yscale -= 1;
						map.zscale -= 1;
						fighter.xscale -= 1;
						fighter.yscale -= 1;
						fighter.zscale -= 1;	
					}
				}
			}
			else if(e.xkey.keycode == 13)//change objectSW
			{
				if(objectSW == MAP)objectSW = OBJECT;
				else if(objectSW == OBJECT)objectSW = ALL;
				else if(objectSW == ALL)objectSW = MAP;
			}
			break;
			
		case EnterNotify:
			if (e.xany.window == quit)XSetWindowBorderWidth(dpy, quit, 2);
			else if (e.xany.window == cls)XSetWindowBorderWidth(dpy, cls, 2);
			break;
			
		case LeaveNotify:
			if (e.xany.window == quit)XSetWindowBorderWidth(dpy, quit, 1);
			else if (e.xany.window == cls)XSetWindowBorderWidth(dpy, cls, 1);
			break;
	}
}

void debugM3d(void)
{
	if(isDebug)
	{
		drawAxis();
		printf("  angle:%3.0f,%3.0f,%3.0f\n",xangle,yangle,zangle);
		printf("-----------------\n");
		printf("  mov:%lf,%lf,%lf\n",fighter.xmov,fighter.ymov,fighter.zmov);
		odump(fighter)
		XSetForeground(dpy, gc, white);
		if(objectSW == OBJECT)XDrawString(dpy, pmap, gc, 700, 580, "ZOOM:OBJECT", 11);
		else if(objectSW == MAP)XDrawString(dpy, pmap, gc, 700, 580, "ZOOM:MAP", 8);
		else if(objectSW == ALL)XDrawString(dpy, pmap, gc, 700, 580, "ZOOM:ALL", 8);		
	}
}

int main(int argc,char *argv[])
{
	puts("Debug mode>>1:ON,other:OFF");
	scanf("%d",&isDebug);
	
	initXWindowSystem();
	
	executeGeometryPipeline();
	while(1)
	{
		XSetForeground(dpy, gc, white);
		XDrawString(dpy, quit, gc, 4, 12, "Quit", 4);
		XDrawString(dpy, cls, gc, 4, 12, "Start", 5);
		
		if(XPending(dpy))
		{//mainloop.
			controlObject();
		}
		else if(game)
		{//animation.
			setAreaClear();
			
			debugM3d();
			
			renderObject(&map,MAP);
			renderObject(&fighter,OBJECT);
			
			cdump(camera)
			getAreaCopy();
			
			XSetForeground(dpy, gc, white);
			XDrawString(dpy, w, gc, 10, 590, "U:Speed_Up,O:Speed_Down,Up/Down/Right/Left:Direction.", 33);
			
			usleep(10000);
			
			if(isDebug)system("clear");
		}
	}
	return 0;
}