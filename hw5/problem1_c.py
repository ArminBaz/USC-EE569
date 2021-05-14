'''
    Name: Armin Bazarjani
    Student ID: 4430621961
    Class: EE569
    Assignment: Homework 5
    Due Date: 4-14-21

    This is the main homework file that contains the code subproblem (C) of problem 1
'''
# import necessary libraries
import matplotlib.pyplot as plt
import torch
from torch import nn
from tqdm import tqdm
from torch.utils.tensorboard import SummaryWriter
from loadDataNegative import load_positive, load_negative, load_concat
from lenet5 import LeNet5
# suppress warnings
import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

# training function
def train(train_loader, test_loader, model, opt, loss_fn, num_epochs, summary_writer_name):
    # construct the summary writer
    writer = SummaryWriter(f'runs/{summary_writer_name}')
    # iterate over all epochs
    for epoch in range(num_epochs):
        loop = tqdm(enumerate(train_loader), total=len(train_loader), leave=False)
        # initialize correct to 0
        correct, total = 0, 0
        # Loop through the dataloader
        for _, (X, y) in loop:
            # Prediction error
            pred = model(X)          # Forward pass
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
        test_acc = test(test_loader, model, loss_fn)

        # update the tensorboard summarywriter
        writer.add_scalar("Training Accuracy",train_acc,epoch+1)
        writer.add_scalar("Testing Accuracy",test_acc,epoch+1)

# testing function that returns the accuracy
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

# run part 1 (test negative images on leNet5 that was trained on positive images)
def part1():
    print('Running part 1...')
    # load in the positive and negative data
    train_loader_pos, test_loader_pos, _, _ = load_positive()
    _, test_loader_neg, _, _ = load_negative()

    # define the model
    leNet = LeNet5("mnist")

    # define the loss and optimizer
    loss_fn = nn.CrossEntropyLoss()
    opt = torch.optim.Adam(params=leNet.parameters(), lr=0.001, weight_decay=0)

    # train the leNet for 10 epochs
    train(train_loader_pos, test_loader_pos, leNet, opt, loss_fn, 10, "leNet5_part1_pos")

    # get the test accuracy on the negative labels
    neg_acc = test(test_loader_neg, leNet, loss_fn)
    pos_acc = test(test_loader_pos, leNet, loss_fn)
    print(f'The normal test accuracy is: {pos_acc}')
    print(f'The accuracy on negative data is: {neg_acc}')


# run part 2 (train the model using both positive and negative images)
def part2():
    print('Running part 2...')
    # load the concatenated datasets
    concat_train, concat_test = load_concat()

    # load the negative testset
    _, neg_test, _, _ = load_negative()

    # define lenet
    leNet = LeNet5("mnist")

    # define the loss and optimizer
    loss_fn = nn.CrossEntropyLoss()
    opt = torch.optim.Adam(params=leNet.parameters(), lr=0.001, weight_decay=0)

    # train on concatenated dataset for 10 epochs
    train(concat_train, concat_test, leNet, opt, loss_fn, 10, "leNet_concat")

    # print out final testing results
    concat_acc = test(concat_test, leNet, loss_fn)
    neg_acc = test(neg_test, leNet, loss_fn)
    print(f'The accuracy of concatenated training on concatenated testset is: {concat_acc}')
    print(f'The accuracy of concatenated training on negative dataset alone is: {neg_acc}')

# main function
def main():
    part1()
    part2()

if __name__ == "__main__":
    main()

