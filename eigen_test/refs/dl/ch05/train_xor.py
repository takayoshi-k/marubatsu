# coding: utf-8
import sys, os
sys.path.append(os.pardir)

import numpy as np
from dataset.mnist import load_mnist
from two_layer_net import TwoLayerNet

W1 = np.array( [[-0.0032973, -0.02393722], [-0.02107663, 0.01280219]] )
W2 = np.array( [[-0.01024167], [-0.01514217]] )

network = TwoLayerNet(input_size=2, hidden_size=2, output_size=1)
network.changeParam(W1, W2)

iters_num = 10
batch_size = 4
learning_rate = 0.1

train_loss_list = []
train_acc_list = []
test_acc_list = []

iter_per_epoch = 1

x_batch = np.array([ [0, 0], [0, 1], [1, 0], [1, 1] ] )
t_batch = np.array([ [0], [1], [1], [0]] )

for i in range(iters_num):

    # 勾配
    #grad = network.numerical_gradient(x_batch, t_batch)
    grad = network.gradient(x_batch, t_batch)
    
    # 更新
    for key in ('W1', 'b1', 'W2', 'b2'):
        network.params[key] -= learning_rate * grad[key]
    
    loss = network.loss(x_batch, t_batch)
    train_loss_list.append(loss)
    print("loss = ", loss)

    # if i % iter_per_epoch == 0:
    if False:
        train_acc = network.accuracy(x_batch, t_batch)
        test_acc = network.accuracy(x_batch, t_batch)
        train_acc_list.append(train_acc)
        test_acc_list.append(test_acc)
        print(train_acc, test_acc)
