clear all
close all
clc

%% Student Information
% Name: Armin Bazarjani
% Student ID: 4430621961
% Email: bazarjan@usc.edu
% Submission Date: 2-23-21

%% Description
% This is a file for problem 1b of homework 2. 
% In this problem, we are tasked with using an open source implementation
% of the Canny edge detection. I decided to use MATLAB's edge function to
% do this. Please change the path variables and threshold variables.

%% Define the paths (Change these paths if 
% Path for the elephant input image
elephant_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/elephant.jpg'; 
% Path for the elephant output image detailing the edges
elephant_edges = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1b/elephant_edges_Canny_thresh.jpg';

% Path for the ski input image
ski_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/ski_person.jpg'; 
% Path for the ski output image detailing the edges
ski_edges = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1b/ski_edges_Canny_thresh.jpg';

%% Declare thresholds for each image
% *NOTE* The thresholds are currently set at what I found to be the best
% values.
% Also, the threshold values must be between 0 and 1

% elephant thresholds
elephant_thresh_low = 0.18;
elephant_thresh_high = .364;

% ski thresholds
ski_thresh_low = 0.18;
ski_thresh_high = 0.364;

% set thresholds into array to pass into edge function
elephant_thresh = [elephant_thresh_low elephant_thresh_high];
ski_thresh = [ski_thresh_low ski_thresh_high];
%% Read in the input images
% read colored images in
elephant_color = imread(elephant_path);
ski_color = imread(ski_path);

% convert colored images to black and white
elephant_gray = rgb2gray(elephant_color);
ski_gray = rgb2gray(ski_color);

%% imshow to make sure the data was read in properly
figure;
subplot(2,2,1),imshow(elephant_color);
subplot(2,2,2),imshow(ski_color);
subplot(2,2,3),imshow(elephant_gray);
subplot(2,2,4),imshow(ski_gray);

%% Run Canny edge detection
edge_elephant = edge(elephant_gray, 'canny', elephant_thresh);
edge_ski = edge(ski_gray, 'canny', ski_thresh);


%% Output results of edge detection
figure;
subplot(2,2,1),imshow(edge_elephant);
subplot(2,2,2),imshow(edge_ski);
subplot(2,2,3),imshow(elephant_gray);
subplot(2,2,4),imshow(ski_gray);

%% Write out the output image
imwrite(edge_elephant, elephant_edges);
imwrite(edge_ski, ski_edges);





