clear all;
close all;
clc

%% Student Information
% Name: Armin Bazarjani
% Student ID: 4430621961
% Email: bazarjan@usc.edu
% Submission Date: 2-23-21

%% Description
% This is a file for problem 1c of homework 2. 
% In this problem, we are tasked with using an open source implementation
% of the Structured Edge edge detection.

%% Define the paths (Change these paths if grading)
% Path for the elephant input image
elephant_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/elephant.jpg'; 
% Path for the elephant output image detailing the edges
elephant_edges = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/elephant_edges_SE.jpg';
% Path for .mat file for elephant SE matrix
elephant_mat = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/elephant_SE.mat';

% Path for the ski input image
ski_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/ski_person.jpg'; 
% Path for the ski output image detailing the edges
ski_edges = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/sk_edges_SE.jpg';
% Path for .mat file for ski SE matrix
ski_mat = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/ski_SE.mat';

% Path to save the binary images
elephant_binary_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/elephant_SE_thresh.jpg';
ski_binary_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/ski_SE_thresh.jpg';

%% Define threshold for binary map
elephant_thresh = 0.18;
ski_thresh= 0.18;

%% Read in images
elephant_I = imread(elephant_path);
ski_I = imread(ski_path);


%% Run structured Edge
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
model=edgesTrain(opts);           % will load model if already trained

model.opts.multiscale=1;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=20;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=1;                 % set to true to enable nms

%% detect edge and visualize results for elephant
elephant_SE = edgesDetect(elephant_I, model);
figure(1); im(elephant_I); figure(2); im(1-elephant_SE);


%% detect edge and visualize results for ski
ski_SE = edgesDetect(ski_I, model);
figure(3); im(ski_I); figure(4); im(1-ski_SE);


%% Write out the edge images
imwrite(ski_SE, ski_edges);
imwrite(elephant_SE, elephant_edges);


%% Write out the edge matrices
save(elephant_mat, 'elephant_SE');
save(ski_mat, 'ski_SE');

%% binarize edge maps based on threshold
ski_binary = zeros(481, 321);
elephant_binary = zeros(321, 481);
for i = 1 : 481
   for j = 1 : 321
      % binarize ski
      if ski_SE(i,j) >= ski_thresh
          ski_binary(i,j) = 255;
      else
        ski_binary(i,j) = 0;
      end
      % binarize elephant
      if elephant_SE(j,i) >= elephant_thresh
          elephant_binary(j,i) = 255;
      else
          elephant_binary(j,i) = 0;
      end
   end
end

%% Print out binary matrices
figure(1); imshow(elephant_binary);
figure(2); imshow(ski_binary);

%% Save the binary images
imwrite(ski_binary, ski_binary_path);
imwrite(elephant_binary, elephant_binary_path);
