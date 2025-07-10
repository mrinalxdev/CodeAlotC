#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct DataPoint
{
    double x1;
    double x2;
    int label;
};

std::vector<DataPoint> createDataset()
{
    std::vector<DataPoint> dataset;
    dataset.push_back({1.0, 2.0, 1});
    dataset.push_back({1.0, 3.0, 1});
    dataset.push_back({2.0, 3.0, 1});
    dataset.push_back({2.0, 2.5, 1});
    dataset.push_back({3.0, 4.0, 1});
    dataset.push_back({3.0, 3.5, 1});
    dataset.push_back({1.0, 0.5, 0});
    dataset.push_back({2.0, 1.0, 0});
    dataset.push_back({2.0, 0.5, 0});
    dataset.push_back({3.0, 2.0, 0});
    dataset.push_back({3.0, 1.5, 0});
    dataset.push_back({4.0, 1.0, 0});
    dataset.push_back({4.0, 2.5, 0});
    return dataset;
}

std::vector<double> trainPerceptron(std::vector<DataPoint> &dataset, double learning_rate, int max_epochs)
{
    srand(time(0));
    std::vector<double> weights(3, 0.0);
    int epoch = 0;
    bool converged = false;
    while (epoch < max_epochs && !converged)
    {
        std::vector<DataPoint> shuffled = dataset;
        for (int i = 0; i < shuffled.size() - 1; i++)
        {
            int j = i + rand() % (shuffled.size() - i);
            DataPoint temp = shuffled[i];
            shuffled[i] = shuffled[j];
            shuffled[j] = temp;
        }
        int mistakes = 0;
        for (const DataPoint &dp : shuffled)
        {
            double feature0 = 1.0;
            double feature1 = dp.x1;
            double feature2 = dp.x2;
            double z = weights[0] * feature0 + weights[1] * feature1 + weights[2] * feature2;
            int prediction = (z >= 0) ? 1 : 0;
            int error = dp.label - prediction;
            if (error != 0)
            {
                mistakes++;
                weights[0] += learning_rate * error * feature0;
                weights[1] += learning_rate * error * feature1;
                weights[2] += learning_rate * error * feature2;
            }
        }
        if (mistakes == 0)
        {
            converged = true;
            std::cout << "Converged at epoch " << epoch << std::endl;
        }
        epoch++;
    }
    if (!converged)
    {
        std::cout << "Stopped after reaching max epochs: " << max_epochs << std::endl;
    }
    return weights;
}

void testPerceptron(std::vector<DataPoint> &dataset, std::vector<double> weights)
{
    int correct = 0;
    for (const DataPoint &dp : dataset)
    {
        double feature0 = 1.0;
        double feature1 = dp.x1;
        double feature2 = dp.x2;
        double z = weights[0] * feature0 + weights[1] * feature1 + weights[2] * feature2;
        int prediction = (z >= 0) ? 1 : 0;
        std::cout << "Point (" << dp.x1 << ", " << dp.x2 << "): predicted = " << prediction << ", true = " << dp.label << std::endl;
        if (prediction == dp.label)
        {
            correct++;
        }
    }
    double accuracy = static_cast<double>(correct) / dataset.size() * 100.0;
    std::cout << "Accuracy: " << accuracy << "%" << std::endl;
}

void printDecisionBoundary(std::vector<double> weights)
{
    double w0 = weights[0];
    double w1 = weights[1];
    double w2 = weights[2];
    if (w2 != 0)
    {
        std::cout << "Decision boundary: x2 = " << (-w0 / w2);
        if (w1 != 0)
        {
            std::cout << " + " << (-w1 / w2) << "*x1";
        }
        std::cout << std::endl;
    }
    else if (w1 != 0)
    {
        std::cout << "Decision boundary: x1 = " << (-w0 / w1) << std::endl;
    }
    else if (w0 != 0)
    {
        std::cout << "No decision boundary (constant non-zero weight)" << std::endl;
    }
    else
    {
        std::cout << "Weights are all zero, no boundary" << std::endl;
    }
}

void visualize(std::vector<DataPoint> &dataset, std::vector<double> weights)
{
    const int GRID_SIZE = 20;
    char grid[GRID_SIZE][GRID_SIZE];

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            grid[i][j] = '.';
        }
    }

    // todo
    for (const DataPoint &dp : dataset)
    {
        int x = static_cast<int>(dp.x1 * (GRID_SIZE / 10.0));
        int y = static_cast<int>(dp.x2 * (GRID_SIZE / 10.0));

        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE)
        {
            grid[y][x] = dp.label == 1 ? '1' : '0';
        }
    }

    // todo : decision boundary
    for (int x = 0; x < GRID_SIZE; ++x)
    {
        double x_val = x * (10.0 / GRID_SIZE);
        if (weights[2] != 0)
        {
            double y_val = (-weights[0] - weights[1] * x_val) / weights[2];
            int y = static_cast<int>(y_val * (GRID_SIZE / 10.0));
            if (y >= 0 && y < GRID_SIZE)
            {
                grid[y][x] = '*';
            }
        }
    }

    // printing up the grid here
    for (int i = GRID_SIZE - 1; i >= 0; --i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            std::cout << grid[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

int main()
{
    std::vector<DataPoint> dataset = createDataset();
    double learning_rate = 0.1;
    int max_epochs = 100;
    std::vector<double> weights = trainPerceptron(dataset, learning_rate, max_epochs);
    std::cout << "Final weights: w0 (bias) = " << weights[0] << ", w1 = " << weights[1] << ", w2 = " << weights[2] << std::endl;
    printDecisionBoundary(weights);
    testPerceptron(dataset, weights);

    std::cout << "Weights: w0=" << weights[0] << ", w1=" << weights[1] << ", w2=" << weights[2] << std::endl;
    visualize(dataset, weights);
    return 0;
}