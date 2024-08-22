import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense

# Create the training set DataFrame
train_data = pd.DataFrame({
    'A': [1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
    'B': [1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1],
    'C': [1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1],
    'D': [0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1],
    'E': [0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1],
    'output': [1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
})

# Split the training set into features (X) and labels (y)
X_train = train_data.drop('output', axis=1)
y_train = train_data['output']

# Create the testing set DataFrame
test_data = pd.DataFrame({
    'A': [1, 1, 0, 0, 1, 1, 1, 1],
    'B': [1, 1, 0, 0, 1, 1, 1, 1],
    'C': [0, 0, 0, 0, 0, 0, 1, 1],
    'D': [0, 1, 0, 1, 0, 1, 1, 1],
    'E': [0, 1, 0, 1, 0, 1, 0, 1],
    'output': [1, 1, 0, 0, 1, 1, 1, 1]
})

# Split the testing set into features (X) and labels (y)
X_test = test_data.drop('output', axis=1)
y_test = test_data['output']

# Standardize the features using StandardScaler
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# Build the neural network model
model = Sequential()
model.add(Dense(10, activation='relu', input_shape=(5,)))
model.add(Dense(1, activation='sigmoid'))

# Compile the model
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(X_train, y_train, epochs=100, batch_size=4, verbose=0)

# Evaluate the model on the testing set
loss, accuracy = model.evaluate(X_test, y_test)

print(f"Accuracy on testing set: {accuracy * 100:.2f}%")
