# Neutral network class
class NeuralNetwork:
    def __init__(self):


        # Output layer weights and biases
        self.weights_output = [[1.0], [1.0]]
        self.bias_output = [-1.5]

    def feedforward(self, inputs):


        # Calculate output of output layer
        output_layer_output = sigmoid(
            (hidden_layer_output * self.weights_output[0][0]) + (hidden_layer_output * self.weights_output[1][0]) +
            self.bias_output[0])

        return output_layer_output


# Sigmoid function
def sigmoid(x):
    return 1 / (1 + pow(2.71828, -x))


# Create neural network instance
neuralNetwork = NeuralNetwork()

# Test the neural network with example
example = [[0, 0], [0, 1], [1, 0], [1, 1]]
for ex in example:
    output = neuralNetwork.feedforward(ex)
    print(neuralNetwork.feedforward(ex))
    print(output)
    print(f"{ex[0]} and {ex[1]} = {round(output)}")


"""
a) How many hidden layers have you used?
    One hidden layer used

b) What are the weights and biases of each node?
    Hidden layer 
        weight node 1 = 0.5
        weight node 2 = 0.5
        bias node 1 = -0.5
        
    Output layer
        weight node 1 = 1.0
        weight node 2 = 1.0
        bias node 1 = -1.5
"""