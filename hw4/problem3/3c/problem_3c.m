%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021


%% Clear Everything
clear all
close all
clc


%% Image paths and dimensions
img_height = 420;
img_width = 640;
dog1 = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Dog_1.png";
dog2 = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Dog_2.png";
dog3 = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Dog_3.png";
cat = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/Cat.png";


%% Read in the images
dog1_data = imread(dog1);
dog2_data = imread(dog2);
dog3_data = imread(dog3);
cat_data = imread(cat);


%% Extract SIFT Features for the images
% dog3
dog3_I = single(rgb2gray(dog3_data));
[f_dog3,d_dog3] = vl_sift(dog3_I);

% dog1
dog1_I = single(rgb2gray(dog1_data));
[f_dog1,d_dog1] = vl_sift(dog1_I);

% dog2
dog2_I = single(rgb2gray(dog2_data));
[f_dog2,d_dog2] = vl_sift(dog2_I);

% cat
cat_I = single(rgb2gray(cat_data));
[f_cat,d_cat] = vl_sift(cat_I);


%% Apply PCA to reduce feature dimensions from 128 to 20
dog1_pca = applyPCA(d_dog1, 20);
dog2_pca = applyPCA(d_dog2, 20);
dog3_pca = applyPCA(d_dog3, 20);
cat_pca = applyPCA(d_cat, 20);


%% Combine the reduced datasets
% the combination will go dog1 -> dog2 -> dog3 -> cat
pca_combined = [];
% find the locations
% dog 1
dog1_start = 1;
dog1_end = size(dog1_pca,1);
pca_combined = [pca_combined; dog1_pca];
% dog 2
dog2_start = dog1_end+1;
dog2_end = dog2_start + size(dog2_pca,1)-1;
pca_combined = [pca_combined; dog2_pca];
% dog 3
dog3_start = dog2_end+1;
dog3_end = dog3_start + size(dog3_pca,1)-1;
pca_combined = [pca_combined; dog3_pca];
% cat
cat_start = dog3_end+1;
cat_end = cat_start + size(cat_pca,1)-1;
pca_combined = [pca_combined; cat_pca];

% sanity check
dog1_check = pca_combined(dog1_start:dog1_end,:);
dog2_check = pca_combined(dog2_start:dog2_end,:);
dog3_check = pca_combined(dog3_start:dog3_end,:);
cat_check = pca_combined(cat_start:cat_end,:);

disp("dog1 is the same? "+isequal(dog1_pca,dog1_check));
disp("dog2 is the same? "+isequal(dog2_pca,dog2_check));
disp("dog3 is the same? "+isequal(dog3_pca,dog3_check));
disp("cat is the same? "+isequal(cat_pca,cat_check));


%% Run K-Means on the combined features (8 centers)
MaxIters = 100000;
numCenters = 8;
predictions = kmeans(pca_combined, numCenters, 'Distance', 'sqeuclidean', 'MaxIter', MaxIters);


%% Split the predictions
dog1_kmean = predictions(dog1_start:dog1_end,:);
dog2_kmean = predictions(dog2_start:dog2_end,:);
dog3_kmean = predictions(dog3_start:dog3_end,:);
cat_kmean = predictions(cat_start:cat_end,:);


%% Use the split predictions to construct histograms for all the images
dog1_hist = calcHist(dog1_kmean, 8);
dog2_hist = calcHist(dog2_kmean, 8);
dog3_hist = calcHist(dog3_kmean, 8);
cat_hist = calcHist(cat_kmean, 8);


%% Calculate similarity between dog3 and all other images
sim_3to1 = calcSimilarity(dog3_hist, dog1_hist);
sim_3to2 = calcSimilarity(dog3_hist, dog2_hist);
sim_3tocat = calcSimilarity(dog3_hist, cat_hist);


%% Output the similarities
disp(" ");disp(" ");disp("The similarity between dog3 and dog1 is: "+sim_3to1);
disp(" ");disp("The similarity between dog3 and dog2 is: "+sim_3to2);
disp(" ");disp("The similarity between dog3 and cat is: "+sim_3tocat);



