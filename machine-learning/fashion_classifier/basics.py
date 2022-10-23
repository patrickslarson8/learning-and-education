#Imports
import keras.layers
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

#Define the data
(x_train, y_train), (x_test, y_test) = tf.keras.datasets.fashion_mnist.load_data()
plt.imshow(x_train[0])
#Uncomment this to show when using pycharm
#plt.show()
x_train = x_train/255.0
x_test = x_test/255.0

#Define the model
model = tf.keras.models.Sequential([
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(512, activation=tf.nn.relu),
    tf.keras.layers.Dense(10, activation=tf.nn.softmax)
])

#Define callbacks
class accuracyCallback(tf.keras.callbacks.Callback):
    def on_epoch_end(self, epoch, logs={}):
        if (logs.get('accuracy') >=.99 ):
            self.model.stop_training = True

callbacks = accuracyCallback()

#Compile the model
model.compile(
    optimizer=tf.optimizers.Adam(),
    loss=tf.losses.sparse_categorical_crossentropy,
    metrics=['accuracy']
)

#train the model
model.fit(
    x_train, y_train,
    epochs=35,
    verbose=1,
    callbacks=[callbacks]
)

#Evaluate with the model
print(model.evaluate(x_test, y_test))
