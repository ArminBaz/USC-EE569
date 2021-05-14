%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021


%% Clear Everything
clear all
close all
clc


%% Image Paths and Dimensions
% define the necessary image
img_height = 360;
img_width = 575;
img_channels = 1;
mosaic_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/composite.png";

% read the image in
mosaic_data = imread(mosaic_path);
mosaic_data = rgb2gray(mosaic_data);
figure(1); imshow(mosaic_data);
mosaic_data = double(mosaic_data);


%% Define all 25 Law Filters
% L5 * Everything
law_holder(:,:,1) = [1 4 6 4 1; 4 16 24 16 4; 6 24 36 24 6; 4 16 24 16 4; 1 4 6 4 1]; % L5 * L5
law_holder(:,:,2) = [-1 -2 0 2 1; -4 -8 0 4 8; -6 -12 0 12 6; -4 -8 0 8 4; -1 -2 0 2 1]; % L5 * E5
law_holder(:,:,3) = [-1 0 2 0 -1; -4 0 8 0 -4; -6 0 12 0 -6; -4 0 8 0 -4; -1 0 2 0 -1]; % L5 * S5
law_holder(:,:,4) = [-1 2 0 -2 1; -4 8 0 -8 4; -6 12 0 -12 6; -4 8 0 -8 4; -1 2 0 -2 1]; % L5 * W5
law_holder(:,:,5) = [1 -4 6 -4 1; 4 -16 24 -16 4; 6 -24 36 -24 6; 4 -16 24 -16 4; 1 -4 6 -4 1]; % L5 * R5

% E5 * Everything
law_holder(:,:,6) = [-1 -4 -6 -4 -1; -2 -8 -12 -8 -2; 0 0 0 0 0; 2 8 12 8 12; 1 4 6 4 1]; % E5 * L5
law_holder(:,:,7) = [1 2 0 -2 -1; 2 4 0 -4 -2; 0 0 0 0 0; -2 -4 0 4 2; -1 -2 0 2 1]; % E5 * E5
law_holder(:,:,8) = [1 0 -2 0 1; 2 0 -4 0 2; 0 0 0 0 0; -2 0 4 0 -2; -1 0 2 0 -1]; % E5 * S5
law_holder(:,:,9) = [1 -2 0 2 -1; 2 -4 0 4 -2; 0 0 0 0 0; -2 4 0 -4 2; -1 2 0 -2 1]; % E5 * W5
law_holder(:,:,10) = [-1 4 -6 4 -1; -2 8 -12 8 -2; 0 0 0 0 0; 2 -8 12 -8 2; 1 -4 6 -4 1]; % E5 * R5

% S5 * Everything
law_holder(:,:,11) = [-1 -4 -6 -4 -1; 0 0 0 0 0; 2 8 12 8 2; 0 0 0 0 0; -1 -4 -6 -4 -1]; % S5 * L5
law_holder(:,:,12) = [1 2 0 -2 -1; 0 0 0 0 0; -2 -4 0 4 2; 0 0 0 0 0; 1 2 0 -2 -1]; % S5 * E5
law_holder(:,:,13) = [1 0 -2 0 1; 0 0 0 0 0; -2 0 4 0 -2; 0 0 0 0 0; 1 0 -2 0 1]; % S5 * S5
law_holder(:,:,14) = [1 -2 0 2 1; 0 0 0 0 0; -2 4 0 -4 2; 0 0 0 0 0; 1 -2 0 2 -1]; % S5 * W5
law_holder(:,:,15) = [-1 4 -6 4 -1; 0 0 0 0 0; 2 -8 12 -8 2; 0 0 0 0 0; -1 4 -6 4 -1]; % S5 * R5

% W5 * Everything
law_holder(:,:,16) = [-1 -4 -6 -4 -1; 2 8 12 8 2; 0 0 0 0 0; -2 -8 -12 -8 -2; 1 4 6 4 1]; % W5 * L5
law_holder(:,:,17) = [1 2 0 -2 -1; -2 -4 0 4 2; 0 0 0 0 0; 2 4 0 -4 -2; -1 -2 0 2 1]; % W5 * E5
law_holder(:,:,18) = [1 0 -2 0 1; -2 0 4 0 -2; 0 0 0 0 0; 2 0 -4 0 2; -1 0 2 0 -1]; % W5 * S5
law_holder(:,:,19) = [1 -2 0 2 -1; -2 4 0 -4 2; 0 0 0 0 0; 2 -4 0 4 -2; -1 2 0 -2 1]; % W5 * W5
law_holder(:,:,20) = [-1 4 -6 4 -1; 2 -8 12 -8 2; 0 0 0 0 0; -2 8 -12 8 -2; 1 -4 6 -4 1]; % W5 * R5

% R5 * Everything
law_holder(:,:,21) = [1 4 6 4 1; -4 -16 -24 -16 -4; 6 24 36 24 6; -4 -16 -24 -16 -4; 1 4 6 4 1]; % R5 * L5
law_holder(:,:,22) = [-1 -2 0 2 1; 4 8 0 -8 -4; -6 -12 0 12 6; 4 8 0 -8 -4; -1 -2 0 2 1]; % R5 * E5
law_holder(:,:,23) = [-1 0 2 0 -1; 4 0 -8 0 4; -6 0 12 0 -6; 4 0 -8 0 4; -1 0 2 0 -1]; % R5 * S5
law_holder(:,:,24) = [-1 2 0 -2 1; 4 -8 0 8 -4; -6 12 0 -12 6; 4 -8 0 8 -4; -1 2 0 -2 1]; % R5 * W5
law_holder(:,:,25) = [1 -4 6 -4 1; -4 16 -24 16 -4; 6 -24 36 -24 6; -4 16 -24 16 -4; 1 -4 6 -4 1]; % R5 * R5


%% Method 1 : Feature Reduction with PCA (1)
window_size = 15;
mosaic_pca = mosaicPCA(mosaic_data, law_holder, window_size, img_height, img_width);
%% Run Kmeans on PCA (2)
MaxIters = 100000;
pred = kmeans(mosaic_pca, 5, 'Distance', 'cityblock', 'MaxIter', MaxIters,'OnlinePhase','on');
%% Reshape (3)
labels = [0, 63, 127, 191, 255];
pred_reshape = reshape(pred, [img_height img_width]);
%% Fill in image (4)
seg_data = zeros(360,575);
for i = 1:360
    for j = 1:575
        seg_data(i,j) = labels(pred_reshape(i,j));
    end
end
seg_data = uint8(seg_data);
%% Display (5)
figure(2); imshow(seg_data)




