### Student Information
Name: Armin Bazarjani
Class: EE-569 Introduction to Digital Image Processing
Date: March 28th, 2021
Assignment: Homework 4


### Tech Information
I am running everything on MacOS (Catalina Version 10.15.7).
To run the python code have these libraries installed:
 - torch
 - torchvision
 - matplotlib
 - tqdm

 ### File Structure
homework 1
         |
         |> Problem 1
         |          |> 1b (main file: problem_1b.py)
         |          |> 1c (main file: problem_1c.py)
         |          |> Extra files
         |          |          |> loadData.py (data loader for problem b)
         |          |          |> loadDataNegative.py (data loader for problem c)
         |          |          |> lenet5.py (contains LeNet-5 PyTorch class)
         |          |          |> hw5_playground.ipynb (jupyter notebook file where I test some things out)

### How to Run
Everything inside of the python files should be self-contained. So, as long as you have all
of the afformentioned packages installed, you should be fine.


### Notes
The scripts all construct different tensorboard SummaryWriter instances to keep track of training progress. 
They will all be stored in a run/ directory. Also, all of the data will be read in accordingly and stored
in a data/ directory.