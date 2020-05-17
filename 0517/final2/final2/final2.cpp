// final2.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdio.h"
#include "math.h"
#include "conio.h"
#include "string.h"
#include "windows.h"
#define PI 3.1415926
#define Sampling 1000//�������� 

int high[12]={1336,1209,1336,1477,1209,1336,1477,1209,1336,1477,1209,1477};
int low[12]={941,697,697,697,770,770,770,852,852,852,941,941};  
double input[Sampling];

int Q=0;    //ѭ�����ӣ�Ŀ������һ��ͼ�ϻ����Ƶ��
int Qm=0;   //������ٸ����ͻ�����Ƶ��


char jianru(void);			//�����ֵ
char Goertzel(void);		//���ø�����㷨ȷ���������֣�����Ƶ��
void DTMF(char number);		//����DTMF�ź�

int main()
{char jian=0;
 int  i;
 int  en=0;
 int  time=0;	    	//�������λ��
 int  number=0;      	//����ĺ���
 char key[50]={0},ok[50]={0};

	printf("Please input how much numbers:");  
	for(i=0;en==0;i++)		//Ҫ�������ֵ�λ��
	{jian=getch();
	 printf("%c",jian);
	 if(jian>47&&jian<58)time=time*10+jian-48; 	//ASCIIתʮ����
	 else if(jian==13){en=1;printf("\n");
	 printf("You have %d numbers\n",time);}
	 }
	Qm=time;					//ȷ����ͼѭ������
	for(i=0;i<time;i++)
    {key[i]=jianru();		//��ȡ��ֵ��ASCII�룬��ת���ɶ�Ӧ�����ֺͷ���
     DTMF(key[i]);			//������Ӧ�ߵ�Ƶ������ӦDTMF��
     ok[i]=Goertzel();      //��������㣬��ߵ�Ƶ�ʣ�ȷ����Ӧ�����룬����ͼ
     Q++;
     }
    printf("\nThe numbers are:\n\t\t ");
    for(i=0;i<time;i++)printf("%d",key[i]);
    printf("\n");
    return 0;
}


//Goertzel�㷨
char Goertzel(void)
{
    int n,k,k1=0,k2=0,loop;		//k1��k2��������������Ƶ�ʡ�
    double Re[N]={0},In[N]={0},output[N]={0},max=0,cimax=0,f1=0,f2=0,temp;     //max��cimaxΪ��ֵ���ʹ�֮Ƶ�ʵ�ֵ
    char ok=0,tu[30];
    FILE *fp;
    unsigned char header[1078];	//ͼ��ͷ�ļ������ص�
    unsigned char pixel[200][3000];
    int i,j;					//���ص�ĸߡ���
    int m=0;					//��һ����Ƶ�ʣ���k��ֵ
    double l;               	//��һ�����ӣ���ͼʱ�������޵�����ռ���ʾͼ

    strcpy(tu,"d:\\DTMF\\outputx.txt");
    for(k=0;k<N;k++)
    {for(n=0;n<N;n++)	//ʹ��ŷ����ʽ����ָ����Ϊ���Ǻ������ٷֱ����ʵ�����鲿��
        {Re[k]+=input[n]*cos(2*PI*n*k/N);
         In[k]+=input[n]*sin(2*PI*n*k/N);}
         output[k]=sqrt(Re[k]*Re[k]+In[k]*In[k]);    
		//��������ף��ɷ�����ȷ����������Ƶ��
        if(k<(N/2))     //ֻ����ǰ�벿�֣��ҵ�һ�����ֵƵ��
		if(output[k]>max)
          {k1=k; 
           max=output[k];
           }
    }

    for(k=0;k<(N/2);k++) //�ҵڶ������ֵƵ��
    	{if(output[k]!=max)
            if(output[k]>cimax)
            {	k2=k;
                cimax=output[k];}
    }
//������Ƶ��ת��Ϊʵ��Ƶ��
    f1=((double)k1)*8000.0/((double)N);
    f2=((double)k2)*8000.0/((double)N); 
    if(f1>f2)
        {   temp=f1;
            f1=f2;
            f2=temp;}
//printf("\n%f\t%f\t%d\t%d\t%f\t%f\n",max,nmax,k1,k2,f1,f2);

    for(k=0;k<12;k++) //���շ���Ƶ�׽��
    {if(fabs(f1-(double)low[k])<20.0&&fabs(f2-(double)high[k])<20.0)
        {if(k<10) ok=k+48;
         else if(k==10) ok=42;
	     else if(k==11) ok=35;
         break;
        }
    }

//���������Ҫ��ͼ���ļ��ȴ洢����ͼʱ�ٶ�ȡ��
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
//��������ļ�ֵ��ASCII���ȡ��Ӧ���ļ����л�ͼ
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
{output[i]=(int)(output[i]*l);}   //��һ����ʾ

//��ͼ�ϻ���������ֵ�Ƶ��  
for(i=Q*300+0,m=0;i<Q*300+256&&m<256&&Q<Qm;i++,m++) 
	{for(j=5;j<5+output[m];j++) pixel[j][i]=0; }

    if((fp=fopen("d:\\DTMF\\pp.bmp","wb"))==NULL){
    printf("\nsorry,can not open 123.bmp\n");}
    fwrite(&header,1,1078,fp);
    fwrite(&pixel,1,600000,fp);
    fclose(fp);
    return ok;
}


//��ÿ������ĺ���
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
		Beep(low[key-48],300);//����ÿ������ʱ����ϵͳ����Ƶ���Ƶ����
//Beep��������Ҫ���������ǰ��ı���Ƶ�ʵ�����������ı����������ʱ�����ڵ�Ƶ�ź������Ƚϵͳ�������������ʱ��϶̼���
		for(w=0;w<4125000;w++){}//�˴���Ϊ��ʱ������������ӳٹ�����������һ������
		Beep(high[key-48],400);//���ڸ�Ƶ������Ϊ����Ϊ������������Ϣ����Ҫ��ʱ���賤һ�㡣
	}
    while(en);
    return number;
}

//����DTMF�ź�
void DTMF(char number)
{   float f1,f2;
    int i;
    f1=(float)low[number];
    f2=(float)high[number];
    for(i=0;i<Sampling;i++)
        input[i]=(float)(sin(2.0*PI*f1*i/8000)+sin(2.0*PI*f2*i/8000));
	printf("gp=%d,dp=%d\n",high[number],low[number]);	
//����ÿ������ʱ��������ĵ�Ƶ���Ƶ
    return;
}
