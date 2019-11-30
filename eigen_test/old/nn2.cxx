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
  struct NNLayer *next;
  struct NNLayer *back;

  NNLayer() : next(NULL), back(NULL)
    {
    };

  ~NNLayer() {};

  virtual Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m) = 0;

  virtual void back_propagation() = 0;

  void setNext(struct NNLayer *n)
    {
      next = n;
      n->back = this;
    };

  struct NNLayer * getNext()
    {
      return next;
    };

};


class Activation : public NNLayer {
  public:
    virtual Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m)
      {
      };
};


struct AffineLayer : public NNLayer {
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> w;
  Eigen::Matrix<float, 1, Eigen::Dynamic> bias;
  Eigen::Matrix<float, 1, Eigen::Dynamic> output;
  bool needActivation;

  AffineLayer() : NNLayer(), needActivation(true)
    {
    }

  AffineLayer(int raws, int cols) : NNLayer(), needActivation(true)
    {
      resize(raws, cols);
    }

  ~AffineLayer() {};

  void resize(int raws, int cols)
    {
      w.resize(raws, cols);
      bias.resize(1, cols);
      output.resize(1, cols);

      w      = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>::Random(raws, cols);
      bias   = Eigen::Matrix<float, 1, Eigen::Dynamic>::Random(1, cols);
      output = Eigen::Matrix<float, 1, Eigen::Dynamic>::Random(1, cols);
    }

  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m)
    {
      output = (m * w) - bias;
      if (needActivation) activate();

      if (next != NULL)
        {
          return next->forward(output);
        }
      else
        {
          return output;
        }
    }

  void back_propagation()
    {
    };

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
  NNLayer *top_layer;
  NNLayer *last_layer;
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> output;

  public:
    NeuralNetwork()
      {
        AffineLayer * tmplayers = new AffineLayer[3];
        tmplayers[0].resize(2,3);
        tmplayers[1].resize(3,5);
        tmplayers[2].resize(5,2);
        // tmplayers[2].setActivation(false);

        tmplayers[0].setNext(&tmplayers[1]);
        tmplayers[1].setNext(&tmplayers[2]);

        top_layer  = &tmplayers[0];
        last_layer = &tmplayers[2];
      };
    ~NeuralNetwork()
      {
        delete [] top_layer;
      };

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> input)
      {
        output = top_layer->forward(input);
        return output;
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
  std::cout << "expect = " << expect << std::endl;
  std::cout << "input * expect = " << input.array() * expect.array() << std::endl;

  std::cout << "input = " << input << std::endl;
  input = input.array() + 1;
  std::cout << "input = " << input << std::endl;
  

  out = nn.forward( input );

  std::cout << "out = " << out << std::endl;

  std::cout << "CrossEnt = " << nn.cross_entropy(expect) << std::endl;

  return 0;
}

