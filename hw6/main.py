import numpy as np
from tensorflow.keras.datasets import mnist,fashion_mnist
from skimage.util import view_as_windows
from pixelhop import Pixelhop
from skimage.measure import block_reduce
from sklearn.metrics import accuracy_score
import xgboost as xgb
import warnings, gc
# suppress warnings
warnings.simplefilter(action='ignore', category=FutureWarning)
import time


np.random.seed(1)

# Preprocess
N_Train_Reduced = 10000    # 10000
N_Train_Full = 60000     # 50000
N_Test = 10000            # 10000

BS = 2000 # batch size


def shuffle_data(X, y):
    shuffle_idx = np.random.permutation(y.size)
    X = X[shuffle_idx]
    y = y[shuffle_idx]
    return X, y


def select_balanced_subset(images, labels, use_num_images):
    '''
    select equal number of images from each classes
    '''
    num_total, H, W, C = images.shape
    num_class = np.unique(labels).size
    num_per_class = int(use_num_images / num_class)

    # Shuffle
    images, labels = shuffle_data(images, labels)

    selected_images = np.zeros((use_num_images, H, W, C))
    selected_labels = np.zeros(use_num_images)

    for i in range(num_class):
        selected_images[i * num_per_class:(i + 1) * num_per_class] = images[labels == i][:num_per_class]
        selected_labels[i * num_per_class:(i + 1) * num_per_class] = np.ones((num_per_class)) * i

    # Shuffle again
    selected_images, selected_labels = shuffle_data(selected_images, selected_labels)

    return selected_images, selected_labels

def Shrink(X, shrinkArg):
    #---- max pooling----
    pool = shrinkArg['pool']
    # TODO: fill in the rest of max pooling
    # Make two different things (if pool is True or if pool is False)
    # if False do nothing, if True, do 2x2 max-pooling
    if pool is False:
        pass
    elif pool is True:
        N, H, W, C = X.shape
        pool_height, pool_width = 2, 2
        stride = 2
        x_reshaped = X.reshape(N, H // pool_height, pool_height,
                            W // pool_width, pool_width, C)
        out = x_reshaped.max(axis=2).max(axis=3)

    #---- neighborhood construction
    win = shrinkArg['win']
    stride = shrinkArg['stride']
    pad = shrinkArg['pad']
    ch = X.shape[-1]

    # code from discussion
    # pad
    if pad > 0:
        out = np.pad(out,((0,0), (pad,pad), (pad,pad), (0,0)), 'reflect')

    # neighborhood construction
    out = view_as_windows(out, (1,win,win,ch), (1,stride,stride,ch))

    # return array
    return out.reshape(out.shape[0], out.shape[1], out.shape[2], -1)

# example callback function for how to concate features from different hops
def Concat(X, concatArg):
    return X

def get_feat(X, num_layers=3):
    output = p2.transform_singleHop(X,layer=0)
    if num_layers>1:
        for i in range(num_layers-1):
            output = p2.transform_singleHop(output, layer=i+1)
    return output


if __name__ == "__main__":
    warnings.filterwarnings("ignore")
    # ---------- Load MNIST data and split ----------
    (x_train, y_train), (x_test,y_test) = mnist.load_data()


    # -----------Data Preprocessing-----------
    x_train = np.asarray(x_train,dtype='float32')[:,:,:,np.newaxis]
    x_test = np.asarray(x_test,dtype='float32')[:,:,:,np.newaxis]
    y_train = np.asarray(y_train,dtype='int')
    y_test = np.asarray(y_test,dtype='int')

    # if use only 10000 images train pixelhop
    x_train_reduced, y_train_reduced = select_balanced_subset(x_train, y_train, use_num_images=N_Train_Reduced)

    x_train /= 255.0
    x_test /= 255.0

    print('Running Module 1...')
    # -----------Module 1: set PixelHop parameters-----------
    # Setup the Shrink Arguments
    shrinkArgs = [{'func':Shrink, 'win':5, 'stride':1, 'pad':2, 'pool':False},
                   {'func':Shrink, 'win':5, 'stride':1, 'pad':0, 'pool':True},
                   {'func':Shrink, 'win':5, 'stride':1, 'pad':0, 'pool':True}]
    # Setup the Saab Arguments
    SaabArgs = [{'num_AC_kernels':-1, 'needBias':False, 'cw':False},
                {'num_AC_kernels':-1, 'needBias':True, 'cw':True},
                {'num_AC_kernels':-1, 'needBias':True, 'cw':True}]
    # Setup Concat Arguments
    concatArg = {'func':Concat}
    
    # -----------Module 1: Train PixelHop -----------
    # Construct PixelHop++ model
    p2 = Pixelhop(depth=3, TH1=0.002, TH2=0.0005, 
                SaabArgs=SaabArgs, shrinkArgs=shrinkArgs, concatArg=concatArg)

    # Train PixelHop++ on the reduced subset
    p2.fit(x_train_reduced)


    print('Running Module 2...')
    # --------- Module 2: get only Hop 3 feature for both training set and testing set -----------
    # you can get feature "batch wise" and concatenate them if your memory is restricted
    train_hop3_feats = get_feat(x_train)
    test_hop3_feats = get_feat(x_test)
    
    
    # --------- Module 2: standardization
    STD = np.std(train_hop3_feats, axis=0, keepdims=1)
    train_hop3_feats = train_hop3_feats/STD
    test_hop3_feats = test_hop3_feats/STD

    # Reshape the features for the next module
    N_train, _, _, X_train = train_hop3_feats.shape
    N_test, _, _, X_test = test_hop3_feats.shape
    train_hop3_reshaped = np.reshape(train_hop3_feats, (N_train, X_train))
    test_hop3_reshaped = np.reshape(test_hop3_feats, (N_test, X_test))
    

    print('Running Module 3...')
    #---------- Module 3: Train XGBoost classifier on hop3 feature ---------

    tr_acc = []
    te_acc = []
    
    clf = xgb.XGBClassifier(n_jobs=-1,
                        objective='multi:softprob',
                        # tree_method='gpu_hist', gpu_id=None,
                        max_depth=6,n_estimators=100,
                        min_child_weight=5,gamma=5,
                        subsample=0.8,learning_rate=0.1,
                        nthread=8,colsample_bytree=1.0)

    print('fitting xgboost...')
    # fit xgboost model
    clf.fit(train_hop3_reshaped, y_train)

    # use the xgboost model to make a prediction on the test set
    pred_train = clf.predict(train_hop3_reshaped)
    pred_test = clf.predict(test_hop3_reshaped)

    # get the accuracy score
    acc_train = accuracy_score(y_train, pred_train)
    acc_test = accuracy_score(y_test, pred_test)