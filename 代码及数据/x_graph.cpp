//

// #include "stdafx.h"



#include <windows.h>  //This lib MUST be included 
#include <conio.h>
#include <tchar.h>
#include<stdlib.h>
#include <stdio.h>
#include<math.h>


extern "C" {WINBASEAPI HWND WINAPI GetConsoleWindow();}
#define WHITE RGB(255,255,255)
#define BLACK RGB(0,0,0)

#define RED RGB(255,0,0)
#define BLUE RGB(0,0,255)
#define GREEN RGB(0,0x80,0)
#define CYAN RGB(0,0xFF,0xFF)
#define MAGENTA RGB(0xFF,0,0xFF)
#define GRAY RGB(0x80,0x80H,0x80)

#define LIGHTRED RGB(255,0xB6,0xC1)
#define LIGHTBLUE RGB(0xad,0xd8,0xe6)
#define LIGHTGREEN RGB(0x90,0xee,0x90)
#define LIGHTCYAN RGB(0xe0,0xFF,0xFF)
#define LIGHTMAGENTA RGB(0xFF,0x14,0x93)
#define LIGHTGRAY RGB(0xd3,0xd3,0xd3)

#define DARKGRAY RGB(0xa9,0xa9,0xa9)
#define YELLOW RGB(0xf0,0xe6,0x8c)
//ȫ������GetConsoleWindow()�������÷�ʽ��


HWND hWndFather;//ȫ��
//**********************************************************//
///////////////////////////////////////////////////////////////////////
typedef struct tagWIN2{
	HWND hwnd;
	HDC hdc;
	RECT rect,rect2;
	char xstr[50],ystr[50];
	int xo,yo;
	double x1,y1,x2,y2;
	double xk,yk,dxPerPixel,dyPerPixel;}WIN2;
WIN2 win2;


#define LRTB_SPACE 25    //����Ĭ�ϵ��������ұ߿����ס� 

double ALPHA=3.1415926/8;
double BEITA=3.1415926/6;

typedef struct tagWIN3{
	HWND hwnd;
	HDC hdc;
	RECT rect;//,rect2;
	//int xo,yo;//moveto3,linTo3������0�㡣
	char xstr[50],ystr[50],zstr[50];
	double x1,y1,z1,x2,y2,z2;
	double dxPerPixel,dyPerPixel,dzPerPixel;//dxPerPixel=1/xk,dyPerPixel=1/yk,dzPerPixel=1/zk.
	double xPixels,yPixels,zPixels,gridPixels;//xPixelsΪ������ĳ���
	//double tx1,tx2,tx3,tx4,ty1,ty2,ty3,ty4,tz1,tz2,tz3,tz4,tc1,tc2,tc3,tc4;
	double t[4][4];//t[0][]=x�У�t[1][]Ϊy�У�t[2][]--z,t[3]--�����С�
}WIN3;
WIN3 win3;

typedef union{
		struct{double x,y,z,c;};
		double A[4];}POINT3,*PPOINT3;


typedef struct{
	int x,y;}SCR2,*PSCR2;


#define ROUND(d) int(floor(d+0.5))

#define SHOWWIN \
SendMessage(HWND_BROADCAST,WM_PAINT,0,0);_getch();

//system("pause");

#define DRAWXYZ \
{POINT ca;SelectObject(win3.hdc,hpen1);line3(0,0,0,40,0,0);\
ca=getxy3();TextOut(win3.hdc,ca.x,ca.y,(LPCWSTR)"x",1);\
SHOWWIN;\
SelectObject(win3.hdc,hpen2);line3(0,0,0,0,40,0);\
ca=getxy3();TextOut(win3.hdc,ca.x,ca.y,(LPCWSTR)"y",1);\
SHOWWIN;\
SelectObject(win3.hdc,hpen3);line3(0,0,0,0,0,40);\
ca=getxy3();TextOut(win3.hdc,ca.x,ca.y,(LPCWSTR)"z",1);\
SHOWWIN;}


////////////////////////////////////////////////////////////////////
//ϸȥ����ߣ�����Ϊ1��/////////////////////////////////////////////
void lineto1(HDC hdc,COLORREF bkcolor,COLORREF linecolor,int x,int y)
{
POINT currentPosition;MoveToEx(hdc,x,y,&currentPosition);
int x0=currentPosition.x,y0=currentPosition.y,temp;

if(x==x0){
	if(y0>y){temp=y0,y0=y,y=temp;}
	for(;y0<y;y0++)SetPixelV(hdc,x0,y0,linecolor);}
else if(y==y0){
	if(x0>x){temp=x0,x0=x,x=temp;}
	for(;x0<x;x0++)SetPixelV(hdc,x0,y0,linecolor);}
else{
	int rf=GetRValue(linecolor),gf=GetGValue(linecolor),bf=GetBValue(linecolor);
	int re=GetRValue(bkcolor)-rf,ge=GetGValue(bkcolor)-gf,be=GetBValue(bkcolor)-bf;
	double k=(double)(y-y0)/(double)(x-x0),k2=1.0/k,t;//����б��������
	if(k>1.0||k<-1.0){//y������
		if(y0>y){temp=x0,x0=x,x=temp,temp=y0,y0=y,y=temp;}		
		for(t=(double)x0;y0<=y;y0++,t+=k2)
			{double e1=t-(int)t,e2=1-e1;
			SetPixelV(hdc,(int)t,y0,RGB(ROUND(re*e1)+rf,ROUND(ge*e1)+gf,ROUND(be*e1)+bf));
			SetPixelV(hdc,(int)t+1,y0,RGB(ROUND(re*e2)+rf,ROUND(ge*e2)+gf,ROUND(be*e2)+bf));}
	}
	if(-1.0<=k&&k<=1.0){//x������
		if(x0>x){temp=x0,x0=x,x=temp,temp=y0,y0=y,y=temp;}		
		for(t=y0;x0<=x;x0++,t+=k)
			{double e1=t-(int)t,e2=1-e1;
			SetPixelV(hdc,x0,(int)t,RGB(ROUND(re*e1)+rf,ROUND(ge*e1)+gf,ROUND(be*e1)+bf));
			SetPixelV(hdc,x0,(int)t+1,RGB(ROUND(re*e2)+rf,ROUND(ge*e2)+gf,ROUND(be*e2)+bf));}	
	}
}
}



///////////////////////////////////////////////////////////////////
//����ߣ���(x0,y0)��(x1,y1)ֱ�ߵķ�(cx,cy)�����ڱ���һ��ȥ��ݱ�//
void purfleline(HDC hdc,COLORREF bkcolor,COLORREF linecolor,int x0,int y0,int x1,int y1,int cx,int cy)
{
#define MIN(a,b) ((a<b)?a:b)
double t0,e0;
//���k2=+-INF,��k=0;���ܱ�֤����������
double k2=(double)(y1-y0)/(double)(x1-x0),k=-1.0/k2;
int t;
int rf=GetRValue(linecolor),gf=GetGValue(linecolor),bf=GetBValue(linecolor);
int re=GetRValue(bkcolor)-rf,ge=GetGValue(bkcolor)-gf,be=GetBValue(bkcolor)-bf;

	if(-1.0>k2||k2>1.0){//y����
			if(cx-x0-k*(cy-y0)>0){
			if(y0>y1){t=x0,x0=x1,x1=t;t=y0,y0=y1,y1=t;}
			for(t0=(double)x0;y0<=y1;y0++,t0-=k)
			{e0=t0-(int)t0;
			COLORREF c=GetPixel(hdc,(int)t0,y0);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,(int)t0,y0,RGB(r,g,b));
			//SetPixelV(hdc,(int)t0,y0,RGB(ROUND(re*e0)+rf,ROUND(ge*e0)+gf,ROUND(be*e0)+bf));
			SetPixelV(hdc,(int)t0+1,y0,linecolor);}
		}else{
			if(y0>y1){t=x0,x0=x1,x1=t;t=y0,y0=y1,y1=t;}
			for(t0=(double)x0;y0<=y1;y0++,t0-=k)
			{e0=1-(t0-(int)t0);
			SetPixelV(hdc,(int)t0,y0,linecolor);
			COLORREF c=GetPixel(hdc,(int)t0+1,y0);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,(int)t0+1,y0,RGB(r,g,b));
			//SetPixelV(hdc,(int)t0+1,y0,RGB(ROUND(re*e0)+rf,ROUND(ge*e0)+gf,ROUND(be*e0)+bf));
			}
		}
	}
	if(-1.0<=k2&&k2<=1.0){//x����
		if(cy-y0-k2*(cx-x0)>0)
		{
			if(x0>x1){t=x0,x0=x1,x1=t;t=y0,y0=y1,y1=t;}
			for(t0=(double)y0;x0<=x1;x0++,t0+=k2)
			{e0=t0-(int)t0;
			COLORREF c=GetPixel(hdc,x0,(int)t0);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,x0,(int)t0,RGB(r,g,b));
			//SetPixelV(hdc,x0,(int)t0,RGB(ROUND(re*e0)+rf,ROUND(ge*e0)+gf,ROUND(be*e0)+bf));
			SetPixelV(hdc,x0,(int)t0+1,linecolor);}//end for
		}else{
			if(x0>x1){t=x0,x0=x1,x1=t;t=y0,y0=y1,y1=t;}
			for(t0=(double)y0;x0<=x1;x0++,t0+=k2)
			{e0=1-t0+(int)t0;
			SetPixelV(hdc,x0,(int)t0,linecolor);
			COLORREF c=GetPixel(hdc,x0,(int)t0+1);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,x0,(int)t0+1,RGB(r,g,b));
			//SetPixelV(hdc,x0,(int)t0+1,RGB(ROUND(re*e0)+rf,ROUND(ge*e0)+gf,ROUND(be*e0)+bf));
			}
		}	
	}

}//end purfleline

void purfleline(HDC hdc,COLORREF bkcolor,COLORREF linecolor,POINT a,POINT b,POINT ref)
{purfleline(hdc,bkcolor,linecolor,a.x,a.y,b.x,b.y,ref.x,ref.y);}
		   

