#include <stdio.h>
#include <math.h>
#include <iostream>
#include <Eigen/Eigen>

#include "NeuralNetwork.h"

using namespace Eigen;

/*
 * Class AffineLayer
 *
 * @desript A kind of Neural Network Layer.
 */
NeuralNetwork::~NeuralNetwork()
{
  NNLayer *tmp;
  for(NNLayer *layer = top_layer; layer != NULL; )
    {
      tmp = layer->getNext();
      delete layer;
      layer = tmp;
    }
}

void NeuralNetwork::createNewLayer(int innum, int outnum, bool is_last)
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
}


void NeuralNetwork::print_layers()
{
  for(NNLayer *layer = top_layer; layer != NULL; layer = layer->getNext())
    {
      std::cout << layer << " --> ";
    };
  std::cout << "NULL" << std::endl;
}


Matrix<float, Dynamic, Dynamic> NeuralNetwork::forward(Matrix<float, Dynamic, Dynamic> input)
{
  output = top_layer->forward(input);
  return output;
}

float NeuralNetwork::backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m)
{
  float ret = ((LastActivation *)last_layer)->loss(m);
  last_layer->backward(train_ratio, m);
  return ret;
}

