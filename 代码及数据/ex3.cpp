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

void built_dtmf(char num){
    float hi,lo;
    int n=0;
    hi = (float)high[num];
    lo = (float)low[num];
    for (int i = 0; i < Sampling; i++)
    {
        input[i]=(float)(sin(2.0*PI*hi*i/8000)+sin(2.0*PI*lo*i/8000));
        printf("gp=%d,dp=%d\n",high[num],low[num]);
    } 
}
void goerzter(){

}

int main(){
    int i=0;
    for (i = 0; i < 10; i++)
    {
        // built_dtmf(i);
        // Beep(low[i],300);
        // for(int w=0;w<4125000;w++){}
        Beep((high[i]+low[i])/2,400);
        
        
        
    }
    printf("hello");
    return 0;
}