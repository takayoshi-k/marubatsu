#include <stdio.h>
#include <math.h>
#include <iostream>
#include <Eigen/Eigen>

using namespace Eigen;


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


/*
 * Class NNLayer
 *
 * @desript Base class of Neural Network Layer.
 */
class NNLayer {
  protected:
    NNLayer *next;
    NNLayer *back;

  public:
    NNLayer() : next(NULL), back(NULL)
      {
      };

    virtual ~NNLayer() {};

    virtual Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> m) = 0;
    virtual Matrix<float, Dynamic, Dynamic> backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m) = 0;
    virtual void resize(int rows, int cols) = 0;

    void setNext(NNLayer *n)
      {
        next = n;
        n->back = this;
      };

    NNLayer * getNext()
      {
        return next;
      };

};


/*
 * Class LastActivation
 *
 * @desript Base class of Final Activation Layer. This has loss() method function.
 */
class LastActivation : public NNLayer {
  public:
    LastActivation() : NNLayer()
      {
      };
    virtual ~LastActivation(){};

    virtual float loss(Matrix<float, Dynamic, Dynamic> expect) = 0;
};


/*
 * Class SoftMaxLayer
 *
 * @desript A kind of LastActivation Layer.
 */
class SoftMaxLayer : public LastActivation {
  Matrix<float, 1, Dynamic> output;
  public:
    SoftMaxLayer() : LastActivation()
      {
      };

    ~SoftMaxLayer() {};

    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> m)
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

    Matrix<float, Dynamic, Dynamic> backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
      {
        Matrix<float, Dynamic, Dynamic> ret = output - m;
        if (back != NULL)
          return back->backward(train_ratio, ret);
        else
          return ret;
      };

    float loss(Matrix<float, Dynamic, Dynamic> expect)
      {
        return cross_entropy(output, expect);
      };

    void resize(int rows, int cols)
      {
      };
};


/*
 * Class ReLULayer
 *
 * @desript A kind of Activation Layer.
 */
class ReLULayer : public NNLayer {
  Matrix<float, 1, Dynamic> output;
  public:
    ReLULayer() : NNLayer()
      {
      };

    ~ReLULayer() {}

    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> m)
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
      };

    Matrix<float, Dynamic, Dynamic> backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
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
      };

    void resize(int rows, int cols)
      {
        output = Matrix<float, 1, Dynamic>::Random(1, cols);
      };
};


/*
 * Class AffineLayer
 *
 * @desript A kind of Neural Network Layer.
 */
class AffineLayer : public NNLayer {
  Matrix<float, Dynamic, Dynamic> w;
  Matrix<float, 1, Dynamic> bias;
  Matrix<float, 1, Dynamic> output;
  Matrix<float, 1, Dynamic> inputx;

  public:
    AffineLayer() : NNLayer()
      {
      }

    AffineLayer(int rows, int cols) : NNLayer()
      {
        resize(rows, cols);
      }

    ~AffineLayer() {};

    void resize(int rows, int cols)
      {
        w.resize(rows, cols);
        bias.resize(1, cols);
        output.resize(1, cols);

        w      = Matrix<float, Dynamic, Dynamic>::Random(rows, cols);
        bias   = Matrix<float, 1, Dynamic>::Random(1, cols);
        output = Matrix<float, 1, Dynamic>::Random(1, cols);
      }

    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> m)
      {
        inputx = m;
        output = (m * w) + bias;

        if (next != NULL)
          {
            return next->forward(output);
          }
        else
          {
            return output;
          }
      }

    Matrix<float, Dynamic, Dynamic> backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
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
      };
};


/*
 * Class AffineLayer
 *
 * @desript A kind of Neural Network Layer.
 */
class NeuralNetwork {
  NNLayer *top_layer;
  NNLayer *last_layer;
  Matrix<float, Dynamic, Dynamic> output;

  public:
    NeuralNetwork() : top_layer(NULL), last_layer(NULL)
      {
      };
    ~NeuralNetwork()
      {
        NNLayer *tmp;
        for(NNLayer *layer = top_layer; layer != NULL; )
          {
            tmp = layer->getNext();
            delete layer;
            layer = tmp;
          }
      };

    void createNewLayer(int innum, int outnum, bool is_last=false)
      {
        NNLayer *new_affine;
        NNLayer *activation;

        new_affine = new AffineLayer(innum, outnum);

        if (is_last)
          {
            activation = new SoftMaxLayer();
          }
        else
          {
            activation = new ReLULayer();
          }

        new_affine->setNext(activation);

        if( last_layer == NULL )
          {
            top_layer = new_affine;
          }
        else
          {
            last_layer->setNext(new_affine);
          }

        last_layer = activation;
      };

    void print_layers()
      {
        for(NNLayer *layer = top_layer; layer != NULL; layer = layer->getNext())
          {
            std::cout << layer << " --> ";
          };
        std::cout << "NULL" << std::endl;
      };

    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> input)
      {
        output = top_layer->forward(input);
        return output;
      };

    float backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
      {
        float ret = ((LastActivation *)last_layer)->loss(m);
        last_layer->backward(train_ratio, m);
        return ret;
      };

};


int main(void)
{
  NeuralNetwork nn;

  float loss;
  Matrix<float, Dynamic, Dynamic> out;

#if 1
  Matrix<float, 1, 2> input[4];
  Matrix<float, 1, 1> expect[4];

  input[0] << 0, 0; expect[0] << 0;
  input[1] << 0, 1; expect[1] << 1;
  input[2] << 1, 0; expect[2] << 1;
  input[3] << 1, 1; expect[3] << 0;

  nn.createNewLayer(2,2);
  nn.createNewLayer(2,1, true);

  loss = 100.f;
  while (loss > 0.001f)
    {
      loss = 0.f;
      for(int i=0; i<4; i++)
        {
          // std::cout << "==== " << i+1 << " ===="  << std::endl;
          out = nn.forward( input[i] );
          loss += nn.backward(0.1f, expect[i]);
          // std::cout << "   out = " << out << std::endl;
        }
      loss /= 4;
      std::cout << "loss = " << loss << std::endl;
    }

  for(int i=0; i<4; i++)
    {
      std::cout << "Expect = " << expect[i] << " Input  = " << input[i] << std::endl;
      out = nn.forward( input[i] );
      std::cout << "Output = " << out << std::endl;
    }

#else
  Matrix<float, 1, 2> input;
  Matrix<float, 1, 2> expect;

  input << 3, 3;
  expect << 0, 1;

  nn.createNewLayer(2, 5);
  nn.createNewLayer(5, 3);
  nn.createNewLayer(3, 2, true);

  std::cout << "tmp input = " << input << std::endl;
  std::cout << "expect = " << expect << std::endl;
  std::cout << "input * expect = " << input.array() * expect.array() << std::endl;

  std::cout << "input = " << input << std::endl;
  input = input.array() + 1;
  std::cout << "input = " << input << std::endl;
  

  out = nn.forward( input );
  std::cout << "out.rows = " << out.rows() << std::endl;
  std::cout << "out.cols = " << out.cols() << std::endl;
  std::cout << "out = " << out << std::endl;

  loss = 100.f;
  while (loss > 0.001f)
    {
      out = nn.forward( input );
      loss = nn.backward(0.1f, expect);
      std::cout << "out = " << out << std::endl;
      std::cout << "loss = " << loss << std::endl;
    }
#endif

  return 0;
}

