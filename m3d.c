/*
 *  m3d.c
 *  xtest
 *
 *  Created by  on 10/02/09.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "m3d.h"
#define RAD	(M_PI / 180.0)

////////////////////////////////////////
//ベクトル関係
////////////////////////////////////////
//ベクトルの足し算
Vector getVectorAdd(Vector *a,Vector *b)
{
	Vector result;
	
	result.x = a->x + b->x;
	result.y = a->y + b->y;
	result.z = a->z + b->z;
	result.w = 1;
	
	return result;
}

//ベクトルの引き算
Vector getVectorSubstract(Vector *a,Vector *b)
{
	Vector result;
	
	result.x = a->x - b->x;
	result.y = a->y - b->y;
	result.z = a->z - b->z;
	result.w = 1;
	
	return result;
}

//ベクトルの大きさを返す
double getVectorSize(Vector *v)
{
	return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

//ベクトルの正規化
Vector getNormalizedVector(Vector *v)
{
	double size = getVectorSize(v);
	Vector normalized;
	
	normalized.x = v->x/size;
	normalized.y = v->y/size;
	normalized.z = v->z/size;
	normalized.w = 1;
	
	return normalized;
}

//ベクトルの初期化
Vector initWithVector(double x,double y,double z)
{
	Vector v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = 1;
	
	return v;
}

//ベクトルの内積
double getVectorDot(Vector *a,Vector *b)
{
	double dot;
	dot = a->x * b->x + a->y * b->y + a->z * b->z;
	return dot;
}

//ベクトルの外積
Vector getVectorCross(Vector *a,Vector *b)
{
	Vector cross;
	cross.x = a->y * b->z - a->z * b->y;
	cross.y = a->z * b->x - a->x * b->z;
	cross.z = a->x * b->y - a->y * b->x;
	cross.w = 1;
	
	return cross;
}

//ベクトルの大きさを変える
Vector getVectorScaled(Vector *v,double k)
{
	Vector result;
	result.x = v->x * k;
	result.y = v->y * k;
	result.z = v->z * k;
	result.w = 1;
	
	return result;
}

//ベクトルの大きさを変え適用
void setVectorScaled(Vector *v,double k)
{
	v->x = v->x * k;
	v->y = v->y * k;
	v->z = v->z * k;
	v->w = 1;
}

//ベクトルと行列の積
Vector getVectorMultipuledMatrix(Vector *v,Matrix *m)
{
	Vector result;
	result.x = m->m[ 0] * v->x + m->m[ 4] * v->y + m->m[ 8] * v->z + m->m[12] * v->w;
	result.y = m->m[ 1] * v->x + m->m[ 5] * v->y + m->m[ 9] * v->z + m->m[13] * v->w;
	result.z = m->m[ 2] * v->x + m->m[ 6] * v->y + m->m[10] * v->z + m->m[14] * v->w;
	result.w = m->m[ 3] * v->x + m->m[ 7] * v->y + m->m[11] * v->z + m->m[15] * v->w;
	
	return getVectorScaled(&result, 1/result.w);
}

//ベクトルと行列の積を適用
void setVectorMultipuledMatrix(Vector *v,Matrix *m)
{
	Vector result;
	result.x = m->m[ 0] * v->x + m->m[ 4] * v->y + m->m[ 8] * v->z + m->m[12] * v->w;
	result.y = m->m[ 1] * v->x + m->m[ 5] * v->y + m->m[ 9] * v->z + m->m[13] * v->w;
	result.z = m->m[ 2] * v->x + m->m[ 6] * v->y + m->m[10] * v->z + m->m[14] * v->w;
	result.w = m->m[ 3] * v->x + m->m[ 7] * v->y + m->m[11] * v->z + m->m[15] * v->w;
	
	*v = getVectorScaled(&result, 1/result.w);
}

////////////////////////////////////////
//行列関係
////////////////////////////////////////
//行列の初期化
Matrix initWithMatrix(double *array)
{
	Matrix result;
	int i;
	for(i = 0;i < 16;i++)
	{
		result.m[i] = array[i];
	}
	
	return result;
}

//行列の積
Matrix getMatrixMultiple(Matrix *a,Matrix *b)
{
	int i,j,k;
	double tmp;
	Matrix result;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			tmp = 0.0;
			for(k=0;k<4;k++)
			{
				tmp += a->m[i*4+k] * b->m[k*4+j];
			}
			result.m[i*4+j] = tmp;
		}
	}
	
	return result;
}

//X軸に対しての回転行列を返す
Matrix getMatrixOfRotateX(double angle)
{
	double rx[] = {
		1	,0			,0			,0,
		0	,cos(angle)	,sin(angle)	,0,
		0	,-sin(angle),cos(angle)	,0,
		0	,0			,0			,1
	};
	
	Matrix rxMatrix = initWithMatrix(rx);
	
	return rxMatrix;
}

//Y軸に対しての回転行列を返す
Matrix getMatrixOfRotateY(double angle)
{
	double ry[] = {
		cos(angle)	,0			,-sin(angle),0,
		0			,1			,0			,0,
		sin(angle)	,0			,cos(angle)	,0,
		0			,0			,0			,1
	};
	
	Matrix ryMatrix = initWithMatrix(ry);
	
	return ryMatrix;
}

//Z軸に対しての回転行列を返す
Matrix getMatrixOfRotateZ(double angle)
{
	double rz[] = {
		cos(angle)	,sin(angle)	,0			,0,
		-sin(angle)	,cos(angle)	,0			,0,
		0			,0			,1			,0,
		0			,0			,0			,1
	};
	
	Matrix rzMatrix = initWithMatrix(rz);
	
	return rzMatrix;
}

//回転行列を返す
Matrix getMatrixOfRotateXYZ(double xangle,double yangle,double zangle)
{
	Matrix result,x,y,z;
	result = initWithIMatrix();
	x = getMatrixOfRotateX(xangle*RAD);
	y = getMatrixOfRotateY(yangle*RAD);
	z = getMatrixOfRotateZ(zangle*RAD);
	result = getMatrixMultiple(&result, &x);
	result = getMatrixMultiple(&result, &y);
	result = getMatrixMultiple(&result, &z);
	
	return result;
}

//回転行列の適用
void setMatrixOfRotateXYZ(Matrix *m,double xangle,double yangle,double zangle)
{
	Matrix result,x,y,z;
	result = initWithIMatrix();
	x = getMatrixOfRotateX(xangle*RAD);
	y = getMatrixOfRotateY(yangle*RAD);
	z = getMatrixOfRotateZ(zangle*RAD);
	result = getMatrixMultiple(&result, &x);
	result = getMatrixMultiple(&result, &y);
	result = getMatrixMultiple(&result, &z);
	
	*m = getMatrixMultiple(m, &result);
}

//平行移動行列を返す
Matrix getMatrixOfMove(double x,double y,double z)
{
	double mov[] = {
		1		,0		,0		,0,
		0		,1		,0		,0,
		0		,0		,1		,0,
		x		,y		,z		,1
	};
	
	Matrix movMatrix = initWithMatrix(mov);
	
	return movMatrix;
}

//平行移動行列の適用
void setMatrixOfMove(Matrix *m,double x,double y,double z)
{
	double mov[] = {
		1		,0		,0		,0,
		0		,1		,0		,0,
		0		,0		,1		,0,
		x		,y		,z		,1
	};
	
	Matrix movMatrix = initWithMatrix(mov);
	
	*m = getMatrixMultiple(m, &movMatrix);
}

//スケーリング行列を返す
Matrix getMatrixOfScale(double sx,double sy,double sz)
{
	double scale[] = {
		sx		,0		,0		,0,
		0		,sy		,0		,0,
		0		,0		,sz		,0,
		0		,0		,0		,1
	};
	
	Matrix scaleMatrix = initWithMatrix(scale);
	
	return scaleMatrix;
}


//スケーリング行列の適用
void setMatrixOfScale(Matrix *m,double sx,double sy,double sz)
{
	double scale[] = {
		sx		,0		,0		,0,
		0		,sy		,0		,0,
		0		,0		,sz		,0,
		0		,0		,0		,1
	};
	
	Matrix scaleMatrix = initWithMatrix(scale);
	
	*m = getMatrixMultiple(m, &scaleMatrix);
}

//単位行列を返す
Matrix initWithIMatrix(void)
{
	Matrix result;
	double array[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	result = initWithMatrix(array);
	
	return result;
}

////////////////////////////////////////
//座標変換関係
////////////////////////////////////////
//ワールド座標変換行列の適用
void setWorldCoordinateSystem(Matrix* m,Matrix scale,Matrix rotate,Matrix move)
{
	//求めたい行列 = 拡大行列 * 回転行列 * 平行移動行列
	//拡大
	*m = getMatrixMultiple(m, &scale);
	//回転
	*m = getMatrixMultiple(m, &rotate);
	//平行移動
	*m = getMatrixMultiple(m, &move);
}

//ビュー座標変換行列の適用
void setViewCoordinateSystem(Matrix* m,Vector camera,Vector target,Vector up)
{
	Matrix view;
	Vector a = getVectorSubstract(&camera, &target);
	Vector z = getNormalizedVector(&a);
	Vector b = getVectorCross(&up, &z);
	Vector x = getNormalizedVector(&b);
	Vector y = getVectorCross(&z, &x);
	double qx = getVectorDot(&camera, &x);
	double qy = getVectorDot(&camera, &y);
	double qz = getVectorDot(&camera, &z);
	double array[] = {
		x.x		,y.x	,z.x	,0,
		x.y		,y.y	,z.y	,0,
		x.z		,y.z	,z.z	,0,
		-qx		,-qy	,-qz	,1
	};
	
	view = initWithMatrix(array);
	
	*m = getMatrixMultiple(m, &view);
}

//プロジェクション(射影)座標変換行列の適用
void setProjectionCoordinateSystem(Matrix* m,double vangle,double aspect,double n,double f)
{
	double sy = cos(vangle * 0.5) / sin(vangle * 0.5);
	double sx =  sy / aspect;
	double sz = f / (f - n);
	double tmp[] = {
		sx		,0		,0		,0,
		0		,sy		,0		,0,
		0		,0		,sz		,1,
		0		,0		,-sz*n	,0
	};
	Matrix projection = initWithMatrix(tmp);
	
	*m = getMatrixMultiple(m, &projection);
}

//スクリーン座標変換行列の適用
void setScreenCoordinateSystem(Matrix* m,double x,double y)
{
	double w,h;
	w = x * 0.5;
	h = y * 0.5;
	
	Matrix screen ={
		w	,0	,0	,0,
		0	,h	,0	,0,
		0	,0	,1	,0,
		w	,h	,0	,1
	};
	
	*m = getMatrixMultiple(m, &screen);
}

////////////////////////////////////////
//オブジェクト関係
////////////////////////////////////////
//オブジェクトの初期化
Object initWithObject(Vector *vector,int vsize,Wire *wire,int wsize,char *name,int nc)
{
	Object result;
	int i;
	
	for(i = 0;i < vsize;i++)result.vector[i] = vector[i];
	for(i = 0;i < wsize;i++)result.wire[i] = wire[i];
	result.vsize = vsize;
	result.wsize = wsize;
	
	result.xmov = 0;
	result.ymov = 0;
	result.zmov = 0;
	result.xmovq = 0;
	result.ymovq = 0;
	result.zmovq = 0;

	
	for(i = 0;i < nc;i++)
	{
		result.name[i] = name[i];
	}
	
	return result;
}

//オブジェクトに変換を適用
void setObjectTransformed(Object *obj,Matrix *mat)
{
	int i;
	for(i=0;i<obj->vsize;i++)
	{
		setVectorTransformed(&obj->vector[i], mat);
	}
}

////////////////////////////////////////
//カメラ関係
////////////////////////////////////////
//カメラの初期化
Camera initWithCamera(Vector eye,Vector at,Vector up)
{
	Camera result = initWithUCamera();
	Wire camWire = {
		0,0
	};
	result.eye = eye;
	result.at = at;
	result.up = up;
	result.point = camWire;

	return result;
}

//カメラの初期化
Camera initWithUCamera(void)
{
	Camera result;
	Wire point = {
		0,0
	};
	
	result.eye = initWithVector(0.0, 0.0, 0.0);
	result.at = initWithVector(0.0, 0.0, 0.0);
	result.up = initWithVector(0.0, 0.0, 0.0);
	result.point = point;
	
	return result;
}

//カメラ変換行列を返す
Camera getCameraTransformed(Camera *cam,Matrix *mat)
{
	Camera result;
	
	result.eye = getVectorTransformed(&cam->eye, mat);
	result.up = getVectorTransformed(&cam->up, mat);
	result.at = getVectorTransformed(&cam->at, mat);
	
	return result;
}

//カメラに変換を適用
void setCameraTransformed(Camera *cam,Matrix *mat)
{
	Camera result = *cam;
	
	result.eye = getVectorMultipuledMatrix(&result.eye, mat);
}

//目標点からの距離
double getDistanceFromAt(Vector *point,Vector *at)
{
	double result;
	
	result = sqrt((point->x - at->x) * (point->x - at->x) + (point->y - at->y) * (point->y - at->y) + (point->z - at->z) * (point->z - at->z));
	
	return result;
}

////////////////////////////////////////
//変換関係
////////////////////////////////////////
//変換の適用
Vector getVectorTransformed(Vector *vec,Matrix *mat)
{	
	Vector result;
	
	result = getVectorMultipuledMatrix(vec, mat);
	return result;
}

//変換のセット
void setVectorTransformed(Vector *vec,Matrix *mat)
{
	*vec = getVectorMultipuledMatrix(vec, mat);
}

////////////////////////////////////////
//描画関係
////////////////////////////////////////
//ワイヤフレームで描画する
void drawObjectWithWireFrame(Object *obj,Camera *cam,int isMap,Display *dpy,Pixmap p,GC gc)
{
	int i,j;
	char c[200];
	double a,b;
	
	a = getDistanceFromAt(&cam->eye, &cam->at);
	b = getDistanceFromAt(obj->vector, &cam->at);
	
	//点をプロットする
	for(i=0;i<obj->vsize;i++)
	{
		if(isMap)XSetForeground(dpy, gc, 0xffff00);
		else XSetForeground(dpy, gc, 0x000000);
		XFillArc(dpy,p,gc, obj->vector[i].x - 5/2, obj->vector[i].y - 5/2, 5, 5, 0, 360*64);
		if(isDebug)
		{
			sprintf(c,"%d",i);
			j = strlen(c);
			XSetForeground(dpy, gc, 0x9400D3);
			XDrawString(dpy, p, gc, obj->vector[i].x + 10 - 5/2, obj->vector[i].y + 10 - 5/2, c, j);
		}
	}
	
	if(!strcmp(obj->name, "X_axis"))XSetForeground(dpy, gc, 0xff0000);
	else if(!strcmp(obj->name, "Y_axis"))XSetForeground(dpy, gc, 0x00ff00);
	else if(!strcmp(obj->name, "Z_axis"))XSetForeground(dpy, gc, 0x0000ff);
	else XSetForeground(dpy, gc, 0x1e90ff);
	
	//ワイヤーを描画する
	for(i = 0;i < obj->wsize;i++)
	{
		if(!(isMap))
		{
			XDrawLine(dpy, p, gc, obj->vector[obj->wire[i].a].x, obj->vector[obj->wire[i].a].y, obj->vector[obj->wire[i].b].x, obj->vector[obj->wire[i].b].y);
		}
	}
	XSetForeground(dpy, gc, 0x000000);
}

//ポリゴンで描画する
/*void drawObjectWithFillPolygon(Object *obj,Display *dpy,Window p,GC gc)
{
	
}*/

////////////////////////////////////////
//頂点関係
////////////////////////////////////////
void setVertexObjectDump(Object obj,double *array[])
{
	int i;
	for(i = 0; i < obj.vsize; i += 3)
	{
		*array[i] = obj.vector[i].x;
		*array[i] = obj.vector[i].y;
		*array[i] = obj.vector[i].z;
	}
}
