#include <stdio.h>
#include <math.h>
#include <iostream>
#include <Eigen/Eigen>

#include "NNUtil.h"

/*
 * Function shape
 *
 * @desript Display shape information of given matrix.
 */
void shape(const char *msg, Matrix<float, Dynamic, Dynamic> in)
{
  std::cout << msg << "("  << in.rows() << "," << in.cols() << ")" << std::endl;
}

/*
 * Function loge()
 *
 * @desript Calculate logf() for each items in given matrix.
 */
Matrix<float, Dynamic, Dynamic> loge(Matrix<float, Dynamic, Dynamic> in)
{
  Matrix<float, Dynamic, Dynamic> out = in;

  out = out.array() + 1e-7f;

  for(int i=0; i<in.cols(); i++)
    {
      out(0, i) = logf(out(0,i));
    }

  return out;
}

/*
 * Function cross_entropy()
 *
 * @desript Calculate Cross Entropy Loss value from 2 input matrix.
 */
float cross_entropy(Matrix<float, Dynamic, Dynamic> output, Matrix<float, Dynamic, Dynamic> expect)
{
  Matrix<float, Dynamic, Dynamic> tmp;
  float out = 0.f;
  tmp = expect.array() * loge(output).array();
  out = tmp.sum();
  return -out;
};


