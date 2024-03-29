#include <stdio.h>
#include <math.h>
#include <iostream>
#include <Eigen/Eigen>

template<int innodes, int outnodes>
struct NNLayer {
  Eigen::Matrix<float, innodes, outnodes> w;
  Eigen::Matrix<float, 1, outnodes> bias;
  Eigen::Matrix<float, 1, outnodes> output;
  bool needActivation;

  NNLayer() : needActivation(true)
    {
      w      = Eigen::Matrix<float, innodes, outnodes>::Random(innodes, outnodes);
      bias   = Eigen::Matrix<float, 1, outnodes>::Random(1, outnodes);
      output = Eigen::Matrix<float, 1, outnodes>::Random(1, outnodes);
    }

  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m)
    {
      output = (m * w);
      output = output - bias;
      if (needActivation)
        return activate();
      else
        return output;
    }

  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> activate()
    {
      Eigen::Matrix<float, 1, outnodes> o = output;
      for(int i=0; i<output.cols(); i++)
        {
          o(0, i) = 1.0/(1.0 + exp(o(0, i)));
        }
      return o;
    }

  void setActivation(bool isneed)
    {
      needActivation = isneed;
    }
};


class NeuralNetwork {
  NNLayer<2, 2> l1;
  NNLayer<2, 2> l2;
  NNLayer<2, 1> l3;

  public:
    NeuralNetwork()
      {
        l3.setActivation(false);
      };

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> forward(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> input)
      {
        return l3.forward( l2.forward( l1.forward( input ) ) );
      };

    void back_propagation()
      {
      };
};


int main(void)
{
  NeuralNetwork nn;

  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> out;
  Eigen::Matrix<float, 1, 2> input;
  input << 3, 3;

  out = nn.forward( input );

  std::cout << "out = " << out << std::endl;

  return 0;
}

