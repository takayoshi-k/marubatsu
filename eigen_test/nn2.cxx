#include <stdio.h>
#include <math.h>
#include <iostream>
#include <Eigen/Eigen>

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> loge(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> in)
{
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> out = in;

  out = out.array() + 1e-7f;

  for(int i=0; i<in.cols(); i++)
    {
      out(0, i) = logf(out(0,i));
    }

  return out;
}

struct NNLayer {
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> w;
  Eigen::Matrix<float, 1, Eigen::Dynamic> bias;
  Eigen::Matrix<float, 1, Eigen::Dynamic> output;
  bool needActivation;
  struct NNLayer *next;

  NNLayer() : needActivation(true), next(NULL)
    {
    }

  NNLayer(int raws, int cols) : needActivation(true), next(NULL)
    {
      resize(raws, cols);
    }

  void resize(int raws, int cols)
    {
      w.resize(raws, cols);
      bias.resize(1, cols);
      output.resize(1, cols);

      w      = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>::Random(raws, cols);
      bias   = Eigen::Matrix<float, 1, Eigen::Dynamic>::Random(1, cols);
      output = Eigen::Matrix<float, 1, Eigen::Dynamic>::Random(1, cols);
    }

  struct NNLayer * setNext(struct NNLayer *n)
    {
      NNLayer *ret = next;
      next = n;
      return ret;
    };

  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m)
    {
      output = (m * w) - bias;
      if (needActivation)
        return activate();
      else
        return output;
    }

  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> activate()
    {
      for(int i=0; i<output.cols(); i++)
        {
          output(0, i) = 1.0/(1.0 + exp(output(0, i)));
        }
      return output;
    }

  void setActivation(bool isneed)
    {
      needActivation = isneed;
    }
};


class NeuralNetwork {
  NNLayer layer[3];
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> output;
  int layer_num;

  public:
    NeuralNetwork()
      {
        layer[0].resize(2,2);
        layer[1].resize(2,2);
        layer[2].resize(2,2);
        layer[2].setActivation(false);
        layer_num = 3;
      };

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> input)
      {
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> ret = input;
        for(int i=0; i<layer_num; i++)
          {
            ret = layer[i].forward(ret);
          }
        output = ret;
        return ret;
      };

    void back_propagation()
      {
      };

    float cross_entropy(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> in)
      {
        Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> tmp;
        float out = 0.f;
        tmp = in.array() * loge(output).array();
        for(int i=0; i<tmp.cols(); i++)
          {
            out -= tmp(0, i);
          }
        return out;
      };
};


int main(void)
{
  NeuralNetwork nn;

  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> out;
  Eigen::Matrix<float, 1, 2> input;
  Eigen::Matrix<float, 1, 2> expect;
  input << 3, 3;

  expect << 2, 5;

  std::cout << "input = " << input << std::endl;
  input = input.array() + 1;
  std::cout << "input = " << input << std::endl;
  

  out = nn.forward( input );

  std::cout << "out = " << out << std::endl;

  std::cout << "CrossEnt = " << nn.cross_entropy(expect) << std::endl;

  return 0;
}

