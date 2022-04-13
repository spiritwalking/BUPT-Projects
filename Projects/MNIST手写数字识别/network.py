import os
import numpy as np
import random
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt

import activations


class NeuralNetwork(object):

    def __init__(
            self,
            sizes=(784, 30, 10),
            learning_rate=1e-2,
            mini_batch_size=16,
            activation_fn="relu",
            keep_prob=0.5
    ):
        """
        初始化神经网络模型

        Parameters
        ----------
        sizes : tuple, 神经网络的形状
        learning_rate : float, 学习率
        mini_batch_size : int, 梯度下降的batch size
        activation_fn: str, 激活函数
        keep_prob: float drop out机制保留神经元概率
        """

        self.sizes = sizes
        self.num_layers = len(sizes)
        self.activation_fn = getattr(activations, activation_fn)
        self.activation_fn_prime = getattr(activations, f"{activation_fn}_deriv")
        self.keep_prob = 1

        # 随机初始化权重及偏差
        self.weights = [np.array([0])] + [np.random.randn(y, x) * np.sqrt(2 / x) for y, x in zip(sizes[1:], sizes[:-1])]
        self.biases = [np.array([0])] + [np.random.randn(y, 1) for y in sizes[1:]]

        # 初始化z、激活值及dropout矩阵
        # z = wx + b, a = activation_fn(z)
        self._zs = [np.zeros(bias.shape) for bias in self.biases]
        self._activations = [np.zeros(bias.shape) for bias in self.biases]
        self.drop = [np.zeros((y, 1)) for y in sizes]

        self.mini_batch_size = mini_batch_size
        self.lr = learning_rate

        self.loss_record = {"train": [], "val": []}

    def fit(self, training_data, validation_data=None, epochs=10):
        """
        训练神经网络

        Parameters
        ----------
        training_data : list of tuple, 训练数据集
        validation_data : list of tuple, 验证数据集
        epochs: int, 遍历训练集次数

        """
        earlystop_cnt = 0
        pre_val_loss = float('inf')
        for epoch in range(epochs):
            random.shuffle(training_data)
            mini_batches = [training_data[k:k + self.mini_batch_size] for k in
                            range(0, len(training_data), self.mini_batch_size)]
            train_loss = 0

            for mini_batch in mini_batches:
                nabla_b = [np.zeros(bias.shape) for bias in self.biases]
                nabla_w = [np.zeros(weight.shape) for weight in self.weights]

                for i in range(self.num_layers-1):
                    self.drop[i] = np.random.rand(self.drop[i].shape[0], self.drop[i].shape[1]) < self.keep_prob

                for x, y in mini_batch:
                    # 前向传播
                    self._forward_prop(x, dropout=True)

                    # 反向传播
                    delta_nabla_b, delta_nabla_w = self._back_prop(x, y)
                    loss = self.get_loss(y)
                    # 获得这个batch的误差矩阵
                    nabla_b = [nb + dnb for nb, dnb in zip(nabla_b, delta_nabla_b)]
                    nabla_w = [nw + dnw for nw, dnw in zip(nabla_w, delta_nabla_w)]
                    train_loss += loss

                # MBGP
                self.weights = [w - (self.lr / self.mini_batch_size) * dw for w, dw in zip(self.weights, nabla_w)]
                self.biases = [b - (self.lr / self.mini_batch_size) * db for b, db in zip(self.biases, nabla_b)]

            train_loss /= len(training_data)
            self.loss_record["train"].append(train_loss)
            # print(train_loss)

            if validation_data:
                accuracy, precision, recall, F1, val_loss = self.validate(validation_data)
                # print(val_loss)
                self.loss_record["val"].append(val_loss)
                print(f"Epoch %d/%d, accuracy:%.4f, precision:%.4f, recall:%.4f, F1:%.4f" % (
                    epoch + 1, epochs, accuracy, precision, recall, F1))

                if val_loss > pre_val_loss:
                    earlystop_cnt += 1
                else:
                    earlystop_cnt = 0
                pre_val_loss = val_loss
                if earlystop_cnt > 2:
                    print(f"Early Stop at epoch {epoch}")
                    self.save()
                    break
            else:
                print(f"Processed epoch {epoch}.")

    def validate(self, validation_data):
        """
        测试模型在给定数据集上的表现

        Parameters
        ----------
        validation_data : list of tuple

        Returns
        -------
        float, accuracy
        float, precision
        float, recall
        float, F1
        float, val_loss
        """
        y_true = []
        y_pred = []
        val_loss = 0

        for x, y in validation_data:
            y_true.append(y)
            y_pred.append(self.predict(x))
            val_loss += self.get_loss(y)

        conf_matrix = confusion_matrix(y_true, y_pred)
        # print(conf_matrix)

        # (TP+TN)/(TP+TN+FP+FN)
        accuracy = np.diag(conf_matrix).sum() / conf_matrix.sum()

        # TP/(TP+FP)
        precision = np.diag(conf_matrix) / conf_matrix.sum(axis=0)
        precision = np.nanmean(precision)

        # TP/(TP+FN)
        recall = np.diag(conf_matrix) / conf_matrix.sum(axis=1)
        recall = np.nanmean(recall)

        # 2PR/(P+R)
        F1 = 2 * precision * recall / (precision + recall)

        return accuracy, precision, recall, F1, val_loss / len(validation_data)

    def predict(self, x):
        """
        对输入的样例进行预测

        Parameters
        ----------
        x : numpy.array

        Returns
        -------
        int, 预测样例的标签

        """
        self._forward_prop(x)
        return np.argmax(self._activations[-1])

    def _forward_prop(self, x, dropout=False):
        self._activations[0] = x

        if dropout:
            self._activations[0] *= self.drop[0]
            self._activations[0] /= self.keep_prob

        for i in range(1, self.num_layers):
            self._zs[i] = self.weights[i].dot(self._activations[i - 1]) + self.biases[i]
            # 最后一层使用softmax
            if i == self.num_layers - 1:
                self._activations[i] = activations.softmax(self._zs[i])
            else:
                self._activations[i] = self.activation_fn(self._zs[i])
                if dropout:
                    self._activations[i] *= self.drop[i]
                    self._activations[i] /= self.keep_prob

    def _back_prop(self, x, y):
        """
        反向传播

        Parameters
        ----------
        x : 样本x
        y : 标签y

        Returns
        -------
        这一个样本的误差

        """
        nabla_b = [np.zeros(bias.shape) for bias in self.biases]
        nabla_w = [np.zeros(weight.shape) for weight in self.weights]

        # 计算误差矩阵
        error = (self._activations[-1] - y)

        nabla_b[-1] = error
        nabla_w[-1] = error.dot(self._activations[-2].transpose())

        for i in range(self.num_layers - 2, 0, -1):
            error = np.multiply(self.weights[i + 1].transpose().dot(error), self.activation_fn_prime(self._zs[i]))
            nabla_b[i] = error
            nabla_w[i] = error.dot(self._activations[i - 1].transpose())

        return nabla_b, nabla_w

    def get_loss(self, y):
        """
        计算某个样本的损失
        """
        if type(y) is not np.ndarray:
            e = np.zeros((10, 1))
            e[y] = 1.0
        else:
            e = y

        f = self._activations[-1]
        loss = 0
        for fi, yi in zip(f, e):
            loss += yi * np.log(fi)
        return -loss[0]

    def load(self, filename='model.npz'):
        """
        载入事先训练好的模型
        """
        model = np.load(os.path.join(os.curdir, 'models', filename), allow_pickle=True)

        self.weights = list(model['weights'])
        self.biases = list(model['biases'])

        self.sizes = [b.shape[0] for b in self.biases]
        self.num_layers = len(self.sizes)

        self._zs = [np.zeros(bias.shape) for bias in self.biases]
        self._activations = [np.zeros(bias.shape) for bias in self.biases]

        self.mini_batch_size = int(model['mini_batch_size'])
        self.lr = float(model['lr'])

    def save(self, filename='model.npz'):
        """
        保存神经网络的超参
        """
        if not os.path.exists(os.path.join(os.curdir, "models")):
            os.mkdir(os.path.join(os.curdir, "models"))

        np.savez_compressed(
            file=os.path.join(os.curdir, 'models', filename),
            weights=self.weights,
            biases=self.biases,
            mini_batch_size=self.mini_batch_size,
            lr=self.lr
        )

    def plot_loss_curve(self):
        plt.plot(self.loss_record['train'], c='r', label='train')
        plt.plot(self.loss_record['val'], c='b', label='val')
        plt.ylim(0.0, 0.65)
        plt.xlabel('Training epochs')
        plt.ylabel('Cross Entropy Loss')
        plt.legend()
        plt.show()
