# COMP 472 Mini Project 1
# Patrick Riachi - 40104015
# Andre Hei Wang Law - 4017 5600
# Chloe Hei Yu Law - 40173275

import pandas as pd
import numpy as np
from sklearn.preprocessing import LabelEncoder
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from sklearn import tree
import matplotlib.pyplot as plt

# Try loading the data from a csv file.
try:
    data = pd.read_csv("training_data.csv")
except FileNotFoundError:
    print("No file found at the provided location.")
    exit()

# Define a function to calculate entropy, a measure of disorder or randomness.
def entropy(target_col):
    elements, counts = np.unique(target_col, return_counts = True)
    entropy = -np.sum([(counts[i]/np.sum(counts))*np.log2(counts[i]/np.sum(counts)) for i in range(len(elements))])
    return entropy

# Define a function to calculate Information Gain. This measure helps to decide which feature to split on at each step in building the tree.
def InfoGain(data, split_feature_name, target_name="class"):
    total_entropy = entropy(data[target_name])
    vals, counts = np.unique(data[split_feature_name], return_counts=True)
    weighted_entropy = np.sum([(counts[i]/np.sum(counts))*entropy(data.where(data[split_feature_name]==vals[i]).dropna()[target_name]) for i in range(len(vals))])
    information_gain = total_entropy - weighted_entropy
    return information_gain

# Replace NaN values with a placeholder string 'None'
data = data.fillna("None")

# Apply label encoder to each column. This converts categorical variables to numerical ones.
# We also store the encoder for each column for future use (to transform back to original labels).
encoders = {}
for column in data.columns:
    le = LabelEncoder()
    data[column] = le.fit_transform(data[column])
    encoders[column] = le

# Split the data into input and output. The 'WillWait' column is our target variable which we want to predict.
input_data = data.drop("WillWait", axis=1)
output_data = data["WillWait"]

# Split the data into training and testing sets. This allows us to evaluate the model on unseen data.
input_train, input_test, output_train, output_test = train_test_split(input_data, output_data, test_size=0.2)

#print("----------------------------")
#print(input_train)
#print("----------------------------")
#print(input_test)
#print("----------------------------")
#print(output_train)
#print("----------------------------")
#print(output_test)
#print("----------------------------")

# Initialize the Decision Tree Classifier object
dtc = DecisionTreeClassifier(criterion="entropy")

# Train the classifier on the training data
dtc.fit(input_train, output_train)

# Store the names of the features
feature_names = list(input_data.columns)

print("This model's performance on unseen samples taken from the testing sets.")
# Compare the predicted output with the true output
comparison = pd.DataFrame({"True Output": output_test, "Predicted Output": dtc.predict(input_test)})
print(comparison)

# Print the accuracy based on the model on the testing data
print("Accuracy:", dtc.score(input_test, output_test))
print("")


# Ask user for input for each feature
user_input = []
for feature in feature_names:
    value = input(f"Please enter value for {feature}: ").strip()  # remove leading and trailing spaces
    user_input.append(value)

# Convert the user input to a DataFrame with correct column names
user_input_df = pd.DataFrame([user_input], columns=feature_names)

# Convert the user input DataFrame to numerical values using the correct label encoder
for column in user_input_df.columns:
    try:
        user_input_df[column] = encoders[column].transform(user_input_df[column])
    except ValueError as e:
        print(f"Invalid value for {column}: {user_input_df[column][0]}. Please enter a value from the training data.")
        exit()

# Predict the output for the user's input
y_pred = dtc.predict(user_input_df)

# Print the predicted output (transform it back to original labels)
print("Predicted output: ", encoders["WillWait"].inverse_transform(y_pred))

# Visualizing the decision tree. We do not include the 'class_names' parameter so the classes are not shown in the plot.
plt.figure(figsize=(20, 10))  # set plot size (denoted in inches)
tree.plot_tree(dtc, feature_names=feature_names, filled=True)
plt.show()