//////////////////////////////////////////////////////////////////
//��ͷ���ƺ������ӵ�a��b�������߶���b���һ����ͷ��///////////////
void arrowline(HDC hdc,int linewidth,COLORREF bkcolor,COLORREF linecolor,int ax,int ay,int bx,int by)
{
int arrowbottomwidth=3*linewidth,arrowhigth=4*linewidth;
double k,k2;
POINT v[7];
if(ay==by){
	v[0].x=ax,v[0].y=ay+linewidth/2;
	v[1].x=ax,v[1].y=ay-linewidth/2;
	v[6].x=bx,v[6].y=v[0].y;
	v[2].x=bx,v[2].y=v[1].y;
	v[5].x=bx,v[5].y=by+arrowbottomwidth/2;
	v[3].x=bx,v[3].y=by-arrowbottomwidth/2;
	v[4].x=bx+((bx>ax)?arrowhigth:-arrowhigth);v[4].y=by;
}else if(ax==bx){
	v[0].x=ax+linewidth/2,v[0].y=ay;
	v[1].x=ax-linewidth/2,v[1].y=ay;
	v[6].x=bx+linewidth/2,v[6].y=by;
	v[2].x=bx-linewidth/2,v[2].y=by;
	v[5].x=bx+arrowbottomwidth/2,v[5].y=by,
	v[3].x=bx-arrowbottomwidth/2,v[3].y=by;
	v[4].x=bx,v[4].y=by+((by>ay)?arrowhigth:-arrowhigth);
}else{
k2=(double)(ax-bx)/(double)(ay-by);
//(x-x0)sqrt(1+k2*k2)=+-wide/2.0
double st=linewidth/(2.0*sqrt(1+k2*k2));
v[0].x=ROUND(ax+st);v[0].y=ROUND(ay-k2*st);
v[1].x=ROUND(ax-st);v[1].y=ROUND(ay+k2*st);
v[6].x=ROUND(bx+st);v[6].y=ROUND(by-k2*st);
v[2].x=ROUND(bx-st);v[2].y=ROUND(by+k2*st);
st=arrowbottomwidth/(2.0*sqrt(1+k2*k2));
v[5].x=ROUND(bx+st);v[5].y=ROUND(by-k2*st);
v[3].x=ROUND(bx-st);v[3].y=ROUND(by+k2*st);
k=-1/k2;
st=arrowhigth/(sqrt(1+k*k));
v[4].x=ROUND(bx+((bx>ax)?st:-st));
v[4].y=ROUND(by-k*((bx>ax)?st:-st));
}
//v[0],v[1],v[2],v[6]����ʱ�빹�ɾ��Ρ�v[3],v[4],v[5]��ʱ�빹�ɼ�ͷ��v[4]�Ǽ�ͷ��

HBRUSH brush=CreateSolidBrush(linecolor);
HBRUSH oldbrush=(HBRUSH)SelectObject(hdc,brush);
HPEN pen=CreatePen(PS_SOLID,1,linecolor);
HPEN oldpen=(HPEN)SelectObject(hdc,pen);

Polygon(hdc,v,7);


int rf=GetRValue(linecolor),gf=GetGValue(linecolor),bf=GetBValue(linecolor);
int re=GetRValue(bkcolor)-rf,ge=GetGValue(bkcolor)-gf,be=GetBValue(bkcolor)-bf;
purfleline(hdc,bkcolor,linecolor,v[0],v[6],v[1]);
purfleline(hdc,bkcolor,linecolor,v[1],v[2],v[0]);
//���ƶ̱ߣ�/////////////////////////////////////
purfleline(hdc,bkcolor,linecolor,v[0],v[1],v[6]);
//����3�㵽6����߶Σ��б�ӱ߷���ʱ��Ҫ�ߵ���
purfleline(hdc,bkcolor,linecolor,v[6],v[5],v[4]);
//���Ƶ�4����5���߱�,�б�ӱ߷���ʱ��Ҫ�ߵ���
purfleline(hdc,bkcolor,linecolor,v[2],v[3],v[4]);
//���Ƶ�6����7���߱�
purfleline(hdc,bkcolor,linecolor,v[3],v[4],v[5]);
//���Ƶ�5����7���߱�
purfleline(hdc,bkcolor,linecolor,v[4],v[5],v[3]);

SelectObject(hdc,oldpen);
DeleteObject(pen);
SelectObject(hdc,oldbrush);
DeleteObject(brush);
}

void arrowline(HDC hdc,int linewidth,COLORREF bkcolor,COLORREF linecolor,POINT a,POINT b)
{arrowline(hdc,linewidth,bkcolor,linecolor,a.x,a.y,b.x,b.y);}


//////////////////////////////////////////////////////////////////
//�����ߣ�ȥ��ݣ�������width>=1./////////////////////////////////
void rectline(HDC hdc,int width,COLORREF bkcolor,COLORREF linecolor,double ax,double ay,double bx,double by)
{
#define MIN(a,b) ((a<b)?a:b)

double k,k2;
double x1,y1,x2,y2,x3,y3,x4,y4;
POINT vt,v[4];


if(fabs(ay-by)<1.0e-6){
	x1=ax,y1=ay+width/2.0;
	x2=ax,y2=ay-width/2.0;
	x3=bx,y3=y1;
	x4=bx,y4=y2;
}
else{
k2=-(ax-bx)/(ay-by);
//(x-x0)sqrt(1+k2*k2)=+-wide/2.0
x1=ax+width/(2.0*sqrt(1+k2*k2));y1=ay+k2*width/(2*sqrt(1+k2*k2));
x2=ax-width/(2.0*sqrt(1+k2*k2));y2=ay-k2*width/(2*sqrt(1+k2*k2));
x3=bx+width/(2.0*sqrt(1+k2*k2));y3=by+k2*width/(2*sqrt(1+k2*k2));
x4=bx-width/(2.0*sqrt(1+k2*k2));y4=by-k2*width/(2*sqrt(1+k2*k2));
}


//11,12,--22,21�㹹�ɾ��Ρ���x11x21(v[0]v[3])����x12x22(v[1]v[2])ƽ�С���x11x12(v[0]v[1])����21-22(v[3]v[2]ƽ�С�
v[0].x=ROUND(x1);v[0].y=ROUND(y1);
v[1].x=ROUND(x2);v[1].y=ROUND(y2);
v[2].x=ROUND(x4);v[2].y=ROUND(y4);
v[3].x=ROUND(x3);v[3].y=ROUND(y3);



///////////////////////
HBRUSH brush=CreateSolidBrush(linecolor);
HBRUSH oldbrush=(HBRUSH)SelectObject(hdc,brush);
Polygon(hdc,v,4);
SelectObject(hdc,oldbrush);
DeleteObject(brush);

int rf=GetRValue(linecolor),gf=GetGValue(linecolor),bf=GetBValue(linecolor);
int re=GetRValue(bkcolor)-rf,ge=GetGValue(bkcolor)-gf,be=GetBValue(bkcolor)-bf;

double t0,t1,e0,e1;

if(v[0].x!=v[3].x&&v[0].y!=v[3].y){
	k=(double)(v[3].y-v[0].y)/(double)(v[3].x-v[0].x),k2=1.0/k;
//	k2=-k2;
	if(k>1.0||k<-1.0){//y������
		if(v[0].y>v[3].y){vt=v[0],v[0]=v[3],v[3]=vt,vt=v[1],v[1]=v[2],v[2]=vt;}
		if(v[0].x>v[1].x){vt=v[0],v[0]=v[1],v[1]=vt,vt=v[3],v[3]=v[2],v[2]=vt;}
		for(t0=(double)v[0].x,t1=(double)v[1].x;v[0].y<=v[3].y;t0+=k2,t1+=k2,v[0].y++,v[1].y++)
		{e0=t0-(int)t0,e1=1-e0;
		COLORREF c=GetPixel(hdc,(int)t0,v[0].y);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,(int)t0,v[0].y,RGB(r,g,b));
		SetPixelV(hdc,(int)t0+1,v[0].y,linecolor);

		SetPixelV(hdc,(int)t1,v[1].y,linecolor);

		c=GetPixel(hdc,(int)t1+1,v[0].y);
		r=MIN(ROUND(re*e1)+rf,GetRValue(c)),g=MIN(ROUND(ge*e1)+gf,GetGValue(c)),b=MIN(ROUND(be*e1)+bf,GetBValue(c));
		SetPixelV(hdc,(int)t1+1,v[1].y,RGB(r,g,b));
		}}
	if(-1<=k&&k<=1.0){//x������
		if(v[0].x>v[3].x){vt=v[0],v[0]=v[3],v[3]=vt,vt=v[1],v[1]=v[2],v[2]=vt;}
		if(v[0].y>v[1].y){vt=v[0],v[0]=v[1],v[1]=vt,vt=v[3],v[3]=v[2],v[2]=vt;}
		for(t0=(double)v[0].y,t1=(double)v[1].y;v[0].x<=v[3].x;t0+=k ,t1+=k ,v[0].x++,v[1].x++)
		{e0=t0-(int)t0,e1=1-e0;
		COLORREF c=GetPixel(hdc,v[0].x,(int)t0);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,v[0].x,(int)t0,RGB(r,g,b));
		SetPixelV(hdc,v[0].x,(int)t0+1,linecolor);
		//SetPixelV(hdc,v[1].x,(int)t1-2,RGB(255,255,0));
		SetPixelV(hdc,v[1].x,(int)t1,linecolor);
		c=GetPixel(hdc,v[1].x,(int)t1+1);
		r=MIN(ROUND(re*e1)+rf,GetRValue(c)),g=MIN(ROUND(ge*e1)+gf,GetGValue(c)),b=MIN(ROUND(be*e1)+bf,GetBValue(c));
		SetPixelV(hdc,v[1].x,(int)t1+1,RGB(r,g,b));
		}}

