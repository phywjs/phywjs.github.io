# To use tensorflow we need to install the package with the command under DOS
# python -m pip install tensorflow
# and similarly for the ploting package matplotlib.  After that, we can import
# them.
import tensorflow as tf
import matplotlib.pyplot as plt

mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()
# here x_train is 60000x28x28 ndarray each drawing is 28x28 byte.
# here y_train is 1D array of 60000, tell you which figure 0 to 9 it is.

d = x_train[0]
# d is 28x28 numpy ndarray
print("this is figure ", y_train[0])
plt.imshow(d)
plt.show()

# call model.compile to define the model
# call model.fit to train the model
# call model.evaluate to test the model
# call model.predict to prediction result



