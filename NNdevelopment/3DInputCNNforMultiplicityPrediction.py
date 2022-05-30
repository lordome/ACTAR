    #!pip install uproot
    #!pip install awkward
    #import uproot

    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    from sklearn.model_selection import train_test_split

    from tensorflow import keras
    import tensorflow as tf
    #import tensorflow_datasets as tfds

    from tensorflow.keras import layers
    from tensorflow.keras.layers import Conv1D, MaxPooling1D, AveragePooling1D
    from tensorflow.keras.utils import to_categorical
    from tensorflow.keras import regularizers

    from tensorflow.keras.callbacks import EarlyStopping, ReduceLROnPlateau
    from tensorflow.keras.preprocessing.image import ImageDataGenerator
    from tensorflow.keras.applications.vgg16 import VGG16
    from tensorflow.keras import optimizers
    from tensorflow.keras.optimizers import Adam
    from tensorflow.keras.applications.vgg16 import preprocess_input
    from tensorflow.keras.models import load_model


    def CNNDefinition(fil, k_size,  Poolsize, dense,
                    input_shape, N_categ,  loss='categorical_crossentropy',
                    metrics=['accuracy'], last_act="softmax", lmb=0.01, l_rate=0.001):
        """
        This CNNDefinition function allows the definition of a CNN network. The Network defined can be exploited for both 
        a regression and for a classification task.

        Parameters ---
        fil : list of the number of filters per each convolutional layer,
        k_size : dimension of the kernels related to each convolutional layer,
        Poolsize : dimension of the pooling layer applied after each convolutional one,
        dense : list of dimensions of dense layers applied after the convolutional step, 
        input_shape :  dimension of the input data , 
        N_categ : number of categories in the classification task (has to be set at 1 for regression),
        loss = 'categorical_crossentropy',  
        metrics = ['accuracy'], 
        last_act : default = "sigmoid", ok for classification. Has to be changed for regression
        """

        model = keras.Sequential()
        model.add(layers.Conv2D(filters=fil[0], kernel_size=k_size[0],
                                padding="same",
                                activation="relu",
                                #strides = 2,
                                input_shape=input_shape,
                                kernel_regularizer=regularizers.l2(lmb)
                                ))
        model.add(layers.MaxPooling2D(Poolsize[0]))
        #   model.add(layers.BatchNormalization())
        #   model.add(layers.SpatialDropout2D(0.3))

        for i in range(1, len(fil)):
            model.add(layers.Conv2D(filters=fil[i], kernel_size=k_size[i],
                                    padding="same", activation="relu", kernel_regularizer=regularizers.l2(lmb)))
        # model.add(layers.BatchNormalization())

            model.add(layers.MaxPooling2D(Poolsize[i]))
        #  model.add(layers.SpatialDropout2D(0.3))

        model.add(layers.Flatten())
        for i in range(len(dense)):
            model.add(layers.Dense(
                dense[i], activation="relu", kernel_regularizer=regularizers.l2(lmb)))

        # model.add(layers.Dropout(0.2))
        model.add(layers.Dense(N_categ, activation=last_act))

        print(model.summary())

        # default optimizer is Adam, different learning rates affect the time the network takes to converge.
        # try different alogrithms and learning rates
        optimizer = keras.optimizers.Adam()
        #optimizer = keras.optimizers.SGD(learning_rate=l_rate)

        optimizer.learning_rate.assign(l_rate)

        model.compile(
            optimizer=optimizer,
            loss=loss,
            metrics=metrics
        )

        return model


    def Fit(model, X_train, Y_train, X_test, Y_test, pat=100, mindelta=0.01, batch=64, epochs=1200, verb=1):
        early_stopping = keras.callbacks.EarlyStopping(
            patience=pat,
            min_delta=mindelta,
            restore_best_weights=True,
        )

        history = model.fit(
            X_train, Y_train,
            validation_data=(X_test, Y_test),
            batch_size=batch,
            epochs=epochs,
            callbacks=[early_stopping],
            verbose=1,
        )

        return pd.DataFrame(history.history)






    #Import data (in mock test - only 200 hits)

    n_data = 20
    test_s = 0.15
    X_train, X_test, y_train, y_test = train_test_split((np.fromfile("3DdataNN.bin").reshape(n_data,128,128,3)), np.loadtxt("vertSize.txt", max_rows  = n_data), test_size=test_s)

    print(X_train.shape)
    ### PREPROCESSING
    X_train = np.array(X_train).reshape(X_train.shape[0] ,X_train.shape[1], X_train.shape[2],3)
    X_test = np.array(X_test).reshape(X_test.shape[0],X_test.shape[1], X_test.shape[2],3)
    # X_train/=X_train.max()
    # X_test/=X_test.max()
    # X_train = np.array([i/i.max() for i in X_train])
    # X_test = np.array([i/i.max() for i in X_test])

    numberCategories = max(np.concatenate((y_train, y_test)))+1
    y_train = keras.utils.to_categorical(y_train)
    y_test = keras.utils.to_categorical(y_test)


    #Network characteristic definition
    l_fil = [128, 128, 64]
    l_ker = [5, 3, 2]
    l_pol = [2,2,3]
    l_dense = []
    model = CNNDefinition(l_fil, l_ker, l_pol, l_dense, (128,128,3), numberCategories, l_rate = 0.01)

    #Fit network accordingly to data loaded in previous step
    history = Fit(model, X_train, y_train, X_test, y_test, batch = 64, epochs = 40)
    plt.plot(history.accuracy)
    plt.plot(history.val_accuracy)
    plt.show()



    #Save model - change name accordingly to input data characteristics
    model.save('model_3DSimple_LargeTest_InputShape128_128_3_Accuracy85Ransac_trainSize30000x85.h5')