//���ƶ̱ߣ�
v[0].x=ROUND(x1);v[0].y=ROUND(y1);
v[1].x=ROUND(x2);v[1].y=ROUND(y2);
v[2].x=ROUND(x4);v[2].y=ROUND(y4);
v[3].x=ROUND(x3);v[3].y=ROUND(y3);
vt=v[0],v[0]=v[3],v[3]=v[2],v[2]=v[1],v[1]=vt;
		k=(double)(v[3].y-v[0].y)/(double)(v[3].x-v[0].x),k2=1.0/k;
	if(k>1.0||k<-1.0){//y������
		if(v[0].y>v[3].y){vt=v[0],v[0]=v[3],v[3]=vt,vt=v[1],v[1]=v[2],v[2]=vt;}
		if(v[0].x>v[1].x){vt=v[0],v[0]=v[1],v[1]=vt,vt=v[3],v[3]=v[2],v[2]=vt;}
		for(t0=(double)v[0].x,t1=(double)v[1].x;v[0].y<=v[3].y;t0+=k2,t1+=k2,v[0].y++,v[1].y++)
		{e0=t0-(int)t0,e1=1-e0;
		COLORREF c=GetPixel(hdc,(int)t0,v[0].y);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,(int)t0,v[0].y,RGB(r,g,b));
		SetPixelV(hdc,(int)t0+1,v[0].y,linecolor);
		//SetPixelV(hdc,(int)t0+2,v[0].y,RGB(255,0,0));
		//SetPixelV(hdc,(int)t1-2,v[1].y,RGB(0,255,0));		
		c=GetPixel(hdc,(int)t1+1,v[0].y);
		r=MIN(ROUND(re*e1)+rf,GetRValue(c)),g=MIN(ROUND(ge*e1)+gf,GetGValue(c)),b=MIN(ROUND(be*e1)+bf,GetBValue(c));
		SetPixelV(hdc,(int)t1,v[1].y,linecolor);
		SetPixelV(hdc,(int)t1+1,v[1].y,RGB(r,g,b));
		}}
	if(-1<=k&&k<=1.0){//x������
		if(v[0].x>v[3].x){vt=v[0],v[0]=v[3],v[3]=vt,vt=v[1],v[1]=v[2],v[2]=vt;}
		if(v[0].y>v[1].y){vt=v[0],v[0]=v[1],v[1]=vt,vt=v[3],v[3]=v[2],v[2]=vt;}
		for(t0=(double)v[0].y,t1=(double)v[1].y;v[0].x<=v[3].x;t0+=k ,t1+=k ,v[0].x++,v[1].x++)
		{e0=t0-(int)t0,e1=1-e0;
		COLORREF c=GetPixel(hdc,v[0].x,(int)t0);
		int r=MIN(ROUND(re*e0)+rf,GetRValue(c)),g=MIN(ROUND(ge*e0)+gf,GetGValue(c)),b=MIN(ROUND(be*e0)+bf,GetBValue(c));
		SetPixelV(hdc,v[0].x,(int)t0,RGB(r,g,b));
		SetPixelV(hdc,v[0].x,(int)t0+1,linecolor);

		//SetPixelV(hdc,v[1].x,(int)t1-2,RGB(255,0,0));		
		c=GetPixel(hdc,v[1].x,(int)t1+1);
		r=MIN(ROUND(re*e1)+rf,GetRValue(c)),g=MIN(ROUND(ge*e1)+gf,GetGValue(c)),b=MIN(ROUND(be*e1)+bf,GetBValue(c));
		SetPixelV(hdc,v[1].x,(int)t1,linecolor);
		SetPixelV(hdc,v[1].x,(int)t1+1,RGB(r,g,b));
		}}
	}
//������߷�ʽЧ���ܲӦ������ͼ���˲���ʽ��
#undef MIN
}












void coord2(double x1,double y1,double x2,double y2)
{
//WIN2 a;
if (x1>x2){win2.x2=x1;win2.x1=x2;}
else if(x1==x2){printf("���������ô���");_getch();exit(0);}
else {win2.x1=x1;win2.x2=x2;}

if(y1>y2){win2.y2=y1;win2.y1=y2;}
else if(y1==y2){printf("���������ô���");_getch();exit(0);}
else {win2.y1=y1;win2.y2=y2;}


//printf("\nx1=%g,y1=%g,x2=%g,y2=%g\n",win2.x1,win2.y1,win2.x2,win2.y2);
//GetClientRect(win2.hwnd,&win2.rect);


win2.rect2.left=win2.rect.left+LRTB_SPACE;
win2.rect2.right=win2.rect.right-LRTB_SPACE;
win2.rect2.top=win2.rect.top+LRTB_SPACE;
win2.rect2.bottom=win2.rect.bottom-LRTB_SPACE;

win2.xk=(double)(win2.rect2.right-win2.rect2.left)/(win2.x2-win2.x1);
win2.yk=(double)(win2.rect2.bottom-win2.rect2.top)/(win2.y2-win2.y1); 
win2.dxPerPixel=1.0/win2.xk;
win2.dyPerPixel=1.0/win2.yk;

win2.xo=(int)(-win2.x1*win2.xk)+win2.rect2.left;
win2.yo=(int)(win2.y2*win2.yk+win2.rect2.top);

//win2.hwnd=hWnd;
//win2.hdc=hdc;
}


