#ifndef __NEURALNETWORK_H__
#define __NEURALNETWORK_H__

#include <Eigen/Eigen>
#include "NNLayers.h"

using namespace Eigen;

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
    ~NeuralNetwork();

    void createNewLayer(int innum, int outnum, bool is_last=false);
    void print_layers();
    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> input);
    float backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m);
};

#endif  // __NEURALNETWORK_H__
