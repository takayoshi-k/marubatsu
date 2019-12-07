#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define INPUTNO   (2)     /* 入力層のセル数 */
#define HIDDENNO  (2)     /* 中間層のセル数 */
#define ALPHA     (1)     /* 学習係数 */
#define MAXNO     (100)   /* 学習データの最大個数 */
#define BIGNUM    (100)   /* 誤差の初期値 */
#define LIMIT     (0.001) /* 誤差の上限値 */
#define RANDOM_SEED      (65535) /* 乱数のシード */

double random_pm_one(void)
{
  double rndno;

  /* 0から1の間の乱数を生成 */
  while( (rndno = (double)rand() / RAND_MAX) == 1.0 ) ;

  /* -1から1の間の乱数を生成 */
  rndno = rndno * 2 - 1;

  return rndno;
}

double sigmoid(double u)
{
  return 1.0/(1.0+exp(-u)) ;
}

/*
 * input[0] ----> w[0][0] ----> 
 * input[1] ----> w[0][1] ---->   b[0] --> hi[0]
 *
 * input[0] ----> w[1][0] ----> 
 * input[1] ----> w[1][1] ---->   b[1] --> hi[1]
 */

struct mid_layer {
  double w[HIDDENNO][INPUTNO];
  double b[HIDDENNO];
  double o[HIDDENNO];
};

int initmidlayer(struct mid_layer *mid)
{
  int i, j;

  for(i=0; i<HIDDENNO; i++)
    {
      for(j=0; j<INPUTNO; j++)
        {
          mid->w[i][j] = random_pm_one();
        }
      mid->b[i] = random_pm_one();
    }
}

struct out_layer {
  double w[HIDDENNO];
  double b;
};


int initoutlayer(struct out_layer *out)
{
  int i;

  for(i=0; i<HIDDENNO; i++)
    {
      out->w[i] = random_pm_one();
    }

  out->b = random_pm_one();
}


void printweight(struct mid_layer *mid, struct out_layer *out)
{
  int i, j;

  for( i=0; i<HIDDENNO; i++ )
    {
      for( j=0; j<INPUTNO; j++ )
        {
          printf("%lf ", mid->w[i][j]);
        }
      printf("%lf(b)\n", mid->b[i]);
    }

  for( i=0; i<HIDDENNO; i++ )
    {
      printf("%lf ", out->w[i]);
    }
  printf("%lf(b)\n", out->b);
}


struct lerning_data {
  double input[INPUTNO];
  double expect;
};

union u_lerning_data{
  struct lerning_data data;
  double input[INPUTNO+1];
};

int get_lerning_data(union u_lerning_data *data)
{
  int data_num = 0;

  int record_num = 0;
  while( scanf("%lf", &data[data_num].input[record_num]) != EOF )
    {
      record_num++;
      if ( record_num > INPUTNO )
        {
          record_num = 0;
          data_num++;
          if ( record_num >= MAXNO )
            {
              fprintf(stderr, "Out of input data number.\n");
              break;
            }
        }
    }

  return data_num;
}


double forward(struct lerning_data *input, struct mid_layer *mid, struct out_layer *out)
{
  int i, j;
  double tmp;

  for (i=0; i<HIDDENNO; i++)
    {
      tmp = 0;
      for (j=0; j<INPUTNO; j++)
        {
          tmp += input->input[j] * mid->w[i][j];
        }
      tmp -= mid->b[i];
      mid->o[i] = sigmoid(tmp);
    }

  tmp = 0;
  for (i=0; i<HIDDENNO; i++)
    {
      tmp += mid->o[i] * out->w[i];
    }
  tmp -= out->b;

  return sigmoid(tmp);
}


void lerning(struct lerning_data *input, struct mid_layer *mid, struct out_layer *out, double result)
{
  int i, j;
  double delta;
  double mid_delta;

  // out layer update;
  delta = (input->expect - result) * result * (1-result);

  for (i=0; i<HIDDENNO; i++)
    {
      out->w[i] += ALPHA * mid->o[i] * delta;
    }
  out->b += ALPHA * (-1.0) * delta;


  // Mid layer update;
  for (i=0; i<HIDDENNO; i++)
    {
      mid_delta = mid->o[i] * (1.0 - mid->o[i]) * out->w[i] * delta;

      for (j=0; j<INPUTNO; j++)
        {
          mid->w[i][j] += ALPHA * input->input[j] * mid_delta;
        }
      mid->b[i] += ALPHA * (-1.0) * mid_delta;
    }
}


int main(void)
{
  struct mid_layer mid;
  struct out_layer out;
  struct lerning_data input[MAXNO];

  int i, j;
  int data_idx;
  int lerning_data_num;
  int lerning_count = 0;

  double output;

  double err = BIGNUM; // 誤差値

  srand(RANDOM_SEED);

  initmidlayer(&mid);
  initoutlayer(&out);
  printweight(&mid, &out);

  lerning_data_num = get_lerning_data((union u_lerning_data *) &input[0]) ;
  printf("学習データの個数:%d\n",lerning_data_num) ;


  while( err > LIMIT )
    {
      err = 0.0;
      for (data_idx = 0; data_idx < lerning_data_num; data_idx++)
        {
          output = forward(&input[data_idx], &mid, &out);

          lerning(&input[data_idx], &mid, &out, output);

          // err += pow( (output - input[data_idx].expect), 2.0 );
          err += (output - input[data_idx].expect) * (output - input[data_idx].expect);
        }
      lerning_count++;

      printf("%d\t%lf\n", lerning_count, err);
    }

  printweight(&mid, &out);

  for (i=0; i<lerning_data_num; i++)
    {
      printf("%d ", i);
      for (j=0; j<INPUTNO; j++)
        {
          printf("%lf ", input[i].input[j]);
        }
      printf(" = expect(%lf), ", input[i].expect);
      printf("result(%lf)\n", forward(&input[i], &mid, &out));
    }

  return 0;
}