void frame2(char xstr[],char ystr[])//,COLORREF color)
{

//POINT dd[]={35,40,60,40,80,80};
//Polygon(win2.hdc,dd,3);
SelectObject(win2.hdc,(HPEN )GetStockObject(BLACK_PEN));
SelectObject(win2.hdc,(HBRUSH )GetStockObject(WHITE_BRUSH));//LTGRAY_BRUSH));
Rectangle(win2.hdc,win2.rect.left,win2.rect.top,win2.rect.right,win2.rect.bottom);//ʹmemdcָ���λͼ��ˢ��huiɫ�� 

#define DGRID 20    //���ص���/ÿ�����
/*
char str[100]={0};

//win2.x1=1.6738e-2,win2.x2=22;
sprintf((char *)str,"(%g,%g)",win2.x1,win2.y1);
//sprintf_s(str,200,(const char *)_T("(%g,%g)"),win2.x1,win2.y1);
int nn=strlen(str);//_tcslen(str);

//printf("str=%s;nn=%d\n",str,nn);getch();

TextOut(win2.hdc,win2.rect2.left,win2.rect2.bottom+6,(LPCWSTR)str,nn);//strlen(str));

sprintf(str,"(%g,%g)",win2.x2,win2.y2);

TextOut(win2.hdc,win2.rect2.right-8*strlen(str)+14,win2.rect2.top-18,(LPCWSTR)str,strlen(str));

sprintf(str,"%s:%g/��",ystr,DGRID/(win2.yk));
TextOut(win2.hdc,win2.rect2.left,win2.rect2.top-18,(LPCWSTR)str,strlen(str));
sprintf(str,"%s:%g/��",xstr,DGRID/(win2.xk));
TextOut(win2.hdc,win2.rect2.right-strlen(str)*8,win2.rect2.bottom+6,(LPCWSTR)str,strlen(str));

*/


WCHAR str[100]={0};//WCHAR�ǿ��ַ�����UNICODE��ʽ����
swprintf_s(str,100,L"(%g,%g)",win2.x1,win2.y1);//100�ǻ���str��С
TextOut(win2.hdc,win2.rect2.left,win2.rect2.bottom+6,(LPCWSTR)str,_tcslen(str));
swprintf_s(str,100,L"(%g,%g)",win2.x2,win2.y2);
TextOut(win2.hdc,win2.rect2.right-8*_tcslen(str)+14,win2.rect2.top-18,(LPCWSTR)str,_tcslen(str));
swprintf_s(str,100,L"%S:%g/��",ystr,DGRID/(win2.yk));
TextOut(win2.hdc,win2.rect2.left,win2.rect2.top-18,(LPCWSTR)str,_tcslen(str));
swprintf_s(str,100,L"%S:%g/��",xstr,DGRID/(win2.xk));//S��д����xstr��ANSI�ַ���
TextOut(win2.hdc,win2.rect2.right-_tcslen(str)*8,win2.rect2.bottom+6,(LPCWSTR)str,_tcslen(str));


int i;
for(i=win2.xo;i<win2.rect2.right;i=i+DGRID){
	if(i>win2.rect2.left){MoveToEx(win2.hdc,i,win2.rect2.bottom,NULL);
	LineTo(win2.hdc,i,win2.rect2.bottom+5);}}

for(i=win2.xo;i>win2.rect2.left;i=i-DGRID){
	if(i<win2.rect2.right){MoveToEx(win2.hdc,i,win2.rect2.bottom,NULL);
	LineTo(win2.hdc,i,win2.rect2.bottom+5);}}

for(i=win2.yo;i<win2.rect2.bottom;i=i+DGRID){
	if(i>win2.rect2.top){MoveToEx(win2.hdc,win2.rect2.left,i,NULL);
	LineTo(win2.hdc,win2.rect2.left-5,i);}}

for(i=win2.yo;i>win2.rect2.top;i=i-DGRID){
	if(i<win2.rect2.bottom){MoveToEx(win2.hdc,win2.rect2.left,i,NULL);
	LineTo(win2.hdc,win2.rect2.left-5,i);}}

if(win2.xo>=win2.rect2.left&&win2.xo<=win2.rect2.right)
	{MoveToEx(win2.hdc,win2.xo,win2.rect2.bottom,NULL);
	LineTo(win2.hdc,win2.xo-2,win2.rect2.bottom+5);
	LineTo(win2.hdc,win2.xo+2,win2.rect2.bottom+5);
	LineTo(win2.hdc,win2.xo,win2.rect2.bottom);
	}

if(win2.yo>=win2.rect2.top&&win2.yo<=win2.rect2.bottom)
	{MoveToEx(win2.hdc,win2.rect2.left,win2.yo,NULL);
	LineTo(win2.hdc,win2.rect2.left-5,win2.yo-2);
	LineTo(win2.hdc,win2.rect2.left-5,win2.yo+2);
	LineTo(win2.hdc,win2.rect2.left,win2.yo);
	}
	
//SetBkColor(win2.hdc ,GetPixel(win2.hdc,3,3));




//SelectObject(win2.hdc,(HBRUSH )GetStockObject(WHITE_BRUSH));
Rectangle(win2.hdc,win2.rect2.left-1,win2.rect2.top-1,win2.rect2.right+1,win2.rect2.bottom+1);


sprintf_s(win2.xstr,50,xstr);
sprintf_s(win2.ystr,50,ystr);
//return 0;
//printf("�ڴˣ�����");system("pause");

//InvalidateRect(win2.hwnd,NULL,true);
//UpdateWindow(win2.hwnd);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
void frame2(char xstr[],char ystr[],int space)
{
win2.rect2.left=win2.rect.left+space;
win2.rect2.right=win2.rect.right-space;
win2.rect2.top=win2.rect.top+space;
win2.rect2.bottom=win2.rect.bottom-space;

win2.xk=(double)(win2.rect2.right-win2.rect2.left)/(win2.x2-win2.x1);
win2.yk=(double)(win2.rect2.bottom-win2.rect2.top)/(win2.y2-win2.y1); 
win2.dxPerPixel=1.0/win2.xk;
win2.dyPerPixel=1.0/win2.yk;
win2.xo=(int)(-win2.x1*win2.xk)+win2.rect2.left;
win2.yo=(int)(win2.y2*win2.yk+win2.rect2.top);

frame2(xstr,ystr);
}

void frame2()
{frame2(win2.xstr,win2.ystr);}

#define ux2(x) (int)(win2.xo+x*win2.xk)
#define uy2(y) (int)(win2.yo-y*win2.yk)



void moveto2(double x,double y)
{MoveToEx(win2.hdc,ux2(x),uy2(y),NULL);}

void lineto2(double x,double y)
{LineTo(win2.hdc,ux2(x),uy2(y));}

void line2(double x1,double y1,double x2,double y2)
{moveto2(x1,y1);lineto2(x2,y2);}





void plot2(double (*functionName)(double))
{double x;
x=win2.x1;
moveto2(x,(double)(*functionName)(x));
for(;x<win2.x2;x=x+win2.dxPerPixel)lineto2(x,(double)(*functionName)(x));
}

void plot2(double (*functionName)(double),double dt)
{double x;
x=win2.x1;
moveto2(x,(double)(*functionName)(x));
for(;x<win2.x2;x=x+dt)lineto2(x,(double)(*functionName)(x));
x=win2.x2;lineto2(x,(double)(*functionName)(x));
}

#define _plotxy2(t,func) \
{double t;\
t=win2.x1;\
moveto2(t,func);\
for(;t<win2.x2;t=t+win2.dxPerPixel)lineto2(t,func);\
t=win2.x2;lineto2(t,func);}

///����t����Ҫ��������
#define plotxy2(penColor,penWidth,t,func) \
{HPEN pen=CreatePen(PS_SOLID,penWidth,penColor),oldpen=(HPEN)SelectObject(win2.hdc,pen);\
_plotxy2(t,func);\
SelectObject(win2.hdc,oldpen);\
DeleteObject(pen);\
}

#define ux3(x,y,z) (int)(x*win3.t[0][0]+y*win3.t[1][0]+z*win3.t[2][0]+win3.t[3][0]+0.5)
#define uy3(x,y,z) (int)(-x*win3.t[0][1]-y*win3.t[1][1]-z*win3.t[2][1]-win3.t[3][1]+0.5)

void moveto3(double x,double y,double z)
{int sx,sy;
sx=(int)(x*win3.t[0][0]+y*win3.t[1][0]+z*win3.t[2][0]+win3.t[3][0]+0.5);
sy=(int)(-x*win3.t[0][1]-y*win3.t[1][1]-z*win3.t[2][1]-win3.t[3][1]+0.5);
MoveToEx(win3.hdc,sx,sy,NULL);}


void moveto3(POINT3 a)
{moveto3(a.x,a.y,a.z);}


void lineto3(double x,double y,double z)
{int sx,sy;
sx=(int)(x*win3.t[0][0]+y*win3.t[1][0]+z*win3.t[2][0]+win3.t[3][0]+0.5);
sy=(int)(-x*win3.t[0][1]-y*win3.t[1][1]-z*win3.t[2][1]-win3.t[3][1]+0.5);
LineTo(win3.hdc,sx,sy);}

//lineto1(win3.hdc,RGB(255,255,255),RGB(0,0,0),sx,sy);}

void lineto3(POINT3 a)
{lineto3(a.x,a.y,a.z);}

void line3(double x1,double y1,double z1,double x2,double y2,double z2)
{moveto3(x1,y1,z1);lineto3(x2,y2,z2);}

void line3(POINT3 a,POINT3 b)
{moveto3(a);lineto3(b);}

void mMul(double* A,double* B,int m,int n,int p,double* C)
{
int i,j,k;
for(i=0;i<m;i++)for(j=0;j<p;j++){
	*(C+i*p+j)=0.0;
	for(k=0;k<n;k++) (*(C+i*p+j))+=(*(A+i*n+k))*(*(B+k*p+j));}

}



POINT3 pt3(double x,double y,double z)
{POINT3 a;a.x=x;a.y=y;a.z=z;a.A[3]=1;return a; }


void shift3(double dx,double dy,double dz)//ƽ�����ꡣ
{
 double RX[4][4]={0};//,beita=0.6;
RX[0][0]=1;
RX[1][1]=1;
RX[2][2]=1;
RX[3][3]=1;
RX[3][0]=dx;RX[3][1]=dy;RX[3][2]=dz;

double temp[4][4];
mMul((double*)RX,(double*)win3.t,4,4,4,(double*)temp);
int i,j;
for(i=0;i<4;i++)for(j=0;j<4;j++)win3.t[i][j]=temp[i][j];
}

void shift3(POINT3 p[],int n,double dx,double dy,double dz)//�ڵ�ǰ������ƽ��һ��㡣
{
int i;
for(i=0;i<n;i++){
p[i].x=p[i].x-dx;
p[i].y=p[i].y-dy;
p[i].z=p[i].z-dz;}
}


void rx3(POINT3 p[],int n,double a)//a����ת�Ƕȡ�
{
 double RX[4][4]={0};//,beita=0.6;
RX[0][0]=1;
RX[1][1]=cos(a);
RX[2][2]=RX[1][1];
RX[1][2]=sin(a);
RX[2][1]=-RX[1][2];
RX[3][3]=1;

POINT3 temp;
int i;
for(i=0;i<n;i++){
	p[i].A[3]=1;
	mMul((double*)p[i].A,(double*)RX,1,4,4,(double*)temp.A);
    p[i].x=temp.x;p[i].y=temp.y;p[i].z=temp.z;}

}

void rx3(double a)//a����ת�Ƕ�,������任��
{
 double RX[4][4]={0};//,beita=0.6;
RX[0][0]=1;
RX[1][1]=cos(a);
RX[2][2]=RX[1][1];
RX[1][2]=sin(a);
RX[2][1]=-RX[1][2];
RX[3][3]=1;

double temp[4][4];
mMul((double*)RX,(double*)win3.t,4,4,4,(double*)temp);
int i,j;
for(i=0;i<4;i++)for(j=0;j<4;j++)win3.t[i][j]=temp[i][j];
}


void sxyz3(double sx,double sy,double sz)
{
	double ST[4][4]={0};
	ST[0][0]=sx;
	ST[1][1]=sy;
	ST[2][2]=sz;
	ST[3][3]=1;

double temp[4][4];
mMul((double*)ST,(double*)win3.t,4,4,4,(double*)temp);
int i,j;
for(i=0;i<4;i++)for(j=0;j<4;j++)win3.t[i][j]=temp[i][j];
}


void ry3(POINT3 p[],int n,double a)//a����ת�Ƕȡ�
{
double RX[4][4]={0};//,beita=0.6;
RX[0][0]=cos(a);
RX[1][1]=1;
RX[2][2]=RX[0][0];
RX[2][0]=sin(a);
RX[0][2]=-RX[2][0];
RX[3][3]=1;

POINT3 temp;
int i;
for(i=0;i<n;i++){
	p[i].A[3]=1;
	mMul((double*)p[i].A,(double*)RX,1,4,4,(double*)temp.A);
    p[i].x=temp.x;p[i].y=temp.y;p[i].z=temp.z;}

}

void ry3(double a)//a����ת�Ƕ�,y��任��
{
 double RX[4][4]={0};//,beita=0.6;
RX[0][0]=cos(a);
RX[1][1]=1;
RX[2][2]=RX[0][0];
RX[2][0]=sin(a);
RX[0][2]=-RX[2][0];
RX[3][3]=1;

double temp[4][4];
mMul((double*)RX,(double*)win3.t,4,4,4,(double*)temp);
int i,j;
for(i=0;i<4;i++)for(j=0;j<4;j++)win3.t[i][j]=temp[i][j];
}


void rz3(POINT3 p[],int n,double a)//a����ת�Ƕȡ�
{
double RX[4][4]={0};//,beita=0.6;
RX[0][0]=cos(a);
RX[0][1]=sin(a);
RX[1][0]=-RX[0][1];
RX[1][1]=RX[0][0];
RX[2][2]=1;
RX[3][3]=1;

POINT3 temp;
int i;
for(i=0;i<n;i++){
	p[i].A[3]=1;
	mMul((double*)p[i].A,(double*)RX,1,4,4,(double*)temp.A);
    p[i].x=temp.x;p[i].y=temp.y;p[i].z=temp.z;}

}

void rz3(double a)//a����ת�Ƕȡ�
{
 double RX[4][4]={0};//,beita=0.6;
RX[0][0]=cos(a);
RX[0][1]=sin(a);
RX[1][0]=-RX[0][1];
RX[1][1]=RX[0][0];
RX[2][2]=1;
RX[3][3]=1;

double temp[4][4];
mMul((double*)RX,(double*)win3.t,4,4,4,(double*)temp);
int i,j;
for(i=0;i<4;i++)for(j=0;j<4;j++)win3.t[i][j]=temp[i][j];
}

///////////////////////////////////////////////////////////
#define _MULRX \
double temp[4][4];\
mMul((double*)RX,(double*)win3.t,4,4,4,(double*)temp);\
int i,j;\
for(i=0;i<4;i++)for(j=0;j<4;j++)win3.t[i][j]=temp[i][j];
////////////////////////////////////////////////////////////

POINT getxy3()
{
POINT currentPosition;
MoveToEx(win3.hdc, 0, 0,&currentPosition);// (LPPOINT) NULL);
MoveToEx(win3.hdc, currentPosition.x,currentPosition.y,NULL);
return currentPosition;
}


void rv3(double w,double x1,double y1,double z1,
				  double x2,double y2,double z2)
{
double x=x2-x1,y=y2-y1,z=z2-z1;
double wy,wz;

//ʸ����y��ת����xoyƽ��Ľǣ�
wy=atan2(z,x);

//ʸ����z����xoyƽ����ת����x��Ľǣ�
wz=atan2(y,sqrt(x*x+z*z));

shift3(x1,y1,z1);
ry3(wy);
rz3(-wz);//atan(sqrt(2)/2));///3.1415926/4);
rx3(w);
rz3(wz);
ry3(-wy);
shift3(-x1,-y1,-z1);
}

void rv3(POINT3 C[],int n,double w,double x1,double y1,double z1,
				  double x2,double y2,double z2)
{
double x=x2-x1,y=y2-y1,z=z2-z1;
double wy,wz;

//ʸ����y��ת����xoyƽ��Ľǣ�
wy=atan2(z,x);

//ʸ����z����xoyƽ����ת����x��Ľǣ�
wz=atan2(y,sqrt(x*x+z*z));

shift3(C,n,x1,y1,z1);
ry3(C,n,wy);
rz3(C,n,-wz);//atan(sqrt(2)/2));///3.1415926/4);
rx3(C,n,w);
rz3(C,n,wz);
ry3(C,n,-wy);
shift3(C,n,-x1,-y1,-z1);
}





void testRot()//����ͨ��������
{
HPEN hpen1=CreatePen(PS_SOLID,1,RGB(255,0,0));
HPEN hpen2=CreatePen(PS_SOLID,1,RGB(0,255,0));
HPEN hpen3=CreatePen(PS_SOLID,1,RGB(0,0,255));
HPEN hpen4=CreatePen(PS_SOLID,1,RGB(200,0,255));
HPEN hpen5=CreatePen(PS_SOLID,1,RGB(0,200,255));
HPEN hpen6=CreatePen(PS_SOLID,1,RGB(200,200,0));

double bb=3.1415926/6;
rx3(bb);
ry3(bb);
DRAWXYZ;

POINT3 C[1]={20,100,0,1};//////{POINT3(20,100,0)};

//POINT3 DCC=C[0];
//{20,100,0,1},
POINT3 A={20,100,0,1};//
double x1=0,y1=0,z1=0,x2=0,y2=0,z2=100;
int i;

line3(x1,y1,z1,x2,y2,z2);

moveto3(A);
for(i=0;i<41;i++){
rv3(C,1,3.1415926/20,x1,y1,z1,x2,y2,z2);
lineto3(C[0]);}

SHOWWIN;
for(i=0;i<21;i++){
rv3(3.1415926/10,0,100,0,200,0,0);
DRAWXYZ;}
SHOWWIN;

}

void rv3()

{

HPEN hpen1=CreatePen(PS_SOLID,3,RGB(255,0,0));
HPEN hpen2=CreatePen(PS_SOLID,3,RGB(0,255,0));
HPEN hpen3=CreatePen(PS_SOLID,3,RGB(0,0,255));
HPEN hpen4=CreatePen(PS_SOLID,3,RGB(200,0,255));
HPEN hpen5=CreatePen(PS_SOLID,3,RGB(0,200,255));
HPEN hpen6=CreatePen(PS_SOLID,3,RGB(200,200,0));

double bb=3.1415926/6;
rx3(bb);
ry3(bb);
shift3(-100,-100,-100);



DRAWXYZ
//////////////////////////////////////////
POINT3 c[3]={20,20,20,1,100,100,100,1,100,100,200,1};
POINT3 org={0,0,0};
double x=-100,y=100,z=100,wy,wz;

#define DRAWLINE SelectObject(win3.hdc,hpen5);line3(c[0],c[1]);lineto3(c[2]);\
SHOWWIN;
//SendMessage(HWND_BROADCAST,WM_PAINT,0,0);system("pause");

#define DRAWLINE2 SelectObject(win3.hdc,hpen6);line3(c[0],c[1]);lineto3(c[2]);\
SHOWWIN;
//SendMessage(HWND_BROADCAST,WM_PAINT,0,0);system("pause");

#define DRAWLINE3 SelectObject(win3.hdc,hpen4);line3(c[0],c[1]);lineto3(c[2]);\
SHOWWIN;
//SendMessage(HWND_BROADCAST,WM_PAINT,0,0);system("pause");


DRAWLINE
shift3(c,3,20,20,20);
DRAWLINE2

x=c[1].x;
y=c[1].y;
z=c[1].z;

//ʸ����y��ת����xoyƽ��Ľǣ�
wy=atan2(z,x);

//ʸ����z����xoyƽ����ת����x��Ľǣ�
wz=atan2(y,sqrt(x*x+z*z));


ry3(c,3,wy);
//DRAWXYZ
DRAWLINE


rz3(c,3,-wz);//atan(sqrt(2)/2));///3.1415926/4);
//DRAWXYZ
printf("\ndd=%f,%f,%f;\n",c[1].x,c[1].y,c[1].z);
DRAWLINE

//��ʸ��ת����x���Ժ�����ʸ��ת��w�Ƕȵĵ�c�������ꡣ


double w=3.1415926/6;
rx3(c,3,w);

//�ָ���z?
rz3(c,3,wz);
DRAWLINE3

//�ָ���y?
ry3(c,3,-wy);
DRAWLINE3

/////
shift3(c,3,-20,-20,-20);
DRAWLINE


//////////////
double x1=20,y1=30,z1=40,x2=200,y2=-200,z2=80,xp=-20,yp=-100,zp=50; 
SelectObject(win3.hdc,hpen4);line3(x1,y1,z1,x2,y2,z2);


SelectObject(win3.hdc,hpen5);lineto3(xp,yp,zp);

shift3(x1,y1,z1);
}


class VERTEX{
public:
	double x;
	double y;
	double z;
public:
	VERTEX();
	virtual ~VERTEX();
	VERTEX(double x1,double y1,double z1){x=x1;y=y1;z=z1;}
};

class EDGE{
public:
	int eNumber;
	int *eIndex;
public:
    EDGE();
	virtual ~EDGE();
};


class box{
public:
	POINT3 v[8];
//	LINESET  l[12];
//	FACESET   f[6];
	double x0,y0,z0;
	double len;
public:
	box();
	virtual ~box();
};



void coord3(double x1,double y1,double z1,double x2,double y2,double z2)
{
if (x1>x2){win3.x2=x1;win2.x1=x2;}
else if(x1==x2){printf("x�������ô���");_getch();exit(1);}
else {win3.x1=x1;win3.x2=x2;}

if(y1>y2){win3.y2=y1;win3.y1=y2;}
else if(y1==y2){printf("y�������ô���");_getch();exit(1);}
else {win3.y1=y1;win3.y2=y2;}

if(z1>z2){win3.z2=z1;win3.z1=z2;}
else if(z1==z2){printf("z�������ô���");_getch();exit(1);}
else {win3.z1=z1;win3.z2=z2;}
/*
GetClientRect(win3.hwnd,&win3.rect2);

win3.rect.left=win3.rect2.left+LRTB_SPACE;
win3.rect.right=win3.rect2.right-LRTB_SPACE;
win3.rect.top=win3.rect2.top+LRTB_SPACE;
win3.rect.bottom=win3.rect2.bottom-LRTB_SPACE;
*/

GetClientRect(win3.hwnd,&win3.rect);

double w,h;
w=win3.rect.right-win3.rect.left;
h=-win3.rect.top+win3.rect.bottom;

//��Ϊ����������ϵ������x��ת30�Ƚ�����y��ת-30�Ƚǣ�
//����ֻ��y���ͶӰ��ƽ���봰����������ȱ�����еı߳����ֻ�д�������߶ȵ�һ�룺
if(h>w)h=w;
win3.xPixels=h/2.0;

win3.zPixels=win3.yPixels=win3.xPixels=(double)(10.0*((int)win3.xPixels/10));
win3.dxPerPixel=(win3.x2-win3.x1)/win3.xPixels;
win3.dyPerPixel=(win3.y2-win3.y1)/win3.yPixels;
win3.dzPerPixel=(win3.z2-win3.z1)/win3.zPixels;

//����������ϵΪ��Ļ�������꣬��ʱ����ԭ���ڻ�ͼ�ͻ��������Ͻǣ�
win3.t[0][0]=1;win3.t[0][1]=0;win3.t[0][2]=0;win3.t[0][3]=0;
win3.t[1][0]=0;win3.t[1][1]=1;win3.t[1][2]=0;win3.t[1][3]=0;
win3.t[2][0]=0;win3.t[2][1]=0;win3.t[2][2]=0;win3.t[2][3]=0;
win3.t[3][0]=0;win3.t[3][1]=0;win3.t[3][2]=0;win3.t[3][3]=1;

//ƽ����������ԭ�㵽��ͼ�ͻ������ģ�
shift3((win3.rect.right+win3.rect.left)/2.0,-(win3.rect.top+win3.rect.bottom)/2.0,0);


rx3(3.1415926/6);
ry3(-3.1415926/6);
rx3(-3.1415926/2);
//rz3(-3.415926/2);


//���û���������任���������꣬ʹ�����������û�������ϵ��
sxyz3(1.0/win3.dxPerPixel,1.0/win3.dyPerPixel,1.0/win3.dzPerPixel);

//���û������£��ƶ���������ԭ��λ�ã�
shift3(-(win3.x1+win3.x2)/2,-(win3.y1+win3.y2)/2,-(win3.z1+win3.z2)/2);

win3.gridPixels=5;
}



void coord3(POINT3 a,POINT3 b)
{coord3(a.x,a.y,a.z,b.x,b.y,b.z);}

void frame3(COLORREF bkcolor,COLORREF dotLineColor,char *xstr,char *ystr,char *zstr)
{
//bkcolorûʹ�ã�dotLinecolor��ʹ���ˣ�����Ϊ��ɫ��
/*
POINT v[6];
v[0].x=ux3(win3.x1,win3.y1,win3.z1),v[0].y=uy3(win3.x1,win3.y1,win3.z1);
v[1].x=ux3(win3.x2,win3.y1,win3.z1),v[1].y=uy3(win3.x2,win3.y1,win3.z1);
v[2].x=ux3(win3.x2,win3.y2,win3.z1),v[2].y=uy3(win3.x2,win3.y2,win3.z1);
v[3].x=ux3(win3.x2,win3.y2,win3.z2),v[3].y=uy3(win3.x2,win3.y2,win3.z2);
v[4].x=ux3(win3.x1,win3.y2,win3.z2),v[4].y=uy3(win3.x1,win3.y2,win3.z2);
v[5].x=ux3(win3.x1,win3.y1,win3.z2),v[5].y=uy3(win3.x1,win3.y1,win3.z2);


HBRUSH brush=CreateSolidBrush(bkcolor);
HBRUSH oldbrush=(HBRUSH)SelectObject(win3.hdc,brush);
*/
//Polygon(win3.hdc,v,6);


//moveto3(win3.x1,win3.y1,win3.z1);
//c=getxy3();
//HPEN pen=CreatePen(PS_SOLID,1,dotLineColor);
//MoveToEx(win3.hdc,v[0].x,v[0].y,NULL);


SelectObject(win3.hdc,(HPEN )GetStockObject(WHITE_PEN));
SelectObject(win3.hdc,(HBRUSH )GetStockObject(WHITE_BRUSH));
Rectangle(win3.hdc,win3.rect.left,win3.rect.top,win3.rect.right,win3.rect.bottom);//��׻�ͼ���� 


//char str[50]={0};

/*

swprintf_s(str,100,L"(%g,%g)",win2.x1,win2.y1);//100�ǻ���str��С
TextOut(win2.hdc,win2.rect2.left,win2.rect2.bottom+6,(LPCWSTR)str,_tcslen(str));

*/

UINT charmode=GetTextAlign(win3.hdc);
POINT m;

/*
sprintf(str," (%g,%g,%g)",win3.x1,win3.y1,win3.z1);
SetTextAlign(win3.hdc,TA_RIGHT|TA_TOP);
moveto3(win3.x1,win3.y1,win3.z1);
m=getxy3();
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)str,strlen(str));

moveto3((win3.x1+win3.x2)/2,win3.y1,win3.z1);
m=getxy3();
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)xstr,strlen(xstr));

moveto3(win3.x1,win3.y1,(win3.z1+win3.z2)/2);
m=getxy3();
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)zstr,strlen(zstr));

SetTextAlign(win3.hdc,TA_LEFT|TA_TOP);
moveto3(win3.x2,(win3.y1+win3.y2)/2,win3.z1);
m=getxy3();
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)ystr,strlen(ystr));

SetTextAlign(win3.hdc,TA_LEFT|TA_BOTTOM);
sprintf(str,"(%g,%g,%g)",win3.x2,win3.y2,win3.z2);
moveto3(win3.x2,win3.y2,win3.z2);
m=getxy3();
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)str,strlen(str));


*/
WCHAR str[100]={0};//WCHAR�ǿ��ַ�����UNICODE��ʽ����


swprintf_s(str,100,L" (%g,%g,%g)",win3.x1,win3.y1,win3.z1);
SetTextAlign(win3.hdc,TA_RIGHT|TA_TOP);
moveto3(win3.x1,win3.y1,win3.z1);
m=getxy3();
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)str,_tcslen(str));

