#include<math.h>
#include<stdio.h>
#define SAMPLING_RATE       8000
#define MAX_BINS            8
#define GOERTZEL_N          92


int         sample_count;
double      q1[ MAX_BINS ];
double      q2[ MAX_BINS ];
double      r[ MAX_BINS ];

/*
 * coef = 2.0 * cos( (2.0 * PI * k) / (float)GOERTZEL_N)) ;
 * Where k = (int) (0.5 + ((float)GOERTZEL_N * target_freq) / SAMPLING_RATE));
 *
 * More simply: coef = 2.0 * cos( (2.0 * PI * target_freq) / SAMPLING_RATE );
 */
double      freqs[ MAX_BINS] = 
{
  697,
  770,
  852,
  941,
  1209,
  1336,
  1477,
  1633
};

double      coefs[ MAX_BINS ] ;


/*----------------------------------------------------------------------------
 *  calc_coeffs
 *----------------------------------------------------------------------------
 * This is where we calculate the correct co-efficients.
 */
void calc_coeffs()
{
  int n;

  for(n = 0; n < MAX_BINS; n++)
  {
    coefs[n] = 2.0 * cos(2.0 * 3.141592654 * freqs[n] / SAMPLING_RATE);
  }
}


/*----------------------------------------------------------------------------
 *  post_testing
 *----------------------------------------------------------------------------
 * This is where we look at the bins and decide if we have a valid signal.
 */
void post_testing()
{
  int         row, col, see_digit;
  int         peak_count, max_index;
  double      maxval, t;
  int         i;
  char *  row_col_ascii_codes[4][4] = {
    {"1", "2", "3", "A"},
    {"4", "5", "6", "B"},
    {"7", "8", "9", "C"},
    {"*", "0", "#", "D"}};


  /* Find the largest in the row group. */
  row = 0;
  maxval = 0.0;
  for ( i=0; i<4; i++ )
  {
    if ( r[i] > maxval )
    {
      maxval = r[i];
      row = i;
    }
  }

  /* Find the largest in the column group. */
  col = 4;
  maxval = 0.0;
  for ( i=4; i<8; i++ )
  {
    if ( r[i] > maxval )
    {
      maxval = r[i];
      col = i;
    }
  }


  /* Check for minimum energy */

  if ( r[row] < 4.0e5 )   /* 2.0e5 ... 1.0e8 no change */
  {
    /* energy not high enough */
  }
  else if ( r[col] < 4.0e5 )
  {
    /* energy not high enough */
  }
  else
  {
    see_digit = true;

    /* Twist check
     * CEPT => twist < 6dB
     * AT&T => forward twist < 4dB and reverse twist < 8dB
     *  -ndB < 10 log10( v1 / v2 ), where v1 < v2
     *  -4dB < 10 log10( v1 / v2 )
     *  -0.4  < log10( v1 / v2 )
     *  0.398 < v1 / v2
     *  0.398 * v2 < v1
     */
    if ( r[col] > r[row] )
    {
      /* Normal twist */
      max_index = col;
      if ( r[row] < (r[col] * 0.398) )    /* twist > 4dB, error */
        see_digit = false;
    }
    else /* if ( r[row] > r[col] ) */
    {
      /* Reverse twist */
      max_index = row;
      if ( r[col] < (r[row] * 0.158) )    /* twist > 8db, error */
        see_digit = false;
    }

    /* Signal to noise test
     * AT&T states that the noise must be 16dB down from the signal.
     * Here we count the number of signals above the threshold and
     * there ought to be only two.
     */
    if ( r[max_index] > 1.0e9 )
      t = r[max_index] * 0.158;
    else
      t = r[max_index] * 0.010;

    peak_count = 0;
    for ( i=0; i<8; i++ )
    {
      if ( r[i] > t )
        peak_count++;
    }
    if ( peak_count > 2 )
      see_digit = false;

    if ( see_digit )
    {
      printf( "%s", row_col_ascii_codes[row][col-4] );
      fflush(stdout);
    }
  }
}


/*----------------------------------------------------------------------------
 *  goertzel
 *----------------------------------------------------------------------------
 */
void goertzel( int sample )
{
  double      q0;
  int        i;

  sample_count++;
  for ( i=0; i<MAX_BINS; i++ )
  {
    q0 = coefs[i] * q1[i] - q2[i] + sample;
    q2[i] = q1[i];
    q1[i] = q0;
  }

  if (sample_count == GOERTZEL_N)
  {
    for ( i=0; i<MAX_BINS; i++ )
    {
      r[i] = (q1[i] * q1[i]) + (q2[i] * q2[i]) - (coefs[i] * q1[i] * q2[i]);
      q1[i] = 0.0;
      q2[i] = 0.0;
    }
    post_testing();
    sample_count = 0;
  }
}

int main (){
    // 导入文件
    post_testing();
    calc_coeffs();
    int m;
    goertzel(m);
}