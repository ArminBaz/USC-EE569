'''
    Name: Armin Bazarjani
    Student ID: 4430621961
    Class: EE569
    Assignment: Homework 5
    Due Date: 4-14-21

    This is a class I made to abstract away the LeNet5 model so I don't have to construct it
    inside of the main homework file.
'''

import torch
from torch import nn
import torch.nn.functional as F

# Construct the leNet5 class
class LeNet5(nn.Module):
    # ask for number of input channels because MNIST are B&W and CIFAR is RGB
    def __init__(self, dataset):
        super(LeNet5, self).__init__()
        # define a dictionary for desired parameters based on the dataset
        self.param_dict = {"mnist":[1,2],
                           "fashion-mnist":[1,2],
                           "cifar":[3,0]}
        # first conv layer (Added a padding because)
        self.conv1 = nn.Conv2d(in_channels=self.param_dict[dataset][0], out_channels=6, kernel_size=5, stride=1, padding=self.param_dict[dataset][1])
        # second conv layer
        self.conv2 = nn.Conv2d(in_channels=6, out_channels=16, kernel_size=5, stride=1)
        # first fully connected layer
        self.fc1 = nn.Linear(in_features=16*5*5, out_features=120)
        # second fully connected layer
        self.fc2 = nn.Linear(in_features=120, out_features=84)
        # third/output fully connected layer
        self.fc3 = nn.Linear(in_features=84, out_features=10)
    
    # forward function
    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)), (2,2))
        x = F.max_pool2d(F.relu(self.conv2(x)), 2)
        x = x.view(x.size(0), -1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        out = self.fc3(x)

        return out