moveto3((win3.x1+win3.x2)/2,win3.y1,win3.z1);
m=getxy3();
swprintf_s(str,100,L"%S",xstr);
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)str,_tcslen(str));

moveto3(win3.x1,win3.y1,(win3.z1+win3.z2)/2);
m=getxy3();
swprintf_s(str,100,L"%S",zstr);
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)str,_tcslen(str));


SetTextAlign(win3.hdc,TA_LEFT|TA_TOP);
moveto3(win3.x2,(win3.y1+win3.y2)/2,win3.z1);
m=getxy3();
swprintf_s(str,100,L"%S",ystr);
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)str,_tcslen(str));


SetTextAlign(win3.hdc,TA_LEFT|TA_BOTTOM);
swprintf_s(str,100,L"(%g,%g,%g)",win3.x2,win3.y2,win3.z2);
moveto3(win3.x2,win3.y2,win3.z2);
m=getxy3();
TextOut(win3.hdc,m.x,m.y,(LPCWSTR)str,_tcslen(str));




SetTextAlign(win3.hdc,charmode);
/////////
HPEN pen=CreatePen(PS_SOLID,1,dotLineColor);
HPEN oldpen=(HPEN)SelectObject(win3.hdc,pen);

line3(win3.x1,win3.y1,win3.z1,win3.x2,win3.y1,win3.z1);
lineto3(win3.x2,win3.y2,win3.z1);
lineto3(win3.x2,win3.y2,win3.z2);
lineto3(win3.x1,win3.y2,win3.z2);
lineto3(win3.x1,win3.y1,win3.z2);
lineto3(win3.x1,win3.y1,win3.z1);

