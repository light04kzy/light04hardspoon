// final2.cpp : 定义控制台应用程序的入口点。
//
#include "stdio.h"
#include "math.h"
#include "conio.h"
#include "string.h"
#include "windows.h"
#define PI 3.1415926
#define Sampling 1000//采样点数 

int high[12]={1336,1209,1336,1477,1209,1336,1477,1209,1336,1477,1209,1477};
int low[12]={941,697,697,697,770,770,770,852,852,852,941,941};  
double input[Sampling];

int Q=0;    //循环因子，目的是在一张图上画多个频谱
int Qm=0;   //输入多少个数就画几个频谱


char jianru(void);			//键入键值
char Goertzel(void);		//利用戈泽尔算法确定输入数字，并画频谱
void DTMF(char number);		//构造DTMF信号

int main()
{char jian=0;
 int  i;
 int  en=0;
 int  time=0;	    	//输入号码位数
 int  number=0;      	//输入的号码
 char key[50]={0},ok[50]={0};

	printf("Please input how much numbers:");  
	for(i=0;en==0;i++)		//要输入数字的位数
	{jian=getch();
	 printf("%c",jian);
	 if(jian>47&&jian<58)time=time*10+jian-48; 	//ASCII转十进制
	 else if(jian==13){en=1;printf("\n");
	 printf("You have %d numbers\n",time);}
	 }
	Qm=time;					//确定画图循环次数
	for(i=0;i<time;i++)
    {key[i]=jianru();		//读取键值的ASCII码，并转换成对应的数字和符号
     DTMF(key[i]);			//根据相应高低频生成相应DTMF波
     ok[i]=Goertzel();      //戈泽尔运算，求高低频率，确定对应的输入，并画图
     Q++;
     }
    printf("\nThe numbers are:\n\t\t ");
    for(i=0;i<time;i++)printf("%d",key[i]);
    printf("\n");
    return 0;
}


