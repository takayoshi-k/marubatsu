#include <math.h>
#include <iostream>
#include <Eigen/Eigen>

template<int innodes, int outnodes>
struct NNLayer {
  Eigen::Matrix<float, innodes, outnodes> w;
  Eigen::Matrix<float, 1, outnodes> bias;
  Eigen::Matrix<float, 1, outnodes> output;

  NNLayer()
    {
      w      = Eigen::Matrix<float, innodes, outnodes>::Random(innodes, outnodes);
      bias   = Eigen::Matrix<float, 1, outnodes>::Random(1, outnodes);
      output = Eigen::Matrix<float, 1, outnodes>::Random(1, outnodes);
    }

  Eigen::Matrix<float, 1, outnodes> forward(Eigen::Matrix<float, innodes, 1> &m)
    {
      output = (m * w) - bias;
      return exp();
    }

  Eigen::Matrix<float, outnodes, 1> exp()
    {
      Eigen::Matrix<float, 1, outnodes> o = output;
      for(int i=0; i<output.cols(); i++)
        {
          o(0, i) = 1.0/(1.0 + exp(o(0, i)));
        }
      return o.transpose();
    }
};

int main(void)
{
  NNLayer<2, 2> l1;
  NNLayer<2, 2> l2;
  Eigen::Matrix<float, 2, 1> out;
  Eigen::Matrix<float, 2, 1> input = Eigen::Matrix<float, 2, 1>::Random(2,1);

  out = l2.forward( l1.forward( input ) );

  std::cout << "out = " << out << std::endl;
  
}