line3(win3.x1,win3.y1,win3.z1,win3.x1,win3.y2,win3.z1);
lineto3(win3.x2,win3.y2,win3.z1);
line3(win3.x1,win3.y2,win3.z1,win3.x1,win3.y2,win3.z2);

double grid,g;
grid=(win3.x2-win3.x1)/10;
for(g=0;g<win3.x2;g=g+grid){line3(g,win3.y1,win3.z1,g,win3.y2,win3.z1);lineto3(g,win3.y2,win3.z2);}
for(g=0;g>win3.x1;g=g-grid){line3(g,win3.y1,win3.z1,g,win3.y2,win3.z1);lineto3(g,win3.y2,win3.z2);}

grid=(win3.y2-win3.y1)/10;
for(g=0;g<win3.y2;g=g+grid){line3(win3.x2,g,win3.z1,win3.x1,g,win3.z1);lineto3(win3.x1,g,win3.z2);}
for(g=0;g>win3.y1;g=g-grid){line3(win3.x2,g,win3.z1,win3.x1,g,win3.z1);lineto3(win3.x1,g,win3.z2);}

grid=(win3.z2-win3.z1)/10;
for(g=0;g<win3.z2;g=g+grid){line3(win3.x1,win3.y1,g,win3.x1,win3.y2,g);lineto3(win3.x2,win3.y2,g);}
for(g=0;g>win3.z1;g=g-grid){line3(win3.x1,win3.y1,g,win3.x1,win3.y2,g);lineto3(win3.x2,win3.y2,g);}

SelectObject(win3.hdc,oldpen);DeleteObject(pen);
sprintf_s(win3.xstr,50,xstr);
sprintf_s(win3.ystr,50,ystr);
sprintf_s(win3.zstr,50,zstr);
//SelectObject(win3.hdc,oldbrush);DeleteObject(brush);
}
/*
//ǳ��ɫ����
line3(win3.x1,win3.y1,win3.z1,win3.x2,win3.y1,win3.z1);
lineto3(win3.x2,win3.y2,win3.z1);
lineto3(win3.x2,win3.y2,win3.z2);
lineto3(win3.x1,win3.y2,win3.z2);
lineto3(win3.x1,win3.y1,win3.z2);
lineto3(win3.x1,win3.y1,win3.z1);

HPEN hPen2=CreatePen(PS_DOT, 1, dotLineColor);
SelectObject(win3.hdc, hPen2);
//ǳ��ɫ����
 lineto3(win3.x1,win3.y2,win3.z1);
 lineto3(win3.x2,win3.y2,win3.z1);
line3(win3.x1,win3.y2,win3.z1,win3.x1,win3.y2,win3.z2);
SelectObject(win3.hdc,oldpen);
DeleteObject(hPen2);
//DeleteObject(hPen);
}
*/

//void frame3()
//{frame3(RGB(0,0,0),RGB(128,128,128));}

void frame3(char *xstr,char *ystr,char *zstr)
	{frame3(WHITE,BLACK,xstr,ystr,zstr);}

void frame3()
{frame3(win3.xstr,win3.ystr,win3.zstr);}

void arrow3(COLORREF linecolor,double ax,double ay,double az,double bx,double by,double bz,int h,int d2)
{
POINT a,b;
moveto3(ax,ay,az);a=getxy3();
moveto3(bx,by,bz);b=getxy3();
arrowline(win3.hdc,3,RGB(255,255,255),linecolor,a,b);

}


void xyz3(COLORREF axiesColor)
{
int h=10,d2=2;

arrow3(axiesColor,win3.x1,0,0,win3.x2,0,0,h,d2);
arrow3(axiesColor,0,win3.y1,0,0,win3.y2,0,h,d2);
arrow3(axiesColor,0,0,win3.z1,0,0,win3.z2,h,d2);
/*
arrow3(RGB(0,0,0),win3.x1,0,0,win3.x2,0,0,h,d2);
arrow3(RGB(0,0,0),0,win3.y1,0,0,win3.y2,0,h,d2);
arrow3(RGB(0,0,0),0,0,win3.z1,0,0,win3.z2,h,d2);*/
}



