from main import load_mnist
from network import NeuralNetwork

if __name__ == "__main__":
    _, _, test_data = load_mnist()
    nn = NeuralNetwork()
    nn.load()

    accuracy, precision, recall, F1, loss = nn.validate(test_data)
    print(f"TEST DATA accuracy:%.4f, precision:%.4f, recall:%.4f, F1:%.4f" % (accuracy, precision, recall, F1))
