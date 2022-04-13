import gzip
import os
import pickle
import wget
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import random

from network import NeuralNetwork


def load_mnist():
    print("Loading MNIST dataset...")

    data_file = gzip.open(os.path.join(os.curdir, "data", "mnist.pkl.gz"), "rb")
    train_data, val_data, test_data = pickle.load(data_file, encoding="latin1")
    data_file.close()

    train_inputs = [np.reshape(x, (784, 1)) for x in train_data[0]]
    train_results = [vectorized_result(y) for y in train_data[1]]
    train_data = list(zip(train_inputs, train_results))

    val_inputs = [np.reshape(x, (784, 1)) for x in val_data[0]]
    val_data = list(zip(val_inputs, val_data[1]))

    test_inputs = [np.reshape(x, (784, 1)) for x in test_data[0]]
    test_data = list(zip(test_inputs, test_data[1]))
    return train_data, val_data, test_data


def vectorized_result(y):
    """
    将标注y向量化
    """
    e = np.zeros((10, 1))
    e[y] = 1.0
    return e


def plot_digit(digit):
    digit_image = digit.reshape(28, 28)
    plt.imshow(digit_image, cmap=mpl.cm.binary, interpolation="nearest")
    plt.axis("off")
    plt.show()


if __name__ == "__main__":
    np.random.seed(42)

    # 设定超参
    layers = (784, 30, 10)
    learning_rate = 0.01
    mini_batch_size = 16
    epochs = 150
    keep_prob = 0.5

    # 包含图片和标注train[(784*1,10*1),...],val,test=[(784*1,1)...]的列表
    train_data, val_data, test_data = load_mnist()
    # plot_digit(train_data[0][0])

    nn = NeuralNetwork(layers, learning_rate, mini_batch_size, "relu", keep_prob)
    nn.fit(train_data, val_data, epochs)
    nn.plot_loss_curve()

    accuracy, precision, recall, F1, loss = nn.validate(test_data)
    print(f"TEST DATA accuracy:%.4f, precision:%.4f, recall:%.4f, F1:%.4f" % (accuracy, precision, recall, F1))

    nn.save()
