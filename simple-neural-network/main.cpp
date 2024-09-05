#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double sigmoidDerivative(double x) {
    return x * (1 - x);
}

class NeuralNetwork {
private:
    vector<vector<double> > weights;
    vector<double> biases;
    vector<double> activations;
    vector<double> outputs;
    vector<double> errors;
    vector<double> deltas;
    vector<int> layerSizes;

public:
    NeuralNetwork(const vector<int>& layers) : layerSizes(layers) {
        for (size_t i = 0; i < layers.size() - 1; i++) {
            vector<double> layerWeights;
            for (int j = 0; j < layers[i + 1] * layers[i]; j++) {
                layerWeights.push_back((double)rand() / RAND_MAX);
            }
            weights.push_back(layerWeights);
            biases.push_back((double)rand() / RAND_MAX);
        }

        for (int size : layers) {
            activations.push_back(0.0);
            if (size != layers.back()) {
                outputs.push_back(0.0);
                errors.push_back(0.0);
                deltas.push_back(0.0);
            }
        }
    }

    void forward(const vector<double>& input) {
        for (size_t i = 0; i < input.size(); i++) {
            activations[i] = input[i];
        }

        for (size_t i = 0; i < weights.size(); i++) {
            for (int j = 0; j < layerSizes[i + 1]; j++) {
                double sum = 0.0;
                for (int k = 0; k < layerSizes[i]; k++) {
                    sum += weights[i][j * layerSizes[i] + k] * activations[i * layerSizes[i] + k];
                }
                sum += biases[i];
                outputs[i * layerSizes[i + 1] + j] = sum;
                activations[(i + 1) * layerSizes[i + 1] + j] = sigmoid(sum);
            }
        }
    }

    void backward(const vector<double>& target) {
        for (int i = layerSizes.size() - 1; i > 0; i--) {
            if (i == layerSizes.size() - 1) {
                for (int j = 0; j < layerSizes[i]; j++) {
                    double output = activations[i * layerSizes[i] + j];
                    errors[i - 1 * layerSizes[i] + j] = (target[j] - output) * sigmoidDerivative(output);
                }
            } else {
                for (int j = 0; j < layerSizes[i]; j++) {
                    double error = 0.0;
                    for (int k = 0; k < layerSizes[i + 1]; k++) {
                        error += errors[i * layerSizes[i + 1] + k] * weights[i][k * layerSizes[i] + j];
                    }
                    double output = activations[i * layerSizes[i] + j];
                    errors[(i - 1) * layerSizes[i] + j] = error * sigmoidDerivative(output);
                }
            }
        }

        for (size_t i = 0; i < weights.size(); i++) {
            for (int j = 0; j < layerSizes[i + 1]; j++) {
                for (int k = 0; k < layerSizes[i]; k++) {
                    double delta = 0.1 * errors[i * layerSizes[i + 1] + j] * activations[i * layerSizes[i] + k];
                    weights[i][j * layerSizes[i] + k] += delta;
                }
                biases[i] += 0.1 * errors[i * layerSizes[i + 1] + j];
            }
        }
    }

    void train(const vector<double>& input, const vector<double>& target) {
        forward(input);
        backward(target);
    }

    vector<double> predict(const vector<double>& input) {
        forward(input);
        vector<double> result(layerSizes.back());
        for (int i = 0; i < layerSizes.back(); i++) {
            result[i] = activations[activations.size() - layerSizes.back() + i];
        }
        return result;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    vector<int> layers = {1, 2, 1};
    NeuralNetwork nn(layers);

    vector<vector<double> > inputs = {{0}, {1}};
    vector<vector<double> > targets = {{0}, {1}};

    for (int epoch = 0; epoch < 10000; epoch++) {
        for (size_t i = 0; i < inputs.size(); i++) {
            nn.train(inputs[i], targets[i]);
        }
    }

    cout << "Prediction for input 0: " << nn.predict({0})[0] << endl;
    cout << "Prediction for input 1: " << nn.predict({1})[0] << endl;

    return 0;
}