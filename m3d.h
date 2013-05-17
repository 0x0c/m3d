/*
 *  m3d.h
 *  xtest
 *
 *  Created by  on 10/02/09.
 *  Copyright 2010 東京工業大学付属科学技術高等学校. All rights reserved.
 *
 */
#define MAX_ELEMENTS 200

//debug macro
int i,isDebug;
#define vdump(v) if(isDebug)printf("VNAME:%s\n",#v);if(isDebug)printf("%3.0f,%3.0f,%3.0f\n",v.x,v.y,v.z);if(isDebug)printf("-----------------\n");
#define mdump(mat)	if(isDebug)printf("MNAME:%s\n",#mat);if(isDebug)for(i = 0;i < 16;i++){if(i != 0 && i % 4 == 0)printf("\n");printf("%f ",mat.m[i]);}if(isDebug)printf("\n-----------------\n");
#define odump(obj)	if(isDebug)printf("ONAME:%s\n",#obj);if(isDebug)printf("NAME:%s\n",obj.name);if(isDebug)for(i = 0;i < obj.vsize;i++){printf("%3d:%f,%f,%f\n",i,obj.vector[i].x,obj.vector[i].y,obj.vector[i].z);}if(isDebug)printf("-----------------\n");
#define cdump(cam)	if(isDebug)printf("CNAME:%s\n",#cam);if(isDebug)printf("eye:%f,%f,%f\n",cam.eye.x,cam.eye.y,cam.eye.z);if(isDebug)printf("at:%f,%f,%f\n",cam.at.x,cam.at.y,cam.at.z);if(isDebug)printf("-----------------\n");

typedef struct{
	double x,y,z,w;
}Vector;

typedef struct{
	double m[16];
}Matrix;

typedef struct{
	int a,b;
}Wire;

typedef struct{
	int a,b,c,d;
}Plane;

typedef struct{
	Vector vector[MAX_ELEMENTS];
	Vector axis;
	Wire wire[MAX_ELEMENTS];
	Plane plane[MAX_ELEMENTS];
	int vsize,wsize;
	double xmov,ymov,zmov,xmovq,ymovq,zmovq;
	double	xscale,yscale,zscale;
	char name[140];
}Object;

typedef struct{
	Vector eye;
	Vector at;
	Vector up;
	Wire point;
	int vsize,wsize;
}Camera;

////////////////////////////////////////
//ベクトル関係
////////////////////////////////////////
//ベクトルの足し算
Vector getVectorAdd(Vector *a,Vector *b);

//ベクトルの引き算
Vector getVectorSubstract(Vector *a,Vector *b);

//ベクトルの大きさを返す
double getVectorSize(Vector *v);

//ベクトルの正規化
Vector getNormalizedVector(Vector *v);

//ベクトルの初期化
Vector initWithVector(double x,double y,double z);

//ベクトルの内積
double getVectorDot(Vector *a,Vector *b);

//ベクトルの外積
Vector getVectorCross(Vector *a,Vector *b);

//ベクトルの大きさを変える
Vector getVectorScaled(Vector *v,double k);

//ベクトルの大きさを変え適用
void setVectorScaled(Vector *v,double k);

//ベクトルと行列の積
Vector getVectorMultipuledMatrix(Vector *v,Matrix *m);

//ベクトルと行列の積を適用
void setVectorMultipuledMatrix(Vector *v,Matrix *m);

////////////////////////////////////////
//行列関係
////////////////////////////////////////
//行列の初期化
Matrix initWithMatrix(double *array);

//行列の積
Matrix getMatrixMultiple(Matrix *a,Matrix *b);

//X軸に対しての回転行列を返す
Matrix getMatrixOfRotateX(double angle);

//Y軸に対しての回転行列を返す
Matrix getMatrixOfRotateY(double angle);

//Z軸に対しての回転行列を返す
Matrix getMatrixOfRotateZ(double angle);

//回転行列を返す
Matrix getMatrixOfRotateXYZ(double xangle,double yangle,double zangle);

//回転行列の適用
void setMatrixOfRotateXYZ(Matrix *m,double xangle,double yangle,double zangle);

//平行移動行列を返す
Matrix getMatrixOfMove(double x,double y,double z);

//平行移動行列の適用
void setMatrixOfMove(Matrix *m,double x,double y,double z);

//スケーリング行列を返す
Matrix getMatrixOfScale(double sx,double sy,double sz);

//スケーリング行列の適用
void setMatrixOfScale(Matrix *m,double sx,double sy,double sz);

//単位行列を返す
Matrix initWithIMatrix(void);

////////////////////////////////////////
//座標変換関係
////////////////////////////////////////
//ワールド座標変換行列の適用
void setWorldCoordinateSystem(Matrix* m,Matrix scale,Matrix rotate,Matrix move);

//ビュー座標変換行列の適用
void setViewCoordinateSystem(Matrix* m,Vector camera,Vector target,Vector up);

//プロジェクション(射影)座標変換行列の適用
void setProjectionCoordinateSystem(Matrix *m,double vangle,double aspect,double n,double f);

//スクリーン座標変換行列の適用
void setScreenCoordinateSystem(Matrix *m,double x,double y);

////////////////////////////////////////
//オブジェクト関係
////////////////////////////////////////
//オブジェクトの初期化
Object initWithObject(Vector *vector,int vsize,Wire *Wire,int wsize,char *name,int nc);

//オブジェクトに変換を適用
void setObjectTransformed(Object *obj,Matrix *mat);

////////////////////////////////////////
//カメラ関係
////////////////////////////////////////
//オブジェクトの初期化
Camera initWithCamera(Vector eye,Vector at,Vector up);

//カメラの初期化
Camera initWithUCamera(void);

//カメラ変換行列を返す
Camera getCameraTransformed(Camera *cam,Matrix *mat);

//オブジェクトに変換を適用
void setCameraTransformed(Camera *cam,Matrix *mat);

//目標点からの距離
double getDistanceFromAt(Vector *point,Vector *at);

////////////////////////////////////////
//変換関係
////////////////////////////////////////
//変換の適用
Vector getVectorTransformed(Vector *vector,Matrix *matrix);

//変換のセット
void setVectorTransformed(Vector *vec,Matrix *mat);

////////////////////////////////////////
//描画関係
////////////////////////////////////////
//ワイヤフレームで描画する
void drawObjectWithWireFrame(Object *obj,Camera *cam,int isMap,Display *dpy,Pixmap p,GC gc);

//ポリゴンで描画する
//void drawObjectWithFillPolygon(Object *obj,Display *dpy,Window p,GC gc)

////////////////////////////////////////
//頂点関係
////////////////////////////////////////
void dumpVertexObject(Object obj,double array);
