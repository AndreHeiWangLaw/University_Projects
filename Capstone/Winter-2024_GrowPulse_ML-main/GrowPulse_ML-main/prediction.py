import firebase_admin
from firebase_admin import credentials, storage, db
import tensorflow as tf
from tensorflow.keras.preprocessing import image
import numpy as np
import schedule
import time
import os

def get_latest_image_path(user_id):
    # Fetch list of files in the specified Firebase Storage path
    bucket = storage.bucket()
    blobs = bucket.list_blobs(prefix=f'Registered Users/{user_id}/SystemB/')

    # Extract image paths and metadata
    image_info = [(blob.name, blob.metadata) for blob in blobs if blob.name.lower().endswith(('.jpg', '.jpeg', '.png'))]

    # Sort the image paths based on the creation time (assuming Firebase maintains order)
    sorted_image_info = sorted(image_info, key=lambda x: x[1].get('timeCreated', 0), reverse=True)

    if sorted_image_info:
        # Get the path of the latest image
        latest_image_path = sorted_image_info[0][0]

        # fetch image from "Firebase Storage"
        blob = bucket.blob(latest_image_path)
        image_blob = blob.download_as_bytes()

        # save the image locally
        local_image_path = 'temp_image.jpg'
        with open(local_image_path, 'wb') as f:
            f.write(image_blob)

        return local_image_path  # Return the path of the locally saved image
    else:
        return None

def predict_result_firebase(image_path):
    # Check if the downloaded image file exists
    if not os.path.exists(image_path):
        print("No image found for prediction.")
        return None

    # dimension of images
    img_width, img_height = 150, 150

    # load and preprocess the image
    img = image.load_img(image_path, target_size=(img_width, img_height))
    img_tensor = image.img_to_array(img)
    img_tensor = np.expand_dims(img_tensor, axis=0)
    img_tensor /= 255.

    # predict the image using the downloaded model
    prediction = model.predict(img_tensor)

    # four classes
    class_names = ['FN', 'N', 'P', 'K']

    # class with the highest probability
    predicted_class = class_names[np.argmax(prediction)]

    return predicted_class

def upload_result_to_firebase(user_id, result):
    try:
        database = db.reference(f'/Registered Users/{user_id}/SystemA/mlResults')   # initialize the Realtime Database
        database.set(result)    # upload the result
        print("Result uploaded successfully.")
    except Exception as e:
        print(f"Error uploading result to Firebase: {e}")

def prediction_task():
    # Get the latest image path from Firebase Storage
    local_image_path = get_latest_image_path(user_id)

    if local_image_path:
        # Predict result from Firebase Storage Image
        prediction = predict_result_firebase(local_image_path)

        if prediction is not None:
            print("Predicted Results:", prediction)
            # Upload the result to Realtime Database
            upload_result_to_firebase(user_id, {'prediction': prediction})
        else:
            print("Prediction failed.")
    else:
        # Upload the result to Realtime Database
        upload_result_to_firebase(user_id, {'prediction': "No Image Found"})
        print("No image found for prediction.")

# firebase credentials
cred = credentials.Certificate('firebase_cred.json')
firebase_admin.initialize_app(cred, {
    'storageBucket': 'growpulse-d7198.appspot.com',
    'databaseURL': 'https://growpulse-d7198-default-rtdb.firebaseio.com'})
user_id = 't739mbVeRScrPUz6vzp09DHU72n1'    # firebase user id

# load the trained model
model = tf.keras.models.load_model('deficiency_model.h5')

# Schedule the job to run every X seconds
schedule.every(5).seconds.do(prediction_task)

# Run the scheduled jobs
while True:
    schedule.run_pending()
    time.sleep(1)