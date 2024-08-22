# Neutral network class
class NeuralNetwork:
    def __init__(self):
        # Weights and bias
        self.weights = [1.0, 1.0]
        self.bias = -1.5

    def feedforward(self, inputs):
        # Calculate output
        output = sigmoid(
            (inputs[0] * self.weights[0]) + (inputs[1] * self.weights[1]) + self.bias)

        return output


# Sigmoid function
def sigmoid(x):
    return 1 / (1 + pow(2.71828, -x))


# Create neural network instance
neuralNetwork = NeuralNetwork()

# Test the neural network with example
example = [[0, 0], [0, 1], [1, 0], [1, 1]]
for ex in example:
    output = neuralNetwork.feedforward(ex)
    print(f"{ex[0]} and {ex[1]} = {round(output)}")

"""
a) How many hidden layers have you used?
    No hidden layer used

b) What are the weights and biases of each node?
            
    Output layer
        weight node 1 = 1.0
        weight node 2 = 1.0
        bias node 1 = -1.5
"""