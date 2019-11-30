#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Eigen/Eigen>

// http://ankokudan.org/d/dl/pdf/pdf-eigennote.pdf

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> exp( Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> &output)
{
    std::cout << "output.rows() = " << output.rows() << std::endl;
    std::cout << "output.cols() = " << output.cols() << std::endl;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> o = output;
    for(int i=0; i<output.cols(); i++)
      {
        o(0, i) = 1.0/(1.0 + exp(o(0, i)));
      }
    return o;
}

int main(void)
{
  Eigen::Matrix<float, 2,2> m1 = Eigen::Matrix<float, 2, 2>::Random(2,2);
  Eigen::Matrix<float, 2,2> m2;
  Eigen::Matrix<float, 2,2> m3;
  Eigen::Matrix<float, 2, 1> v1;
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> A;
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> B;

  m2 <<
    1, 0,
    1, 2;

  m3 <<
    0, 1,
    2, 0;

  v1 << 1, 4;

  std::cout << "m1 = " << m1 << std::endl;
  std::cout << "m2 = " << m2 << std::endl;
  std::cout << "m3 = " << m3 << std::endl;
  std::cout << "m2 - m3 = " << m2 - m3 << std::endl;
  std::cout << "m2 * m3 = " << m2 * m3 << std::endl;
  std::cout << "v1 = " << v1 << std::endl;

  std::cout << "m2.size() = " << m2.size() << std::endl;
  std::cout << "v1.rows() = " << v1.rows() << std::endl;
  std::cout << "v1.cols() = " << v1.cols() << std::endl;

  std::cout << "v1 * m3 = " << v1.transpose() * m3 << std::endl;
  A = v1.transpose() * m3;

  std::cout << "v1.rows() = " << v1.rows() << std::endl;
  std::cout << "v1.cols() = " << v1.cols() << std::endl;
  std::cout << "A.rows() = " << A.rows() << std::endl;
  std::cout << "A.cols() = " << A.cols() << std::endl;

  B = exp(A);

  std::cout << "B.rows() = " << B.rows() << std::endl;
  std::cout << "B.cols() = " << B.cols() << std::endl;
  std::cout << "B = " << B << std::endl;

  return 0;
}