//Goertzel算法
char Goertzel(void)
{
    int n,k,k1=0,k2=0,loop;		//k1、k2是两个幅度最大的频率。
    double Re[N]={0},In[N]={0},output[N]={0},max=0,cimax=0,f1=0,f2=0,temp;     //max和cimax为幅值最大和次之频率的值
    char ok=0,tu[30];
    FILE *fp;
    unsigned char header[1078];	//图像头文件和像素点
    unsigned char pixel[200][3000];
    int i,j;					//像素点的高、宽
    int m=0;					//归一化的频率，即k的值
    double l;               	//归一化因子，作图时能在有限的纵向空间显示图

    strcpy(tu,"d:\\DTMF\\outputx.txt");
    for(k=0;k<N;k++)
    {for(n=0;n<N;n++)	//使用欧拉公式将复指数化为三角函数，再分别计算实部和虚部。
        {Re[k]+=input[n]*cos(2*PI*n*k/N);
         In[k]+=input[n]*sin(2*PI*n*k/N);}
         output[k]=sqrt(Re[k]*Re[k]+In[k]*In[k]);    
		//计算幅度谱，由幅度谱确定功率最大的频点
        if(k<(N/2))     //只计算前半部分，找第一个最大值频点
		if(output[k]>max)
          {k1=k; 
           max=output[k];
           }
    }

    for(k=0;k<(N/2);k++) //找第二个最大值频点
    	{if(output[k]!=max)
            if(output[k]>cimax)
            {	k2=k;
                cimax=output[k];}
    }
//将采样频点转化为实际频率
    f1=((double)k1)*8000.0/((double)N);
    f2=((double)k2)*8000.0/((double)N); 
    if(f1>f2)
        {   temp=f1;
            f1=f2;
            f2=temp;}
//printf("\n%f\t%f\t%d\t%d\t%f\t%f\n",max,nmax,k1,k2,f1,f2);

    for(k=0;k<12;k++) //最终分析频谱结果
    {if(fabs(f1-(double)low[k])<20.0&&fabs(f2-(double)high[k])<20.0)
        {if(k<10) ok=k+48;
         else if(k==10) ok=42;
	     else if(k==11) ok=35;
         break;
        }
    }

//将计算后需要画图的文件先存储，画图时再读取。
    tu[14]=ok;
    if((fp=fopen(tu,"wb"))==NULL)printf("\nsorry,can not open output.dat\n");
    fwrite(&output,sizeof(double),N,fp);
    fwrite(&max,sizeof(double),1,fp);
    fwrite(&cimax,sizeof(double),1,fp);
    fclose(fp);

    if((fp=fopen("d:\\DTMF\\pp.bmp","rb"))==NULL)
    {printf("\nsorry,can not open 123.bmp\n");}
    fread(&header,1,1078,fp);
    fread(&pixel,1,600000,fp);
    fclose(fp);
//根据输入的键值的ASCII码读取相应的文件进行画图
if(k==48)fp=fopen("d:\\DTMF\\output0.txt","rb");
if(k==49)fp=fopen("d:\\DTMF\\output1.txt","rb");
if(k==50)fp=fopen("d:\\DTMF\\output2.txt","rb");
if(k==51)fp=fopen("d:\\DTMF\\output3.txt","rb");
if(k==52)fp=fopen("d:\\DTMF\\output4.txt","rb");
if(k==53)fp=fopen("d:\\DTMF\\output5.txt","rb");
if(k==54)fp=fopen("d:\\DTMF\\output6.txt","rb");
if(k==55)fp=fopen("d:\\DTMF\\output7.txt","rb");
if(k==56)fp=fopen("d:\\DTMF\\output8.txt","rb");
if(k==57)fp=fopen("d:\\DTMF\\output9.txt","rb");
if(k==42)fp=fopen("d:\\DTMF\\output#.txt","rb");
if(k==35)fp=fopen("d:\\DTMF\\output*.txt","rb");

    if((fp)==NULL)printf("\nsorry,can not open output.txt\n");
    fread(&output,sizeof(double),N,fp);
    fread(&max,sizeof(double),1,fp);
    fread(&cimax,sizeof(double),1,fp);
    fclose(fp);

    if(cimax>max)
        max=cimax;
	l=190.000/max;
    for(i=0;i<N;i++)
{output[i]=(int)(output[i]*l);}   //归一化显示

//在图上画出多个数字的频谱  
for(i=Q*300+0,m=0;i<Q*300+256&&m<256&&Q<Qm;i++,m++) 
	{for(j=5;j<5+output[m];j++) pixel[j][i]=0; }

    if((fp=fopen("d:\\DTMF\\pp.bmp","wb"))==NULL){
    printf("\nsorry,can not open 123.bmp\n");}
    fwrite(&header,1,1078,fp);
    fwrite(&pixel,1,600000,fp);
    fclose(fp);
    return ok;
}


//读每次输入的号码
char jianru()
{   long w;
    char key,en=1;
    int  number;
    do{ key=getch();
        printf("%c",key);
        if(key>47&&key<58)
        {   en=0;number=key-48;}
        else if(key==42)
        {	en=0; number=10;}
        else if(key==35)
        {	en=0; number=11;}
		else printf("input error!\n");
		Beep(low[key-48],300);//输入每个数字时候由系统发高频与低频声音
//Beep函数的主要功能是输出前面的变量频率的声音，后面的变量是输出的时间由于低频信号声音比较低沉，故声音发出时间较短即可
		for(w=0;w<4125000;w++){}//此处作为延时，低音输出后，延迟过后可以输出下一个声音
		Beep(high[key-48],400);//由于高频声音较为尖锐，为了区分声音信息，需要把时间设长一点。
	}
    while(en);
    return number;
}

//构造DTMF信号
void DTMF(char number)
{   float f1,f2;
    int i;
    f1=(float)low[number];
    f2=(float)high[number];
    for(i=0;i<Sampling;i++)
        input[i]=(float)(sin(2.0*PI*f1*i/8000)+sin(2.0*PI*f2*i/8000));
	printf("gp=%d,dp=%d\n",high[number],low[number]);	
//输入每个数字时候输出他的低频与高频
    return;
}
