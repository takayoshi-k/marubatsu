#include <stdio.h>
#include <iostream>
#include <Eigen/Eigen>
#include "NeuralNetwork.h"


int main(void)
{
  NeuralNetwork nn;

  float loss;
  Matrix<float, Dynamic, Dynamic> out;

#if 1
  Matrix<float, 1, 2> input[4];
  Matrix<float, 1, 1> expect[4];
  Matrix<float, Dynamic, Dynamic> W1(2,2);
  Matrix<float, Dynamic, Dynamic> W2(2,1);

  input[0] << 0, 0; expect[0] << 0;
  input[1] << 0, 1; expect[1] << 1;
  input[2] << 1, 0; expect[2] << 1;
  input[3] << 1, 1; expect[3] << 0;

  W1 << -0.0032973, -0.02393722, -0.02107663, 0.01280219 ;
  W2 << -0.01024167, -0.01514217;

  nn.createNewLayer(2,2, false, &W1);
  nn.createNewLayer(2,1, true,  &W2);

  loss = 100.f;
  nn.printnet();
  // while (loss > 0.001f)
  for(int j=0; j<10; j++)
    {
      loss = 0.f;
      for(int i=0; i<4; i++)
        {
          // std::cout << "==== " << i+1 << " ===="  << std::endl;
          out = nn.forward( input[i] );
          loss += nn.backward(0.1f, expect[i]);
          nn.printnet();
          // std::cout << "   out = " << out << std::endl;
        }
      loss /= 4;
      std::cout << "loss = " << loss << std::endl;
      nn.printnet();
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