void initgraph00(WCHAR title[],int x,int y,int width,int higth)
{
system("color f0");//���ÿ���̨���ڱ���Ϊ��ɫ��ǰ��Ϊ��ɫ��
//_getch();//system("pause");//��ͣ

CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
//���ؿ���̨��꣬�������ͼ��
//_getch();
HWND hWnd=GetConsoleWindow();
HDC hDC=GetDC(hWnd);//ͨ�����ھ���õ��ô��ڵ��豸�������

SetConsoleTitle((LPCWSTR)title);//�޸Ĵ��ڱ��⡣
SetWindowPos(hWnd,HWND_TOPMOST ,x,y,width,higth,SWP_SHOWWINDOW|SWP_NOCOPYBITS
			 ); 
SetWindowLong(hWnd,//handletowindow
GWL_STYLE,//intnlndex,//offsetofvaluetoset
WS_POPUP//WS_POPUPWINDOW//WS_OVERLAPPEDWINDOW//CS_HREDRAW | CS_VREDRAW//LVS_ICON// LVS_SMALLICON LVS_LIST LVS_REPORT LONGdwNewLong//newvalue
);//�޸Ĵ��ڷ��

  
RECT r;
win2.hwnd=hWnd;win2.hdc=hDC;
win3.hwnd=hWnd;win3.hdc=hDC;

GetClientRect(hWnd,&r);
win2.rect=r;win3.rect=r;
}


void initgraph(WCHAR title[],int x,int y,int width,int higth)
{
system("color f0");//���ÿ���̨���ڱ���Ϊ��ɫ��ǰ��Ϊ��ɫ��
//_getch();//system("pause");//��ͣ

CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
//���ؿ���̨��꣬�������ͼ��
//_getch();
HWND hWnd=GetConsoleWindow();
HDC hDC=GetDC(hWnd);//ͨ�����ھ���õ��ô��ڵ��豸�������

SetConsoleTitle((LPCWSTR)title);//�޸Ĵ��ڱ��⡣
SetWindowPos(hWnd,HWND_TOPMOST ,x,y,width,higth,SWP_SHOWWINDOW|SWP_NOCOPYBITS
			 ); 
/*
SetWindowLong(hWnd,//handletowindow
GWL_STYLE,//intnlndex,//offsetofvaluetoset
WS_POPUP//WS_POPUPWINDOW//WS_OVERLAPPEDWINDOW//CS_HREDRAW | CS_VREDRAW//LVS_ICON// LVS_SMALLICON LVS_LIST LVS_REPORT LONGdwNewLong//newvalue
);//�޸Ĵ��ڷ��
*/

  
RECT r;
win2.hwnd=hWnd;win2.hdc=hDC;
win3.hwnd=hWnd;win3.hdc=hDC;

GetClientRect(hWnd,&r);
win2.rect=r;win3.rect=r;
}




///ģ�º�����ʽ������x,y��Ҫ��������
#define _plotxyz3(x,y,fxy) \
{double x,y,grid=win3.gridPixels*win3.dyPerPixel;\
for(y=win3.y1;y<=win3.y2;y=y+grid)\
{x=win3.x1,moveto3(x,y,fxy);\
for(x=win3.x1;x<=win3.x2;x=x+win3.dxPerPixel)\
lineto3(x,y,fxy);}\
y=win3.y2;\
{x=win3.x1,moveto3(x,y,fxy);\
for(x=win3.x1;x<=win3.x2;x=x+win3.dxPerPixel)\
lineto3(x,y,fxy);}\
grid=win3.gridPixels*win3.dxPerPixel;\
for(x=win3.x1;x<=win3.x2;x=x+grid)\
{y=win3.y1,moveto3(x,y,fxy);\
for(y=win3.y1;y<=win3.y2;y=y+win3.dyPerPixel)\
lineto3(x,y,fxy);}\
x=win3.x2;\
{y=win3.y1,moveto3(x,y,fxy);\
for(y=win3.y1;y<=win3.y2;y=y+win3.dyPerPixel)\
lineto3(x,y,fxy);}\
}




///ģ�º�����ʽ������x,y��Ҫ��������
#define plotxyz3(penColor,penWidth,x,y,fxy) \
{HPEN hPen=CreatePen(PS_SOLID, penWidth, penColor);\
HPEN hOldPen=(HPEN)SelectObject(win3.hdc, hPen);\
_plotxyz3(x,y,fxy);\
SelectObject(win2.hdc, hOldPen);\
DeleteObject(hPen);\
}



void plot3(COLORREF color,double (*fxy)(double ,double))
{double x,y;
double grid=win3.gridPixels*win3.dyPerPixel;
HPEN hPen=CreatePen(PS_SOLID, 1, color);
HPEN hOldPen=(HPEN)SelectObject(win2.hdc, hPen);\
for(y=win3.y1;y<=win3.y2;y=y+grid)\
{x=win3.x1,moveto3(x,y,(*fxy)(x,y));\
for(x=win3.x1;x<=win3.x2;x=x+win3.dxPerPixel)\
lineto3(x,y,(*fxy)(x,y));}\
y=win3.y2;\
{x=win3.x1,moveto3(x,y,(*fxy)(x,y));\
for(x=win3.x1;x<=win3.x2;x=x+win3.dxPerPixel)\
lineto3(x,y,(*fxy)(x,y));}\
grid=win3.gridPixels*win3.dxPerPixel;\
for(x=win3.x1;x<=win3.x2;x=x+grid)\
{y=win3.y1,moveto3(x,y,(*fxy)(x,y));\
for(y=win3.y1;y<=win3.y2;y=y+win3.dyPerPixel)\
lineto3(x,y,(*fxy)(x,y));}\
x=win3.x2;\
{y=win3.y1,moveto3(x,y,(*fxy)(x,y));\
for(y=win3.y1;y<=win3.y2;y=y+win3.dyPerPixel)\
lineto3(x,y,(*fxy)(x,y));}\
DeleteObject(hPen);
}

void plot3(COLORREF gridcolor,COLORREF linecolor,COMPLEX p[],int M)
{
int i;
HPEN hPen=CreatePen(PS_SOLID, 1, gridcolor);
HPEN hOldPen=(HPEN)SelectObject(win2.hdc, hPen);\
for(i=0;i<M;i++)line3(i,0,0,i,p[i].r,p[i].i);

HPEN hPen2=CreatePen(PS_SOLID, 1, linecolor);
SelectObject(win2.hdc, hPen2);\
moveto3(0,p[0].r,p[0].i);
for(i=0;i<M;i++)lineto3(i,p[i].r,p[i].i);

SelectObject(win2.hdc,hOldPen);
DeleteObject(hPen2);
DeleteObject(hPen);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////




enum USEWIN{USE_CONSOLE,USE_WINDOW};

class winx{
public:
	HINSTANCE hinstance;
	HWND hwnd0;
	HDC hdc0;
	RECT rect0;
//	int xo,yo;
//	double x1,y1,x2,y2;
//	double xk,yk,dxPerPixel,dyPerPixel;
public:
	winx(USEWIN win=USE_CONSOLE,char *title="xxgcʵ��ƽ̨",int x0=0,int y0=0,int width=800,int height=600){
		hinstance=(HINSTANCE)GetModuleHandle(NULL);
		if(win==USE_CONSOLE){
				system("color f0");//���ÿ���̨���ڱ���Ϊ��ɫ��ǰ��Ϊ��ɫ��

				CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
				SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
				//���ؿ���̨��꣬�������ͼ��

				hwnd0=GetConsoleWindow();
				hdc0=GetDC(hwnd0);//ͨ�����ھ���õ��ô��ڵ��豸�������

				SetConsoleTitle((LPCWSTR)title);//�޸Ĵ��ڱ��⡣
				SetWindowPos(hwnd0,HWND_TOPMOST,x0,y0,width,height,SWP_SHOWWINDOW);   

//win2.hwnd=hWnd;win2.hdc=hDC;
//win3.hwnd=hWnd;win3.hdc=hDC;
		}
		else{}
	};
};










void xy2(COLORREF color)
{
 arrowline(win2.hdc,3,RGB(255,255,255),color,win2.rect2.left,win2.yo,win2.rect2.right-12,win2.yo);
arrowline(win2.hdc,3,RGB(255,255,255),color,win2.xo,win2.rect2.bottom,win2.xo,win2.rect2.top+12);
}


void window2(WCHAR title[],double x1,double y1,double x2,double y2,char *xAxis,char *yAxis)
//	void window2(char *title,double x1,double y1,double x2,double y2,char *xAxis,char *yAxis)
{

//setvp2(0,0,600,500,title,"");
initgraph(title,0,0,800,600);
coord2(x1,y1,x2,y2);
frame2(xAxis,yAxis);
}

void clearviewport(COLORREF bkcolor)
{
POINT v[4];
v[0].x=win2.rect2.left,v[0].y=win2.rect2.top;

v[1].x=win2.rect2.left,v[1].y=win2.rect2.bottom;
v[2].x=win2.rect2.right,v[2].y=win2.rect2.bottom;
v[3].x=win2.rect2.right,v[3].y=win2.rect2.top;


HBRUSH brush=CreateSolidBrush(bkcolor);
HPEN pen=CreatePen(PS_NULL,1,bkcolor);

HBRUSH oldbrush=(HBRUSH)SelectObject(win2.hdc,brush);
HPEN oldpen=(HPEN)SelectObject(win2.hdc,pen);
Polygon(win2.hdc,v,4);
SelectObject(win2.hdc,oldpen);DeleteObject(pen);
SelectObject(win2.hdc,oldbrush);DeleteObject(brush);
}




void setvp3(int scrx,int scry,int width,int heigth,WCHAR title[],char *notice)
{
initgraph(title,scrx,scry,width,heigth);
}


void window3(WCHAR title[],double x1,double y1,double z1,double x2,double y2,double z2,char *xAxis,char *yAxis,char *zAxis)
{
//setvp3(0,0,800,600,title,"");
initgraph(title,0,0,800,600);
coord3(x1,y1,z1,x2,y2,z2);
frame3(LIGHTGRAY,BLACK,xAxis,yAxis,zAxis);
}

//�����ά�û����ڵ�ͼ�Ρ�
void clear3()
{
SelectObject(win3.hdc,(HPEN )GetStockObject(BLACK_PEN));
SelectObject(win3.hdc,(HBRUSH )GetStockObject(WHITE_BRUSH));//LTGRAY_BRUSH));
Rectangle(win3.hdc,win3.rect.left,win3.rect.top,win3.rect.right,win3.rect.bottom);
}

BOOL ERASEDRAW2=true,ERASEDRAW3=true;


void instKeyCtr3()
{
int N=10;
int x1=(win3.rect.left+win3.rect.right)/2,y=win3.rect.bottom-N,x2;
x2=x1+50;
x1=x1-50;

HPEN pen1=CreatePen(PS_SOLID,11,BLACK),oldpen=(HPEN)SelectObject(win3.hdc,pen1);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);

HPEN pen2=CreatePen(PS_SOLID,9,WHITE);SelectObject(win3.hdc,pen2);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);


