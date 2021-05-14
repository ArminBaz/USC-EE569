clear all
close all
clc

%% Student Information
% Name: Armin Bazarjani
% Student ID: 4430621961
% Email: bazarjan@usc.edu
% Submission Date: 2-23-21

%% Description
% This is a file for problem 1d of homework 2. 
% In this problem we have to output the precision and recall of our edges
% given ground truth images.

%% Set elephant and ski height and width
elephant_height = 321;
elephant_width = 481;
elephant_channels = 1;

ski_height = 481;
ski_width = 321;
ski_channels = 1;

%% Define path to output from sobel filter (Change if Grading)
% Because the sobel filter was designed in c++, we have to read the
% gradient magnitude for each image (elephant and ski) by themselves.

% NOTE: I didn't include a Canny path because it requires to be processed
% in real time due to the fact that I have construct the edges from the
% start. For this reason, Canny will be seperate entirely.

% elephant sobel path
elephant_sobel_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1a/elephant_sobel.txt';
% ski sobel path
ski_sobel_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1a/ski_sobel.txt';


% elephant structured edge path
elephant_SE_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/elephant_SE.mat';
% ski structured edge path
ski_SE_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/problem1/1c/ski_SE.mat';

% elephant ground truth path
elephant_gt = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/elephant_GT.mat';
% ski ground truth path
ski_gt = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/ski_person_GT.mat';


%% Define the number of thresholds
thresh_num = 10;

%% Read in the sobel and structured edge results into matrices
elephant_sobel = load(elephant_sobel_path);
ski_sobel = load(ski_sobel_path);

ski_SE = load(ski_SE_path);
ski_SE = ski_SE.ski_SE;

elephant_SE = load(elephant_SE_path);
elephant_SE = elephant_SE.elephant_SE;


%% Normalize the sobel filters
for i = 1 : 481
   for j = 1 : 321
      ski_sobel(i,j) = ski_sobel(i,j)/(255);
      elephant_sobel(j,i) = elephant_sobel(j,i)/(255);
   end
end


%% Part (1) calulating mean precision and recall for each GT
% Example: es_F_1 is shorthand for elephant_sobel_FScore_part1
[eS_meanP, eS_meanR, eS_F_1, eS_P_vec, eS_R_vec] = prf_1(elephant_gt, elephant_sobel);
[sS_meanP, sS_meanR, sS_F_1, sS_P_vec, sS_R_vec] = prf_1(ski_gt, ski_sobel);

[eSE_meanP, eSE_meanR, eSE_F_1, eSE_P_vec, eSE_R_vec] = prf_1(elephant_gt, elephant_SE);
[sSE_meanP, sSE_meanR, sSE_F_1, sSE_P_vec, sSE_R_vec] = prf_1(ski_gt, ski_SE);



%% Part (2) calculating mean precision and recall for each threshold across each GT
[es_F_2, es_T_2] = prf_2(elephant_gt, elephant_sobel, thresh_num);
[ss_F_2, ss_T_2] = prf_2(ski_gt, ski_sobel, thresh_num);

[eSE_F_2, eSE_T_2] = prf_2(elephant_gt, elephant_SE, thresh_num);
[sSE_F_2, sSE_T_2] = prf_2(ski_gt, ski_SE, thresh_num);


%%  Read in original image and convert rgb2gray for Canny
% define the paths to the original images
elephant_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/elephant.jpg';
ski_path = '/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework2/HW2_materials/ski_person.jpg';

% read in colored images
elephant_color = imread(elephant_path);
ski_color = imread(ski_path);

% convert colored images to black and white
elephant_gray = rgb2gray(elephant_color);
ski_gray = rgb2gray(ski_color);


%% Canny Part (1)
% For the first part, we will simply run the Canny edge detection without
% defining any threshold values
elephant_canny = edge(elephant_gray, 'canny');
ski_canny = edge(ski_gray, 'canny');

[eC_meanP_1, eC_meanR_1, eC_F_1, eC_P_1, eC_R_1] = prf_1(elephant_gt, elephant_canny);
[sC_meanP_1, sC_meanR_1, sC_F_1, sC_P_1, sC_R_1] = prf_1(ski_gt, ski_canny);
%% Canny Part (2)
% For the second part, we will threshold Canny with high and low values
% based on each threshold we used for the previous problem
% 
% Thus, if we have 10 thresholds, we will have 45 unique high and low
% combinations.

% calculate the number of low/high pairs
new_thresh_num = thresh_num+1;
num_pairs = nchoosek(new_thresh_num, 2);
% holders for the low high pars
low = zeros(num_pairs,1);
high = zeros(num_pairs,1);
% rename thresholds for ease of use
thresholds = zeros(new_thresh_num,1);
thresholds(2:new_thresh_num) = es_T_2;
thresholds(1) = 0;
% counter to keep track of location in low/high pairs
count = 1;

% loop through the thresholds and construct low and high pairs
for i = 1 : new_thresh_num-1
    for j = i+1 : new_thresh_num
        low(count) = thresholds(i);
        high(count) = thresholds(j);
        count = count+1;
    end
end

% holders for the different F scores for each pair
eC_F_2 = zeros(num_pairs,1);
sC_F_2 = zeros(num_pairs,1);

% loop through all the pairs
for i = 1 : num_pairs
    % construct edges
    elephant_edge = edge(elephant_gray, 'canny', [low(i),high(i)]);
    ski_edge = edge(ski_gray, 'canny', [low(i),high(i)]);
    
    % get F-scores of edges
    [~, ~, eC_F, ~, ~] = prf_1(elephant_gt, elephant_edge);
    [~, ~, sC_F, ~, ~] = prf_1(ski_gt, ski_edge);
    
    % add F-scores to list
    eC_F_2(i) = eC_F;
    sC_F_2(i) = sC_F ;
end
