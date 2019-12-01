#ifndef __NNLAYERS_H__
#define __NNLAYERS_H__

#include <Eigen/Eigen>

using namespace Eigen;

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

    void setNext(NNLayer *n);
    NNLayer * getNext();
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

    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> m);
    Matrix<float, Dynamic, Dynamic> backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m);
    float loss(Matrix<float, Dynamic, Dynamic> expect);

    void resize(int rows, int cols) {};
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

    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> m);
    Matrix<float, Dynamic, Dynamic> backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m);
    void resize(int rows, int cols);
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

    AffineLayer(int rows, int cols, Matrix<float, Dynamic, Dynamic> *default_w=NULL) : NNLayer()
      {
        resize(rows, cols);
        if(default_w != NULL)
          {
            w = *default_w;
          }
      }

    ~AffineLayer() {};

    void resize(int rows, int cols);
    Matrix<float, Dynamic, Dynamic> forward(Matrix<float, Dynamic, Dynamic> m);
    Matrix<float, Dynamic, Dynamic> backward(float train_ratio, Matrix<float, Dynamic, Dynamic> m);
};

#endif  // __NNLAYERS_H__
