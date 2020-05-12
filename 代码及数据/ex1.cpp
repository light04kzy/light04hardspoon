// #include <stdafx.h>
// #include <E:\code\git\light04hardspoon\代码及数据\x_math.cpp>
// #include <E:\code\git\light04hardspoon\代码及数据\x_graph.cpp>
#include "x_math.cpp"
#include "x_graph.cpp"

 

double step(double t){
    return (t>=0)?1:0;
}

double squarewave(double t,double T,double h)
{
    double a;
    if(T<=0||h<=0){
        printf("squareWave() input error\n");exit(1);
    }
    a=fmod(t,T);
    if(a>0)return (a<=h)?1.0:0.0;
    else return (a>(-T+h))?0.0:1.0;
}

double hfunc(double a,double b){
    double fz,fm;
    static double v;//对无穷大数值的处理
    fz=a*a+b*b;
    fm=((a-1)*(a-1)+b*b)*((a-3)*(a-3)+b*b);
    if(fm==0.0)return v;
    if(fm<1.0)v=sqrt(fz/fm);
    else v=sqrt(fz*fm)/fm;
    return v;
}

double squarewave2(double t,double T[2]){
    double a;

    if(T[0]<=0||T[1]<=0)

    {printf("squareWave() input error\n");exit(1);}

    a=fmod(t,T[0]);

    if(a>0)return (a<=T[1])?1.0:0.0;

    else return (a>(-T[0]+T[1]))?0.0:1.0;
}

double hfunc2(double a,double *b){
    double fz,fm;

    static double v;//对无穷大数值的处理

    fz=a*a+*b**b;

    fm=((a-1)*(a-1)+*b**b)*((a-3)*(a-3)+*b**b);

    if(fm==0.0)return v;

    if(fm<1.0)v=sqrt(fz/fm);

    else v=sqrt(fz*fm)/fm;

    return v;
}

int main(int argc, char* argv[]){
//1、编译通过正常
//2、在此检验函数在特殊点的返回值是否正确。

    printf("step()=%f\n",step(0));

    printf("正无穷=%f\n",INF);

    printf("负无穷=%f\n",-INF);

    _getch();

    return 0;
}