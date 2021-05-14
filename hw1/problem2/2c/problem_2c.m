clear all;
close all;
clc;
% Armin Bazarjani
% 4430621961
% bazarjan@usc.edu
% 2-7-21
%% Description
% This is a file for problem 2c of homework 1. 
% In this problem, we are tasked with using an open source implementation
% of the Non-Local Means (NLM) filter. I decided to use MATLAB's open
% source implementation.


%% Define the paths
% Path for the noisy input image
fruits_noisy_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework1/HW1_images/Fruits_gray_noisy.raw"; 
% Path for the denoised output image
fruits_denoised_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework1/problem2/fruits_nlm.raw";


%% Read in the noisy image
image_data = readraw(fruits_noisy_path, 400, 500, 1);


%% Define hyperparameters to use with NLM
% Personal Best PSNR (32.1126) with: 11.85, 17, 3
% Degrees of smoothing (positive double)
DoS = 11.85;
% Search window size (odd valued positive integer)
SwS = 17;
% Comparison window size (odd valued positive integer)
CwS = 3;


%% Run NLM
[filteredImage, estDoS] = imnlmfilt(image_data,'DegreeOfSmoothing',DoS,'SearchWindowSize',SwS,'ComparisonWindowSize',CwS);


%% Write out the denoised image
writeraw(fruits_denoised_path, filteredImage);