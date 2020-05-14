#include<windows.h>
#define turn 54

#define A 1600
#define B 800
#define C 400 //一拍
#define D 200
#define E 100
#define F 50
//低音
#define L1 262
#define L2 294
#define L3 330
#define L4 349
#define L5 392
#define L6 440
#define L7 493
//高音
#define N1 532
#define N2 588
#define N3 660
#define N4 698
#define N5 784
#define N6 880
#define N7 988
//半弦音
#define H1 1046
#define H2 1175
#define H3 1319
#define H4 1397
#define H5 1568
#define H6 1760
#define H7 1976

void sung() {
	Beep(N1, D); Beep(N5, D); Beep(H3, D); Beep(N1, D); Beep(N5, C); Beep(0, C);
	Beep(N2 + turn, D); Beep(N4, D); Beep(H1, D); Beep(N2, D); Beep(N4, C); Beep(0, C);
	Beep(L5, D); Beep(L7, D); Beep(N4, D); Beep(N7, D); Beep(L7, C); Beep(0, C);
	Beep(N1, D); Beep(N2, E); Beep(N3, E); Beep(H1, D); Beep(N1, D); Beep(N3, C); Beep(0, C);

	Beep(0, D); Beep(N3, D); Beep(N3, D); Beep(N5, D); Beep(N5, D); Beep(H1, D); Beep(H1, D); Beep(N7, D);
	Beep(N7, D); Beep(N6, D); Beep(N3, D); Beep(N6, D + C); Beep(0, C);

	Beep(0, D); Beep(N6, D); Beep(N6, D); Beep(N7, D); Beep(N7, D); Beep(H3, D); Beep(H3, D); Beep(N7, D);
	Beep(N7, D); Beep(N5, D); Beep(N3, D); Beep(N5, D + C); Beep(0, C);

	Beep(0, D); Beep(N3, D); Beep(N3, D); Beep(N5, D); Beep(N5, D); Beep(H1, D); Beep(H1, D); Beep(N7, D);
	Beep(N7, D); Beep(N6, D); Beep(N3, D); Beep(N6, D + C); Beep(N6, D); Beep(N7, D);

	Beep(N7 + turn, D); Beep(N6, D); Beep(N7, D); Beep(H3, C + C); Beep(H2, D); Beep(H1, D + D + C); Beep(0, C * 3);
	//2
	Beep(0, D); Beep(N3, D); Beep(N3, D); Beep(N5, D); Beep(N5, D); Beep(H1, D); Beep(H1, D); Beep(N7, D);
	Beep(N7, D); Beep(N6, D); Beep(N3, D); Beep(N6, D + C); Beep(0, C);

	Beep(0, D); Beep(N6, D); Beep(N6, D); Beep(N7, D); Beep(N7, D); Beep(H3, D); Beep(H3, D); Beep(N7, D);
	Beep(N7, D); Beep(N3, D); Beep(N5, D + C); Beep(0, C);

	Beep(0, D); Beep(N3, D); Beep(N3, D); Beep(N5, D); Beep(N5, D); Beep(H1, D); Beep(H1, D); Beep(N7, D);
	Beep(N7, D); Beep(H1, D); Beep(N3, D); Beep(N6, D); Beep(0, D); Beep(N6, D); Beep(H1, D); Beep(N7, D);

	Beep(N7 + turn, D); Beep(N6, D); Beep(N7, D); Beep(H3, C + C); Beep(H2, D); Beep(H1, D + D + C); Beep(0, D);
	Beep(H3, D); Beep(H2, E); Beep(H1, E + D); Beep(N7, D);
	Beep(N6, D); Beep(N6, D); Beep(N6, D); Beep(N6, D); Beep(N6, D); Beep(H3, D); Beep(H2, D); Beep(H2, D * 3);
	Beep(0, C);
}

int main()
{
	sung();
    return 0;
}
