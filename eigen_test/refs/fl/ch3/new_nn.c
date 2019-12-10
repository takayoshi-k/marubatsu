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

struct nn_layer {
  int inputs;
  int outputs;
  double *W;
  double *bias;
  double *out;
  double *delta;
};

#define FOR_OUTPUTS(layer, i) for ((i)=0; (i)<layer->outputs; (i)++)
#define FOR_INPUTS(layer, i) for ((i)=0; (i)<layer->inputs; (i)++)
#define LAYER_W(layer, ins, outs) layer->W[outs*(layer->inputs) + ins]

struct nn_layer * init_nn_layer(int in_neurons, int out_neurons)
{
  int i, j;
  struct nn_layer *ret = (struct nn_layer *)malloc(sizeof(struct nn_layer));

  if (ret != NULL)
    {
      ret->inputs = in_neurons;
      ret->outputs = out_neurons;

      ret->W    = (double *)malloc(sizeof(double) * in_neurons * out_neurons);
      if (ret->W == NULL)
        {
          free(ret);
          return NULL;
        }

      ret->bias = (double *)malloc(sizeof(double) * out_neurons);
      if (ret->bias == NULL)
        {
          free(ret->W);
          free(ret);
          return NULL;
        }

      ret->out  = (double *)malloc(sizeof(double) * out_neurons);
      if (ret->out == NULL)
        {
          free(ret->bias);
          free(ret->W);
          free(ret);
          return NULL;
        }

      ret->delta  = (double *)malloc(sizeof(double) * out_neurons);
      if (ret->out == NULL)
        {
          free(ret->out);
          free(ret->bias);
          free(ret->W);
          free(ret);
          return NULL;
        }

      // Initialize random values
      FOR_OUTPUTS(ret, i)
        {
          FOR_INPUTS(ret, j)
            {
              LAYER_W(ret, j, i) = random_pm_one();
            }
          ret->bias[i] = random_pm_one();
        }
    }

  return ret;
}


void init_input(struct nn_layer *input, double *in_data)
{
  int out_idx;
  FOR_OUTPUTS(input, out_idx)
    {
      input->out[out_idx] = in_data[out_idx];
    }
}


void free_nn_layer(struct nn_layer *layer)
{
  free(layer->W);
  free(layer->bias);
  free(layer->out);
  free(layer->delta);
  free(layer);
}


struct nn_layer *forward_nn_layer(struct nn_layer *layer, struct nn_layer *input)
{
  int out_idx, in_idx;

  FOR_OUTPUTS(layer, out_idx)
    {
      layer->out[out_idx] = 0;
      FOR_INPUTS(layer, in_idx)
        {
          layer->out[out_idx] += input->out[in_idx] * LAYER_W(layer, in_idx, out_idx);
        }
      layer->out[out_idx] = sigmoid( layer->out[out_idx] - layer->bias[out_idx] );
    }

  return layer;
}

struct nn_layer *learning_nn_layer( struct nn_layer *layer,
                                    struct nn_layer *input,
                                    struct nn_layer *expect)
{
}


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

/*

delta = last_delta * out * (1-out)

current_w += sum(alpha * delta * back_o)
 
mid_delta = delta * (current_o * (1-current_o)) * forward_w;

mid_w += sum(alpha * mid_delta * back_o)

delta = last_delta * (out * (1-out))
mid_delta = delta * (current_o * (1-current_o)) * forward_w;

*/

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

void sync_params(struct mid_layer *mid, struct out_layer *out, struct nn_layer *mid_nn, struct nn_layer *out_nn)
{
  int i, j;

  // Sync mid
  for (i=0; i<HIDDENNO; i++)
    {
      for (j=0; j<INPUTNO; j++)
        {
          LAYER_W(mid_nn, j, i) = mid->w[i][j];
        }
      mid_nn->bias[i] = mid->b[i];
    }

  // Sync out
  for (i=0; i<HIDDENNO; i++)
    {
      LAYER_W(out_nn, i, 0) = out->w[i];
    }
  out_nn->bias[0] = out->b;
}


void print_nn(struct nn_layer *nn)
{
  int in_idx, out_idx;

  printf("NN(%d,%d) ", nn->inputs, nn->outputs);
  FOR_OUTPUTS(nn, out_idx)
    {
      FOR_INPUTS(nn, in_idx)
        {
          printf("%lf ", LAYER_W(nn, in_idx, out_idx));
        }
      printf("%lf(b)\n", nn->bias[out_idx]);
    }
}


int test_nn_layer(struct lerning_data *input, int num, struct mid_layer *mid, struct out_layer *out)
{
  int i;

  struct nn_layer **input_layer;
  struct nn_layer *mid_nn_layer;
  struct nn_layer *out_nn_layer;

  input_layer = (struct nn_layer**)malloc(sizeof(struct nn_layer*) * num);

  for(i=0; i<num; i++)
    {
      input_layer[i] = init_nn_layer(1, 2);
      init_input(input_layer[i], input[i].input);
    }

  mid_nn_layer = init_nn_layer(2, 2);
  out_nn_layer = init_nn_layer(2, 1);

  sync_params(mid, out, mid_nn_layer, out_nn_layer);
  printf("Synced parameters....\n");
  printweight(mid, out);
  print_nn(mid_nn_layer);
  print_nn(out_nn_layer);


  // for(i=0; i<num; i++)
  for(i=0; i<1; i++)
    {
      forward_nn_layer( out_nn_layer,
                        forward_nn_layer( mid_nn_layer, input_layer[i] )
                      );
    }
  printf("\nout_nn_layer = %lf\n", out_nn_layer->out[0]);

  free_nn_layer(mid_nn_layer);
  free_nn_layer(out_nn_layer);
  free(input_layer);
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

