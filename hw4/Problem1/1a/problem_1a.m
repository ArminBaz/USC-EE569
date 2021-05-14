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
img_height = 128;
img_width = 128;
img_channels = 1;
train_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/train";
test_path = "/Users/arminbazarjani/Desktop/CurrentClasses/ee569/homework4/HW4_material/test";


%% Loop through the train and test paths and store each image path in an array
train_names = {};
test_names = {};

train_files = dir(fullfile(train_path,'*.raw')); % gets all raw train files
test_files = dir(fullfile(test_path,'*.raw')); % gets all raw test files

% loop through train files and append to train_names
for i = 1:length(train_files)
    % get the base and full file paths
    base_name = train_files(i).name;
    full_name = fullfile(train_path, base_name);
    
    % append full name to train names array
    train_names = [train_names, full_name];
end

% loop through test files and append to test_names
for i = 1:length(test_files)
    % get the base and full file paths
   base_name = test_files(i).name;
   full_name = fullfile(test_path, base_name);
   
   % append full name to test names array
   test_names = [test_names, full_name];
end


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


%% Loop through the training images and get the feature vectors for each
feature_holder_train = {};
% loop through all the image paths
for i = 1:size(train_names,2)
    % read the image in
    train_path = train_names(i);
    train_data = readraw(train_path, img_height, img_width, img_channels);
    train_data = double(train_data);
    
    % get the feature vector
    feature_vec_train = lawFilters(train_data, law_holder);
    
    % append feature vector to holder
    feature_holder_train = [feature_holder_train, feature_vec_train];
end


%% Loop through the testing images and get the feature vectors for each
feature_holder_test = {};
% loop through all the image paths
for i = 1:size(test_names,2)
    % read the image in
    test_path = test_names(i);
    test_data = readraw(test_path, img_height, img_width, img_channels);
    test_data = double(test_data);
    
    % get the feature vector
    feature_vec_test = lawFilters(test_data, law_holder);
    
    % append feature vector to holder
    feature_holder_test = [feature_holder_test, feature_vec_test];
end


%% Combine the features into a big matrix
features_train = [];
for i = 1:size(feature_holder_train,2)
    matrix_ = cell2mat(feature_holder_train(i));
    features_train = [features_train; matrix_];
end

features_test = [];
for i = 1:size(feature_holder_test,2)
    matrix_ = cell2mat(feature_holder_test(i));
    features_test = [features_test; matrix_];
end


%% Find Discriminant Power of the training features
discrim_powers = findDiscrimPower(features_train);

disp("index with min discrim power: " + 10);
disp("index with max discrim power: " + 11);


%% Apply PCA to training features
[coeff_train, score_train, latent_train] = pca(features_train);
features_train_pca = features_train * coeff_train(:,1:3);

[coeff_test, score_test, latent_test] = pca(features_test);
features_test_pca = features_test * coeff_train(:,1:3);


%% Call KNN function on test features to label the points
% 1 : blanket
% 2 : brick
% 3 : grass
% 4 : rice
true_labels = [3, 2, 1, 3, 1, 1, 2, 4, 3, 2, 4, 4];
pred_labels = [];
for i = 1:size(features_test_pca)
    test_vec = features_test_pca(i,:);
    label = KNN(features_train_pca, test_vec);
    pred_labels = [pred_labels, label];
end


%% Calculate Accuracy of using KNN w/ Mahalanobis distance and PCA
num_correct = 0;
for i = 1:12
    if true_labels(i) == pred_labels(i)
        num_correct = num_correct + 1;
    end
end

acc = num_correct/12;

disp("The Accuracy of Law Filter with PCA for feature extraction suplemented with KNN using Mahalanobis distance for classification is: " + acc);


%% Visualize the embeddings using after applying PCA
% Split train set
blanket_train = features_train_pca(1:9,:);
brick_train = features_train_pca(10:18,:);
grass_train = features_train_pca(19:27,:);
rice_train = features_train_pca(28:36,:);

scatter3(blanket_train(:,1),blanket_train(:,2),blanket_train(:,3));
hold on
scatter3(brick_train(:,1),brick_train(:,2),brick_train(:,3));
hold on
scatter3(grass_train(:,1),grass_train(:,2),grass_train(:,3));
hold on
scatter3(rice_train(:,1),rice_train(:,2),rice_train(:,3));
legend('Blanket','Brick','Grass','Rice');
xlabel('1st Principal Component')
ylabel('2nd Principal Component')
zlabel('3rd Principal Component')

%% Construct Training Labels
% 1 : blanket
% 2 : brick
% 3 : grass
% 4 : rice
labels_train = [1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4];


%% Save The 25-D and 3-D feature matrices for use in 1(b)
save('../features_train.mat', 'features_train');  % Nx25 training matrix
save('../features_train_pca.mat', 'features_train_pca');  % Nx3 training matrix
save('../features_test.mat', 'features_test');  % Nx25 testing matrix
save('../features_test_pca.mat', 'features_test_pca');  % Nx3 testing matrix
save('../labels_test.mat', 'true_labels');  % labels of test samples
save('../labels_train.mat', 'labels_train');  % labels of training samples





