'''
    Name: Armin Bazarjani
    Student ID: 4430621961
    Class: EE569
    Assignment: Homework 5
    Due Date: 4-14-21

    This is a class I made to help me load in the different datasets and whether or not I want to 
    apply different transformations to the datasets.
'''

import torch
from torchvision import datasets
from torchvision import transforms

# Function to read in specified dataset
def load_data(dataset, normalize):
    if dataset == "fashion-mnist" or dataset == "mnist":
        # Normalize the training dataset with some augmentations
        transform_train = transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.485,), (0.229,))
        ])
        # Normalize the test set same as training wet without augmentation
        transform_test = transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.485,), (0.229,))
        ])
        if normalize is True:
            if dataset == "fashion-mnist":
                trainset = datasets.FashionMNIST(root="data", train=True, download=True, transform=transform_train)
                testset = datasets.FashionMNIST(root="data", train=False, download=True, transform=transform_test)
            elif dataset == "mnist":
                trainset = datasets.MNIST(root="data", train=True, download=True, transform=transform_train)
                testset = datasets.MNIST(root="data", train=False, download=True, transform=transform_test)
        else:
            if dataset == "fashion-mnist":
                trainset = datasets.FashionMNIST(root="data", train=True, download=True, transform=None)
                testset = datasets.FashionMNIST(root="data", train=False, download=True, transform=None)
            elif dataset == "mnist":
                trainset = datasets.MNIST(root="data", train=True, download=True, transform=None)
                testset = datasets.MNIST(root="data", train=False, download=True, transform=None)
        
        
    elif dataset == "cifar":
        transform_train = transforms.Compose([
            transforms.ToTensor(), 
            transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
        ])

        # Normalize the test set same as training set without augmentation
        transform_test = transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
        ])
        
        if normalize is True:
            trainset = datasets.CIFAR10(root="data", train=True, download=True, transform=transform_train)
            testset = datasets.CIFAR10(root="data", train=False, download=True, transform=transform_test)
        else:
            trainset = datasets.CIFAR10(root="data", train=True, download=True, transform=None)
            testset = datasets.CIFAR10(root="data", train=False, download=True, transform=None)
        
    # load the data into loaders to use for the model
    if dataset == "mnist" or dataset == "fashion-mnist":
        trainloader=torch.utils.data.DataLoader(trainset, batch_size=64, shuffle=True, num_workers=2)
    elif dataset == "cifar":
        trainloader=torch.utils.data.DataLoader(trainset, batch_size=100, shuffle=True, num_workers=2)
    testloader=torch.utils.data.DataLoader(testset, batch_size=64, shuffle=False, num_workers=2)
    
    # return the train and test loaders
    return trainloader, testloader