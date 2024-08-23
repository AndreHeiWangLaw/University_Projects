import tensorflow as tf
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense, Dropout
from tensorflow.keras.layers import BatchNormalization
import os

# dataset location
current_folder = os.path.dirname(os.path.abspath(__file__))
dataset_path = os.path.join(current_folder, 'FNNPK')

# dimension of images
img_width, img_height = 150, 150

# data preprocessing
train_datagen = ImageDataGenerator(
    rescale=1./255,             # rescale pixel value between 0-1
    shear_range=0.2,            # shear transformation
    zoom_range=0.2,             # zoom transformation
    rotation_range=45,          # rotation transformation
    width_shift_range=0.2,      # horizontal shift
    height_shift_range=0.2,     # vertical shift
    horizontal_flip=True,       # vertical flip
    validation_split=0.20)       # allocate 20% of data for validation

# generate training data
train_generator = train_datagen.flow_from_directory(
    dataset_path,                           # path of dataset
    target_size=(img_width, img_height),    # image size
    batch_size=32,
    class_mode='categorical',               # encode labels
    subset='training')                      # training data subset

# generate validation data
validation_generator = train_datagen.flow_from_directory(
    dataset_path,                           # path of dataset
    target_size=(img_width, img_height),    # image size
    batch_size=32,
    class_mode='categorical',               # encode labels
    subset='validation')                    # validation data subset

# create sequential model
model = Sequential([
    Conv2D(32, (3, 3), activation='relu', input_shape=(img_width, img_height, 3)),      # Convolutional layers w/ 32 filters
    MaxPooling2D(2, 2),                     # max pooling layer
    Conv2D(64, (3, 3), activation='relu'),
    MaxPooling2D(2, 2),
    Conv2D(128, (3, 3), activation='relu'),
    MaxPooling2D(2, 2),
    Conv2D(256, (3, 3), activation='relu'),
    MaxPooling2D(2, 2),
    #BatchNormalization(),                   # batch normalization layer
    Conv2D(512, (3, 3), activation='relu'),
    MaxPooling2D(2, 2),
    Flatten(),                              # flatten data for dense layers
    Dense(128, activation='relu'),
    Dropout(0.5),                           # dropout layer with dropout rate of 0.5
    Dense(4, activation='softmax')          # 4 classes
])

# compile model
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

# validating model
history = model.fit(
    train_generator,
    steps_per_epoch=train_generator.samples // train_generator.batch_size,
    epochs=20,          # number of times dataset is passed forward/backward
    validation_data=validation_generator,
    validation_steps=validation_generator.samples // validation_generator.batch_size)

# obtain final training and validation accuracies
train_accuracy = history.history['accuracy'][-1]
val_accuracy = history.history['val_accuracy'][-1]

# print accuracies
print("Training Accuracy: {:.2f}%".format(train_accuracy * 100))
print("Validation Accuracy: {:.2f}%".format(val_accuracy * 100))

# save and store trained model
model.save('deficiency_model.h5')