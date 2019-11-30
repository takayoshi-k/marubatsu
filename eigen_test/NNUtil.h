#ifndef __NNUTIL_H__
#define __NNUTIL_H__

#include <Eigen/Eigen>

using namespace Eigen;

void shape(const char *msg, Matrix<float, Dynamic, Dynamic> in);
Matrix<float, Dynamic, Dynamic> loge(Matrix<float, Dynamic, Dynamic> in);
float cross_entropy(Matrix<float, Dynamic, Dynamic> output, Matrix<float, Dynamic, Dynamic> expect);

#endif  //  __NNUTIL_H__
