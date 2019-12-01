
#include "NNUtil.h"
#include "NNLayers.h"

void NNLayer::setNext(NNLayer *n)
{
  next = n;
  n->back = this;
}

NNLayer * NNLayer::getNext()
{
  return next;
};


/*
 **** Impementation of SoftMax
 */
Matrix<float, Dynamic, Dynamic> SoftMaxLayer::forward(Matrix<float, Dynamic, Dynamic> m)
{
  float sum;
  float max = m.maxCoeff(); // Maxvalue in the matrix.

  output = m.array() - max; // Measures of overflow.

  output = output.array().exp();
  sum = output.sum();
  output = output.array() / sum;

  if (next != NULL)
    return next->forward(output);
  else
    return output;
};

Matrix<float, Dynamic, Dynamic> SoftMaxLayer::backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
{
  Matrix<float, Dynamic, Dynamic> ret = output - m;
  if (back != NULL)
    return back->backward(train_ratio, ret);
  else
    return ret;
};

float SoftMaxLayer::loss(Matrix<float, Dynamic, Dynamic> expect)
{
  return cross_entropy(output, expect);
};


/*
 **** Impementation of ReLULayer
 */
Matrix<float, Dynamic, Dynamic> ReLULayer::forward(Matrix<float, Dynamic, Dynamic> m)
{
  output = m;
  for(int i=0; i<output.cols(); i++)
    {
      output(0, i) = output(0, i) <= 0 ? 0 : output(0,i);
    }

  if (next != NULL)
    return next->forward(output);
  else
    return output;
}

Matrix<float, Dynamic, Dynamic> ReLULayer::backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
{
  Matrix<float, Dynamic, Dynamic> ret = m;
  for(int i=0; i<ret.cols(); i++)
    {
      ret(0, i) = output(0, i) <= 0 ? 0 : ret(0,i);
    }

  if (back != NULL)
    return back->backward(train_ratio, ret);
  else
    return ret;
}

void ReLULayer::resize(int rows, int cols)
{
  output = Matrix<float, 1, Dynamic>::Random(1, cols);
}

/*
 **** Impementation of AffineLayer
 */
void AffineLayer::resize(int rows, int cols)
{
  w.resize(rows, cols);
  bias.resize(1, cols);
  output.resize(1, cols);

  w      = Matrix<float, Dynamic, Dynamic>::Random(rows, cols);
  bias   = Matrix<float, 1, Dynamic>::Zero(1, cols);
  output = Matrix<float, 1, Dynamic>::Random(1, cols);
}

Matrix<float, Dynamic, Dynamic> AffineLayer::forward(Matrix<float, Dynamic, Dynamic> m)
{
  inputx = m;
  output = (m * w) - bias;

  if (next != NULL)
    {
      return next->forward(output);
    }
  else
    {
      return output;
    }
}

Matrix<float, Dynamic, Dynamic> AffineLayer::backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
{
  Matrix<float, Dynamic, Dynamic> ret;
  Matrix<float, Dynamic, Dynamic> dw;
  float db;

  ret = m * w.transpose();
  dw = inputx.transpose() * m;
  db = m.sum();

  w = w - (dw * train_ratio);
  bias = bias.array() - (db * train_ratio);

  if (back != NULL)
    return back->backward(train_ratio, ret);
  else
    return ret;
}

