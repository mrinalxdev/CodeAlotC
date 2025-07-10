# Writing Perceptron in C++

A single layer neural network for binary classification using a step function

The perceptron will be trained on a small 2D dataset (10-15 points) with binary labels (0 and 1, but we will use -1 and 1 for the step function sometimes? Actually, we can adjust).

note: the step function typically uses 0 and 1, but the perceptron algorithm often uses -1 and 1 for the labels to make the update rule symmetric.

Steps:

1.  We'll generate a small linearly separable 2D dataset. Alternatively, we can hard-code a small dataset or read from a simple structure.
2.  We'll implement the perceptron training algorithm:
    - Initialize weights (including bias) to small random numbers or zeros.
    - For each epoch (or until no mistakes), iterate over the data.
    - For each data point, compute the prediction:
      z = w0*x0 + w1*x1 + ... + wn\*xn (where x0 is the bias term, always 1, and w0 is the bias weight)
    - Apply step function: if z >= 0, then output 1; else output 0 (or -1? Let's decide: we'll use labels {0,1} for output, but internally we can use 0 and 1 for step function).
