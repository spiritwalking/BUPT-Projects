import numpy as np


def sigmoid(z):
    return 1.0 / (1.0 + np.exp(-z))


def sigmoid_deriv(z):
    return sigmoid(z) * (1 - sigmoid(z))


def softmax(z):
    return np.exp(z) / np.sum(np.exp(z))


def tanh(z):
    return np.tanh(z)


def tanh_deriv(z):
    return 1 - tanh(z) * tanh(z)


def relu(z):
    return np.maximum(z, 0)


def relu_deriv(z):
    return z > 0