HPEN pen3=CreatePen(PS_SOLID,1,BLACK);SelectObject(win3.hdc,pen3);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);

HPEN pen4=CreatePen(PS_SOLID,1,RED);SelectObject(win3.hdc,pen4);
	MoveToEx(win3.hdc,x1,y-5,NULL);
	LineTo(win3.hdc,x1,y+5);

SelectObject(win3.hdc,oldpen);//�ָ�ԭ���ı�.
DeleteObject(pen1);
DeleteObject(pen2);
DeleteObject(pen3);
DeleteObject(pen4);
}

void instKeyCtr2()
{
int N=10;
int x1=(win2.rect.left+win2.rect.right)/2,y=win2.rect.bottom-N,x2;
x2=x1+50;
x1=x1-50;

HPEN pen1=CreatePen(PS_SOLID,11,BLACK),oldpen=(HPEN)SelectObject(win2.hdc,pen1);
	MoveToEx(win2.hdc,x1,y,NULL);
	LineTo(win2.hdc,x2,y);

HPEN pen2=CreatePen(PS_SOLID,9,WHITE);SelectObject(win2.hdc,pen2);
	MoveToEx(win2.hdc,x1,y,NULL);
	LineTo(win2.hdc,x2,y);


HPEN pen3=CreatePen(PS_SOLID,1,BLACK);SelectObject(win3.hdc,pen3);
	MoveToEx(win2.hdc,x1,y,NULL);
	LineTo(win2.hdc,x2,y);

HPEN pen4=CreatePen(PS_SOLID,1,RED);SelectObject(win3.hdc,pen4);
	MoveToEx(win2.hdc,x1,y-5,NULL);
	LineTo(win2.hdc,x1,y+5);

SelectObject(win2.hdc,oldpen);//�ָ�ԭ���ı�.
DeleteObject(pen1);
DeleteObject(pen2);
DeleteObject(pen3);
DeleteObject(pen4);
}


//�ú���Ϊѭ����ȡ�������룬��ESC������������ѭ���˳���
//��+=.>�ļ���������(b-a)/n��������x[0]�е���ֵ��
//��_-,<�ļ���������(b-a)/n��������x[0]�е���ֵ��
BOOL keyCtr2(double x[1],double a,double b,int n)
{
	int k=0;double dx;
	if(n==0||b==a){printf("keyCtr()�����������\n");exit(1);}
	dx=(b-a)/n;
for(;k!=27;k=_getch())
	{    if(k=='+'||k=='='||k=='.'||k=='>'){x[0]+=dx;x[0]=(x[0]>b)?b:x[0];}
	else if(k=='-'||k=='_'||k==','||k=='<'){x[0]-=dx;x[0]=(x[0]<a)?a:x[0];}
	else continue;

if(ERASEDRAW2)frame2(win2.xstr,win2.ystr);

int N=10;
int x1=(win2.rect.left+win2.rect.right)/2,y=win2.rect.bottom-N,x2;
x2=x1+50;
x1=x1-50;

HPEN pen1=CreatePen(PS_SOLID,11,BLACK),oldpen=(HPEN)SelectObject(win2.hdc,pen1);
	MoveToEx(win2.hdc,x1,y,NULL);
	LineTo(win2.hdc,x2,y);

HPEN pen2=CreatePen(PS_SOLID,9,WHITE);SelectObject(win2.hdc,pen2);
	MoveToEx(win2.hdc,x1,y,NULL);
	LineTo(win2.hdc,x2,y);

HPEN pen3=CreatePen(PS_SOLID,1,BLACK);SelectObject(win3.hdc,pen3);
	MoveToEx(win2.hdc,x1,y,NULL);
	LineTo(win2.hdc,x2,y);

HPEN pen4=CreatePen(PS_SOLID,1,RED);SelectObject(win3.hdc,pen4);
	MoveToEx(win2.hdc,x1+(int)((x[0]-a)/dx*100/n+0.5),y-5,NULL);
   	LineTo(win2.hdc,x1+(int)((x[0]-a)/dx*100/n+0.5),y+5);

SelectObject(win2.hdc,oldpen);//�ָ�ԭ���ı�.
DeleteObject(pen1);
DeleteObject(pen2);
DeleteObject(pen3);
DeleteObject(pen4);
return true;
	}	
return false;
}

BOOL keyCtr3(double x[1],double a,double b,int n)
{
	int k=0;double dx;
	if(n==0||b==a){printf("keyCtr()�����������\n");exit(1);}
	dx=(b-a)/n;
for(;k!=27;k=_getch())
	{    if(k=='+'||k=='='||k=='.'||k=='>'){x[0]+=dx;x[0]=(x[0]>b)?b:x[0];}
	else if(k=='-'||k=='_'||k==','||k=='<'){x[0]-=dx;x[0]=(x[0]<a)?a:x[0];}
	else continue;

if(ERASEDRAW2)frame3(win3.xstr,win3.ystr,win3.zstr);

int N=10;
int x1=(win3.rect.left+win3.rect.right)/2,y=win3.rect.bottom-N,x2;
x2=x1+50;
x1=x1-50;

HPEN pen1=CreatePen(PS_SOLID,11,BLACK),oldpen=(HPEN)SelectObject(win3.hdc,pen1);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);

HPEN pen2=CreatePen(PS_SOLID,9,WHITE);SelectObject(win3.hdc,pen2);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);

HPEN pen3=CreatePen(PS_SOLID,1,BLACK);SelectObject(win3.hdc,pen3);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);

HPEN pen4=CreatePen(PS_SOLID,1,RED);SelectObject(win3.hdc,pen4);
	MoveToEx(win3.hdc,x1+(int)((x[0]-a)/dx*100/n+0.5),y-5,NULL);
   	LineTo(win3.hdc,x1+(int)((x[0]-a)/dx*100/n+0.5),y+5);

SelectObject(win3.hdc,oldpen);//�ָ�ԭ���ı�.
DeleteObject(pen1);
DeleteObject(pen2);
DeleteObject(pen3);
DeleteObject(pen4);
return true;
	}	
return false;
}



//ֻ�����ͼ��
void clearvp2(){
HBRUSH oldbrush=(HBRUSH)SelectObject(win2.hdc,(HBRUSH )GetStockObject(WHITE_BRUSH));
HPEN oldpen=(HPEN)SelectObject(win2.hdc,(HPEN )GetStockObject(BLACK_PEN));
//HPEN pen3=CreatePen(PS_SOLID,1,BLACK);
//SelectObject(win2.hdc,pen3);
Rectangle(win2.hdc,win2.rect2.left-1,win2.rect2.top-1,win2.rect2.right+1,win2.rect2.bottom+1);
SelectObject(win2.hdc,oldpen);
SelectObject(win2.hdc,oldbrush);
}



void instKeyCtr(double x[1],double a,double b,int N)
{
int H=10;
int x1=(win3.rect.left+win3.rect.right)/2,y=win3.rect.bottom-H,x2;
x2=x1+50;
x1=x1-50;

HPEN pen1=CreatePen(PS_SOLID,11,BLACK),oldpen=(HPEN)SelectObject(win3.hdc,pen1);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);

HPEN pen2=CreatePen(PS_SOLID,9,WHITE);SelectObject(win3.hdc,pen2);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);


HPEN pen3=CreatePen(PS_SOLID,1,BLACK);SelectObject(win3.hdc,pen3);
	MoveToEx(win3.hdc,x1,y,NULL);
	LineTo(win3.hdc,x2,y);

int gg=(int)((*x-a)/(b-a)*100);
HPEN pen4=CreatePen(PS_SOLID,1,RED);SelectObject(win3.hdc,pen4);
	MoveToEx(win3.hdc,x1+gg,y-5,NULL);
	LineTo(win3.hdc,x1+gg,y+5);//����λ�ñ����

SelectObject(win3.hdc,oldpen);//�ָ�ԭ���ı�.
DeleteObject(pen1);
DeleteObject(pen2);
DeleteObject(pen3);
DeleteObject(pen4);
}


BOOL keyCtr(double x[1],double a,double b,int n)
{
	int k=0;double dx;
	if(n==0||b==a){printf("keyCtr()�����������\n");exit(1);}
	dx=(b-a)/n;
for(;k!=27;k=_getch())
	{    if(k=='+'||k=='='||k=='.'||k=='>'){x[0]+=dx;x[0]=(x[0]>b)?b:x[0];}
	else if(k=='-'||k=='_'||k==','||k=='<'){x[0]-=dx;x[0]=(x[0]<a)?a:x[0];}
	else continue;

return true;
	}	
return false;
}



////////////////////////////////
//////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
////////////����Ϊ������/////////////