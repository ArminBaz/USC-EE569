'''
    Name: Armin Bazarjani
    Student ID: 4430621961
    Class: EE569
    Assignment: Homework 5
    Due Date: 4-14-21

    This is a class I made for problem 1 (C) to help me with loading in the negative
    values into train and test sets for mnist
'''

import torch
from torchvision import datasets
from torchvision import transforms
# for negative transform
import PIL.ImageOps

# Construct a class for a custom transform
class Negative(object):
    def __init__(self):
        pass
    
    def __call__(self, img):
        img = 1-img
        return img

# Function to read in MNIST without inverting the images
def load_positive():
    # Normalize the training dataset with some augmentations
    transform_train = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1285,), (0.3057,))
    ])
    # Normalize the test set same as training wet without augmentation
    transform_test = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1285,), (0.3057,))
    ])

    trainset = datasets.MNIST(root="data", train=True, download=True, transform=transform_train)
    testset = datasets.MNIST(root="data", train=False, download=True, transform=transform_test)
    
    trainloader=torch.utils.data.DataLoader(trainset, batch_size=64, shuffle=True, num_workers=2)
    testloader=torch.utils.data.DataLoader(testset, batch_size=64, shuffle=False, num_workers=2)
    
    # return the train and test loaders
    return trainloader, testloader, trainset, testset

# Function to read in MNIST with the images inverted
def load_negative():
    # Normalize the training dataset with some augmentations
    transform_train = transforms.Compose([
        transforms.ToTensor(),
        Negative(), # New negative transform class
        transforms.Normalize((0.8654,), (0.3117,))
    ])
    # Normalize the test set same as training wet without augmentation
    transform_test = transforms.Compose([
        transforms.ToTensor(),
        Negative(), # New negative transform class
        transforms.Normalize((0.8654,), (0.3117,))
    ])

    trainset = datasets.MNIST(root="data", train=True, download=True, transform=transform_train)
    testset = datasets.MNIST(root="data", train=False, download=True, transform=transform_test)
    
    trainloader=torch.utils.data.DataLoader(trainset, batch_size=64, shuffle=True, num_workers=2)
    testloader=torch.utils.data.DataLoader(testset, batch_size=64, shuffle=False, num_workers=2)
    
    # return the train and test loaders
    return trainloader, testloader, trainset, testset

def load_concat():
    # load the datasets
    _, _, train_pos, test_pos = load_positive()
    _, _, train_neg, test_neg = load_negative()
    
    # concatenate the datasets
    concat_train = torch.utils.data.ConcatDataset([train_pos, train_neg]) 
    concat_test = torch.utils.data.ConcatDataset([test_pos, test_neg])
    
    # construct dataloaders on the concatenated datasets
    trainloader = torch.utils.data.DataLoader(concat_train, batch_size=64, shuffle=True, num_workers=2)
    testloader = torch.utils.data.DataLoader(concat_test, batch_size=64, shuffle=True, num_workers=2)
    
    return trainloader, testloader