'''
    Name: Armin Bazarjani
    Student ID: 4430621961
    Class: EE569
    Assignment: Homework 5
    Due Date: 4-14-21

    This is the main homework file that contains the code subproblem (B) of problem 1
'''

# for matrix manipulation
import numpy as np
# deep learning
import torch
from torch import nn
# suppress annoying warnings
import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)
# for keeping track of training progress
from tqdm import tqdm
# import tensorboard for viewing training
from torch.utils.tensorboard import SummaryWriter
# import the load data function
from loadData import load_data
# import lenet5
from lenet5 import LeNet5

# training function
def train(dataset):
    # Hyperparameters
    normalize = [True]
    learning_rates = {"mnist":[0.0001, 0.0005, 0.001],
                      "fashion-mnist":[0.0001, 0.0005, 0.001],
                      "cifar":[0.0001, 0.0005, 0.0008]}
    weight_decays = [0, 0.0005]
    num_epochs = 200

    # create a textfile to store the accuracies of each run
    f = open(dataset+"_accuracies.txt", "w")

    # dictionary for early stopping of training based on accuracy
    early_stop = {"mnist":0.99, 
                  "fashion-mnist":0.90, 
                  "cifar":0.65}

    for norm in normalize:
        for learning_rate in learning_rates[dataset]:
            for decay in weight_decays:
                # read in the correct dataset
                train_loader, test_loader = load_data(dataset, norm)

                # define a new model to train
                leNet = LeNet5(dataset)

                # define the loss and optimizer
                loss_fn = nn.CrossEntropyLoss()
                opt = torch.optim.Adam(params=leNet.parameters(), lr=learning_rate, weight_decay=decay)

                # initialize the summaryWriter
                writer = SummaryWriter(f'runs/{dataset}/Norm: {norm}, LR: {learning_rate}, Decay: {decay}')

                print(f'Training with Norm: {norm}, LR: {learning_rate}, Decay: {decay}...')
                # Loop through all the epochs
                for epoch in range(num_epochs):
                    # initialize tqdm for a nice progress bar
                    loop = tqdm(enumerate(train_loader), total=len(train_loader), leave=False)
                    # initialize correct to 0
                    correct, total = 0, 0
                    # Loop through the dataloader
                    for _, (X, y) in loop:
                        # Prediction error
                        pred = leNet(X)          # Forward pass
                        loss = loss_fn(pred, y)  # Loss calculation

                        # Backpropagation
                        opt.zero_grad()          # Zero the gradient
                        loss.backward()          # Calculate updates
                        
                        # Gradient Descent
                        opt.step()               # Apply updates

                        # check if correct and update the total number correct
                        correct += (pred.argmax(1) == y).type(torch.float).sum().item()

                        # update the total size with the size of the batch
                        total += len(y)

                        # Update progress bar
                        loop.set_description(f"Epoch [{epoch+1}/{num_epochs}]")
                        loop.set_postfix(loss = loss.item())

                    
                    # calculate the training accuracy
                    train_acc = correct/total

                    # get the testing accuracy
                    test_acc = test(test_loader, leNet, loss_fn)

                    # update the tensorboard summarywriter
                    writer.add_scalar("Training Accuracy",train_acc,epoch+1)
                    writer.add_scalar("Testing Accuracy",test_acc,epoch+1)

                    # check early stopping
                    if test_acc >= early_stop[dataset]:
                        break

                # get the final testing accuracy and output to text file
                final_test_acc = test(test_loader, leNet, loss_fn)
                print(f'Final Test Accuracy: {final_test_acc}')
                f.write(f'Model Params [Norm: {norm}, LR: {learning_rate}, Decay: {decay}] - Final Accuracy after {epoch} epochs : {final_test_acc}')
                f.write('\n\n')
                # close the tensorboard writer
                writer.close()
    
    f.close()
    

# testing function
def test(dataloader, model, loss_fn):
    size = len(dataloader.dataset)
    # put the moel in evaluation mode so we aren't storing anything in the graph
    model.eval()
    test_loss, test_acc = 0, 0
    with torch.no_grad():
        for X, y in dataloader:
            # get the prediction
            pred = model(X)
            # add to the loss
            test_loss += loss_fn(pred, y).item()
            # the argmax of the prediction gives us the article of clothing that was predicted, if it matches, add 1 to correct
            test_acc += (pred.argmax(1) == y).type(torch.float).sum().item()
    test_loss /= size
    test_acc /= size
    return test_acc
    #print(f"Test Error: \n Accuracy: {(100*correct):>0.1f}%, Avg loss: {test_loss:>8f} \n")

# function to run training and testing with different hyperparameters on MNIST
def mnist():
    train(dataset="mnist")

# function to run training and testing with different hyperparameters on fashionMNIST
def fashion():
    train(dataset="fashion-mnist")

# function to run training and testing with different hyperparameters on cifar10
def cifar():
    train(dataset="cifar")


# Main function
def main():
    print("Running Experiments on MNIST...")
    mnist()
    print("Running Experiments on Fashion MNIST...")
    fashion()
    print("Running Experiments on Cifar10...")
    cifar()

# Main
if __name__ == "__main__":
    main()